/*
===============================================================================
 Name        : TeoricoClase6_DAC.c
 Author      : $(Luis Lenta)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lpc17xx_dac.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_clkpwr.h"
#include <cr_section_macros.h>
// TODO: insert other include files here

// TODO: insert other definitions and declarations here
uint32_t pclock;
uint32_t pclksel;
void configTMR0(void);
void configDAC(void);

int main(void) {

	configTMR0();
	configDAC();
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
void configTMR0(void){
	TIM_TIMERCFG_Type prescaler;
		prescaler.PrescaleOption = TIM_PRESCALE_USVAL;
		prescaler.PrescaleValue = 1;
		TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &prescaler);

		INPUT
		TIM_MATCHCFG_Type match;
		match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
		match.IntOnMatch = ENABLE;
		match.MatchChannel = 0;
		match.MatchValue = 10; // 100 Hz -> 10000 us => 10000/1024 = 9.76 ~= 10
		match.ResetOnMatch = ENABLE;
		match.StopOnMatch = DISABLE;
		TIM_ConfigMatch(LPC_TIM0, &match);

		TIM_Cmd(LPC_TIM0, ENABLE);
		NVIC_EnableIRQ(TIMER0_IRQn);
}
void configDAC(void){
	PINSEL_CFG_Type dac;
		dac.Funcnum = PINSEL_FUNC_2;
		dac.OpenDrain = PINSEL_PINMODE_NORMAL;
		dac.Pinmode = PINSEL_PINMODE_TRISTATE;
		dac.Pinnum = PINSEL_PIN_26;
		dac.Portnum = PINSEL_PORT_0;
		PINSEL_ConfigPin(&dac);

		DAC_Init(LPC_DAC);
}
void TIMER0_IRQHandler() {
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
	NVIC_ClearPendingIRQ(TIMER0_IRQn);

	volatile static uint32_t contador = 1023;
	DAC_UpdateValue(LPC_DAC, 1023-contador);
	contador--;
	if (contador == 0 )
		contador = 1023;
}
