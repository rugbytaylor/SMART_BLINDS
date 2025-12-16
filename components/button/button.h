#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include "driver/gpio.h"

// Initialize a GPIO pin as a button (active low, pull-up)
void button_init(gpio_num_t pin);

// returns true if button is currently pressed
bool button_pressed(gpio_num_t pin);

#endif
