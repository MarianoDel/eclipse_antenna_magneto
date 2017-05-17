#include "stm32f0xx.h"
#include "main.h"
#include "stm32f0xx_adc.h"
#include "stdio.h"
#include "string.h"
#include "adc.h"

//ANTENA ELEGIDA
//#define ANTENA0		//toroidal diametro grande
//#define ANTENA1	//toroidal diametro mediana
//#define ANTENA1B	//toroidal diametro mediana DE=110mm DI=45
//#define ANTENA2	//cilindrica chica
//#define ANTENA3	//cilindrica mediana
//#define ANTENA4	//cilindrica grande
//#define ANTENA5	//cilindrica muy chica OJOS
//#define ANTENA6	//cilindrica vieja de madera
//#define ANTENA7 //pencil tunel
//#define ANTENA8 //pencil plato doble
//#define ANTENAA1	//Ernesto viejas
//#define ANTENAA2	//Ernesto viejas
//#define ANTENAA3	//Ernesto viejas
//#define ANTENAA5	//Ernesto viejas
//#define ANTENAA4	//Ernesto viejas
//#define ANTENAA6 //antenas Ernesto tunel grande 350mm dia
//#define ANTENAA7 //antenas Ernesto tunel mediano 240mm dia
//#define ANTENAA1 //antenas Ernesto tunel chico 150mm dia
#define ANTENAA9	//Nuevas Luis

//--- VARIABLES EXTERNAS ---//
volatile unsigned char timer_1seg = 0;
volatile unsigned short timer_standby = 0;
volatile unsigned short timer_led_comm = 0;
volatile unsigned char buffrx_ready = 0;
volatile unsigned char *pbuffrx;

const char s_ok [] = {"ok\r\n"};

//--- VARIABLES GLOBALES ---//
//antena, R [ohms], L [mHy], Imax [A], Tmax [°C] todos 000.00
#ifdef ANTENA0 //toroidal diametro grande
const char s_antena [] = { "ant0,012.27,087.90,001.80,065.00\r\n" };
#endif

#ifdef ANTENA1 //toroidal diametro mediana
const char s_antena [] = { "ant1,023.85,151.70,001.10,065.00\r\n" };
#endif

#ifdef ANTENA2 //cilindrica chica
const char s_antena [] = { "ant2,005.70,011.10,002.80,065.00\r\n" };
#endif

#ifdef ANTENA3 //cilindrica mediana
//const char s_antena [] = { "ant3,002.73,019.00,004.50,065.00\r\n" };
const char s_antena [] = { "ant3,003.50,019.00,003.50,065.00\r\n" };
#endif

#ifdef ANTENA4 //cilindrica grande
//const char s_antena [] = { "ant4,003.44,023.00,004.50,065.00\r\n" };
const char s_antena [] = { "ant4,004.00,022.00,003.50,065.00\r\n" };
#endif

#ifdef ANTENA5 //cilindrica muy chica OJOS
const char s_antena [] = { "ant5,052.00,130.00,000.60,065.00\r\n" };
#endif

#ifdef ANTENA6 //cilindrica vieja de madera
const char s_antena [] = { "ant6,010.60,078.20,002.00,065.00\r\n" };
#endif

#ifdef ANTENA7 //pencil tunel
const char s_antena [] = { "ant7,012.70,064.80,002.00,065.00\r\n" };
#endif

#ifdef ANTENA8 //pencil plato doble
const char s_antena [] = { "ant8,022.60,157.20,001.10,065.00\r\n" };
#endif

//antena, R [ohms], L [mHy], Imax [A], Tmax [°C] todos 000.00
#ifdef ANTENAA9 //toroidal diametro mediana antena nueva DE=110 DI=45
const char s_antena [] = { "anta,023.80,133.00,001.10,065.00\r\n" };
#endif

#ifdef ANTENAA1 //antenas Ernesto viejas CHICOS
const char s_antena [] = { "anta,008.60,045.00,002.50,065.00\r\n" };
#endif

#ifdef ANTENAA2 //antenas Ernesto viejas CHICOS
const char s_antena [] = { "anta,009.00,051.40,003.20,065.00\r\n" };
#endif

