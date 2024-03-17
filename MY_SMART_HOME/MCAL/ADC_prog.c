/**********************************************************
*Author: Sherif
*SWC:
*Layer:
*Version:
***********************************************************/

#include "../LIBRARIES/STD.h"
#include "../LIBRARIES/BIT_MATH.h"

#include "ADC_registers.h"
#include "ADC_config.h"
#include "ADC_private.h"
#include "ADC_interface.h"


static uint8* ADC_puint8_Reading = NULL;
static void (*ADC_pv_CallBackNotification)(void) = NULL;
uint8 ADC_uint8_BusyState = ADC_NOTBUSY;
/*global variable to set the ADC Asynch source*/
static uint8 ADC_uint8_ADCISRSource = LOW;

void ADC_init(void)
{
	/*choosing the reference voltage*/
#if VOLTAGR_REF == AREF
	CLEARBIT(ADMUX, REFS1);
	CLEARBIT(ADMUX, REFS0);
#elif VOLTAGR_REF==AVCC_WITH_CAP
	CLEARBIT(ADMUX, REFS1);
	SETBIT(ADMUX, REFS0);
#elif VOLTAGR_REF==INTERNAL_VREF
	SETBIT(ADMUX, REFS1);
	SETBIT(ADMUX, REFS0);
#endif

#if  ADC_RESOLUTION_MODE==ADC_RESOLUTION_8_BITS
	/*setting the bits for Left adjust*/
	SETBIT(ADMUX, ADCLAR);
#elif ADC_RESOLUTION_MODE==ADC_RESOLUTION_10_BITS
		CLEARBIT(ADMUX, ADCLAR);
#endif
	
#if ADC_CONVERSION_MODE	== ADC_SINGLE_CONVERSION_MODE

	/*set ADC as single conversion*/
	CLEARBIT(ADCSRA, ADATE);
#elif ADC_CONVERSION_MODE	== ADC_FREE_RUNNING_MODE
	SETBIT(ADCSRA, ADATE);

	#if ADC_AUTO_TRIGGER_SOURCE	== FREE_RUNNING_MODE
			CLEARBIT(SFIOR,SFIOR_ADTS_2);
			CLEARBIT(SFIOR,SFIOR_ADTS_1);
			CLEARBIT(SFIOR,SFIOR_ADTS_0);
	#elif ADC_AUTO_TRIGGER_SOURCE == ANALOG_COMPARATOR_MODE
			CLEARBIT(SFIOR,SFIOR_ADTS_2);
			CLEARBIT(SFIOR,SFIOR_ADTS_1);
			SETBIT(SFIOR,SFIOR_ADTS_0);
	#elif ADC_AUTO_TRIGGER_SOURCE == EXTERNAL_INTERRUPT_REQ_0
			CLEARBIT(SFIOR,SFIOR_ADTS_2);
			SETBIT(SFIOR,SFIOR_ADTS_1);
			CLEARBIT(SFIOR,SFIOR_ADTS_0);
	#elif ADC_AUTO_TRIGGER_SOURCE == TIMER0_COMPARE_MATCH
			CLEARBIT(SFIOR,SFIOR_ADTS_2);
			SETBIT(SFIOR,SFIOR_ADTS_1);
			SETBIT(SFIOR,SFIOR_ADTS_0);
	#elif ADC_AUTO_TRIGGER_SOURCE == TIMER0_OVERFLOW
			SETBIT(SFIOR,SFIOR_ADTS_2);
			CLEARBIT(SFIOR,SFIOR_ADTS_1);
			CLEARBIT(SFIOR,SFIOR_ADTS_0);
	#elif ADC_AUTO_TRIGGER_SOURCE == TIMER1_COMPARE_MATCH_B
			SETBIT(SFIOR,SFIOR_ADTS_2);
			CLEARBIT(SFIOR,SFIOR_ADTS_1);
			SETBIT(SFIOR,SFIOR_ADTS_0);
	#elif ADC_AUTO_TRIGGER_SOURCE == TIMER1_OVERFLOW
			SETBIT(SFIOR,SFIOR_ADTS_2);
			SETBIT(SFIOR,SFIOR_ADTS_1);
			CLEARBIT(SFIOR,SFIOR_ADTS_0);
	#elif ADC_AUTO_TRIGGER_SOURCE == TIMER1_COMPARE_CAPTURE_MODE
			SETBIT(SFIOR,SFIOR_ADTS_2);
			SETBIT(SFIOR,SFIOR_ADTS_1);
			SETBIT(SFIOR,SFIOR_ADTS_0);
	#endif
#endif

#if ADC_INTERRUPT_MODE == ADC_INTERRUPT_OFF
	/*disable interrupt*/
	CLEARBIT(ADCSRA, ADIE);
#elif ADC_INTERRUPT_MODE == ADC_INTERRUPT_ON
	SETBIT(ADCSRA, ADIE);
#endif

	/*choosing the prescalar*/
	CLEARBIT(ADCSRA,ADPS2);
	CLEARBIT(ADCSRA,ADPS1);
	CLEARBIT(ADCSRA,ADPS0);
	ADCSRA |=ADC_PRESCALAR;

	/*enable ADC*/
	SETBIT(ADCSRA, ADEN);
}

