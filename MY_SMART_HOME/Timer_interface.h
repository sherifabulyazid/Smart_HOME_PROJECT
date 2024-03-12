/*
 * Timer0_interface.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Sherif Abuelyazid
 */

#ifndef TIMER_INTERFACE_H_
#define TIMER_INTERFACE_H_

/******************************************TIMER0****************************************************/
void Timer0_CTC(void);
uint8 Timer0_CTC_CallBack( void (*Copy_Timer0_CTC_CallBackFun)(void) , uint32 copy_time_ms);
void Timer0_Normal(void);
uint8 Timer0_Normal_CallBack(void (*Copy_Timer0_Normal_CallBackFun)(void) , uint32 copy_time_ms);
void Timer0_PWM_Fast(void);
void Timer0_PWM_Phase_Correct(void);
void Delay_mS(uint32 copy_dtime_ms);
/******************************************TIMER1****************************************************/
void Timer1_Servo(uint8 copy_Angle);
uint8 ADC_To_Angle(uint8 copy_ADC_val);
void Timer1_Normal(void);
uint8 Timer1_Normal_CallBack(void (*Copy_Timer0_Normal_CallBackFun)(void) , uint32 copy_time_ms);
void Timer1_CTC(void);
uint8 Timer1_CTC_CallBack( void (*Copy_Timer0_CTC_CallBackFun)(void) , uint32 copy_time_ms);
void Timer1_PWM_Fast_8_bit(void);
void Timer1_PWM_Fast_9_bit(void);
void Timer1_PWM_Fast_10_bit(void);
void Timer1_PWM_Phase_Correct_8_bit(void);
void Timer1_PWM_Phase_Correct_9_bit(void);
void Timer1_PWM_Phase_Correct_10_bit(void);
void Timer1_PWM_Fast_ICR1(uint16 copy_Req_Freq);
void Timer1_PWM_Phase_Correct_ICR1(uint16 copy_Req_Freq);
/******************************************TIMER1****************************************************/
void Timer1_Servo(uint8 copy_Angle);
uint8 ADC_To_Angle(uint8 copy_ADC_val);


#endif /* TIMER_INTERFACE_H_*/
