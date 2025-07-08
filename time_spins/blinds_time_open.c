
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pigpiod_if2.h>


#define SERVO_PIN 18 // pin 12 on Pi Zero W
#define SPIN_COUNT_FOR_BLINDS 4
#define TIME_PER_ROTATION 1
#define SPIN_SPEED_CCW 1850
#define SPIN_SPEED_CW 1150
#define NEUTRAL 1500
#define OPEN_DIRECTION 1 // direction to close the blinds (0 is close)
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

int check_blinds_state(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) { perror("Error opening file"); return 0;}

	//OPEN==1 and CLOSED==2
    char line[7];
    while (fgets(line, sizeof(line), file)) {
      if (strstr(line, "open") != NULL) { fclose(file); return 1;}
      else if (strstr(line,"closed")!=NULL) {fclose(file); return 2;}
    }
    fclose(file);
    return 3;
}

int write_txt_open(const char *filename) {
    FILE *file = fopen(filename, "w");
    fprintf(file, "open\n");
    fclose(file);
    return 0;
}


int main() {
    // check if already closed
    int blinds_state = check_blinds_state(STATE_TXT);
    if (blinds_state == 1) { return 0;} //if already open, done!
    else {
	//CREATE LOCK FILE
	FILE *lock = fopen(LOCK_FILE, "w");
	if (!lock) { perror("Failed to create lock file"); return 3;}
	fclose(lock);
	sleep(3);

	printf("Starting blinds_time_open...\n");
	int pi;
	if (!setup_pi(&pi)) { return 1;}
	spin_servo_direction(&pi, OPEN_DIRECTION);

	int written = write_txt_open(STATE_TXT);
	if (written == 1) {perror("Error writing to file"); return 2;}

	cleanup(&pi);
	remove(LOCK_FILE);
	return 0;
    }
}