#ifdef ANTENAA3 //antenas Ernesto T02
const char s_antena [] = { "anta,007.20,041.40,004.00,065.00\r\n" };
#endif

#ifdef ANTENAA4 //antenas Ernesto viejas CHICOS
const char s_antena [] = { "anta,022.60,157.20,001.10,065.00\r\n" };
#endif

#ifdef ANTENAA5 //antenas Ernesto viejas CHICOS
const char s_antena [] = { "anta,022.60,157.20,001.10,065.00\r\n" };
#endif

#ifdef ANTENAA6 //antenas Ernesto tunel grande 350mm dia
const char s_antena [] = { "anta,009.00,051.00,003.50,065.00\r\n" };
#endif

#ifdef ANTENAA7 //antenas Ernesto tunel mediano 240mm dia
const char s_antena [] = { "anta,009.00,045.00,003.50,065.00\r\n" };
#endif

#ifdef ANTENAA8 //antenas Ernesto tunel chico 150mm dia
const char s_antena [] = { "anta,010.60,007.82,002.00,065.00\r\n" };
#endif

static __IO uint32_t TimingDelay;

//--- FUNCIONES DEL MODULO ---//
unsigned short ADC_Conf (void);
unsigned short ReadADC1 (unsigned char);
int GetTemp (unsigned short);
void Delay(__IO uint32_t nTime);



