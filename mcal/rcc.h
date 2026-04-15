#ifndef RCC_H
#define RCC_H

#include "../core/BIT_MATH.h"
#include "../core/STD_TYPES.h"


#define RCC_BASE_ADDRESS 0x40023800

typedef struct 
{ 
    volatile uint32 RCC_CR;
    volatile uint32 RCC_PLLCFGR;
    volatile uint32 RCC_CFGR;
    volatile uint32 RCC_CIR;
    volatile uint32 RCC_AHB1RSTR;
    volatile uint32 RCC_AHB2RSTR;
    volatile uint32 Reserved1;
    volatile uint32 Reserved2;
    volatile uint32 RCC_APB1RSTR;
    volatile uint32 RCC_APB2RSTR;
    volatile uint32 Reserved3;
    volatile uint32 Reserved4;
    volatile uint32 RCC_AHB1ENR;
    volatile uint32 RCC_AHB2ENR;
    volatile uint32 Reserved5;
    volatile uint32 Reserved6;
    volatile uint32 RCC_APB1ENR;
    volatile uint32 RCC_APB2ENR;
}rcc;

#define RCC ((volatile rcc*) RCC_BASE_ADDRESS)

#define RCC_AHB1 0
#define RCC_APB1 1
#define RCC_APB2 2

#define GPIOA_AHB1_BIT  0
#define GPIOB_AHB1_BIT  1
#define GPIOC_AHB1_BIT  2
#define SYSCFG_APB2_BIT 14


void RCC_EnableClock(uint8 BusId, uint8 PeripheralBit);



#endif