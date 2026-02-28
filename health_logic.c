#include "health_logic.h"

// Private internal variables
static int current_hp;
static int total_session_seconds;
static int score_accumulator;
static SessionState current_state;

void Health_Init(void) {
    current_hp = 100;
    total_session_seconds = 0;
    score_accumulator = 0;
    current_state = STATE_IDLE;
}

void Health_Tick(void) {
    // We will build the decay and scoring math here
}

void Health_QuickActionPressed(void) {
    // Add health, cap at 100
}

void Health_BreakTogglePressed(void) {
    // Switch between CODING and BREAK states
}

void Health_EndSessionPressed(void) {
    // Calculate final area-under-curve score and set state to FINISHED
}

int Health_GetCurrentHP(void) {
    return current_hp;
}

int Health_GetFinalScore(void) {
    // Return the calculated 0-100 score
    return 0; 
}

SessionState Health_GetState(void) {
    return current_state;
}