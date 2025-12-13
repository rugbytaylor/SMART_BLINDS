#pragma once

typedef enum {
    BLINDS_ST_INIT,
    BLINDS_ST_OPEN,
    BLINDS_ST_CLOSED,
    BLINDS_ST_ROTATING
} blinds_state_t;

// transitions of the SM
void blinds_SM_transitions(blinds_state_t blinds_state);

// actions of the SM
void blinds_SM_actions(blinds_state_t blinds_state);