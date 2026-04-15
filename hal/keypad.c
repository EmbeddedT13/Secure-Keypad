#include "keypad.h"

void Keypad_Init(void){

    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_ROW_A, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_ROW_B, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_ROW_C, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_ROW_D, GPIO_MODE_OUTPUT);

    GPIO_WritePin(KEYPAD_PORT, KEYPAD_ROW_A, GPIO_STATE_HIGH);
    GPIO_WritePin(KEYPAD_PORT, KEYPAD_ROW_B, GPIO_STATE_HIGH);
    GPIO_WritePin(KEYPAD_PORT, KEYPAD_ROW_C, GPIO_STATE_HIGH);
    GPIO_WritePin(KEYPAD_PORT, KEYPAD_ROW_D, GPIO_STATE_HIGH);

    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_COL_1, GPIO_MODE_INPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_COL_2, GPIO_MODE_INPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_COL_3, GPIO_MODE_INPUT);
    GPIO_SetPinMode(KEYPAD_PORT, KEYPAD_COL_4, GPIO_MODE_INPUT); 

    GPIO_SetPullType(KEYPAD_PORT,KEYPAD_COL_1, GPIO_PR_PU);
    GPIO_SetPullType(KEYPAD_PORT,KEYPAD_COL_2, GPIO_PR_PU);
    GPIO_SetPullType(KEYPAD_PORT,KEYPAD_COL_3, GPIO_PR_PU);
    GPIO_SetPullType(KEYPAD_PORT,KEYPAD_COL_4, GPIO_PR_PU);

}


uint8 Keypad_GetPressedKey(void){

    const uint8 Keypad_Buttons[4][4]= {{'7','8','9','/'}, {'4','5','6','*'}, {'1','2','3','-'}, {'C','0','=','+'}};

    uint8 RowIter = 0;
    uint8 ColIter = 0;
    uint8 PressedKey = KEYPAD_NO_KEY_PRESSED;

    for(RowIter = KEYPAD_ROW_A; RowIter < KEYPAD_ROW_A + 4; RowIter++){

        GPIO_WritePin(KEYPAD_PORT, RowIter, GPIO_STATE_LOW);

        for(ColIter = KEYPAD_COL_1; ColIter < KEYPAD_COL_1 + 4; ColIter++){

            if(GPIO_ReadPin(KEYPAD_PORT, ColIter) == GPIO_STATE_LOW){
                 PressedKey = Keypad_Buttons[RowIter][ColIter-4];
            }
        }
        
        GPIO_WritePin(KEYPAD_PORT, RowIter, GPIO_STATE_HIGH);
        if(PressedKey != KEYPAD_NO_KEY_PRESSED) return PressedKey;
    }

    return KEYPAD_NO_KEY_PRESSED;
}