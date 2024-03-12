/*
 * EEPROM_Program.c
 *
 *  Created on: Jan 23, 2024
 *      Author: hp
 */
#include <util/delay.h>
#include "STD.h"
#include "BIT_MATH.h"
#include "IIC_Interface.h"
#include "EEPROM_Config.h"
#include "EEPROM_Interface.h"
#include "EEPROM_Private.h"
#include "IIC_Registers.h"

void EEPROM_INIT(void)
{
	TWI_InitMaster(0);
}
void EEPROM_voidSendDataByte(uint16 Copy_uint16LocationAddress, uint8 Copy_uint8DataByte)
{
	uint8 Local_uint8AddressPacket;

	Local_uint8AddressPacket = EEPROM_FIXED_ADDRESS | (A2_CONNECTION <<2) | (uint8)(Copy_uint16LocationAddress >>8);

	/*2k EEPROM*/
//  Local_uint8AddressPacket = EEPROM_FIXED_ADDRESS | A2_CONNECTION <<2 | A1_CONNECTION <<1 | A0_CONNECTION;

	/*Send start condition*/
	TWI_SendStartCondition();

	/*Send the address packet*/
	TWI_SendSlaveAddressWithWrite(Local_uint8AddressPacket);

	/*Send the rest 8bits of the location address*/
	TWI_MasterWriteDataByte((uint8)Copy_uint16LocationAddress);

	/*Send the data byte to the memory location*/
	TWI_MasterWriteDataByte(Copy_uint8DataByte);

	/*Send stop condition*/
	TWI_SendStopCondition();

	/*Delay until the write cycle is finished*/
	_delay_ms(10);
}

uint8 EEPROM_uint8ReadDataByte(uint16 Copy_uint16LocationAddress)
{
	uint8 Local_uint8AddressPacket, Local_uint8Data;

	Local_uint8AddressPacket = EEPROM_FIXED_ADDRESS | (A2_CONNECTION <<2) | (uint8)(Copy_uint16LocationAddress >>8);

	/*2k EEPROM*/
//	Local_uint8AddressPacket = EEPROM_FIXED_ADDRESS | A2_CONNECTION <<2 | A1_CONNECTION <<1 | A0_CONNECTION;

	/*Send start condition*/
	TWI_SendStartCondition();

	/*Send the address packet with write request*/
	TWI_SendSlaveAddressWithWrite(Local_uint8AddressPacket);

	/*Send the rest 8bits of the location address*/
	TWI_MasterWriteDataByte((uint8)Copy_uint16LocationAddress);

	/*Send repeated start to change write request into read request*/
	TWI_SendRepeatedStart();

	/*Send the address packet with read request*/
	TWI_SendSlaveAddressWithRead(Local_uint8AddressPacket);

	/*Get the data from memory*/
	TWI_MasterReadDataByte(&Local_uint8Data);

	/*send the stop condition*/
	TWI_SendStopCondition();

	return Local_uint8Data;
}

void EEPROM_voidSend4Numbers(uint16 Copy_uint16LocationAddress, uint8* Array_Recieve_Str,uint8 cop_EEPROM_maxNum)
{
	uint8 *local_ptrArr= Array_Recieve_Str;
	int i=0;
	for(i=0;i<cop_EEPROM_maxNum;i++)
	{
		EEPROM_voidSendDataByte(Copy_uint16LocationAddress,*local_ptrArr);
		Copy_uint16LocationAddress++;
		local_ptrArr += 1;
	}
}

void EEPROM_voidRead4Numbers(uint16 Copy_uint16LocationAddress, uint8* Array_Recieve_Str,uint8 cop_EEPROM_maxNum)
{
	uint8 *local_ptrArr= Array_Recieve_Str;
	int i=0;
	for(i=0;i<cop_EEPROM_maxNum;i++)
	{
		local_ptrArr[i] = EEPROM_uint8ReadDataByte(Copy_uint16LocationAddress);
		Copy_uint16LocationAddress++;
	}
}
void EEPROM_voidRemoveUser(uint16 Copy_uint16LocationAddress)
{
	/*this function deletes the user id and password*/
	int i=0;
	for(i=0;i<8;i++)
	{
		EEPROM_voidSendDataByte(Copy_uint16LocationAddress, 0xff);
		Copy_uint16LocationAddress++;
	}
}

void EEPROM_voidSendString(uint16 Copy_uint16LocationAddress, uint8* str)
{
	while(*str!= '\0')
	{
		EEPROM_voidSendDataByte(Copy_uint16LocationAddress++,*str);
		str +=1;
	}
}


