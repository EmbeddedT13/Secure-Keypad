#include "keypad.h"

void Keypad_Init(void){

    /* Configure Row pins as outputs */
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_ROW_A, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_ROW_B, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_ROW_C, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_ROW_D, GPIO_MODE_OUTPUT);

    /* Drive all Rows HIGH by default to prevent false triggers */
    GPIO_WritePin(KEYPAD_PORT, KEYPAD_ROW_A, GPIO_STATE_HIGH);
    GPIO_WritePin(KEYPAD_PORT, KEYPAD_ROW_B, GPIO_STATE_HIGH);
    GPIO_WritePin(KEYPAD_PORT, KEYPAD_ROW_C, GPIO_STATE_HIGH);
    GPIO_WritePin(KEYPAD_PORT, KEYPAD_ROW_D, GPIO_STATE_HIGH);

    /* Configure Column pins as inputs */
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_COL_1, GPIO_MODE_INPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_COL_2, GPIO_MODE_INPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_COL_3, GPIO_MODE_INPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_COL_4, GPIO_MODE_INPUT); 

    /* Enable Internal Pull-Up resistors so Columns read HIGH when idle */
    GPIO_SetPullType(KEYPAD_PORT,KEYPAD_COL_1, GPIO_PR_PU);
    GPIO_SetPullType(KEYPAD_PORT,KEYPAD_COL_2, GPIO_PR_PU);
    GPIO_SetPullType(KEYPAD_PORT,KEYPAD_COL_3, GPIO_PR_PU);
    GPIO_SetPullType(KEYPAD_PORT,KEYPAD_COL_4, GPIO_PR_PU);

}


uint8 Keypad_GetPressedKey(void){

    /* Physical layout map of the 4x4 keypad matrix */
    const uint8 Keypad_Buttons[4][4]= {
        {'7','8','9','/'}, 
        {'4','5','6','*'}, 
        {'1','2','3','-'}, 
        {'C','0','=','+'}
    };

    uint8 RowIter = 0;
    uint8 ColIter = 0;
    uint8 PressedKey = KEYPAD_NO_KEY_PRESSED;

    /* Walking Zero Algorithm: Loop through each row */
    for(RowIter = KEYPAD_ROW_A; RowIter < KEYPAD_ROW_A + 4; RowIter++){

        /* Drive the current row LOW to test it */
        GPIO_WritePin(KEYPAD_PORT, RowIter, GPIO_STATE_LOW);

        /* Read all 4 columns for this specific row */
        for(ColIter = KEYPAD_COL_1; ColIter < KEYPAD_COL_1 + 4; ColIter++){

            /* If a column reads LOW, the button at this row/col intersection is pressed */
            if(GPIO_ReadPin(KEYPAD_PORT, ColIter) == GPIO_STATE_LOW){
                 PressedKey = Keypad_Buttons[RowIter][ColIter-4];
            }
        }
        
        /* Reset the current row back to HIGH before moving to the next one */
        GPIO_WritePin(KEYPAD_PORT, RowIter, GPIO_STATE_HIGH);
        /* If a key was found, immediately return it to the application layer */
        if(PressedKey != KEYPAD_NO_KEY_PRESSED) return PressedKey;
    }

    return KEYPAD_NO_KEY_PRESSED;
}