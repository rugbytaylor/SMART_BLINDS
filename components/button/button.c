#include "button.h"
#include "esp_log.h"
static const char *TAG = "button";

void button_init(gpio_num_t pin)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
}

bool button_pressed(gpio_num_t pin)
{
    return (gpio_get_level(pin) == 0);
}
