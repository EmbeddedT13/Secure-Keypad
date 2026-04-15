#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include "../core/BIT_MATH.h"
#include "../core/STD_TYPES.h"
#include "../mcal/gpio.h"


#define SEV_SEG_PORT       GPIOA
#define SEV_SEG_START_PIN  GPIO_PIN00

#define SEV_SEG_MASK 0x7F /*mask used for writing pins A0 to A6 without corrupting other pins*/

void SevSeg_Init(void);
void SevSeg_Display(uint8 Number);
void SevSeg_Clear(void);


#endif