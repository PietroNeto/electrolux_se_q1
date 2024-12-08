/*------------------------------------------------------------------------------
* File main.c
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
* FUNCTION DEFINITIONS
*-----------------------------------------------------------------------------*/

// Abstracted time function - should use timer instead
uint32_t getCurrentTime(void);

// Abstracted read button state function - should read GPIO register (depends on arch)
uint32_t isButtonPressed(void);

int main(void) {
    StateMachineInit();

    while (1) {
        uint32_t currentTime = getCurrentTime();
        //printf("current time: %d\n", currentTime);
        //printf("button state: %d\n",isButtonPressed());

        // Update button state
        ReadButtonState(isButtonPressed(), currentTime);

        // Process state machine
        StateMachineMainLoop(currentTime);
    }

    return 0;
}


/* ----------------------------------------------------------------------------------------
* !!! FOR TEST ONLY !!!
* ---------------------------------------------------------------------------------------*/

#ifdef __linux__ // Compile this code only if the architecture is Linux - TESTED

#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>

uint32_t getCurrentTime(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // Get monotonic time to avoid issues with system clock changes
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000); // Convert to milliseconds
}

uint32_t isButtonPressed(void) {
    static uint32_t buttonState = 0; // Holds the last known state of the button
    struct termios oldt, newt;
    int oldf;
    int ch;

    // Save old terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Set terminal to non-canonical mode (no line buffering, immediate key detection)
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Save old file descriptor flags and set non-blocking mode
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // Check if a key is pressed
    ch = getchar(); // Non-blocking getchar
    if (ch != EOF) {
        if (ch == '\n') { 
            buttonState = 1; // Button is pressed
        }
    } else {
        // If no key is detected, assume the button is released
        buttonState = 0;
    }

    // Restore old file descriptor flags and terminal settings
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return buttonState;
}


#endif // __linux__

#ifdef _WIN32 // Compile this code only if the architecture is Windows - NOT TESTED

#include <windows.h>
#include <conio.h>

uint32_t getCurrentTime(void) {
    static LARGE_INTEGER frequency;
    static int initialized = 0;
    LARGE_INTEGER counter;

    // Initialize the frequency (only once)
    if (!initialized) {
        QueryPerformanceFrequency(&frequency);
        initialized = 1;
    }

    // Get the performance counter
    QueryPerformanceCounter(&counter);

    // Convert to milliseconds
    return (uint32_t)((counter.QuadPart * 1000) / frequency.QuadPart);
}


uint32_t isButtonPressed(void) {
    if (_kbhit()) {                 // Check if a key is pressed
        char ch = _getch();         // Get the key
        if (ch == '\r') {           // Check for Enter key (carriage return '\r')
            return 1;               // Enter key detected
        }
    }
    return 0;                       // No key pressed or not Enter
}

#endif // _WIN32