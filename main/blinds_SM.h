#ifndef BLINDS_SM_H
#define BLINDS_SM_H

#include "driver/gpio.h"

typedef enum {
    BLINDS_ST_INIT,
    BLINDS_ST_CLOSED,
    BLINDS_ST_OPEN,
} blinds_state_t;

// transitions then actions of the SM all in one function
// intended use when waking from deep sleep
void blinds_SM_run(blinds_state_t *blinds_state);

#endif