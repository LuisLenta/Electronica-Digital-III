/*
===============================================================================
 Name        : timer0_basico.c
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
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

// TODO: insert other definitions and declarations here
#define INPUT		(uint32_t) 0
#define OUTPUT		(uint32_t) 1
#define PORT_ZERO	(uint32_t) 0
#define RED_LED 	((uint32_t) (1<<22))

void init(void);
void configTMR0(void);
void configGPIO(void);

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
	configTMR0();
	configGPIO();
	return;
}

void configTMR0(void) {
	TIM_TIMERCFG_Type timer;
	TIM_MATCHCFG_Type match;

	timer.PrescaleOption = TIM_PRESCALE_USVAL; // Activo en modo us (micro seg)
	timer.PrescaleValue = 10; // Voy a contar a los 1000 micro seg (1 ms)

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer); // Inicializo el timer

	match.MatchChannel = 0;
	match.IntOnMatch = ENABLE;
	match.ResetOnMatch = ENABLE;
	match.StopOnMatch = DISABLE;
	match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	match.MatchValue = 10000;

	TIM_ConfigMatch(LPC_TIM0, &match);

	TIM_Cmd(LPC_TIM0, ENABLE);

	NVIC_EnableIRQ(TIMER0_IRQn);
	return;
}

void configGPIO(void) {
	PINSEL_CFG_Type gpio;

	gpio.Portnum = PINSEL_PORT_0;			// Puerto 0
	gpio.Pinnum = PINSEL_PIN_22;			// Pin 22
	gpio.Pinmode = PINSEL_PINMODE_PULLUP;	// Res. Int. Pull-Up
	gpio.Funcnum = PINSEL_FUNC_0;			// Func. GPIO
	gpio.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&gpio);
	GPIO_SetDir(PORT_ZERO, RED_LED, OUTPUT);
}

void TIMER0_IRQHandler(void) {
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);

	if (GPIO_ReadValue(PORT_ZERO)&RED_LED) {
		GPIO_ClearValue(PORT_ZERO, RED_LED);
	} else {
		GPIO_SetValue(PORT_ZERO, RED_LED);
	}

	return;
}
