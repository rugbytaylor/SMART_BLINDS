#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "blinds_SM.h"

#define TASK_DELAY_MS       1000 // in ms


void blinds_task(void *arg)
{
    blinds_state_t blinds_state = BLINDS_ST_INIT;
    while (1) {
        blinds_SM_transitions(blinds_state);
        blinds_SM_actions(blinds_state);
        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_MS));
    }
}

void app_main(void)
{
    xTaskCreate(blinds_task, "blinds_task", 2048, NULL, 5, NULL);
}