uint8 ADC_StartConversion(uint8 copy_channel)
{
	/*choosing the ADC channel*/
	ADMUX &= 0b11100000;
	ADMUX |= copy_channel;
	/*start conversion*/
	SETBIT(ADCSRA,ADSC);
	
	while(!GETBIT(ADCSRA,ADIF));
	
	/*clearing the flag*/
	SETBIT(ADCSRA,ADIF);
	return ADCH;
}
uint8 ADC_uint8_StartConversionSynch(uint8 copy_uint8_channel, uint8* copy_puint8_reading)
{
	uint8 localError=TRUE;
	uint32 localTimeOutCounter=0;
	if (ADC_uint8_BusyState == ADC_NOTBUSY)
	{
		ADC_uint8_BusyState=ADC_BUSY;
		/*choosing the ADC channel*/
		ADMUX &= ADMUX_CLEAR;
		ADMUX |= copy_uint8_channel;
		/*start conversion*/
		SETBIT(ADCSRA,ADSC);
		/*Polling (busy waiting) until the conversion complete flag is set or counter reach timeout value*/
		while(GETBIT(ADCSRA,ADIF)==FALSE &&localTimeOutCounter!= ADC_uint32_TIMEOUT)
		{
			localTimeOutCounter++;
		}
		if(GETBIT(ADCSRA,ADIF)==TRUE)
		{
			/*clearing the flag*/
			SETBIT(ADCSRA,ADIF);
			/*assigning digital value */
			*copy_puint8_reading = ADCH;
			ADC_uint8_BusyState = ADC_NOTBUSY;
		}
		else if(localTimeOutCounter== ADC_uint32_TIMEOUT)
		{
			localError=FALSE;
			ADC_uint8_BusyState = ADC_NOTBUSY;

		}


	}
	else
	{
		localError=FALSE;
	}
	return localError;

}
uint8 ADC_uint8_StartConversionAsynch(uint8 copy_uint8_channel, uint8* copy_puint8_reading, void (*copy_pv_NotificationFunction)(void))
{

	uint8 localError=TRUE;
	if (ADC_uint8_BusyState == ADC_BUSY)
	{
		localError = FALSE;
	}
	else
	{
		if ((copy_puint8_reading == NULL) || (copy_pv_NotificationFunction == NULL))
			{
				localError=FALSE;
			}
			else
			{
				ADC_uint8_BusyState=ADC_BUSY;
				/*make ISR source single channel Asynchronous*/
				ADC_uint8_ADCISRSource = SINGLE_CHANNEL_ASYNCH;
				/*initialize the reading variable*/
				ADC_puint8_Reading=copy_puint8_reading;
				/*initialize the call back function*/
				ADC_pv_CallBackNotification=copy_pv_NotificationFunction;
				/*choosing the ADC channel*/
				ADMUX &= ADMUX_CLEAR;
				ADMUX |= copy_uint8_channel;
				/*ADC interrupt Enable*/
				SETBIT(ADCSRA,ADIE);
				/*start conversion*/
				SETBIT(ADCSRA,ADSC);
			}
	}
	return localError;
}

void __vector_16 (void) __attribute__((signal));
void __vector_16(void)
{
 if(ADC_uint8_ADCISRSource==SINGLE_CHANNEL_ASYNCH)
 {
	 	/*Read the ADC result*/
		*ADC_puint8_Reading=ADCH;
		/*call the call back function in the main application*/
		ADC_pv_CallBackNotification();
		/*disable ADC interrupt*/
		CLEARBIT(ADCSRA,ADIE);
		/*set the ADC to NOTBUSY to start new conversion if requested*/
		ADC_uint8_BusyState = ADC_NOTBUSY;
 }
 else if(ADC_uint8_ADCISRSource==CHAIN_ASYNCH)
 {

 }
}

