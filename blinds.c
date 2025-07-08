
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pigpiod_if2.h>


#define SERVO_PIN 18 // pin 12 on Pi Zero W
#define BUTTON_PIN 17 // pin 11 on Pi Zero W
#define SPIN_COUNT_FOR_BLINDS 4
#define TIME_PER_ROTATION 1
#define SPIN_SPEED_CCW 1850
#define SPIN_SPEED_CW 1150
#define NEUTRAL 1500
#define OPEN_DIRECTION 1 // direction to open the blinds
#define CLOSE_DIRECTION 0 // direction to close the blinds
#define STATE_TXT "/home/rjt21/smart_blinds_proj/time_spins/OC_switch.txt"
#define LOCK_FILE "/home/rjt21/smart_blinds_proj/GPIO.lock"



int setup_pi(int *pi) {
    *pi = pigpio_start(NULL, NULL);
    if (*pi<0) {
	printf("Failed to connect to pigpiod: %d\n", *pi);
	return 0;
    }

	// servo setup
    int result = set_mode(*pi, SERVO_PIN, PI_OUTPUT);
    if (result <0) {
	printf("set_mode servo failed: %d\n", result);
	return 0;
    }
    result = set_PWM_frequency(*pi, SERVO_PIN, 50);
    if (result<0) {
	printf("set_PWM_frequency failed: %d\n");
	return 0;
    }

	// button setup
    result = set_mode(*pi, BUTTON_PIN, PI_INPUT);
    if (result <0) {
	printf("set_mode button failed: %d\n");
	return 0;
    }
    
    result = set_pull_up_down(*pi, BUTTON_PIN, PI_PUD_OFF);
    if (result<0) {
	printf("set_pull_up_down failed: %d\n");
	return 0;
    }
	// 2 lines below for testing purposes
   // result = gpio_read(*pi, BUTTON_PIN);
   // printf("Initial read GPIO %d: %d\n", BUTTON_PIN, result);

    return 1; //SUCCESSFULL SETUP
}

void cleanup(int *pi) {
    set_servo_pulsewidth(*pi, SERVO_PIN, NEUTRAL);
    time_sleep(0.5);
    set_servo_pulsewidth(*pi, SERVO_PIN, 0);
    pigpio_stop(pi);
}



void spin_servo_direction(int *pi, int rotation_direction) {
    char* state = rotation_direction ? "opening" : "closing";
    printf("Starting servo spins. %s the blinds...\n", state);
    int speed = rotation_direction ? SPIN_SPEED_CW : SPIN_SPEED_CCW;
    set_servo_pulsewidth(*pi, SERVO_PIN, speed);
    double total_time = TIME_PER_ROTATION * SPIN_COUNT_FOR_BLINDS;
    time_sleep(total_time);
    printf("%d spins completed\n", SPIN_COUNT_FOR_BLINDS);
    set_servo_pulsewidth(*pi, SERVO_PIN, NEUTRAL);
}

int button_read(int *pi, int button_pin) {
    int pos = gpio_read(*pi, button_pin);
    //printf("button reading : %d\n", pos);
    return pos;
}

int get_blinds_state(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
	perror("Error opening file");
	return 0;
    }

    char line[7]; //state is the txt "open" or "closed"
    while (fgets(line, sizeof(line), file)) {
	if (strstr(line, "open") != NULL) {
	    fclose(file);
	    return 1;
	}
    }
    fclose(file);
    return 2;
}

int write_blinds_txt(const char *filename, int open1_close2) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) { perror("Error opening file"); return 0;}
    
    if (open1_close2 == 1) { fprintf(file, "open\n");}
    else if (open1_close2 == 2) { fprintf(file, "closed\n");}
    fclose(file);
    return 3;
}

int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) { fclose(file); return 1;}
    return 0;
}




int main() {
    printf("Starting button_blinds...\n"); // just so i get a new line
    // initializers
    int pi;
    if (!setup_pi(&pi)) { return 1;}
    static int change_state = 0;
    int blinds_state = get_blinds_state(STATE_TXT);
	if (blinds_state == 0) { return 2;} //error opening file


    while (1) {
	switch (change_state) {
	    case 0: // nothing happening. Checks time and button
		//CHECK IF LOCK_FILE EXISTS
		if (file_exists(LOCK_FILE)) { change_state = 3; break;}
		
		if (button_read(&pi, BUTTON_PIN) == 1) {
		    printf("button has been pressed!\n");
		    change_state = blinds_state;
		    time_sleep(0.2);
		}
		time_sleep(0.1);
		break;
	    case 1: // blinds are currently open. working to close
		spin_servo_direction(&pi, CLOSE_DIRECTION);
		write_blinds_txt(STATE_TXT, 2); //2 to write closed
		blinds_state = 2; //now blinds are closed
		change_state = 0;
		break;
	    case 2: // blinds are currently closed. working to open
		spin_servo_direction(&pi, OPEN_DIRECTION);
		write_blinds_txt(STATE_TXT, 1); //1 to write open
		blinds_state = 1; //now blinds are open
		change_state = 0;
		break;
	    case 3: // pause program and gpio use for time_spins
		cleanup(&pi);
		sleep(15);
		change_state = 0;
		blinds_state = get_blinds_state(STATE_TXT);
		break;
	    default:
		change_state = 0;
	} //switch statement
    } //while loop

}
