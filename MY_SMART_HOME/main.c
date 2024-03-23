#include "LIBRARIES/STD.h"
#include "LIBRARIES/BIT_MATH.h"
#include <util/delay.h>
#include "MCAL/DIO_Interface.h"
#include "MCAL/ADC_interface.h"
#include "HAL/LCD_Interface.h"
#include "HAL/Keypad_Interface.h"
#include "HAL/Keypad_config.h"
#include "MCAL/Timer_interface.h"
#include "HAL/EEPROM_Interface.h"
#include "MCAL/UART_interface.h"

uint8 stringCompare(uint8* str1, uint8* str2, uint8 Num);
void Get_password_keypad(void);
void Get_UserID_keypad(void);
void Get_password_UART(void);
void Register_password(void);
void Display_Options(void);
void RemoveUser(uint8* copy_userID);
uint8 checkLatestUser(void);
void add_user(void);
uint8 checkUserID(void);
void TurnOnLEDs(void);
void TurnOffLEDs(void);
void AC_Control(void);
#define ADMIN_PASS_LOCATION 0x00
#define USER_PASS_LOCATION 0x14
#define USER_ID_LOCATION 0x10
#define MAX_USER_NUM 6

static uint8 valueADC=0;
static uint32 analogue=0;
static uint8 temprature=0;
static uint8 passReal[4]={0};
static uint8 passArr[4];
static uint8 IDReal[4]={0};
static uint8 IDArr[4];
static uint8 passStatus=2;
static uint8 iterator=0;
static uint8 choosenOption =0;
static uint8 trials=0;
static uint32 DimmingAdjTimer=20000;
static uint8 ADMIN_or_USER=0;
static uint8 LedsNum=0;

