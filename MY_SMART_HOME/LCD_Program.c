/*
 * LCD_Program.c
 *
 *  Created on: Dec 13, 2023
 *      Author: Sherif
 */
#include <util/delay.h>
#include "STD.h"
#include "BIT_MATH.h"
#include "DIO_Interface.h"
#include "DIO_Register.h"
#include "LCD_Interface.h"

#define MODE_4BITS 1
#define MODE_8BITS 2

#define MODE MODE_4BITS // <-- Enter choice : ( MODE_4BITS or MODE_8BITS)

#if(MODE==MODE_8BITS)

void LCD_init(void)
{
	_delay_ms(20);
	LCD_cmd(0x38); //Function set
	_delay_ms(1);
	LCD_cmd(0x0c);//Display on/off control
	_delay_ms(1);
	LCD_cmd(0x01);// Display clear
	_delay_ms(1.53);
	LCD_cmd(0x06); //Entry mode set   06
	_delay_ms(1);
}


void LCD_cmd(uint8 cmd)
{
	LCD_DATA =cmd; //Data lines are set to send command - PORTB=LCD_DATA
	DIO_setPinVAlue(LCD_CONTROL,RS,OUTPUT_LOW); //RS =0 to send instruction code
	DIO_setPinVAlue(LCD_CONTROL,RW,OUTPUT_LOW); //RW =0 to write
	// to write data, EN needs negative edge
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_HIGH);
	_delay_ms(2);
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_LOW);
}

void LCD_Write_data(uint8 data)
{
	LCD_DATA =data; //data lines are set to send data - PORTB=LCD_DATA
	DIO_setPinVAlue(LCD_CONTROL,RS,OUTPUT_HIGH); //RS =1 to display data
	DIO_setPinVAlue(LCD_CONTROL,RW,OUTPUT_LOW); //RW =0 to write
	// to write data, EN needs negative edge
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_HIGH);
	_delay_ms(2);
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_LOW);
}

#elif(MODE==MODE_4BITS)
void LCD_init(void)
{
	_delay_ms(20);
	//LCD_cmd(0x33);
	LCD_cmd(0x32);
	LCD_cmd(0x28);//Function set
	LCD_cmd(0x0c);//Display on/off control
	LCD_cmd(0x06);//Entry mode set
	LCD_cmd(0x01);// Display clear
/*	_delay_ms(20);
	LCD_cmd(0x20); //Function set
	LCD_cmd(0x80); //Function set
	LCD_cmd(0x0e);//Display on/off control
	LCD_cmd(0x01);// Display clear
	_delay_ms(1.53);
	LCD_cmd(0x06); //Entry mode set
	_delay_ms(1);
*/
}

void LCD_cmd(uint8 cmd)
{
	LCD_DATA =(LCD_DATA & 0x0f)|(cmd & 0xf0); //Data lines are set to send command - PORTB=LCD_DATA
	DIO_setPinVAlue(LCD_CONTROL,RS,OUTPUT_LOW); //RS =0 to send instruction code
	DIO_setPinVAlue(LCD_CONTROL,RW,OUTPUT_LOW); //RW =0 to write
	// to write data, EN needs negative edge
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_HIGH);
	_delay_ms(1);
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_LOW);

	LCD_DATA =(LCD_DATA & 0x0f)|(cmd<<4); //Data lines are set to send command - PORTB=LCD_DATA
	DIO_setPinVAlue(LCD_CONTROL,RS,OUTPUT_LOW); //RS =0 to send instruction code
	DIO_setPinVAlue(LCD_CONTROL,RW,OUTPUT_LOW); //RW =0 to write
	// to write data, EN needs negative edge
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_HIGH);
	_delay_ms(1);
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_LOW);
}

void LCD_Write_data(uint8 data)
{
	LCD_DATA =(LCD_DATA & 0x0f)|(data & 0xf0); //Data lines are set to send command - PORTB=LCD_DATA
	DIO_setPinVAlue(LCD_CONTROL,RS,OUTPUT_HIGH); //RS =1 to send data
	DIO_setPinVAlue(LCD_CONTROL,RW,OUTPUT_LOW); //RW =0 to write
	// to write data, EN needs negative edge
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_HIGH);
	_delay_ms(1);
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_LOW);

	LCD_DATA =(LCD_DATA & 0x0f)|(data<<4); //Data lines are set to send command - PORTB=LCD_DATA
	DIO_setPinVAlue(LCD_CONTROL,RS,OUTPUT_HIGH); //RS =1 to send data
	DIO_setPinVAlue(LCD_CONTROL,RW,OUTPUT_LOW); //RW =0 to write
	// to write data, EN needs negative edge
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_HIGH);
	_delay_ms(1);
	DIO_setPinVAlue(LCD_CONTROL,EN,OUTPUT_LOW);
}
#endif

void LCD_GoToXY(uint8 copy_xPosition, uint8 copy_yPosition)
{
	if(copy_xPosition==FirstLine)
	{
		LCD_cmd(0x80 + copy_yPosition);
	}
	else if(copy_xPosition==SecondLine)
	{
		LCD_cmd(0xc0 + copy_yPosition);

	}
	else{}
}
void LCD_SendString(const char *copy_string)
{
	uint8 local_iterator=0;
		while(copy_string[local_iterator]!='\0')
		{
			if(local_iterator<16)
			{
				LCD_Write_data(copy_string[local_iterator]);
				local_iterator++;
			}
			else{
				LCD_GoToXY(SecondLine,local_iterator-16);
				LCD_Write_data(copy_string[local_iterator]);
				local_iterator++;
			}
		}
}
void LCD_ClearDisplay(void)
{
	LCD_cmd(0x01);// Display clear

}
void LCD_uint8_WriteNumber(uint8 copy_number)
{
	if(copy_number<10)
	{
		LCD_Write_data(copy_number+'0');
		_delay_ms(1000);
	}
	else if(copy_number<100)
	{
		LCD_Write_data(copy_number/10+'0');
		LCD_Write_data(copy_number%10+'0');
		_delay_ms(1000);
	}
	else if (copy_number<1000)
	{
		LCD_Write_data(copy_number/100+'0');
		LCD_Write_data((copy_number%100)/10+'0');
		LCD_Write_data(copy_number%10+'0');
		_delay_ms(1000);
	}
	else{}
}
