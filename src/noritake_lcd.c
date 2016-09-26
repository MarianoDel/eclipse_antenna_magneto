/*
 * noritake_lcd.c
 *
 *  Created on: Oct 16, 2013
 *      Author: Mariano
 */

#include "main.h"
//#include <string.h>
//#include "stm32f0x_gpio.h"
//#include "stm32f0x_tim.h"
//#include "stm32f0xx.h"

//#define ENA_SERIAL_LCD GPIOA_PIN0_ON
//#define DISA_SERIAL_LCD GPIOA_PIN0_OFF

//VARIABLES EXTERNAS


//variable global para comandos
unsigned char command [10];

//funciones
void LCD_Init(void)
{
	Wait_ms(2);
	//TXD = 1;
	//GPIOA_PIN0_ON;
	Wait_ms(120);
	
	//seteo init
	command[0] = ESC;
	command[1] = 0x40;
	command[2] = 0x00;
	LCD_Ext_Command(command);	  

}

#ifdef SYNC
void LCD_Str(unsigned char * pStr)
{
	unsigned char nStrLen=strlen(pStr);
	
	//TODO meter line_idle
	ENA_SERIAL_LCD;
	USARTx_Send((char *) pStr);
	Wait_ms(nStrLen);
    DISA_SERIAL_LCD;
}

void LCD_Command(unsigned char command)
{
	//SCIStartTransmit(command);
	USARTx_Send(command);
}

//espera el listado de comandos extendidos en un puntero
void LCD_Ext_Command(unsigned char * pcomm)
{
	LCD_Str(pcomm);
}
#endif

#ifdef ASYNC
void LCD_Str(unsigned char * pstr)
{
	unsigned char i = 0;

	while (*(pstr+i) != '\0')
	{
	    //while (SBUSY);
	    LCD_Command(*(pstr+i));
	    i++;
	}
}

#ifdef PARI
void LCD_Command(unsigned char command)
{
	unsigned char paridad = 0;
	TXD = 0;
	timer_bit = 0;
	Mtim_OneShot();
	while (!timer_bit);
	for (i = 0; i < 8; i++)
	{
		TXD = command & 0x01;

		timer_bit = 0;
		Mtim_OneShot();
		if (command & 0x01)
			paridad++;
		command >>= 1;
		while (!timer_bit);

	}
	TXD = paridad & 0x01;
	timer_bit = 0;
	Mtim_OneShot();
	while (!timer_bit);
}
#endif

#ifdef NOPARI
void LCD_Command(unsigned char command)
{	
	unsigned char i;
	TXD = 0;
	timer_bit = 0;
	Mtim_OneShot();
	while (!timer_bit);
	for (i = 0; i < 8; i++)
	{
		TXD = command & 0x01;

		timer_bit = 0;
		Mtim_OneShot();
		command >>= 1;
		while (!timer_bit);

	}
	
	TXD = 1;
	timer_bit = 0;
	Mtim_OneShot();
	while (!timer_bit);
}
#endif

//espera el listado de comandos extendidos en un puntero
void LCD_Ext_Command(unsigned char * pcomm)
{
	LCD_Str(pcomm);
}
#endif
