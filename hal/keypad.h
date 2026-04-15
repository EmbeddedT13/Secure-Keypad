#ifndef KEYPAD_H
#define KEYPAD_H


#include "../core/BIT_MATH.h"
#include "../core/STD_TYPES.h"
#include "../mcal/gpio.h"

#define KEYPAD_PORT      GPIOB /*The port which the keypad is connected to*/

/* Row Pins */
#define KEYPAD_ROW_A     GPIO_PIN00
#define KEYPAD_ROW_B     GPIO_PIN01
#define KEYPAD_ROW_C     GPIO_PIN02
#define KEYPAD_ROW_D     GPIO_PIN03

/* Column Pins */
#define KEYPAD_COL_1  GPIO_PIN04
#define KEYPAD_COL_2  GPIO_PIN05
#define KEYPAD_COL_3  GPIO_PIN06
#define KEYPAD_COL_4  GPIO_PIN07

/* Standard return value when the user is not pressing any buttons */
#define KEYPAD_NO_KEY_PRESSED 0xFF


void Keypad_Init(void);
uint8 Keypad_GetPressedKey(void);

#endif