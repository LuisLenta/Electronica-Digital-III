/*
===============================================================================
 Name        : EjercicioParcial_Horno.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "lpc17xx_adc.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"

#define OUTPUT 	  	1
#define INPUT 	 	0
#define LED 		(1<<0)
#define FREQ_ADC    200000
#define SIZE 		100
#define ADDRDATO	0x1000025   //Direccion donde se guarda la temperatura deseada
#define MAXTEMP		160
#define MAXRES		4095
#define PERCENT		10

void configPins(void);
void configGPIO(void);
void configADC(void);
void configSysTick(void);


void checkAndSaveTEMP(uint16_t TMP);


static float array[SIZE];
float volatile *const tRef = (float*) ADDRDATO;

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {

    volatile static int i = 0 ;
    while(1) {
        i++ ;
    }
    return 0 ;
}

void configPins(void){

	PINSEL_CFG_Type pin;

	pin.Portnum = PINSEL_PORT_0;             //Puerto 0
	pin.Pinnum 	= PINSEL_PIN_0;				 //Pin 0
	pin.Funcnum = PINSEL_FUNC_0;			 //Funcion de GPIO
	pin.OpenDrain = PINSEL_PINMODE_NORMAL; 	 //Sin opendrain
	pin.Pinmode  = PINSEL_PINMODE_PULLDOWN;  //Pulldown por tipo de funcionamiento

	PINSEL_ConfigPin(&pin);

	//La configuracion de este pin no es necesaria.
	pin.Portnum = PINSEL_PORT_0;
	pin.Pinnum = PINSEL_PIN_23;
	pin.Funcnum = PINSEL_FUNC_1;
	pin.OpenDrain = PINSEL_PINMODE_NORMAL;
	pin.Pinmode = PINSEL_PINMODE_TRISTATE;

	PINSEL_ConfigPin(&pin);

}
void configGPIO(void){
	GPIO_SetDir(PINSEL_PORT_0, (1<<0), OUTPUT);
	GPIO_SetDir(PINSEL_PORT_0, (1<<23), INPUT);
}
void configADC(void){

	ADC_Init(LPC_ADC, FREQ_ADC);   //Clock de ADC en 200Hz
	ADC_BurstCmd(LPC_ADC,DISABLE);   //Desactivo modo burst, no es necesario ya que esta asi por defecto despues de un reset
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,ENABLE);


	NVIC_EnableIRQ(ADC_IRQn);
}
void configSysTick(void){
	SYSTICK_InternalInit(10);
	SYSTICK_IntCmd(ENABLE);
	SYSTICK_Cmd(ENABLE);
}
void ADC_IRQHandler (void){

	uint16_t ADC_Data = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0);
	checkAndSaveTEMP(ADC_Data);
}
void SysTick_Handler (void){
	SYSTICK_ClearCounterFlag();
	static int i = 100;
	i--;
	if(i==0){
		i=100;
		ADC_StartCmd(LPC_ADC,ADC_START_NOW);
	}
}
void checkAndSaveTEMP(uint16_t value){

	static int i = SIZE - 1;
	(i < 0) ? i = SIZE - 1 : i--;					//Si ya llene todo el arreglo vuelvo a empezar.
	float temp = ((value * MAXTEMP) / MAXRES);		//Convierto a grados el valor tomado por el ADC
	array[i] = temp;								//Guardo el valor

	float minTemp = (*tRef - (*tRef/PERCENT));   	//Temperatura minima, es decir, cuando baja del 10%


	if(temp < minTemp)
		GPIO_SetValue(PINSEL_PORT_0, LED);			//Si la temperatura decendio mas del 10%, entonces, prendo el LED
	if(temp > *tRef)
		GPIO_ClearValue(PINSEL_PORT_0,LED);			//Si la temperatura es mayor a la de referencia, entonces, apago el LED.

}
