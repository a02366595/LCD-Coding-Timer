#include "health_logic.h"

// Private internal variables
volatile int current_hp;
static int total_session_seconds;
static int score_accumulator;
static SessionState current_state;
extern volatile int timerEnable;

void Health_Init(void) {
    current_hp = 100;
    total_session_seconds = 0;
    score_accumulator = 0;
    current_state = STATE_CODING; // Assume we start coding right away

}

void Health_Tick(void) {
    if (current_state == STATE_FINISHED) return; 
    
    total_session_seconds++;

    if (current_state == STATE_CODING) {
        if (timerEnable == 1) {
            current_hp -= 1; // Drain 1HP per second
            if (current_hp < 0) current_hp = 0;
        }
    } else if (current_state == STATE_BREAK) {
        current_hp += 2; // Heal 2HP per second while on break
        if (current_hp > 100) current_hp = 100;
    }
    
    score_accumulator += current_hp;
}

void Health_QuickActionPressed(void) {
    if (current_state != STATE_FINISHED){
        current_hp += 15; //Water boost
        if (current_hp > 100) current_hp = 100;
    }
}

void Health_BreakTogglePressed(void) {
    if (current_state == STATE_CODING) {
        current_state = STATE_BREAK;
    } else if (current_state == STATE_BREAK){
        current_state = STATE_CODING;
    }
}

void Health_EndSessionPressed(void) {
    // Calculate final area-under-curve score and set state to FINISHED
    current_state = STATE_FINISHED;
}

int Health_GetCurrentHP(void) {
    return current_hp;
}

int Health_GetFinalScore(void) {
    if (total_session_seconds == 0) return 100;
    // Return the calculated 0-100 score
    return (score_accumulator / total_session_seconds); 
}

SessionState Health_GetState(void) {
    return current_state;
}

#include <stdio.h> 

void Health_FormatDisplayString(char* buffer){
    // FIXED: All strings are now strictly 15 chars or less to prevent RAM crashing
    if (current_state == STATE_CODING) {
        sprintf(buffer, "HP:%3d [CODE]  ", current_hp);
    } else if (current_state == STATE_BREAK){
        sprintf(buffer, "HP:%3d [BREAK] ", current_hp);
    } else if (current_state == STATE_FINISHED) {
        sprintf(buffer, "Score: %d/100  ", Health_GetFinalScore());
    } else {
        sprintf(buffer, "Ready...       ");
    }
}