//-------------------------------------------//
// @brief  Main program.
// @param  None
// @retval None
//------------------------------------------//
int main(void)
{
	unsigned short adc_sample = 0;
	char str1 [20];
	unsigned short ts_cal1, ts_cal2;
	int temp = 0;
	short dx = 0;
	unsigned char state = 0;
	unsigned char a = 0;
	unsigned char answer = 0;

	//!< At this stage the microcontroller clock setting is already configured,
    //   this is done through SystemInit() function which is called from startup
    //   file (startup_stm32f0xx.s) before to branch to application main.
    //   To reconfigure the default setting of SystemInit() function, refer to
    //   system_stm32f0xx.c file

	//GPIO Configuration.
	GPIO_Config();

	//TIM Configuration.
	Timer_1_Init();
	//Timer_2_Init();
	//Timer_3_Init();
	//Timer_4_Init();

	//UART configuration.
	USART_Config();

	//TODO:
	//ACTIVAR SYSTICK TIMER
	 if (SysTick_Config(48000))
	 {
		 /* Capture error */
		 while (1);
	 }
	//SENSAR TEMPERATURA 	((OK))
	//ENVIAR ONE WIRE		((OK))

	//PRUEBA DE SYSTICK
	 /*
	 while(1)
	 {
		 if (LED_COMM)
			 LED_COMM_OFF;
		 else
			 LED_COMM_ON;

		 Delay(1);
	 }
	 */
	 //FIN PRUEBA DE SYSTICK

	//ADC configuration.
	if (ADC_Conf() == 0)
		USARTx_Send("\r\nERROR DE CALIBRACION...");
	else
		USARTx_Send("\r\nCALIBRACION de ADC OK...");

	//Activo sensor de temp
	ADC_TempSensorCmd(ENABLE);
	//calibracion de fabrica del sensor
	ts_cal1 = *((uint16_t*)0x1FFFF7B8);
	ts_cal2 = *((uint16_t*)0x1FFFF7C2);

	USARTx_Send("\r\nts_cal1: ");
	memset(str1, 0, sizeof(str1));
    sprintf(str1, "%d", ts_cal1);
    USARTx_Send(str1);
    USARTx_Send("\r\nts_cal2: ");
	memset(str1, 0, sizeof(str1));
    sprintf(str1, "%d", ts_cal2);
    USARTx_Send(str1);
    dx = ts_cal1 - ts_cal2;
    Wait_ms(2000);


    //--- PROGRAMA PRUEBA PUERTO SERIE Y TEMP ---//
    /*
    while(1)
    {
		if (!timer_1seg)
		{
			timer_1seg = 5;
			USARTx_Send("\r\nLa temp: ");
			adc_sample = ReadADC1(16);
			memset(str1, 0, sizeof(str1));

			temp = adc_sample * (-80);
			temp = temp / dx;
			temp = temp + 367;

			sprintf(str1, "%d", temp);
			USARTx_Send(str1);

			Wait_ms(100);
			USARTx_Send("\r\nEl adc: ");
			memset(str1, 0, sizeof(str1));
			sprintf(str1, "%d", adc_sample);
			USARTx_Send(str1);
		}

		if (TXD_IN)
			TX_SERIE_OFF;
		else
			TX_SERIE_ON;
    }
    */
    //--- FIN PROGRAMA PRUEBA PUERTO SERIE Y TEMP ---//


    //--- PROGRAMA PRUEBA LED Y TIMER_STANDBY ---//
    /*
    while (1)
    {
		if (!timer_standby)
		{
			if (LED_COMM)
				LED_COMM_OFF;
			else
				LED_COMM_ON;
			timer_standby = 1000;
		}
    }
	*/
    //--- FIN PROGRAMA PRUEBA LED Y TIMER_STANDBY ---//

    //--- PROGRAMA STRING COMUNICACION ONE-WIRE ---//
    /*
	while(1)
	{
		USARTx_receive ();
		if (TXD_IN)
			TX_SERIE_OFF;
		else
			TX_SERIE_ON;

	}
	*/
    //--- FIN PROGRAMA STRING COMUNICACION ONE-WIRE ---//


	//--- Main loop ---//
	while(1)
	{
		//PROGRAMA DE PRODUCCION
		switch (state)
		{
			case STAND_BY:
				//envio la info de la antena cada 1 seg hasta que me contesten
				LED_COMM_OFF;
				if (!timer_standby)
				{
					LED_COMM_ON;
					state = TX_SERIE_NC;
					USARTx_Send((char *)s_antena);
				}

				if (buffrx_ready)
				{
					state = RX_SERIE;
					buffrx_ready = 0;
				}
				break;

			case CONNECT:
				//cuando se agota timer_1_seg salgo a STAND_BY
				//me fijo si debo contestar algo
				LED_COMM_ON;
				if (answer == KEEP_ALIVE)
				{
					Wait_ms(5);
					answer = 0;
					LED_COMM_OFF;
					state = TX_SERIE;
					USARTx_Send((char *)s_ok);
				}

				if (answer == GET_PARAMS)
				{
					Wait_ms(5);
					answer = 0;
					LED_COMM_OFF;
					state = TX_SERIE;
					//apago RX

					USARTx_Send((char *)s_antena);
				}

				if (answer == GET_TEMP)
				{
					Wait_ms(5);
					answer = 0;
					LED_COMM_OFF;
					state = TX_SERIE;

					//muestreo temp
					adc_sample = ReadADC1(16);
//				    temp = adc_sample * (-80);
//				    temp = temp / dx;
//				    temp = temp + 367;

					temp = GetTemp(adc_sample);
					//ajuste posterior
					temp = temp - 30;

				    memset(str1, 0, sizeof(str1));
				    sprintf(str1, "temp,%03d.00\r\n", temp);
					USARTx_Send(str1);
				}

				if (buffrx_ready)
				{
					state = RX_SERIE;
					buffrx_ready = 0;
				}

				if (!timer_1seg)	//mas de 10 segs sin comunicacion
				{
					state = STAND_BY;
				}
				break;

			case TX_LCD:
				//espero terminar de transmitir
				if (!(USART1->CR1 & 0x80))
				{
					state = TX_LCD2;
					timer_standby = 2;
				}

				if (TXD_IN)
					TX_LCD_OFF;
				else
					TX_LCD_ON;

				break;

			case TX_LCD2:
				//espero terminar de transmitir
				if (!timer_standby)
				{
					state = STAND_BY;
					TX_LCD_OFF;			//dejo la linea en idle
				}

				if (TXD_IN)
					TX_LCD_OFF;
				else
					TX_LCD_ON;

				break;

			case TX_SERIE:
				//espero terminar de transmitir
				if (!(USART1->CR1 & 0x80))
				{
					state = TX_SERIE2;
					timer_standby = 2;
				}

				if (TXD_IN)
					TX_SERIE_OFF;
				else
					TX_SERIE_ON;

				break;

			case TX_SERIE2:
				//espero terminar de transmitir
				if (TXD_IN)
					TX_SERIE_OFF;
				else
					TX_SERIE_ON;

				if (!timer_standby)
				{
					state = CONNECT;
					TX_SERIE_OFF;			//dejo la linea en idle
					LED_COMM_ON;
				}
				break;

			case TX_SERIE_NC:
				//espero terminar de transmitir
				if (!(USART1->CR1 & 0x80))
				{
					state = TX_SERIE2_NC;
					timer_standby = 2;
				}

				if (TXD_IN)
					TX_SERIE_OFF;
				else
					TX_SERIE_ON;

				break;

			case TX_SERIE2_NC:
				//espero terminar de transmitir
				if (TXD_IN)
					TX_SERIE_OFF;
				else
					TX_SERIE_ON;

				if (!timer_standby)
				{
					state = STAND_BY;
					TX_SERIE_OFF;			//dejo la linea en idle
					timer_standby = 1000;
					LED_COMM_OFF;
				}
				break;

			case RX_SERIE:
				//reviso que me llego, igual paso al estado conectado
				//si entiendo el mensaje
				answer = 0;
				a = InterpretarMsg ((char *)pbuffrx);

				switch (a)
				{
					case GET_PARAMS:
						answer = GET_PARAMS;
						timer_1seg = 10;
						break;

					case GET_TEMP:
						answer = GET_TEMP;
						timer_1seg = 10;
						break;

					case SET_DISPLAY:
						break;

					case CMD_DISPLAY:
						break;

					case KEEP_ALIVE:
						answer = KEEP_ALIVE;
						timer_1seg = 10;
						break;

					case ERROR:
					default:
						//rx_error++;
						//state = STAND_BY;
						break;
				}
				if (timer_1seg == 0)
					state = STAND_BY;
				else
					state = CONNECT;

				break;

			case TEMP_SENSE:
				break;

			default:
				state = STAND_BY;
				break;
		}
	}

	return 0;
}
//--- End of file ---//

