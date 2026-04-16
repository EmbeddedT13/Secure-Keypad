#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include "../core/BIT_MATH.h"
#include "../core/STD_TYPES.h"
#include "../mcal/gpio.h"


#define SEV_SEG_1_PORT       GPIOA      /*The port which the seven segment is connected to*/
#define SEV_SEG_1_START_PIN  GPIO_PIN00 /*The starting pin for the seven segment*/

#define SEV_SEG_2_PORT       GPIOD      /*The port which the seven segment is connected to*/
#define SEV_SEG_2_START_PIN  GPIO_PIN00 /*The starting pin for the seven segment*/

/* Mask used for safely writing pins 0 to 6 simultaneously without corrupting the rest of the port */
#define SEV_SEG_MASK 0x7F /*mask used for writing pins A0 to A6 without corrupting other pins*/

void SevSeg_Init(void);
void SevSeg_Display(uint8 Number);
void SevSeg_Clear(void);


#endif