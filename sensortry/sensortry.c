/*
 * chocolate_cannon.c
 *
 * Created: 5/14/2017 9:44:27 PM
 *  Author: ASUS
 */ 


//#include <avr/io.h>
#include "mpu6050_twi.h"
#include <avr/io.h>
//#include "uart.c"
#include "util/delay.h"
#include <string.h>
#include <stdio.h>
#ifndef F_CPU
#define F_CPU 1000000 // 16 MHz clock speed
#endif

#define D0 eS_PORTD0
#define D1 eS_PORTD1
#define D2 eS_PORTD2
#define D3 eS_PORTD3
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include "lcd.h"

float Acc_x,Acc_y,Acc_z,Temperature,Gyro_x,Gyro_y,Gyro_z;

int intLen(float x)
{
	int temp = (int)x,rem,len=0;
	while(temp)
	{
		len++;
		temp /= 10;
	}
	
	return len;
	
}

void floatToStr(float x,char ret[])
{
	int len = intLen(x);

	//while()
	x *=10000;
	//printf("%f\n",x);

	int val = (int)x,rem,i=0,j;

	//char ret[6];

	while(val)
	{
		rem = val % 10;
		ret[i++] = (rem + '0');
		//printf("%c\n",ret[i-1]);
		val /= 10;
	}

	for( i=0,j=4;i<j;i++,j--)
	{
		char temp = ret[i];
		ret[i] = ret[j];
		ret[j] = temp;
	}
	
	ret[len] = '.';
	ret[5] = '\0';



	//return ret;
}

int main()
{
	DDRD = 0xFF;
	DDRC = 0xFF;
	DDRB=0xFF;
	PORTB=0x00;
	char buffer[20], float_[10];
	float Xa,Ya,Za,t;
	float Xg=0,Yg=0,Zg=0;
	I2C_Init();		/* Initialize I2C */
	Gyro_Init();		/* Initialize Gyro */
	//uart_init();
	//stdout = &uart_output;
	//stdin  = &uart_input;
	int i;
	Lcd8_Init();
	 char buf[6];
	 
	
	while(1)
	{
		Read_RawValue(&Acc_x, &Acc_y, &Acc_z, &Temperature, &Gyro_x, &Gyro_y, &Gyro_z);

		/* Divide raw value by sensitivity scale factor */
		Xa = Acc_x/16384.0;										
		Ya = Acc_y/16384.0;
		Za = Acc_z/16384.0;
		
		
		//Xa=Xa*9.8;
		floatToStr(Xa,buf);
		Lcd8_Set_Cursor(1,1);
		Lcd8_Write_String(buf);
		_delay_ms(1000);
		Lcd8_Clear();
	/*	if(Xa>9.0)
		{
			PORTB=0xff;
			//_delay_ms(500);
			//PORTB=0x00;
		}
		
		else
		{
			PORTB=0x00;
			//_delay_ms(2000);
		} */
	
		Ya=Ya*9.8;
	
		if(Ya>9.0)
		{
			PORTB=0xff;
			//_delay_ms(500);
			//PORTB=0x00;
		}
		
		else
		{
			PORTB=0x00;
			//_delay_ms(2000);
		} 
		
		Xa=0.0;
		Ya=0.0;
		
		
		Xg = Gyro_x/16.4;
		Yg = Gyro_y/16.4;
		Zg = Gyro_z/16.4;

		
		t = (Temperature/340.00)+36.53;

		
	}
}