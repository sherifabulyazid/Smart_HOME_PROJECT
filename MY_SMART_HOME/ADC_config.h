/**********************************************************
*Author: Mahmoud Khaled Alnmr
*SWC:
*Layer:
*Version:
***********************************************************/
#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

#define AREF	1
#define AVCC_WITH_CAP 2
#define INTERNAL_VREF 3

#define VOLTAGR_REF		AREF

#define ADC_uint32_TIMEOUT			50000
/*
 * ADC modes: 10bits resolution and 8 bits resolution
 * for 10 bits disable left adjust
 * for 8 bits enable left adjust
 */
#define ADC_RESOLUTION_8_BITS		1
#define ADC_RESOLUTION_10_BITS		2

#define ADC_RESOLUTION_MODE			ADC_RESOLUTION_8_BITS


/*
 * ADC conversion mode:
 * single conversion mode: clear ADATE bit in ADCSRA
 * free running mode set ADATE bit in ADCSRA
 */
#define ADC_SINGLE_CONVERSION_MODE		0
#define ADC_FREE_RUNNING_MODE			1

#define ADC_CONVERSION_MODE				ADC_SINGLE_CONVERSION_MODE

/*
 *  if free running is choosen, you need to choose the trigger source
 *  you need to set ADTS2, ADTS1, ADTS0 in SFIOR register
 */
#define FREE_RUNNIG_MODE				1
#define ANALOG_COMPARATOR_MODE			2
#define EXTERNAL_INTERRUPT_REQ_0		3
#define TIMER0_COMPARE_MATCH			4
#define TIMER0_OVERFLOW					5
#define TIMER1_COMPARE_MATCH_B			6
#define TIMER1_OVERFLOW					7
#define TIMER1_COMPARE_CAPTURE_MODE		8

#define ADC_AUTO_TRIGGER_SOURCE			FREE_RUNNING_MODE

/*
 * ADC INTERRUPT enable or disable
 * to enable set ADIE bit in ADCSRA REGISTER
 * to disable clear the bit(working with polling)
 */
#define ADC_INTERRUPT_OFF		1
#define ADC_INTERRUPT_ON		2

#define ADC_INTERRUPT_MODE		ADC_INTERRUPT_OFF

#define DIVISION_FACTOR_2			0
/*#define DIVISION_FACTOR_2			1*/
#define DIVISION_FACTOR_4			2
#define DIVISION_FACTOR_8			3
#define DIVISION_FACTOR_16			4
#define DIVISION_FACTOR_32			5
#define DIVISION_FACTOR_64			6
#define DIVISION_FACTOR_128			7

#define ADC_PRESCALAR 				DIVISION_FACTOR_128

#define ADMUX_CLEAR					0b11100000


#endif /* ADC_CONFIG_H_ */