int main (void)
{
	DIO_SetPortDirection(DIO_PORTB,PORT_OUTPUT);//LCD data
	DIO_setPinDirection(DIO_PORTA,PIN5,OUTPUT);// LCD EN
	DIO_setPinDirection(DIO_PORTA,PIN6,OUTPUT); // LCD R/W
	DIO_setPinDirection(DIO_PORTA,PIN7,OUTPUT); //LCD RS

	DIO_setPinDirection(DIO_PORTC,PIN7,OUTPUT);//Buzzer
	DIO_setPinDirection(DIO_PORTD,PIN2,OUTPUT);//LED1
	DIO_setPinDirection(DIO_PORTD,PIN3,OUTPUT);//LED2
	DIO_setPinDirection(DIO_PORTC,PIN6,OUTPUT);//LED3
	DIO_setPinDirection(DIO_PORTA,PIN3,OUTPUT);//LED4
	DIO_setPinDirection(DIO_PORTA,PIN4,OUTPUT);//LED5

	DIO_setPinDirection(DIO_PORTD,PIN7,OUTPUT);//Lights Dimming
	DIO_setPinDirection(DIO_PORTD,PIN4,OUTPUT);//Fan
	DIO_setPinDirection(DIO_PORTD,PIN5,OUTPUT);//Servo motor
	Keypad_Init();
	ADC_init();
	LCD_init();
	UART_Init();
	EEPROM_INIT();

	LCD_SendString("Welcome To SMART HOME");
	_delay_ms(1000);
	LCD_ClearDisplay();
	_delay_ms(2);
	LCD_SendString("1- ADMIN  2-USER");
	do
	{
		ADMIN_or_USER=get_button_pressed();
	}while(ADMIN_or_USER==0xff );

	if (ADMIN_or_USER=='1')
	{
		if(EEPROM_uint8ReadDataByte(ADMIN_PASS_LOCATION)==0xff)
		{
			Register_password();
			Get_password_UART();

		}
		else
		{
			Get_password_UART();
		}
	}
	else if(ADMIN_or_USER=='2')
	{
		Get_UserID_keypad();
		Get_password_keypad();
	}

	while(1)
	{
		if (ADMIN_or_USER=='1')
		{
			EEPROM_voidRead4Numbers(ADMIN_PASS_LOCATION,passReal,4);
		}
		else if (ADMIN_or_USER=='2')
		{
			uint8 ID_Status=0;
			ID_Status=checkUserID();
			if(ID_Status>0)
			{
				EEPROM_voidRead4Numbers(USER_PASS_LOCATION+0x10*(ID_Status-1),passReal,4);
			}
			else
			{
				LCD_SendString("WRONG ID!");
				LCD_GoToXY(SecondLine,0);
				LCD_SendString("System Fail!!");
				DIO_setPinVAlue(DIO_PORTC,PIN7,OUTPUT_HIGH);
				_delay_ms(1500);
				LCD_ClearDisplay();
				_delay_ms(2);
				break;
			}
		}
		passStatus = stringCompare(passReal,passArr,4);
		if(passStatus==FALSE)
		{	/* check password again, you have only 3 trials then system crashes */
			trials++;
			if(trials==3)
			{
				LCD_SendString("System Fail");
				DIO_setPinVAlue(DIO_PORTC,PIN7,OUTPUT_HIGH);
				_delay_ms(1500);
				LCD_ClearDisplay();
				_delay_ms(2);
				break;
			}
			LCD_SendString("Wrong Password");
			_delay_ms(1000);
			LCD_ClearDisplay();
			_delay_ms(2);
			LCD_SendString("Enter Pass Again");
			LCD_GoToXY(SecondLine, 3);
			if(ADMIN_or_USER=='1')
			{
				UART_sendString("Enter Password Again: \n");
				UART_recieve_string(passArr);
			}
			else if (ADMIN_or_USER=='2')
			{
				iterator=0;
				while(iterator<4)
				{
					do
					{
						passArr[iterator]=get_button_pressed();
					}while(passArr[iterator]==0xff);
					LCD_Write_data('*');
					iterator++;
				}
				_delay_ms(1000);
				LCD_ClearDisplay();
				_delay_ms(2);

			}

		}
		else if(passStatus==TRUE)
		{
			Display_Options();
			AC_Control();
			if (ADMIN_or_USER=='1')
			{
				UART_sendString("  Enter Option: ");
				choosenOption=UART_Receive();
			}
			else if (ADMIN_or_USER=='2')
			{
				do
				{
					choosenOption=get_button_pressed();

				}while(choosenOption==0xff);
			}
			switch(choosenOption)
			{
				case '0':/* Read Current Temperature and Display it on LCD */

					valueADC= ADC_StartConversion(ADC1);
					analogue=(uint32)valueADC*5000UL/256UL; //in mV
					temprature=analogue/10;
					LCD_SendString("Temp=");
					LCD_GoToXY(FirstLine, 6);
					LCD_uint8_WriteNumber(temprature);
					LCD_SendString(" 'C");
					_delay_ms(3000);
					LCD_ClearDisplay();
					_delay_ms(2);
					break;
				case '1':/*AC on*/
					DIO_setPinVAlue(DIO_PORTD,PIN4,OUTPUT_HIGH);
					_delay_ms(2000);
					break;
				case '2':/*AC off*/
					DIO_setPinVAlue(DIO_PORTD,PIN4,OUTPUT_LOW);
					_delay_ms(2000);
					break;
				case '3':/*Leds on*/
					TurnOnLEDs();
					break;
				case '4':/*Leds off*/
					TurnOffLEDs();
					break;
				case '5':/*LEDs Dimming*/
					LCD_SendString("Use Potentiometer");
					while(DimmingAdjTimer>0)
					{
						valueADC=ADC_StartConversion(ADC0);
						Timer2_PWM_Controllable(valueADC);
						DimmingAdjTimer--;
					}
					DimmingAdjTimer=20000;
					LCD_ClearDisplay();
					_delay_ms(2);
					break;
				case '6':/*Door Open*/
					/*Set Servo angle to 90 degree*/
					if(ADMIN_or_USER=='1')
					{
						Timer1_Servo(90);
					}
					else
					{
						LCD_SendString("NOT ACCESSIBLE!");
						_delay_ms(1000);
						LCD_ClearDisplay();
						_delay_ms(2);
					}
					break;
				case '7':/*Door Close*/
					/*Set Servo angle to 90 degree*/
					Timer1_Servo(0);
					break;
				case '8':/*Add User*/
					if(ADMIN_or_USER=='1')
					{
						add_user();
					}
					else
					{

					}
					break;
				case '9':/*Remove User*/
					if(ADMIN_or_USER=='1')
					{
						LCD_SendString("Enter User ID");
						UART_sendString(" Enter User ID: ");
						UART_recieve_string(IDArr);
						LCD_ClearDisplay();
						_delay_ms(2);
						RemoveUser(IDArr);
					}
					else
					{

					}
					break;
				default:
					break;
			}
		}
	}
	return 0;
}


