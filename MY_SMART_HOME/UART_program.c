/*
 * UART_Program.c
 *
 *  Created on: 5 Jan 2024
 *      Author: Sherif
 */
#include "STD.h"
#include "BIT_MATH.h"
#include "UART_Register.h"
#include "UART_Config.h"
#include "UART_Interface.h"

void UART_Init(void)
{
	 uint8 ucsrc=0u;
	CLEARBIT(ucsrc,UMSEL);//ASYNCRONAS

	CLEARBIT(ucsrc,UPM1); /*PARTIY DISABLED*/
	CLEARBIT(ucsrc,UPM0);

	SETBIT(ucsrc,UCSZ0);/*8 BIT MODE*/
	SETBIT(ucsrc,UCSZ1);
	CLEARBIT(UCSRB,UCSZ2);

	SETBIT(UCSRB,TXEN);/* TRANSMIT ENABLE*/
	SETBIT(UCSRB,RXEN); /*RECIVE ENABLE*/

	SETBIT(ucsrc,URSEL);

	UCSRC = ucsrc;

	UBRRL=103;// Baud rate 9600
}

void UART_Transmit(uint8 Copy_Data)
{

	while((GETBIT(UCSRA,UDRE)) == 0);

	UDR = Copy_Data;
}
uint8 UART_Receive(void)
{

	while((GETBIT(UCSRA,RXC)) == 0);

	return UDR;
}

void UART_sendString(uint8 *copy_UART_ptr)
{
	if (copy_UART_ptr != NULL)
	{
		while (*copy_UART_ptr != '\0')
		{
			UART_Transmit(*copy_UART_ptr);
			copy_UART_ptr++;
		}
	}
	else
	{
		/* Do Nothing */
	}
}

void  UART_recieve_string(uint8 * copy_UART_ptr)
{
	uint8 i=0;
	uint8* local_ptr=copy_UART_ptr;
	if(local_ptr != NULL)
	{
		local_ptr[i]=UART_Receive();
		while(local_ptr[i] != 0x0D) // Enter key =0x0D in Ascii
		{
			i++;
			local_ptr[i]=UART_Receive();
		}
	}
	else
	{
		/* Do Nothing*/
	}

}
