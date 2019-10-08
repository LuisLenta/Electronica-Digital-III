/*
===============================================================================
 Name        : pwm_adc.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : Realice el software necesario para generar una señal PWM en un
 	 	 	   pin de salida de la lpc1769 a utilizando el módulo TIMER0. Se
 	 	 	   desea un periodo de 500 micro segundos (us). A los fines de
 	 	 	   establecer el ciclo de trabajo, se deberá realizar una medición
 	 	 	   del ADC cada un segundo (1 sec) y este valor se utilizara para
 	 	 	   establecer el ciclo de trabajo de la señal PWM entre 0% y 100%,
 	 	 	   de manera que para un valor de tensión de entrada al ADC de
 	 	 	   Vent=0V (cero volts) el ciclo de trabajo del PWM sea 0% y cuando
 	 	 	   el valor de entrada Vent=3,3V al conversor A/D c el ciclo de
 	 	 	   trabajo será establecido al 100%.Considerar un pclock de 25Mhz
 	 	 	   e indique los valores de los registros MATCH.
 ===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "lpc17xx_adc.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"

// TODO: insert other definitions and declarations here
#define INPUT		0
#define OUTPUT		1
#define T			5
#define MAXRES		4095
#define clock		200000
#define LED			(1 << 25)
#define CHANNEL0	(1 << 23)

void init(void);
void adcConfig(void);
void gpioConfig(void);
void timerConfig(void);
void pinselConfig(void);
void systickConfig(void);

uint32_t convert(uint32_t);

static uint32_t MATVAL = 0;

int main(void) {
	init();
    volatile static int i = 0 ;
    while(1) { i++ ; }
    return 0 ;
}

void init(void) {
	adcConfig();
	gpioConfig();
	timerConfig();
	pinselConfig();
	systickConfig();
	TIM_Cmd(LPC_TIM0, ENABLE);
	FIO_ClearValue(PINSEL_PORT_3, LED);
}

void adcConfig(void) {
	ADC_Init(LPC_ADC, clock);						// Inicio y config. clock
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);	// Activo ch0
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,ENABLE);		// Activo int. ch9
	ADC_BurstCmd(LPC_ADC, DISABLE);					// Burst desactivado

	NVIC_EnableIRQ(ADC_IRQn);						// Activo int. ADC
}

void gpioConfig(void) {
	FIO_SetDir(PINSEL_PORT_3,LED,OUTPUT);			// Out -> Led
	FIO_SetDir(PINSEL_PORT_0,CHANNEL0,INPUT);		// In  -> Ch0
}

void timerConfig(void) {
	TIM_TIMERCFG_Type timer;						// Modo Timer
	timer.PrescaleOption = TIM_PRESCALE_USVAL;		// Modo useg
	timer.PrescaleValue = 1000;						// Pescaler 1000us = 1ms
	TIM_Init(LPC_TIM0,TIM_TIMER_MODE,&timer);		// Cargo config.

	TIM_MATCHCFG_Type match;
	match.MatchChannel = TIM_MR0_INT;				// Match 0
	match.MatchValue = MATVAL;						// MatchVal = 5
	match.IntOnMatch = ENABLE;						// Activo int. match
	match.ResetOnMatch = ENABLE;					// Reinicio en match
	match.StopOnMatch = DISABLE;					// Sigo contando en matc
	match.ExtMatchOutputType = TIM_EXTMATCH_LOW;	// Act. out match
	// En match bajo el pin MAT0.0 (LED verde)
	TIM_ConfigMatch(LPC_TIM0,&match);				// Cargo config.
	NVIC_EnableIRQ(TIMER0_IRQn);					// Act. int. tmr0
}

void pinselConfig(void) {
	PINSEL_CFG_Type pin_cfg;

	// Configuración pin P3.25 (LED)
	pin_cfg.Portnum = PINSEL_PORT_3;				// Puerto 3
	pin_cfg.Pinnum = PINSEL_PIN_25;					// Pin 25
	pin_cfg.Pinmode = PINSEL_PINMODE_PULLUP;		// Res. Pull-up
	pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;		// Sin OpenDrain
	pin_cfg.Funcnum = PINSEL_FUNC_0;				// GPIO

	PINSEL_ConfigPin(&pin_cfg);						// Cargo config,

	// Configuración pin P0.23 (ADC)
	pin_cfg.Portnum = PINSEL_PORT_0;				// Puerto 0
	pin_cfg.Pinnum = PINSEL_PIN_23;					// Pin 23
	pin_cfg.Pinmode = PINSEL_PINMODE_TRISTATE;		// Sin resist.
	pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;		// Sin OoenDrain
	pin_cfg.Funcnum = PINSEL_FUNC_1;				// ADC

	PINSEL_ConfigPin(&pin_cfg);						// Cargo config.

	// FALTA CONFIGURAR LOS PINES DEL MATCH (MAT0[0])
}

void systickConfig(void) {
	SYSTICK_InternalInit(10);						// Int. cada 10ms
	SYSTICK_IntCmd(ENABLE);							// Activo int.
	SYSTICK_Cmd(ENABLE);							// Activo la cuenta
}

void SysTick_Handler(void) {
	SYSTICK_ClearCounterFlag();						// Bajo flag
	static int i = 100;
	if (i == 0) {
		i = 100;
		ADC_StartCmd(LPC_ADC, ADC_START_NOW);		// Inicio conversión
	}
	i--;
}

void ADC_IRQHandler(void) {
	// Leo y convierto el valor
	MATVAL = convert(ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0));
}

void TIMER0_IRQHandler(void) {
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);		// Bajo flag
	if (GPIO_ReadValue(PINSEL_PORT_3) & LED) {		// Se apagó el led
		// Actualizo el match para la parte baja
		TIM_UpdateMatchValue(LPC_TIM0,TIM_MR0_INT,(T*100-T*MATVAL));
		// Cambio config para poner encender el led en match
		TIM_SetMatchExt(LPC_TIM0,TIM_MR0_INT,TIM_EXTMATCH_HIGH);
	} else { 										// Se encendió el led
		// Actualizo el match para la parte alta
		TIM_UpdateMatchValue(LPC_TIM0,TIM_MR0_INT,T*MATVAL);
		// Cambio config para poner encender el led en match
		TIM_SetMatchExt(LPC_TIM0,TIM_MR0_INT,TIM_EXTMATCH_LOW);
	}
}

uint32_t convert(uint32_t value) {
	return value * 100 / 4096;
}