void Display_Options(void)
{

	LCD_SendString("0-Display Temp");
	LCD_GoToXY(SecondLine, 0);
	LCD_SendString("AC: 1-ON");
	LCD_GoToXY(SecondLine, 9);
	LCD_SendString(" 2-OFF");
	_delay_ms(1000);
	LCD_ClearDisplay();
	_delay_ms(2);
	LCD_SendString("LEDs: 3-ON");
	LCD_GoToXY(FirstLine, 10);
	LCD_SendString(" 4-OFF");
	LCD_GoToXY(SecondLine, 0);
	LCD_SendString("5-Dimming");
	_delay_ms(1200);
	LCD_ClearDisplay();
	LCD_SendString("6-Door Open");
	LCD_GoToXY(SecondLine, 0);
	LCD_SendString("7-Door Close");
	_delay_ms(1000);
	LCD_ClearDisplay();
	if(ADMIN_or_USER=='1')
	{
		LCD_SendString("8-Add User");
		LCD_GoToXY(SecondLine, 0);
		LCD_SendString("9-Remove User");
		_delay_ms(1000);
		LCD_ClearDisplay();
		_delay_ms(2);
	}
}


void Get_password_keypad(void)
{
	iterator=0;
	LCD_ClearDisplay();
	_delay_ms(2);
	LCD_SendString("Enter Password");
	LCD_GoToXY(SecondLine, 0);
	while(iterator<4)
	{
		do
		{
			passArr[iterator]=get_button_pressed();

		}while(passArr[iterator]==0xff);
		LCD_Write_data('*');
		iterator++;
	}
	_delay_ms(500);
	LCD_ClearDisplay();
	_delay_ms(2);

}
void Get_UserID_keypad(void)
{
	LCD_ClearDisplay();
	_delay_ms(2);
	LCD_SendString("Enter ID");
	LCD_GoToXY(SecondLine, 0);
	while(iterator<4)
	{
		do
		{
			IDArr[iterator]=get_button_pressed();

		}while(IDArr[iterator]==0xff);
		LCD_Write_data(IDArr[iterator]);
		iterator++;
	}
	_delay_ms(500);
	LCD_ClearDisplay();
	_delay_ms(2);

}
void Get_password_UART(void)
{
	LCD_ClearDisplay();
	_delay_ms(2);
	LCD_SendString("Enter Password");
	UART_sendString("Enter Password: ");
	LCD_GoToXY(SecondLine, 0);

	UART_recieve_string(passArr);

	LCD_SendString("****");
	_delay_ms(500);
	LCD_ClearDisplay();
	_delay_ms(2);
}

