#include "exti.h"


void EXTI_Init(uint8 Port, uint8 Line, uint8 Trigger){

    uint8 RegisterIndex = Line / 4;  /*Calculate which of the 4 registers you want to accsess*/
    uint8 StartBit = (Line % 4) * 4; /*Calculate the staring bit to start writing from*/

    WRITE_BIT_FIELD(SYSCFG->EXTICR[RegisterIndex], 0X0F, StartBit, Port); /*Write the Port number to teh intended register*/

    if(Trigger == EXTI_TR_RISING) SET_BIT(EXTI->RTSR, Line); /*Rising Edge*/

    else if(Trigger == EXTI_TR_FALLING) SET_BIT(EXTI->FTSR, Line); /* Falling Edge*/

    else if(Trigger == EXTI_TR_RISING_FALLING){ /*Both Rising and Falling edge*/
        SET_BIT(EXTI->RTSR, Line);
        SET_BIT(EXTI->FTSR, Line);
    }
}

void EXTI_EnableInterrupt(uint8 Line){

    SET_BIT(EXTI->IMR, Line);   /*Enable Interrupt for that line*/
}

void EXTI_DisableInterrupt(uint8 Line){

    CLEAR_BIT(EXTI->IMR, Line); /*Disable Interrupt for that line*/
}

void EXTI_ClearPendingFlag(uint8 Line){

    EXTI->PR = (1U << Line); /*Clear Pending flag by setting it*/
}