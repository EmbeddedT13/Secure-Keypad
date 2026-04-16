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
void EXTIGPIO_PIN09_5_IRQHandler(void);

int main(void) {
    /*SYSTEM CLOCKS*/
    RCC_EnableClock(RCC_AHB1, 0); /* PORTA */
    RCC_EnableClock(RCC_AHB1, 1); /* PORTB */
    RCC_EnableClock(RCC_AHB1, 2); /* PORTC */
    RCC_EnableClock(RCC_AHB1, 3); /* PORTD */

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
    /* Activate internal pull-ups for GPIO_PIN08 and GPIO_PIN09 */
    GPIO_SetPullType(GPIOA, GPIO_PIN08, GPIO_PR_PU);
    GPIO_SetPullType(GPIOA, GPIO_PIN09, GPIO_PR_PU);

    /* Route and Enable Line GPIO_PIN08 (Emergency Reset) */
    EXTI_Init(EXTI_PORTA, GPIO_PIN08, EXTI_TR_FALLING);
    EXTI_EnableInterrupt(GPIO_PIN08);

    /* Route and Enable Line GPIO_PIN09 (Doorbell) -> Toggle on Rising and Falling Edge */
    EXTI_Init(EXTI_PORTA, GPIO_PIN09, EXTI_TR_RISING_FALLING);
    EXTI_EnableInterrupt(GPIO_PIN09);

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
    
    if (READ_BIT(EXTI->PR, GPIO_PIN08)) {
        StateMachine_EmergencyReset();
        EXTI_ClearPendingFlag(GPIO_PIN08); 
    }

    if (READ_BIT(EXTI->PR, GPIO_PIN09)) {
        StateMachine_DoorbellTrigger();
        EXTI_ClearPendingFlag(GPIO_PIN09); 
    }
}