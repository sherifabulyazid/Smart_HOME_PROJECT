/**********************************************************
*Author: Mahmoud Khaled Alnmr
*SWC:
*Layer:
*Version:
***********************************************************/
#ifndef ADC_REGISTERS_H_
#define ADC_REGISTERS_H_




#define ADMUX    (*(volatile uint8 *)0x27) 	/*ADC multiplexer selection register*/

#define REFS1	7    /*Reference selection bit1 */
#define REFS0	6	 /*Reference selection bit1 */
#define ADCLAR	5	/*ADC left adjust result*/
#define MUX4	4
#define MUX3	3
#define MUX2	2
#define MUX1	1
#define MUX0	0



#define ADCSRA   (*(volatile uint8 *)0x26)
#define ADEN	7
#define ADSC	6
#define ADATE	5
#define ADIF	4
#define ADIE	3
#define ADPS2	2
#define ADPS1	1
#define ADPS0	0

#define ADCL     (*(volatile uint8 *)0x24)
#define ADCH     (*(volatile uint8 *)0x25)

#define SFIOR				*((volatile uint8*)0x50)	/*special function IO register		 */
#define SFIOR_ADTS_2		7
#define SFIOR_ADTS_1		6
#define SFIOR_ADTS_0		5
#endif /* ADC_REGISTERS_H_ */
