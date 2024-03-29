/*
===============================================================================
 Name        : EjercicioParcial_PWM.c
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
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_gpdma.h"

#define INPUT 		1
#define OUTPUT		0
#define FREQADC		200000				//200 KHz freq adc
#define MAXRES		4095				//Max res 12 bits
#define PINADC		(1<<0)				//Pin de entrada ADC
#define MAXPERC		100					//100%
#define GREENLED	(1<<25)

void configSystick(void);
void configPins(void);
void configGPIO(void);
void configTimer(void);
void configAdc(void);
void init(void);

static int MATVAL = 500;                //Match Value
static int ENCENDIDO = 1;

int main(void) {

	init();

    while(1) {}
    return 0 ;
}
void configPins(void){
	PINSEL_CFG_Type pin;

	//Configuro LED verde como salida
	pin.Portnum = PINSEL_PORT_3;
	pin.Pinnum = PINSEL_PIN_25;
	pin.Funcnum = PINSEL_FUNC_0;
	pin.Pinmode = PINSEL_PINMODE_PULLUP;
	pin.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pin);

	//Config entrada ADC
	pin.Portnum = PINSEL_PORT_0;
	pin.Pinnum = PINSEL_PIN_23;
	pin.Funcnum = PINSEL_FUNC_1;
	pin.Pinmode = PINSEL_PINMODE_TRISTATE;
	pin.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pin);

}
void configGPIO(void){

	GPIO_SetDir(PINSEL_PORT_3, GREENLED, OUTPUT);
	GPIO_SetDir(PINSEL_PORT_0, PINADC, INPUT);
}
void configSystick(void){
	SYSTICK_InternalInit(10);  //10ms
	SYSTICK_IntCmd(ENABLE);
	SYSTICK_Cmd(ENABLE);
}
void configAdc(void){
	ADC_Init(LPC_ADC,FREQADC);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
	ADC_BurstCmd(LPC_ADC, DISABLE);
	ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, SET);

	NVIC_EnableIRQ(ADC_IRQn);
}
void configTimer(void){
	TIM_TIMERCFG_Type timer;

	timer.PrescaleOption = TIM_PRESCALE_USVAL;  //Timer en microseg
	timer.PrescaleValue = 1000;    				//1000 us => 1 ms

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer);

	TIM_MATCHCFG_Type match;

	match.MatchChannel = ADC_CHANNEL_0;
	match.IntOnMatch = ENABLE;
	match.StopOnMatch = DISABLE;
	match.ResetOnMatch = ENABLE;
	match.MatchValue = MATVAL;					//Inicio el Match en 500ms
	TIM_ConfigMatch(LPC_TIM0, &match);
	NVIC_EnableIRQ(TIMER0_IRQn);
}
void init(void){
	configPins();
	configGPIO();
	configAdc();
	configSystick();
}
void ADC_IRQHandler(void){

	uint16_t ADC_Data = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0);
	MATVAL = (int) (ADC_Data *(MAXPERC/MAXRES));						//Calculo nuevo valor de match
	TIM_UpdateMatchValue(LPC_TIM0, ADC_CHANNEL_0, MATVAL);				//Actualizo match
	//En la primera conversion lanzo el timer
	if(ENCENDIDO){
		ENCENDIDO = 0;					//Para que no vuelva a entrar a este bloque
		configTimer();
		TIM_Cmd(LPC_TIM0,ENABLE);
	}
}
void TIMER0_IRQHandler(void){
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT); //Bajo flag

	if(MATVAL == 100){
		GPIO_SetValue(PINSEL_PORT_3, GREENLED);
	}
	else{
		TIM_UpdateMatchValue(LPC_TIM0, ADC_CHANNEL_0, MATVAL);
	}
}
void Systick_Handler(void){
	SYSTICK_ClearCounterFlag();

	static int i = 100;
	i--;

	if(i==0){
		i=100;
		ADC_StartCmd(LPC_ADC, ADC_START_NOW);
		GPIO_SetValue(PINSEL_PORT_3, GREENLED);
	}
}