uint8 stringCompare(uint8* str1, uint8* str2, uint8 Num)
{
	uint8 i=0;
	for(i=0;i<Num;i++)
	{
		if (str1[i]==str2[i])
		{
			continue;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}
void Register_password(void)
{
	LCD_SendString("Register Password");
	UART_sendString("Register Password: ");
	UART_recieve_string(passReal);
	EEPROM_voidSend4Numbers(ADMIN_PASS_LOCATION,passReal,4);
	LCD_ClearDisplay();
	_delay_ms(2);
}

void add_user(void)
{
	uint8 usersCounter = 0;
	usersCounter = checkLatestUser();
	/* send by bluetooth to mobile app "Enter User ID" and display it on lcd*/
	LCD_ClearDisplay();
	LCD_SendString("Enter User ID ");
	UART_sendString("Enter User ID ");
	/*Read from mobile and save it in ID Array*/
	UART_recieve_string(IDReal);
	/*Save the ID in EEPROM*/
	EEPROM_voidSend4Numbers( USER_ID_LOCATION+ 0x10 * usersCounter , IDReal , 4);
	/* send by bluetooth to mobile app "Enter User password"*/
	LCD_ClearDisplay();
	LCD_SendString("Enter Password ");
	UART_sendString("Enter Password ");
	/*Read from mobile and save it in Password Array*/
	UART_recieve_string(passReal);
	/*Read from mobile and save it in password array*/
	/*Save the password in EEPROM*/
	EEPROM_voidSend4Numbers( USER_PASS_LOCATION+0x10*usersCounter , passReal , 4);
}
uint8 checkLatestUser(void)
{
	uint8 local_latestUser = 0;

	while (EEPROM_uint8ReadDataByte((USER_ID_LOCATION + 0x10 * local_latestUser)) != 0xFF)
	{
		local_latestUser++;
	}

	return local_latestUser;
}

void RemoveUser(uint8* copy_userID)
{   /* this function deletes 8 consequent locations (ID + Password) in eeprom */
	uint8 local_temp_ID_Arr[4]={0};
	uint8 i=0;
	for(i=0;i<MAX_USER_NUM;i++)
	{
		EEPROM_voidRead4Numbers(USER_ID_LOCATION+0x10*i ,local_temp_ID_Arr,4);
		/*check if id exist?*/
		if (stringCompare(copy_userID, local_temp_ID_Arr, 4)== FALSE )
		{
			continue;
		}
		else
		{
			/*remove user id and password*/
			EEPROM_voidRemoveUser(USER_ID_LOCATION + 0x10* i);
			LCD_SendString("User's been deleted Successfully");
			UART_sendString("User's been deleted Successfully");
			_delay_ms(1000);
			break;
		}
	}
	if (i == MAX_USER_NUM )
	{
		/*Send by uart "ID does not exist"*/
		LCD_SendString("ID does not exist");
		UART_sendString("ID does not exist");
		_delay_ms(1000);
		LCD_ClearDisplay();
		_delay_ms(2);
	}
}

uint8 checkUserID(void)
{
	uint8 localCheck=0;
	uint8 local_counter=0;
	while(local_counter< MAX_USER_NUM)
	{
		EEPROM_voidRead4Numbers((USER_ID_LOCATION + (0x10 * local_counter)),IDReal, 4);
		localCheck = stringCompare(IDArr,IDReal,4);
		if(localCheck==FALSE)
		{
			local_counter++;
			continue;
		}
		else
		{
			return local_counter+1;
		}
	}
	return FALSE;
}

void TurnOnLEDs(void)
{
	LCD_SendString("Num of LEDs: ");
	if (ADMIN_or_USER== '1')
	{
		UART_sendString(" Num of LEDs:");
		LedsNum=UART_Receive();
	}
	else if (ADMIN_or_USER== '2')
	{
		do
		{
			LedsNum=get_button_pressed();

		}while(LedsNum==0xff);
	}
	if('1'==LedsNum)
	{
		DIO_setPinVAlue(DIO_PORTD,PIN2,OUTPUT_HIGH);//LED1

	}
	else if('2'==LedsNum)
	{
		DIO_setPinVAlue(DIO_PORTD,PIN2,OUTPUT_HIGH);//LED1
		DIO_setPinVAlue(DIO_PORTD,PIN3,OUTPUT_HIGH);//LED2
	}
	else if('3'==LedsNum)
	{
		DIO_setPinVAlue(DIO_PORTD,PIN2,OUTPUT_HIGH);//LED1
		DIO_setPinVAlue(DIO_PORTD,PIN3,OUTPUT_HIGH);//LED2
		DIO_setPinVAlue(DIO_PORTC,PIN6,OUTPUT_HIGH);//LED3
	}
	else if('4'==LedsNum)
	{
		DIO_setPinVAlue(DIO_PORTD,PIN2,OUTPUT_HIGH);//LED1
		DIO_setPinVAlue(DIO_PORTD,PIN3,OUTPUT_HIGH);//LED2
		DIO_setPinVAlue(DIO_PORTC,PIN6,OUTPUT_HIGH);//LED3
		DIO_setPinVAlue(DIO_PORTA,PIN3,OUTPUT_HIGH);//LED4
	}
	else if('5'==LedsNum)
	{
		DIO_setPinVAlue(DIO_PORTD,PIN2,OUTPUT_HIGH);//LED1
		DIO_setPinVAlue(DIO_PORTD,PIN3,OUTPUT_HIGH);//LED2
		DIO_setPinVAlue(DIO_PORTC,PIN6,OUTPUT_HIGH);//LED3
		DIO_setPinVAlue(DIO_PORTA,PIN3,OUTPUT_HIGH);//LED4
		DIO_setPinVAlue(DIO_PORTA,PIN4,OUTPUT_HIGH);//LED5
	}

}
void TurnOffLEDs(void)
{
	DIO_setPinVAlue(DIO_PORTD,PIN2,OUTPUT_LOW);//LED1
	DIO_setPinVAlue(DIO_PORTD,PIN3,OUTPUT_LOW);//LED2
	DIO_setPinVAlue(DIO_PORTC,PIN6,OUTPUT_LOW);//LED3
	DIO_setPinVAlue(DIO_PORTA,PIN3,OUTPUT_LOW);//LED4
	DIO_setPinVAlue(DIO_PORTA,PIN4,OUTPUT_LOW);//LED5
}


void AC_Control(void)
{
	valueADC= ADC_StartConversion(ADC1);
	analogue=(uint32)valueADC*5000UL/256UL; //in mV
	temprature=analogue/10;
	if(temprature>28)
	{
		DIO_setPinVAlue(DIO_PORTD,PIN4,OUTPUT_HIGH);
	}
	else if (temprature<21)
	{
		DIO_setPinVAlue(DIO_PORTD,PIN4,OUTPUT_LOW);
	}
	else
	{
		/* No Change */
	}
}
