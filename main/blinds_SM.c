#include "blinds_SM.h"
#include "button.h"
#include "servo.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "blinds_SM";
#define BTN_PIN         GPIO_NUM_0 // button pin mapping
#define SERVO_PIN       GPIO_NUM_18 // servo pin mapping
#define SM_START_OPEN   false // set to true if blinds start open
#define NUM_ROTATIONS   4 // future addition

static uint8_t rotate_direction = 0; //0=closing, 1=opening

// does SM transitions and actions all in one function
void blinds_SM_run(blinds_state_t *blinds_state)
{
    switch (*blinds_state) {
        
        case BLINDS_ST_INIT:
            ESP_LOGI(TAG, "INIT");

            if (SM_START_OPEN) {
                *blinds_state = BLINDS_ST_OPEN;
            } else {
                *blinds_state = BLINDS_ST_CLOSED;
            }
            break;

        case BLINDS_ST_CLOSED:
            ESP_LOGI(TAG, "CLOSED -> OPENING");
            servo_rotate_open(SERVO_PIN, NUM_ROTATIONS);
            *blinds_state = BLINDS_ST_OPEN;
            break;

        case BLINDS_ST_OPEN:
            ESP_LOGI(TAG, "OPEN -> CLOSING");
            servo_rotate_close(SERVO_PIN, NUM_ROTATIONS);
            *blinds_state = BLINDS_ST_CLOSED;
            break;

        default:
            ESP_LOGW(TAG, "Unknown state, resetting");
            *blinds_state = BLINDS_ST_CLOSED;
            break;
        }
}