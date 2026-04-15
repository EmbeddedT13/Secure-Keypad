#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include "../core/BIT_MATH.h"
#include "../core/STD_TYPES.h"


#define SEV_SEG_PORT  GPIOA
#define SEV_SEG_START_PIN GPIO_PIN00

void SevSeg_Init(void);
void SevSeg_Display(uint8 Number);
void SevSeg_Clear(void);


#endif