unsigned short ADC_Conf (void)
{
	unsigned short cal = 0;
	ADC_InitTypeDef ADC_InitStructure;

	if (!RCC_ADC_CLK)
		RCC_ADC_CLK_ON;

	ADC_ClockModeConfig(ADC1, ADC_ClockMode_SynClkDiv4);

	// preseteo de registros a default
	ADC_DeInit(ADC1);
	ADC_StructInit(&ADC_InitStructure);
	ADC_Init(ADC1, &ADC_InitStructure);

	//software by setting bit ADCAL=1.
	//Calibration can only be initiated when the ADC is disabled (when ADEN=0).
	//ADCAL bit stays at 1 during all the calibration sequence.
	//It is then cleared by hardware as soon the calibration completes
	cal = ADC_GetCalibrationFactor(ADC1);

	// Enable ADC1
	ADC_Cmd(ADC1, ENABLE);

	return cal;
}

unsigned short ReadADC1 (unsigned char channel)
{
	GPIOA_PIN4_ON;
	// Set channel and sample time
	//ADC_ChannelConfig(ADC1, channel, ADC_SampleTime_7_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_16, ADC_SampleTime_239_5Cycles);

	// Start the conversion
	ADC_StartOfConversion(ADC1);
	// Wait until conversion completion
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	// Get the conversion value
	GPIOA_PIN4_OFF;	//tarda 20us en convertir
	return ADC_GetConversionValue(ADC1);
}

/* Temperature sensor calibration value address */
#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7C2))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7B8))
#define VDD_CALIB ((uint16_t) (330))
#define VDD_APPLI ((uint16_t) (300))

int GetTemp (unsigned short adc_temp)
{
    int32_t temperature; /* will contain the temperature in degree Celsius */
    //temperature = (((int32_t) ADC1->DR * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
    temperature = (((int32_t) adc_temp * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
    temperature = temperature * (int32_t)(110 - 30);
    temperature = temperature / (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
    temperature = temperature + 30;

    return temperature;

}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}
