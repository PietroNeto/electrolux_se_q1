/*------------------------------------------------------------------------------
* File machine_control.h
*
* Electrolux - Softare Engineer Test - Question 2
* State Machine
*
* Autor: Pietro Di Bernardo Neto
* Data: 08/12/2024
*-----------------------------------------------------------------------------*/

#ifndef MACHINE_CONTROL_H
#define MACHINE_CONTROL_H

/*------------------------------------------------------------------------------
* INCLUDES
*-----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*------------------------------------------------------------------------------
* ENUMERATIONS
*-----------------------------------------------------------------------------*/
typedef enum {
    STATE_MACHINE_ENTER = 0,
    STATE_OFF,
    STATE_ON,
    STATE_PROTECT,
} MachineStates;

typedef enum {
    EVENT_NULL = 0,
    BUTTON_PRESSED,
    BUTTON_RELEASED,
    TIMEOUT,
} MachineEvents;

/*------------------------------------------------------------------------------
* STRUCTURE
*-----------------------------------------------------------------------------*/
struct __attribute__((packed)) machineControl {
    MachineStates bytCurrentState;        // Current state of the state machine
    MachineStates bytNextState;           // Next state of the state machine
    MachineEvents bytStateMachineEvt;     // Event triggering the state transition
    uint32_t buttonPressTime;             // Timestamp when the button was pressed
    uint32_t protectStartTime;            // Timestamp for entering PROTECT state
};

/*------------------------------------------------------------------------------
* GLOBAL VARIABLES
*-----------------------------------------------------------------------------*/
extern struct machineControl gStcMachineControl;

/*------------------------------------------------------------------------------
* FUNCTION PROTOTYPES
*-----------------------------------------------------------------------------*/
void StateMachineInit(void);
void StateMachineMainLoop(uint32_t currentTime);
void ReadButtonState(bool isButtonPressed, uint32_t currentTime);

#endif // MACHINE_CONTROL_H
