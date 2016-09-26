/*
 * comm.c
 *
 *  Created on: Oct 29, 2013
 *      Author: Mariano
 */

#include "main.h"
#include "comm.h"
//externas

extern const char s_ok [];

//globales de este modulo
const char s_get_gauss [] = {"get_gauss"};
const char s_get_temp [] = {"get_temp"};
const char s_get_params [] = {"get_params"};
const char s_channel [] = {"channel"};
const char s_set_display [] = {"set_display"};
const char s_cmd_display [] = {"cmd_display"};
const char s_keepalive [] = {"keepalive"};


char pCmd [20];

//Funciones del modulo
unsigned char InterpretarMsg (char * pStr)	//TODO copiar payload solo en las cuestiones de LCD y no en los comandos
{

	//if (strncmp(s_get_params, pStr, sizeof(s_get_params) - 1) == 0)
	if (strncmp(pStr, s_get_params, sizeof(s_get_params) - 1) == 0)
		return GET_PARAMS;

	//if (strncmp(s_get_temp, pStr, sizeof(s_get_temp) - 1) == 0)
	if (strncmp(pStr, s_get_temp, sizeof(s_get_temp) - 1) == 0)
		return GET_TEMP;

	if (strncmp(s_set_display, pStr + 1, sizeof(s_set_display) - 1) == 0)
		return SET_DISPLAY;

	//if (strncmp(s_keepalive, pStr, sizeof(s_keepalive) - 1) == 0)
	if (strncmp(pStr, s_keepalive, sizeof(s_keepalive) - 1) == 0)
		return KEEP_ALIVE;

//	if (strncmp(s_cmd_display, pStr + 1, sizeof(s_cmd_display) - 1) == 0)
//	{
//		memcpy(pc, pStr + sizeof(s_cmd_display) + 4, (a - sizeof(s_cmd_display) - 4));
//		return CMD_DISPLAY;
//	}
	//ahora tengo el comando y ademas el payload en pc
	
	return ERROR;	
}

void AnalizarMsg (char * pStr)
{
	unsigned char a;
	
	//reviso si el mensaje es para mi o si es canal 0, no asignado
	/*
	if (!Check_Own_Channel(pStr))
		return;
	*/
	a = InterpretarMsg (pStr);
	
	switch (a)
	{
		case GET_PARAMS:
			break;
			
		case GET_TEMP:
			break;
			
		case GET_GAUSS:
			break;

		case CHANNEL:
			break;

		case SET_DISPLAY:
			break;

		case CMD_DISPLAY:
			break;
			
		case ERROR:
		default:
			break;
	}
}
