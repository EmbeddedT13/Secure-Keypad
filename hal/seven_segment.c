#include "seven_segment.h"

/*Lookup table that maps to 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 in the seven segment disply*/
const uint8 Lookup_Table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};


void SevSeg_Init(void){

    for (uint8 pin = 0; pin < 7; pin++)
    {
        GPIO_SetPinMode(GPIOA, SEV_SEG_START_PIN + pin, GPIO_MODE_OUTPUT);
    }
    
}

void SevSeg_Display(uint8 Number){

    if(Number >= 0 && Number <= 9){

        uint16 Mask = SEV_SEG_MASK << SEV_SEG_START_PIN;
        uint16 Value = Lookup_Table[Number] << SEV_SEG_START_PIN;

        GPIO_WriteGroup(SEV_SEG_PORT, Mask, Value);

    }
}

void SevSeg_Clear(void){

    uint16 Mask = SEV_SEG_MASK << SEV_SEG_START_PIN;

    GPIO_WriteGroup(SEV_SEG_PORT, Mask, 0x00);
}
