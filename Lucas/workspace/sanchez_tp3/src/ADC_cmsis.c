/*
===============================================================================
 Name        : sanchez_tp3.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

// TODO: insert other include files here
#include <cr_section_macros.h>
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_adc.h"

// TODO: insert other definitions and declarations here
#define INPUT		(uint32_t) 0
#define OUTPUT		(uint32_t) 1

#define PORT_ZERO	(uint32_t) 0
#define PORT_THREE	(uint32_t) 3

#define RED_LED 	(1<<22)
#define GREEN_LED	(1<<25)
#define BLUE_LED	(1<<26)

#define clock		200000
#define	ticks		1000000

void init(void);
void configADC(void);
void configGPIO(void);
void configSysTick(void);
void configTimer(void);
void encenderLEDS(uint16_t);
void apagarLEDS(void);
void prenderVerde(void);
void prenderAmarillo(void);
void prenderRojo(void);
void bajarMatch(void);

static int j = 0;

int main(void) {
    // TODO: insert code here
	init();

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) { i++ ; }
    return 0 ;
}

void init(void) {
	configGPIO();
	configSysTick();
	configTimer();
	configADC();
	return;
}

void configADC(void) {
	// Config. pin analógico
	PINSEL_CFG_Type analog_pin;

	analog_pin.Portnum = PINSEL_PORT_0; 	// Puerto 0
	analog_pin.Pinnum = PINSEL_PIN_23;		// Pin 23 (ADC0[0])
	analog_pin.Funcnum = PINSEL_FUNC_1;		// ADC
	analog_pin.Pinmode = PINSEL_PINMODE_TRISTATE; // Res. int Pullup
	analog_pin.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&analog_pin);
	GPIO_SetDir(PINSEL_PORT_0, PINSEL_PIN_23, INPUT);

	// Config. del módulo ADC
	ADC_Init(LPC_ADC, clock);						// Config. clock
	ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, SET);		// Config. int. ch0
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE); // Habilito ch0
	ADC_BurstCmd(LPC_ADC, DISABLE);					// Ráfaga desactivado
	//LPC_ADC->ADINTEN&=~(1<<8);

	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_SetPriority(ADC_IRQn, 9);
}

void configGPIO(void) {
	PINSEL_CFG_Type red;					// Creo las instancias
	PINSEL_CFG_Type green;
	PINSEL_CFG_Type blue;

	red.Portnum = PINSEL_PORT_0;			// Puerto 0
	red.Pinnum = PINSEL_PIN_22;				// Pin 22
	red.Pinmode = PINSEL_PINMODE_PULLUP;	// Res. int Pullup
	red.OpenDrain = PINSEL_PINMODE_NORMAL;	// OpenDrain Normal
	red.Funcnum = PINSEL_FUNC_0;			// GPIO

	PINSEL_ConfigPin(&red);					// Cargo la config.

	green.Portnum = PINSEL_PORT_3;			// Puerto 3
	green.Pinnum = PINSEL_PIN_25;			// Pin 25
	green.Pinmode = PINSEL_PINMODE_PULLUP;	// Res. int Pullup
	green.OpenDrain = PINSEL_PINMODE_NORMAL;// OpenDrain Normal
	green.Funcnum = PINSEL_FUNC_0;			// GPIO

	PINSEL_ConfigPin(&green);				// Cargo la config.

	blue.Portnum = PINSEL_PORT_3;			// Puerto 3
	blue.Pinnum = PINSEL_PIN_26;			// Pin 26
	blue.Pinmode = PINSEL_PINMODE_PULLUP;	// Res. int Pullup
	blue.OpenDrain = PINSEL_PINMODE_NORMAL;	// OpenDrain Normal
	blue.Funcnum = PINSEL_FUNC_0;			// GPIO

	PINSEL_ConfigPin(&blue);				// Cargo la config.

	GPIO_SetDir(PINSEL_PORT_0, RED_LED, OUTPUT); // Salida
	GPIO_SetDir(PINSEL_PORT_3, GREEN_LED, OUTPUT);
	GPIO_SetDir(PINSEL_PORT_3, BLUE_LED, OUTPUT);
}

void configSysTick(void) {
	SysTick_Config(ticks);
	return;
}

void configTimer(void) {
	TIM_TIMERCFG_Type timer;
	TIM_MATCHCFG_Type match;

	timer.PrescaleOption = TIM_PRESCALE_USVAL;
	timer.PrescaleValue  = 1000;

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer);

	match.MatchChannel = 0;
	match.IntOnMatch = ENABLE;
	match.ResetOnMatch = ENABLE;
	match.StopOnMatch = DISABLE;
	match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	match.MatchValue = 10000;

	TIM_ConfigMatch(LPC_TIM0, &match);

	NVIC_SetPriority(TIMER0_IRQn, 20);
	NVIC_EnableIRQ(TIMER0_IRQn);
	return;
}

void TIMER0_IRQHandler(void) {
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
	j = 1;
	return;
}

void SysTick_Handler(void) {
	static int i = 100;
	i--;

	if (i == 0) { // Tomo muestras del ADC cada 1 segundo
		i = 100;
		ADC_StartCmd(LPC_ADC, ADC_START_NOW);		// Start ADC
	}
	// Parpadeo led rojo cada 5ms
	if ((i == 0 || i == 50) && (j == 1)) {
		if (GPIO_ReadValue(PORT_ZERO)&RED_LED) {
			GPIO_ClearValue(PORT_ZERO, RED_LED);
		} else {
			GPIO_SetValue(PORT_ZERO, RED_LED);
		}
	}
	return;
}

void ADC_IRQHandler(void) {
	uint16_t valor = ADC_ChannelGetData(LPC_ADC, ADC_ADINTEN0); // Leo ADC
	encenderLEDS(valor * 100 / 4096);							// Convierto a °C
	return;
}

void encenderLEDS(uint16_t valor) {
	apagarLEDS();
	if (valor < 50) {
		prenderVerde();
		bajarMatch();
	} else if (valor < 80) {
		prenderAmarillo();
		bajarMatch();
	} else {
		prenderRojo();
		TIM_Cmd(LPC_TIM0, ENABLE);
	}
	return;
}

void apagarLEDS(void) {
	GPIO_SetValue(PORT_ZERO, RED_LED);
	GPIO_SetValue(PORT_THREE, GREEN_LED);
	GPIO_SetValue(PORT_THREE, BLUE_LED);
}

void prenderVerde(void) {
	GPIO_ClearValue(PINSEL_PORT_3, GREEN_LED);
}

void prenderAmarillo(void) {
	GPIO_ClearValue(PINSEL_PORT_3, GREEN_LED);
	GPIO_ClearValue(PINSEL_PORT_0, RED_LED);
}

void prenderRojo(void) {
	GPIO_ClearValue(PINSEL_PORT_0, RED_LED);
}

void bajarMatch(void) {
	TIM_Cmd(LPC_TIM0, DISABLE);
	j = 0;
}
