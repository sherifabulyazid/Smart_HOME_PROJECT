/*
 * IIC_Registers.h
 *
 *  Created on: Jan 21, 2024
 *      Author: Sherif
 */

#ifndef IIC_REGISTERS_H_
#define IIC_REGISTERS_H_

#define TWBR		*((volatile uint8*)0x20)       /*TWI Bit Rate Register*/

#define TWCR		*((volatile uint8*)0x56)		/*TWI Control Register*/
#define TWCR_TWINT	7							/*Interrupt Flag*/
#define TWCR_TWEA	6                           /*Enable Acknowledge*/
#define TWCR_TWSTA	5                           /*Start Condition*/
#define TWCR_TWSTO	4                           /*Stop Condition*/
#define TWCR_TWWC	3                           /*Write Collision*/
#define TWCR_TWEN	2                           /*TWI Enable*/
#define TWCR_TWIE	0                           /*Interrupt Enable*/

#define TWDR 		*((volatile uint8*)0x23)       /*TWI Data Register*/

#define	TWAR		*((volatile uint8*)0x22)       /*TWI Address Register*/
#define TWAR_TWGCE	0							  /*General Call recognition Enable*/

#define TWSR		*((volatile uint8*)0x21)       /*TWI Status Register*/
#define TWSR_TWPS0	0							/*Prescaler Bit0*/
#define TWSR_TWPS1	1							/*Prescaler Bit1*/




#endif /* IIC_REGISTERS_H_ */
