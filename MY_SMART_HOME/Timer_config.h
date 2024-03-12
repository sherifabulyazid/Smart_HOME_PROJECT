/*
 * Timer0_config.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Sherif Abuelyazid
 */

#ifndef TIMER_CONFIG_H_
#define TIMER_CONFIG_H_

/*Clock Select*/
#define NO_CLOCK_SOURCE 		0X00
#define CLOCK_DIV_BY_1 			0X01
#define CLOCK_DIV_BY_8 			0X02
#define CLOCK_DIV_BY_64 		0X03
#define CLOCK_DIV_BY_256        0X04
#define CLOCK_DIV_BY_1024       0X05
#define EXTERNAL_CLOCK_FALLING  0X06
#define EXTERNAL_CLOCK_RISING   0X07
#define CLOCKSELECT		 CLOCK_DIV_BY_8  // <<<<<---------

/**********************************************Timer0**************************************************/

/* Compare Output Mode, non-PWM Mode
   COM01 COM00
 */
#define OC0_disconnected 0x00
#define OC0_Toggle 		0x10
#define OC0_Clear  		0x20
#define OC0_Set 		0x30
#define OC0_MODE	 OC0_Toggle  // <<<<<---------

/* Compare Output Mode, PWM Mode
   COM01 COM00
 */
#define OC0_disconnected    0x00
#define non_inverting  		0x20
#define inverting 		    0x30
#define OC0_PWM_MODE	 non_inverting    // <<<<<---------


/*compare match value*/
#define TIMER0_COMPARE_MATCH_VALUE		100   // <<<<<---------

/******************************************************************************************************/
/**********************************************Timer1**************************************************/
/******************************************************************************************************/

/* Compare Output Mode, non-PWM Mode
   COM01 COM00
 */

#define OC1A_disconnected 	0x00
#define OC1A_Toggle 		0x40
#define OC1A_Clear  		0x80
#define OC1A_Set 			0xc0
#define OC1A_MODE	 OC1A_disconnected  // <<<<<---------

#define OC1B_disconnected 	0x00
#define OC1B_Toggle 		0x10
#define OC1B_Clear  		0x20
#define OC1B_Set 			0x30
#define OC1B_MODE	 OC1B_Toggle  // <<<<<---------

#define OC1A_PWM_disconnected 	0x00
#define OC1A_PWM_NonInverting  	0x80
#define OC1A_PWM_Inverting 		0xc0
#define OC1A_PWM_MODE	 OC1A_PWM_NonInverting  // <<<<<---------

#define OC1B_PWM_disconnected 	0x00
#define OC1B_PWM_NonInverting  	0x20
#define OC1B_PWM_Inverting 		0x30
#define OC1B_PWM_MODE	 OC1B_PWM_NonInverting  // <<<<<---------
/*compare match value "OCR1A" */
#define TIMER1_COMPARE_MATCH_VALUE_A		1800   // <<<<<---------
#define TIMER1_COMPARE_MATCH_VALUE_B		1500   // <<<<<---------

#endif /* TIMER_CONFIG_H_*/
