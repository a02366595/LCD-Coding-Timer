#ifndef HEALTH_LOGIC_H
#define HEALTH_LOGIC_H

#include <stdint.h>
#include <stdbool.h>

// State Machine Definitions
typedef enum {
    STATE_IDLE,
    STATE_CODING,
    STATE_BREAK,
    STATE_FINISHED
} SessionState;

// Initialization
void Health_Init(void);

// The Core Logic Loop (Your partner calls this from a 1-second timer interrupt)
void Health_Tick(void);

// Button Inputs (Your partner calls these when a hardware button is pressed)
void Health_QuickActionPressed(void); // e.g., Drink water
void Health_BreakTogglePressed(void); // Step away from keyboard
void Health_EndSessionPressed(void);  // Finish coding

// Getters for the LCD display
int Health_GetCurrentHP(void);
int Health_GetFinalScore(void);
SessionState Health_GetState(void);

#endif