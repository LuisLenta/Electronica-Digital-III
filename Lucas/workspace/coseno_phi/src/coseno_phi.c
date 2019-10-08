/*
===============================================================================
 Name        : coseno_phi.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : 	1) Diseñe una función cuyo algoritmo encuentre los máximos de ambas formas de onda y
					mida y almacene la diferencia temporal entre máximos en un registro.
				2) Diseñe un algoritmo que a partir de la función anterior calcule el promedio de 100
					mediciones de la diferencia temporal entre ambas formas de onda y a partir de ese
					valor calcule φ y almacene el resultado en un registros. Recuerde que para 2π=360=>20ms (50hz)
				3) Diseñe una función que encienda un led si la corriente está atrasada respecto a la
					tensión de alimentación o encienda otro led en caso contrario.
				4) Se quiere que todo lo mencionado antes se ejecute si se presiona un pulsador
					(interrupción externa)
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "lpc17xx_systick.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_adc.h"

// TODO: insert other definitions and declarations here
#define INPUT		0
#define OUTPUT		1
#define SIZE		100
#define	clock		200000
#define EXTI0_BTN	(1 << 10)
#define	RED_LED		(1 << 22)
#define GREEN_LED	(1 << 25)
#define CHANNEL0	(1 << 23)
#define CHANNEL1	(1 << 24)

void systickConfig(void);
void pinselConfig(void);
void timerConfig(void);
void extiConfig(void);
void gpioConfig(void);
void adcConfig(void);
void init(void);

uint32_t convertADC(uint16_t);
uint32_t convert2dgr(uint32_t);
void calculatePhi(void);
void calculateAVG(void);
void turnOnLed(uint32_t);
void ledRedOn(void);
void ledGreenOn(void);

static uint32_t ON = 0;
static uint32_t array[SIZE];
static uint32_t AVG = 0;

int main(void) {
	init();

    volatile static int i = 0 ;
    while(1) { i++; }
    return 0 ;
}

void init(void) {
	systickConfig();
	pinselConfig();
	timerConfig();
	extiConfig();
	gpioConfig();
	adcConfig();
}

void systickConfig(void) {
	SYSTICK_InternalInit(10);
	SYSTICK_IntCmd(ENABLE);
}

void adcConfig(void) {
	ADC_Init(LPC_ADC, clock);					// Inicio y config. clock
	ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, SET);	// Activo int. ch0
	ADC_IntConfig(LPC_ADC, ADC_ADINTEN1, SET);	// Activo int. ch1
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE); // Activo ch0
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_1, ENABLE); // Activo ch1
	ADC_BurstCmd(LPC_ADC, ENABLE); 				// Activo BurstMode

	NVIC_EnableIRQ(ADC_IRQn);					// Activo int. por ADC
}

void pinselConfig(void) {
	PINSEL_CFG_Type pin_cfg;

	// Configuración pin P0.23 (Ch0 ADC)
	pin_cfg.Portnum = PINSEL_PORT_0; 			// Puerto 0
	pin_cfg.Pinnum = PINSEL_PIN_23;				// Pin 23
	pin_cfg.Pinmode = PINSEL_PINMODE_TRISTATE;	// Sin resistencia
	pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;	// Sin OpenDrain
	pin_cfg.Funcnum = PINSEL_FUNC_1;			// Función 1 (ADC)

	PINSEL_ConfigPin(&pin_cfg);					// Cargo config.

	// Configuración pin P0.24 (Ch1 ADC)
	pin_cfg.Portnum = PINSEL_PORT_0;			// Puerto 0
	pin_cfg.Pinnum = PINSEL_PIN_24;				// Pin 24
	pin_cfg.Pinmode = PINSEL_PINMODE_TRISTATE;	// Sin resistencia
	pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;	// Sin OpenDrain
	pin_cfg.Funcnum = PINSEL_FUNC_1;			// Función 1 (ADC)

	PINSEL_ConfigPin(&pin_cfg);					// Cargo config.

	// Configuración pin P0.22 (LED ROJO)
	pin_cfg.Portnum = PINSEL_PORT_0;			// Puerto 0
	pin_cfg.Pinnum = PINSEL_PIN_22;				// Pin 22
	pin_cfg.Pinmode	= PINSEL_PINMODE_TRISTATE;	// Sin resistencia
	pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;	// Sin OpenDrain
	pin_cfg.Funcnum = PINSEL_FUNC_0;			// Función 0 (GPIO)

	PINSEL_ConfigPin(&pin_cfg);					// Cargo config.

	// Configuración pin P3.25 (LED VERDE)
	pin_cfg.Portnum = PINSEL_PORT_3;			// Puerto 3
	pin_cfg.Pinnum = PINSEL_PIN_25;				// Pin 25
	pin_cfg.Pinmode = PINSEL_PINMODE_TRISTATE;	// Sin resistencia
	pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;	// Sin OpenDrain
	pin_cfg.Funcnum = PINSEL_FUNC_0;			// Función 0 (GPIO)

	PINSEL_ConfigPin(&pin_cfg);					// Cargo config.
}

void gpioConfig(void) {
	GPIO_SetDir(PINSEL_PORT_0, RED_LED, OUTPUT); 	// Out -> Led rojo
	GPIO_SetDir(PINSEL_PORT_3, GREEN_LED, OUTPUT);	// Out -> Led verde
	GPIO_SetDir(PINSEL_PORT_2, EXTI0_BTN, INPUT);	// In -> Pulsador
	GPIO_SetDir(PINSEL_PORT_0, CHANNEL0, INPUT);	// In -> Ch0
	GPIO_SetDir(PINSEL_PORT_0, CHANNEL1, INPUT);	// In -> Ch1
}

void timerConfig(void) {
	TIM_TIMERCFG_Type timer;

	timer.PrescaleOption = TIM_PRESCALE_USVAL;	// Modo useg
	timer.PrescaleValue = 1000;					// 1000us = 1ms

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer);	// Cargo config.
}

void extiConfig(void) {
	EXTI_InitTypeDef exti_cfg;

	// Configuración EXTI0
	exti_cfg.EXTI_Line = EXTI_EINT0;				// EINT0
	exti_cfg.EXTI_Mode = EXTI_MODE_LEVEL_SENSITIVE;	// Por nivel bajo
	exti_cfg.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;

	EXTI_Config(&exti_cfg);							// Cargo config.
}

void SysTick_Handler(void) {
	SYSTICK_ClearCounterFlag();
	static int i = 100;
	i--;
	if (i == 0) {
		i = 100;
		calculateAVG();
	}
}

void EINT0_IRQHandler(void) {
	EXTI_ClearEXTIFlag(EXTI_EINT0);				// Bajo flag
	// Verifico el estado del sistema
	if (ON == 0) { // Si está apagado, lo enciendo
		ADC_StartCmd(LPC_ADC, ADC_START_CONTINUOUS);
	} else { // Si está encendido, lo apago
		ADC_StartCmd(LPC_ADC, ADC_START_NOW);
	}
	ON = ~ON;									// Cambio de estado flag
}

void ADC_IRQHandler(void) {
	static int i = 0;						// Corriente
	static int v = 0;						// Tensión

	// Verifico qué canal interrupió
	if (ADC_ChannelGetStatus(LPC_ADC, ADC_CHANNEL_0, SET)) {
		// Leo el valor convertido e inicio el timer si es pico
		uint32_t aux = convertADC(ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0));
		(aux > i) ? i = aux : TIM_Cmd(LPC_TIM0, ENABLE) ;
	} else { // Leo el valor convertido y detengo el timer si es pico
		uint32_t aux = convertADC(ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_1));
		(aux > v) ? v = aux : calculatePhi() ;
		i=3; v=3;
	}
}

void calculatePhi(void) {
	TIM_Cmd(LPC_TIM0, DISABLE);				// Detengo el timer
	static int j = 0;						// Contador del arreglo
	array[j] = convert2dgr(LPC_TIM0->TC);	// Guardo el valor en grados
	turnOnLed(LPC_TIM0->TC);				// Enciendo led correspondiente
	(j == SIZE-1) ? j = 0 : j++ ;			// Me desplazo en el array
	TIM_ResetCounter(LPC_TIM0);				// Reseteo TMR0
}

void calculateAVG(void) {
	uint32_t aux = 0;
	for (int i = SIZE ; i > 0 ; i--) {
		aux = aux + array[i-1];				// Sumo los val del array
	}
	AVG = aux / SIZE;						// Calculo el promedio
}

void turnOnLed(uint32_t value) {
	(value >= 10) ? ledRedOn() : ledGreenOn() ;
}

uint32_t convert2dgr(uint32_t value) {
	return value * 360 / 20;				// ms a grados
}

uint32_t convertADC(uint16_t value) {
	return value * 330 / 4096;
}

void ledRedOn(void) {
	GPIO_ClearValue(PINSEL_PORT_0, RED_LED);	// Si i atrasa a v prendo
	GPIO_SetValue(PINSEL_PORT_3, GREEN_LED);	// el led rojo
}

void ledGreenOn(void) {
	GPIO_SetValue(PINSEL_PORT_0, RED_LED);		// Si i adelanta a v prendo
	GPIO_ClearValue(PINSEL_PORT_3, GREEN_LED);	// el led verde
}
