#include "state_machine.h"
#include "../hal/keypad.h"
#include "../hal/seven_segment.h"
#include "../hal/led.h"


#define ENTER_CRITICAL()  uint32 primask_save; \
                          __asm volatile ("MRS %0, PRIMASK" : "=r"(primask_save)); \
                          __asm volatile ("CPSID I" ::: "memory")

#define EXIT_CRITICAL()   __asm volatile ("MSR PRIMASK, %0" :: "r"(primask_save) : "memory")

typedef enum {
    STATE_LOCKED,
    STATE_UNLOCKED,
    STATE_ALARM
} SystemState_t;

#define LOCKOUT_THRESHOLD  10
#define PASSWORD_LENGTH    4

static SystemState_t  CurrentState    = STATE_LOCKED;
static uint8          PasswordBuffer[PASSWORD_LENGTH];
static uint8          DigitCount      = 0;
static uint8          FailedAttempts  = 0;
static uint8          LastPressedKey  = KEYPAD_NO_KEY_PRESSED;

static const uint8 CorrectPassword[PASSWORD_LENGTH] = {'1', '2', '3', '4'};

/* ------------------------------------------------------------------ */
static void ClearProgressLEDs(void) {
    LED_Off(LED_PROG_1);
    LED_Off(LED_PROG_2);
    LED_Off(LED_PROG_3);
    LED_Off(LED_PROG_4);
}

static void ClearAllIndicators(void) {
    ClearProgressLEDs();
    LED_Off(LED_SUCCESS);
    LED_Off(LED_ALARM);
}

static void UpdateProgressBar(uint8 count) {
    ClearProgressLEDs();
    if (count >= 1) LED_On(LED_PROG_1);
    if (count >= 2) LED_On(LED_PROG_2);
    if (count >= 3) LED_On(LED_PROG_3);
    if (count >= 4) LED_On(LED_PROG_4);
}

/* ------------------------------------------------------------------ */
void StateMachine_Init(void) {
    /* No IRQs are active yet when this is called from main() before the
       while(1) loop, so no critical section is needed here. */
    CurrentState   = STATE_LOCKED;
    DigitCount     = 0;
    FailedAttempts = 0;
    LastPressedKey = KEYPAD_NO_KEY_PRESSED;
    ClearAllIndicators();
    SevSeg_Display(FailedAttempts);
}

/*
 * Called from ISR context (EXTI9_5_IRQHandler).
 * Because we are INSIDE an interrupt, the main loop is already suspended
 * for the duration of this function — no additional guard is needed here.
 * The race is from the OTHER direction: main loop corrupting state that
 * the ISR then reads, which is guarded by ENTER/EXIT_CRITICAL in Update().
 */
void StateMachine_EmergencyReset(void) {
    if (CurrentState == STATE_ALARM)
    {
        CurrentState   = STATE_LOCKED;
        DigitCount     = 0;
        FailedAttempts = 0;
        LastPressedKey = KEYPAD_NO_KEY_PRESSED;
        ClearAllIndicators();
        SevSeg_Display(FailedAttempts);
    }
}

void StateMachine_DoorbellTrigger(void) {
    /* Single atomic toggle — no multi-step read-modify-write on shared
       state, so no critical section needed. */
    LED_Toggle(LED_DOORBELL);
}

/* ------------------------------------------------------------------ */
void StateMachine_Update(void) {
    uint8 key = Keypad_GetPressedKey();

    if (key == KEYPAD_NO_KEY_PRESSED) {
        LastPressedKey = KEYPAD_NO_KEY_PRESSED;
        return;
    }
    if (key == LastPressedKey) {
        return;
    }
    LastPressedKey = key;

    /*
     * CRITICAL SECTION — protect the shared state variables.
     *
     * Why here and not around the whole function?
     * - Keypad_GetPressedKey() only touches GPIOB (not shared with ISR).
     * - The debounce check above only touches LastPressedKey (not shared
     *   with ISR — the ISR resets it too, but that race is harmless: at
     *   worst we re-process the same key once).
     * - The actual state transitions below ARE the race: if the ISR fires
     *   between reading DigitCount and writing PasswordBuffer[DigitCount],
     *   we corrupt the buffer. So we lock only around this block.
     */
    ENTER_CRITICAL();

    if (CurrentState == STATE_LOCKED) {

        if (key == '#') {
            EXIT_CRITICAL();
            StateMachine_DoorbellTrigger();
            return;
        }

        if (key == '*') {
            EXIT_CRITICAL();
            return;
        }

        if (key >= '0' && key <= '9') {
            PasswordBuffer[DigitCount] = key;
            DigitCount++;
            UpdateProgressBar(DigitCount);

            if (DigitCount == PASSWORD_LENGTH) {
                uint8 isCorrect = 1;
                for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
                    if (PasswordBuffer[i] != CorrectPassword[i]) {
                        isCorrect = 0;
                        break;
                    }
                }

                if (isCorrect) {
                    ClearProgressLEDs();
                    LED_On(LED_SUCCESS);
                    FailedAttempts = 0;
                    SevSeg_Display(FailedAttempts);
                    DigitCount = 0;
                    CurrentState = STATE_UNLOCKED;
                } else {
                    ClearProgressLEDs();
                    DigitCount = 0;
                    if (FailedAttempts < 10) FailedAttempts++;
                    SevSeg_Display(FailedAttempts);
                    if (FailedAttempts >= LOCKOUT_THRESHOLD) {
                        LED_On(LED_ALARM);
                        CurrentState = STATE_ALARM;
                    }
                }
            }
        }
    }
    else if (CurrentState == STATE_UNLOCKED) {
        if (key == '*') {
            LED_Off(LED_SUCCESS);
            DigitCount = 0;
            CurrentState = STATE_LOCKED;
        }
        /* All other keys ignored per spec */
    }
    else if (CurrentState == STATE_ALARM) {
        /* All keypad input ignored — only ISR exits this state */
        (void)key;
    }

    EXIT_CRITICAL();
}