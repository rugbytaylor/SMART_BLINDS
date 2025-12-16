#include "servo.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "servo";

#define SERVO_FREQ_HZ        50
#define SERVO_PERIOD_US     20000

#define SERVO_NEUTRAL_US    1500
#define SERVO_OPEN_US       1700
#define SERVO_CLOSE_US      1300
#define FULL_SPIN_DURATION_MS 2000 // duration for full open/close cycle

static bool initialized = false;
static ledc_channel_t channel = LEDC_CHANNEL_0;

// ms to duty cycle conversion
static uint32_t us_to_duty(uint32_t us)
{
    return (us * ((1 << 16) - 1)) / SERVO_PERIOD_US;
}

void servo_init(gpio_num_t pin)
{
    if (initialized) { return;}

    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .freq_hz = SERVO_FREQ_HZ
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t ch = {
        .gpio_num = pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = channel,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ch);

    initialized = true;
}

static void servo_stop()
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel,
                  us_to_duty(SERVO_NEUTRAL_US));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}

static void servo_move_timed(int direction, uint32_t duration_ms)
{
    uint32_t us = (direction == SERVO_DIR_OPEN) ?
                  SERVO_OPEN_US : SERVO_CLOSE_US;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, us_to_duty(us));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);

    vTaskDelay(pdMS_TO_TICKS(duration_ms));

    servo_stop();
}


void servo_rotate_open(gpio_num_t servo_pin, uint8_t num_rotations)
{
    servo_init(servo_pin);
    servo_move_timed(SERVO_DIR_OPEN, FULL_SPIN_DURATION_MS);
}

void servo_rotate_close(gpio_num_t servo_pin, uint8_t num_rotations)
{
    servo_init(servo_pin);
    servo_move_timed(SERVO_DIR_CLOSE, FULL_SPIN_DURATION_MS);
}

void servo_deinit()
{
    if (!initialized) { return;}
    // stop PWM output on servo pin
    ledc_stop(LEDC_LOW_SPEED_MODE, channel, 0);
    initialized = false;
}