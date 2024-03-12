/*
 * Keypad_Program.c
 *
 *  Created on: Dec 16, 2023
 *      Author: hp
 */
#include "STD.h"
#include "BIT_MATH.h"
#include "DIO_Register.h"
#include "DIO_Interface.h"
#include "LCD_Interface.h"
#include "Keypad_config.h"
#include "Keypad_Interface.h"

void Keypad_Init(void)
{
	/*
		DIO_SetPortDirection(DIO_PORTC, KEYPAD_Direction); // Keypad
		DIO_SetPortValue(DIO_PORTC,KEYPAD_Value); //Keypad
	*/

		DIO_setInternalPullUp(ROWS_PORT,ROW1);
		DIO_setInternalPullUp(ROWS_PORT,ROW2);
		DIO_setInternalPullUp(ROWS_PORT,ROW3);
		DIO_setInternalPullUp(ROWS_PORT,ROW4);

		DIO_setPinDirection(COLUMNS_PORT,COL1,OUTPUT);
		DIO_setPinDirection(COLUMNS_PORT,COL2,OUTPUT);
		DIO_setPinDirection(COLUMNS_PORT,COL3,OUTPUT);
		DIO_setPinDirection(COLUMNS_PORT,COL4,OUTPUT);

}
uint8 get_button_pressed(void)
{
	uint8 row=0,column=0;
	uint8 buttonStatus=0xff;
	uint8 buttonPressed = 0xff;
	uint8 buttonArray[ROW_NUM][COL_NUM]={{'7','8','9','/'},
										 {'4','5','6','*'},
										 {'1','2','3','-'},
										 {'C','0','=','+'}};

	for(column=COL1;column<=COL4;column++)
	{
		DIO_setPinVAlue(COLUMNS_PORT,column,OUTPUT_LOW);
		for(row=ROW1;row<=ROW4;row++)
		{
			DIO_GetPinValue(ROWS_PORT,row,&buttonStatus);
			if(buttonStatus==LOW)
			{
				while(buttonStatus==LOW)
				{
					DIO_GetPinValue(ROWS_PORT,row,&buttonStatus);
				}
				DIO_setPinVAlue(COLUMNS_PORT,column,OUTPUT_HIGH);
				buttonPressed= buttonArray[row-ROW1][column-COL1];
				return buttonPressed;
			}
		}
		DIO_setPinVAlue(COLUMNS_PORT,column,OUTPUT_HIGH);
	}

	return buttonPressed;
}

