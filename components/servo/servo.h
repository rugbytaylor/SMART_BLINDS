#ifndef SERVO_H
#define SERVO_H

#include "driver/gpio.h"

#define SERVO_DIR_OPEN      1
#define SERVO_DIR_CLOSE     0

// initializes servo PWM
void servo_init(gpio_num_t servo_pin);

// rotates servo to open blinds based time. future update for num_rotations
void servo_rotate_open(gpio_num_t servo_pin, uint8_t num_rotations);

// rotates servo to close blinds based on time. future update for num_rotations
void servo_rotate_close(gpio_num_t servo_pin, uint8_t num_rotations);

// deinitializes servo PWM
void servo_deinit();

#endif