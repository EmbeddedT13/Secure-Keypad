#include "seven_segment.h"

/*Lookup table that maps to 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 in the seven segment disply*/
const uint8 Lookup_Table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};


void SevSeg_Init(void){

    /* Initialize the 7 continuous pins as outputs */
    for (uint8 pin = 0; pin < 7; pin++)
    {
        GPIO_SetPinMode(SEV_SEG_1_PORT, SEV_SEG_1_START_PIN + pin, GPIO_MODE_OUTPUT);
        GPIO_SetPinMode(SEV_SEG_2_PORT, SEV_SEG_2_START_PIN + pin, GPIO_MODE_OUTPUT);
    }
    
}

void SevSeg_Display(uint8 Number){

    /* Boundary check to prevent reading memory outside the lookup table */
    if(Number >= 0 && Number <= 9){

        /* Shift the mask and the lookup value to align with the starting hardware pin */
        uint16 Mask1 = SEV_SEG_MASK << SEV_SEG_1_START_PIN;
        uint16 Value1 = Lookup_Table[Number] << SEV_SEG_1_START_PIN;

        /* Write the entire digit to the port in a single cycle */
        GPIO_WriteGroup(SEV_SEG_1_PORT, Mask1, Value1);

        /* Turn OFF the Tens digit to remove leading zeros/sticky numbers */
        uint16 Mask2 = SEV_SEG_MASK << SEV_SEG_2_START_PIN;
        uint16 Value2 = Lookup_Table[0] << SEV_SEG_2_START_PIN;
        GPIO_WriteGroup(SEV_SEG_2_PORT, Mask2, Value2);

    }

    if(Number > 9){

        uint8 num1 = Number % 10;
        uint8 num2 = Number / 10;

        /* Shift the mask and the lookup value to align with the starting hardware pin */
        uint16 Mask1 = SEV_SEG_MASK << SEV_SEG_1_START_PIN;
        uint16 Mask2 = SEV_SEG_MASK << SEV_SEG_2_START_PIN;
        uint16 Value1 = Lookup_Table[num1] << SEV_SEG_1_START_PIN;
        uint16 Value2 = Lookup_Table[num2] << SEV_SEG_2_START_PIN;


        /* Write the entire digit to the port in a single cycle */
        GPIO_WriteGroup(SEV_SEG_1_PORT, Mask1, Value1);
        GPIO_WriteGroup(SEV_SEG_2_PORT, Mask2, Value2);

    }
}

void SevSeg_Clear(void){

    /* Turn off all 7 segments by writing 0x00, masking out the rest of the port */
    uint16 Mask1 = SEV_SEG_MASK << SEV_SEG_1_START_PIN;
    uint16 Mask2 = SEV_SEG_MASK << SEV_SEG_2_START_PIN;

    GPIO_WriteGroup(SEV_SEG_1_PORT, Mask1, 0x00);
    GPIO_WriteGroup(SEV_SEG_2_PORT, Mask2, 0x00);
}
