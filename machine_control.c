/*------------------------------------------------------------------------------
* File machine_control.c
*
* Electrolux - Softare Engineer Test - Question 2
* State Machine
*
* Autor: Pietro Di Bernardo Neto
* Data: 08/12/2024
*-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
* INCLUDES
*-----------------------------------------------------------------------------*/
#include "machine_control.h"
#include <stdio.h>

/*------------------------------------------------------------------------------
* GLOBAL VARIABLES
*-----------------------------------------------------------------------------*/
struct machineControl gStcMachineControl;

/*------------------------------------------------------------------------------
* FUNCTION DEFINITIONS
*-----------------------------------------------------------------------------*/

// Initialize the state machine
void StateMachineInit(void) {
    gStcMachineControl.bytCurrentState = STATE_OFF;
    gStcMachineControl.bytNextState = STATE_OFF;
    gStcMachineControl.bytStateMachineEvt = EVENT_NULL;
    gStcMachineControl.buttonPressTime = 0;
    gStcMachineControl.protectStartTime = 0;
}

// Update state machine based on current state and event
void StateMachineMainLoop(uint32_t currentTime) {
    switch (gStcMachineControl.bytCurrentState) {
        case STATE_OFF:
            if (gStcMachineControl.bytStateMachineEvt == BUTTON_PRESSED) {
                gStcMachineControl.bytNextState = STATE_ON;
                gStcMachineControl.buttonPressTime = currentTime;
            }
            break;

        case STATE_ON:
            if (gStcMachineControl.bytStateMachineEvt == BUTTON_RELEASED) {
                gStcMachineControl.bytNextState = STATE_PROTECT;
                gStcMachineControl.protectStartTime = currentTime;
            }
            break;

        case STATE_PROTECT:
            if (currentTime - gStcMachineControl.protectStartTime >= 10000) {
                gStcMachineControl.bytNextState = STATE_OFF;
            }
            break;

        default:
            break;
    }

    // Update current state
    if (gStcMachineControl.bytCurrentState != gStcMachineControl.bytNextState) {
        printf("State Transition: %d -> %d\n",
               gStcMachineControl.bytCurrentState, gStcMachineControl.bytNextState);
        gStcMachineControl.bytCurrentState = gStcMachineControl.bytNextState;
    }

    // Reset the event after processing
    gStcMachineControl.bytStateMachineEvt = EVENT_NULL;
}

// Read button state and trigger events
void ReadButtonState(bool isButtonPressed, uint32_t currentTime) {
    if (isButtonPressed) {
        if (gStcMachineControl.bytCurrentState == STATE_OFF) {
            gStcMachineControl.bytStateMachineEvt = BUTTON_PRESSED;
        }
    } else {
        if (gStcMachineControl.bytCurrentState == STATE_ON) {
            gStcMachineControl.bytStateMachineEvt = BUTTON_RELEASED;
        }
    }
}
