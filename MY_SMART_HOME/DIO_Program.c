/*
 * DIO_Program.c
 *
 *  Created on: Dec 8, 2023
 *      Author: hp
 */
#include "STD.h"
#include "BIT_MATH.h"

#include "DIO_Config.h"
#include "DIO_Interface.h"
#include "DIO_Private.h"
#include "DIO_Register.h"

uint8 DIO_setPinDirection(uint8  Copy_Port ,  uint8 Copy_Pin ,  uint8 Copy_Direction)
{
	uint8 local_ErrorStatus=0;
	if (Copy_Pin <= 7)
	{
		if(Copy_Direction == OUTPUT)
		{
			switch(Copy_Port)
			{
				case DIO_PORTA:
					SETBIT(DDRA,Copy_Pin);
					break;
				case DIO_PORTB:
					SETBIT(DDRB,Copy_Pin);
					break;
				case DIO_PORTC:
					SETBIT(DDRC,Copy_Pin);
						break;
				case DIO_PORTD:
					SETBIT(DDRD,Copy_Pin);
					break;
				default:
					local_ErrorStatus=1;
					break;
			}
		}
		else if (Copy_Direction == INPUT)
		{
			switch(Copy_Port)
			{
				case DIO_PORTA:
					CLEARBIT(DDRA,Copy_Pin);
					break;
				case DIO_PORTB:
					CLEARBIT(DDRB,Copy_Pin);
					break;
				case DIO_PORTC:
					CLEARBIT(DDRC,Copy_Pin);
						break;
				case DIO_PORTD:
					CLEARBIT(DDRD,Copy_Pin);
					break;
				default:
					local_ErrorStatus=1;
					break;
			}
		}

		else
		{
			local_ErrorStatus=1;
		}
	}
	return local_ErrorStatus;
}

uint8 DIO_setPinVAlue(uint8  Copy_Port ,  uint8 Copy_Pin ,  uint8 Copy_Status)
{
	uint8 local_ErrorStatus=0;
	if (Copy_Pin <= 7)
	{
		if(Copy_Status == OUTPUT_HIGH)
		{
			switch(Copy_Port)
			{
				case DIO_PORTA:
					SETBIT(PORTA,Copy_Pin);
					break;
				case DIO_PORTB:
					SETBIT(PORTB,Copy_Pin);
					break;
				case DIO_PORTC:
					SETBIT(PORTC,Copy_Pin);
						break;
				case DIO_PORTD:
					SETBIT(PORTD,Copy_Pin);
					break;
				default:
					local_ErrorStatus=1;
					break;
			}
		}
		else if (Copy_Status == OUTPUT_LOW)
		{
			switch(Copy_Port)
			{
				case DIO_PORTA:
					CLEARBIT(PORTA,Copy_Pin);
					break;
				case DIO_PORTB:
					CLEARBIT(PORTB,Copy_Pin);
					break;
				case DIO_PORTC:
					CLEARBIT(PORTC,Copy_Pin);
						break;
				case DIO_PORTD:
					CLEARBIT(PORTD,Copy_Pin);
					break;
				default:
					local_ErrorStatus=1;
					break;
			}
		}

		else
		{
			local_ErrorStatus=1;
		}
	}
	return local_ErrorStatus;
}

uint8 DIO_SetPortDirection (uint8 Copy_Port, uint8 Copy_Direction )
{
	uint8 local_ErrorStatus=0;
		switch(Copy_Port)
					{
						case DIO_PORTA:
							DDRA= Copy_Direction;
							break;
						case DIO_PORTB:
							DDRB=Copy_Direction;
							break;
						case DIO_PORTC:
							DDRC=Copy_Direction;
								break;
						case DIO_PORTD:
							DDRD=Copy_Direction;
							break;
						default:
							local_ErrorStatus=1;
							break;
					}

		return local_ErrorStatus;
}


uint8 DIO_SetPortValue  (uint8 Copy_Port ,   uint8 Copy_Value )

{
	uint8 local_ErrorStatus=0;
		switch(Copy_Port)
					{
						case DIO_PORTA:
							PORTA=Copy_Value;
							break;
						case DIO_PORTB:
							PORTB=Copy_Value;
							break;
						case DIO_PORTC:
							PORTC=Copy_Value;
								break;
						case DIO_PORTD:
							PORTD=Copy_Value;
							break;
						default:
							local_ErrorStatus=1;
							break;
					}


	return local_ErrorStatus;
}

uint8 DIO_GetPinValue (uint8 Copy_uint8_Port , uint8 Copy_uint8_Pin , uint8* Copy_uint8_Value)
{
	uint8 local_ErrorStatus=0;
	if(Copy_uint8_Value!=NULL &&Copy_uint8_Pin <=7 )
	{

		switch(Copy_uint8_Port)
		{
			case DIO_PORTA:
				*Copy_uint8_Value = GETBIT(PINA , Copy_uint8_Pin);
				break;
			case DIO_PORTB:
				*Copy_uint8_Value = GETBIT(PINB , Copy_uint8_Pin);
				break;
			case DIO_PORTC:
				*Copy_uint8_Value = GETBIT(PINC , Copy_uint8_Pin);
				break;
			case DIO_PORTD:
				*Copy_uint8_Value = GETBIT(PIND , Copy_uint8_Pin);
				break;
			default:
				local_ErrorStatus=1;
				break;
		}
	}
	else
	{
		local_ErrorStatus=1;
	}
	return local_ErrorStatus;
}

uint8 DIO_setInternalPullUp(uint8  Copy_Port ,  uint8 Copy_Pin)
{
	uint8 local_ErrorStatus=0;
	if (Copy_Pin <= 7)
	{
			switch(Copy_Port)
			{
				case DIO_PORTA:
					CLEARBIT(DDRA, Copy_Pin);
					SETBIT(PORTA, Copy_Pin);
					break;
				case DIO_PORTB:
					CLEARBIT(DDRB,Copy_Pin);
					SETBIT(PORTB, Copy_Pin);
					break;
				case DIO_PORTC:
					CLEARBIT(DDRC,Copy_Pin);
					SETBIT(PORTC, Copy_Pin);
						break;
				case DIO_PORTD:
					CLEARBIT(DDRD,Copy_Pin);
					SETBIT(PORTD, Copy_Pin);
					break;
				default:
					local_ErrorStatus=1;
					break;
			}
	}

	return local_ErrorStatus;
}

