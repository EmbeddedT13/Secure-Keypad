#include "state_machine.h"
#include "../hal/keypad.h"
#include "../hal/seven_segment.h"
#include "../hal/led.h"

typedef enum {
    STATE_IDLE,
    STATE_ENTERING_PIN,
    STATE_UNLOCKED,
    STATE_ALARM
} SystemState_t;

static SystemState_t CurrentState = STATE_IDLE;
static uint8 PasswordBuffer[4] = {0};
static uint8 DigitCount = 0;
static uint8 LastPressedKey = KEYPAD_NO_KEY_PRESSED;
static uint8 FailedAttempts = 0; /* Caps at 9 per spec */

static const uint8 CorrectPassword[4] = {'1', '2', '3', '4'}; 

/* Helper: Clears LEDs but leaves the 7-segment alone */
static void ClearAllIndicators(void) {
    LED_Off(LED_PROG_1);
    LED_Off(LED_PROG_2);
    LED_Off(LED_PROG_3);
    LED_Off(LED_PROG_4);
    LED_Off(LED_SUCCESS);
    LED_Off(LED_ALARM);
}

void StateMachine_Init(void) {
    CurrentState = STATE_IDLE;
    DigitCount = 0;
    FailedAttempts = 0;
    LastPressedKey = KEYPAD_NO_KEY_PRESSED;
    ClearAllIndicators();
    SevSeg_Display(FailedAttempts); /* Safely displays 0 on startup */
}

void StateMachine_EmergencyReset(void) {
    CurrentState = STATE_IDLE;
    DigitCount = 0;
    FailedAttempts = 0;
    ClearAllIndicators();
    SevSeg_Display(FailedAttempts); 
}

void StateMachine_DoorbellTrigger(void) {
    /* Simple, bulletproof toggle logic */
    LED_Toggle(LED_DOORBELL);
}

void StateMachine_Update(void) {
    uint8 key = Keypad_GetPressedKey();

    /* Software Debounce */
    if (key == KEYPAD_NO_KEY_PRESSED) {
        LastPressedKey = KEYPAD_NO_KEY_PRESSED;
        return; 
    }
    if (key == LastPressedKey) {
        return; 
    }
    LastPressedKey = key; 

    /* Global Reset via Keypad 'C' */
    if (key == 'C') {
        StateMachine_EmergencyReset();
        return;
    }

    switch (CurrentState) {
        
        case STATE_IDLE:
        case STATE_ALARM:
        case STATE_UNLOCKED:
            /* If we are recovering from a previous attempt, ANY number key starts a new attempt */
            if (key >= '0' && key <= '9') {
                ClearAllIndicators(); 
                DigitCount = 0;
                PasswordBuffer[DigitCount] = key;
                DigitCount++;
                
                LED_On(LED_PROG_1);
                CurrentState = STATE_ENTERING_PIN;
            }
            break;

        case STATE_ENTERING_PIN:
            if (key >= '0' && key <= '9') {
                PasswordBuffer[DigitCount] = key;
                DigitCount++;
                
                /* Cascade the Progress Bar LEDs */
                if (DigitCount == 2) LED_On(LED_PROG_2);
                if (DigitCount == 3) LED_On(LED_PROG_3);
                if (DigitCount == 4) LED_On(LED_PROG_4);

                /* Verification Phase */
                if (DigitCount == 4) {
                    uint8 isCorrect = 1;
                    for (uint8 i = 0; i < 4; i++) {
                        if (PasswordBuffer[i] != CorrectPassword[i]) {
                            isCorrect = 0;
                            break;
                        }
                    }

                    if (isCorrect) {
                        LED_On(LED_SUCCESS);
                        FailedAttempts = 0; /* Reset fails on success */
                        SevSeg_Display(FailedAttempts);
                        CurrentState = STATE_UNLOCKED;
                    } else {
                        LED_On(LED_ALARM);
                        if(FailedAttempts < 9) { /* Hard cap at 9 */
                            FailedAttempts++;
                        }
                        SevSeg_Display(FailedAttempts);
                        CurrentState = STATE_ALARM;
                    }
                }
            }
            break;
    }
}