#include "STD.h"
#include "BIT_MATH.h"
#include <util/delay.h>
#include "DIO_Interface.h"
#include "ADC_interface.h"
#include "LCD_Interface.h"
#include "Keypad_Interface.h"
#include "Keypad_config.h"
#include "Timer_interface.h"
#include "EEPROM_Interface.h"
#include "UART_interface.h"

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

#define ADMIN_PASS_LOCATION 0x00
#define USER_PASS_LOCATION 0x14
#define USER_ID_LOCATION 0x10
#define MAX_USER_NUM 3
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
static uint8 angle=0;
static uint32 servo_adjust_time=20000;
static uint8 ADMIN_or_USER=0;

int main (void)
{
	DIO_SetPortDirection(DIO_PORTB,PORT_OUTPUT);//LCD data
	DIO_setPinDirection(DIO_PORTA,PIN5,OUTPUT);// LCD EN
	DIO_setPinDirection(DIO_PORTA,PIN6,OUTPUT); // LCD R/W
	DIO_setPinDirection(DIO_PORTA,PIN7,OUTPUT); //LCD RS

	DIO_setPinDirection(DIO_PORTD,PIN7,OUTPUT);//Lights
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
				_delay_ms(1000);
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
				_delay_ms(1000);
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
				case '1':
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
				case '2':
					DIO_setPinVAlue(DIO_PORTD,PIN4,OUTPUT_HIGH);
					break;
				case '3':
					DIO_setPinVAlue(DIO_PORTD,PIN4,OUTPUT_LOW);
					break;
				case '4':
					DIO_setPinVAlue(DIO_PORTD,PIN7,OUTPUT_HIGH);
					break;
				case '5':
					DIO_setPinVAlue(DIO_PORTD,PIN7,OUTPUT_LOW);
					break;
				case '6':
					/*control servo motor using potentiometer*/
					while(servo_adjust_time)
					{
						valueADC=ADC_StartConversion(ADC0);
						angle=ADC_To_Angle(valueADC);
						Timer1_Servo(angle);
						servo_adjust_time--;
					}
					servo_adjust_time=20000;
					break;
				case '7':
					if(ADMIN_or_USER=='1')
					{
						add_user();
					}
					else
					{
						LCD_SendString("NOT ACCESSIBLE!");
					}
					break;
				case '8':
					if(ADMIN_or_USER=='1')
					{
						LCD_SendString("Enter User ID");
						UART_sendString("Enter User ID: ");
						UART_recieve_string(IDArr);
						LCD_ClearDisplay();
						_delay_ms(2);
						RemoveUser(IDArr);					}
					else
					{
						LCD_SendString("NOT ACCESSIBLE!");
					}
					break;
				default:
					break;
			}
		}
	}
	return 0;
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
	LCD_SendString("Enter Password");
	UART_sendString("Enter Password: ");
	LCD_GoToXY(SecondLine, 0);

	UART_recieve_string(passArr);

	LCD_SendString("****");
	_delay_ms(500);
	LCD_ClearDisplay();
	_delay_ms(2);
}

void Display_Options(void)
{

	LCD_SendString("1-Display Temp");
	LCD_GoToXY(SecondLine, 0);
	LCD_SendString("Fan: 2-ON");
	LCD_GoToXY(SecondLine, 9);
	LCD_SendString(" 3-OFF");
	_delay_ms(1000);
	LCD_ClearDisplay();
	_delay_ms(2);
	LCD_SendString("LEDs: 4-ON");
	LCD_GoToXY(FirstLine, 10);
	LCD_SendString(" 5-OFF");
	LCD_GoToXY(SecondLine, 0);
	LCD_SendString("6-Curtains Angle");
	_delay_ms(1000);
	LCD_ClearDisplay();
	LCD_SendString("7-Add User");
	LCD_GoToXY(SecondLine, 0);
	LCD_SendString("8-Remove User");
	_delay_ms(1000);
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
	EEPROM_voidSend4Numbers( USER_PASS_LOCATION+10*usersCounter , passReal , 4);
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
			break;
		}
	}
	if (i == (MAX_USER_NUM - 1))
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
