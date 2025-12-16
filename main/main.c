#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "blinds_SM.h"
#include "esp_sleep.h"
#include "esp_pm.h"
#include "esp_log.h"
#include "button.h"
#include "servo.h"
#include "config.h"

static const char *TAG = "main_blinds";

// keeps track of state across deep sleep cycles
RTC_DATA_ATTR static blinds_state_t blinds_state = BLINDS_ST_INIT;

// GPIO wakeup configuration on active-low button
static void configure_gpio_wakeup(void)
{
    esp_sleep_enable_ext0_wakeup(BTN_PIN, 0);
}

// task to handle blinds state machine and deep sleep
static void blinds_task(void *arg)
{
    blinds_SM_run(&blinds_state);
    // configure GPIO wakeup
    configure_gpio_wakeup();
    esp_deep_sleep_start();
}


void app_main(void)
{
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_causes();
    button_init(BTN_PIN);

    if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
        ESP_LOGI(TAG, "Woke up from btn press");
    } else {
        ESP_LOGI(TAG, "cold_boot");
        blinds_state = BLINDS_ST_INIT;
    }

    xTaskCreate(blinds_task, "blinds_task", 2048, NULL, 5, NULL);
}
