/*
===============================================================================
 Name        : PracticoClase5_Timer.c
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
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#define INPUT		(uint32_t) 0
#define OUTPUT		(uint32_t) 1

void init(void);
void config_GPIO(void);
void config_TMR0(void);
int main(void) {

	init();
    // TODO: insert code here

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
void init(void){
	config_GPIO();
	config_TMR0();
}
void config_GPIO(void){
	PINSEL_CFG_Type gpio;

	gpio.Portnum = PINSEL_PORT_0;
	gpio.Pinnum = PINSEL_PIN_22;
	gpio.Pinmode = PINSEL_PINMODE_PULLUP;
	gpio.Funcnum = PINSEL_FUNC_0;
	gpio.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&gpio);
	GPIO_SetDir(PINSEL_PORT_0, (1<<22), OUTPUT);
}

void config_TMR0(void){
	TIM_TIMERCFG_Type timer;
	TIM_MATCHCFG_Type match;

	timer.PrescaleOption = TIM_PRESCALE_USVAL; // Activo en modo us (micro seg)
	timer.PrescaleValue = 10; // Voy a contar a los 1000 micro seg (1 ms)

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer);

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

