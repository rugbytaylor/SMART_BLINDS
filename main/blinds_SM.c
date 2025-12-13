#include "blinds_SM.h"

// provides the transitions of the state machine
void blinds_SM_transitions(blinds_state_t blinds_state)
{
    switch (blinds_state) {
        case BLINDS_ST_INIT:
            // Initialization logic here
            break;

        case BLINDS_ST_OPEN:
            // Logic for open state here
            break;
        
        case BLINDS_ST_CLOSED:
            // Logic for closed state here
            break;
        
        case BLINDS_ST_ROTATING:
            // Logic for rotating state here
            break;
        
        default:
            blinds_state = BLINDS_ST_INIT;
            break;
    }
}

// provides the actions for each state
void blinds_SM_actions(blinds_state_t blinds_state)
{
    switch (blinds_state) {
        case BLINDS_ST_INIT:
            // Initialization logic here
            break;

        case BLINDS_ST_OPEN:
            // Logic for open state here
            break;
        
        case BLINDS_ST_CLOSED:
            // Logic for closed state here
            break;
        
        case BLINDS_ST_ROTATING:
            // Logic for rotating state here
            break;
        
        default:
            blinds_state = BLINDS_ST_INIT;
            break;
    }
}