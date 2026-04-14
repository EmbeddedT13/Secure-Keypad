#ifndef EXTI_H
#define EXTI_H

#include "../core/BIT_MATH.h"
#include "../core/STD_TYPES.h"


/*External Interrupt Struct*/
typedef struct{
    volatile uint32 IMR;    /*Address offset: 0x00*/
    volatile uint32 EMR;    /*Address offset: 0x04*/
    volatile uint32 RTSR;   /*Address offset: 0x08*/
    volatile uint32 FTSR;   /*Address offset: 0x0C*/
    volatile uint32 SWIER;  /*Address offset: 0x10*/
    volatile uint32 PR;     /*Address offset: 0x14*/


}exti;

/*System Configuration Struct*/
typedef struct{
    volatile uint32 MEMRMP;    /*Address offset: 0x00*/
    volatile uint32 PMC;       /*Address offset: 0x04*/
    volatile uint32 EXTICR[4]; /*Address offset: 0x08 to 0x14*/
} SYSCFG_t;

/*Base Addresses*/
#define EXTI_base 0x40013C00
#define SYSCFG_base 0x40013800

/*Pointers*/
#define EXTI ((exti *) EXTI_base)
#define SYSCFG ((SYSCFG_t *) SYSCFG_base)

/*API Definitions*/

/*Lines*/
#define EXTI_LINE_0  0
#define EXTI_LINE_1  1
#define EXTI_LINE_2  2
#define EXTI_LINE_3  3
#define EXTI_LINE_4  4
#define EXTI_LINE_5  5
#define EXTI_LINE_6  6
#define EXTI_LINE_7  7
#define EXTI_LINE_8  8
#define EXTI_LINE_9  9
#define EXTI_LINE_10 10
#define EXTI_LINE_11 11
#define EXTI_LINE_12 12
#define EXTI_LINE_13 13
#define EXTI_LINE_14 14
#define EXTI_LINE_15 15


/*Triggers*/
#define EXTI_TR_RISING 0x01
#define EXTI_TR_FAILING 0x02
#define EXTI_TR_FAILING 0x03

/*Ports*/
#define EXTI_PORTA  0  /* Binary: 0000 */
#define EXTI_PORTB  1  /* Binary: 0001 */
#define EXTI_PORTC  2  /* Binary: 0010 */
#define EXTI_PORTD  3  /* Binary: 0011 */
#define EXTI_PORTE  4  /* Binary: 0100 */
#define EXTI_PORTH  7  /* Binary: 0111 */

#endif