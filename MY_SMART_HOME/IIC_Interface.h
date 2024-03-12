/*
 * IIC_Interface.h
 *
 *  Created on: Jan 12, 2024
 *      Author: Sherif
 */

#ifndef IIC_INTERFACE_H_
#define IIC_INTERFACE_H_

typedef enum {
	NoError,
	StartConditionErr,
	RepeatedStartError,
	SlaveAddressWithWriteErr,
	SlaveAddressWithReadErr,
	MasterWriteByteErr,
	MasterReadByteErr,
}TWI_ErrStatus;

/*Set master address to 0 if master will not be addressed*/
void TWI_InitMaster(uint8 Copy_uint8Address);

void TWI_InitSlave(uint8 Copy_uint8Address);

TWI_ErrStatus TWI_SendStartCondition(void);

TWI_ErrStatus TWI_SendRepeatedStart(void);

TWI_ErrStatus TWI_SendSlaveAddressWithWrite(uint8 Copy_uint8SlaveAddress);

TWI_ErrStatus TWI_SendSlaveAddressWithRead(uint8 Copy_uint8SlaveAddress);

TWI_ErrStatus TWI_MasterWriteDataByte(uint8 Copy_uint8DataByte);

TWI_ErrStatus TWI_MasterReadDataByte(uint8* Copy_puint8DataByte);

void TWI_SendStopCondition(void);

void DisableAck(void);

void EnableAck(void);



#endif /* IIC_INTERFACE_H_ */
