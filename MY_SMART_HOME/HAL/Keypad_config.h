/*
 * Keypad_config.h
 *
 *  Created on: Dec 16, 2023
 *      Author: hp
 */

#ifndef KEYPAD_CONFIG_H_
#define KEYPAD_CONFIG_H_

#define ROW_NUM 4
#define COL_NUM 4
//#define KEYPAD_PORT DIO_PORTC
//#define KEYPAD_Direction 0xf0 //Lower nibble of port is input , Higher nibble is output
//#define KEYPAD_Value 0xff     // set pull up resistance for input pins , set output pins as high

#define ROWS_PORT DIO_PORTC
#define COLUMNS_PORT DIO_PORTB
#define ROW1 PIN2
#define ROW2 PIN3
#define ROW3 PIN4
#define ROW4 PIN5
#define COL1 PIN0
#define COL2 PIN1
#define COL3 PIN2
#define COL4 PIN3
#endif /* KEYPAD_CONFIG_H_ */
