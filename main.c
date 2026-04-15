#include "core/STD_TYPES.h"
#include "core/BIT_MATH.h"
#include "mcal/rcc.h"
#include "mcal/gpio.h"
#include "mcal/exti.h"
#include "mcal/nvic.h"
#include "hal/keypad.h"
#include "hal/seven_segment.h"
#include "hal/led.h"
#include "app/state_machine.h"

/* Forward declare the ISR so the compiler maps it correctly */
void EXTI9_5_IRQHandler(void);

int main(void) {
    /*SYSTEM CLOCKS*/
    RCC_EnableClock(RCC_AHB1, 0); /* PORTA */
    RCC_EnableClock(RCC_AHB1, 1); /* PORTB */
    RCC_EnableClock(RCC_AHB1, 2); /* PORTC */

    /* Enable APB2 Clock for the SYSCFG Multiplexer */
    RCC_EnableClock(RCC_APB2, 14);
    
    /*INITIALIZE HAL*/
    SevSeg_Init();
    Keypad_Init();
    
    LED_Init(LED_PROG_1);
    LED_Init(LED_PROG_2);
    LED_Init(LED_PROG_3);
    LED_Init(LED_PROG_4);
    LED_Init(LED_SUCCESS);
    LED_Init(LED_ALARM);
    LED_Init(LED_DOORBELL);

    /*INITIALIZE EXTI*/
    /* Activate internal pull-ups for PA8 and PA9 */
    GPIO_SetPullType(GPIOA, 8, GPIO_PR_PU);
    GPIO_SetPullType(GPIOA, 9, GPIO_PR_PU);

    /* Route and Enable Line 8 (Emergency Reset) */
    EXTI_Init(EXTI_PORTA, 8, EXTI_TR_FALLING);
    EXTI_EnableInterrupt(8);

    /* Route and Enable Line 9 (Doorbell) -> Toggle on Falling Edge */
    EXTI_Init(EXTI_PORTA, 9, EXTI_TR_FALLING);
    EXTI_EnableInterrupt(9);

    /*INITIALIZE NVIC*/
    NVIC_EnableInterrupt(23);

    /*RUN APPLICATION*/
    StateMachine_Init();

    while (1) {
        StateMachine_Update();
    }

    return 0;
}

/*INTERRUPT SERVICE ROUTINE*/
void EXTI9_5_IRQHandler(void) {
    
    if (READ_BIT(EXTI->PR, 8)) {
        StateMachine_EmergencyReset();
        EXTI_ClearPendingFlag(8); 
    }

    if (READ_BIT(EXTI->PR, 9)) {
        StateMachine_DoorbellTrigger();
        EXTI_ClearPendingFlag(9); 
    }
}