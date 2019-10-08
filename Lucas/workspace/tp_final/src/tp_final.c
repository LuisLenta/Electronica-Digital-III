/*
===============================================================================
 Name        : tp_final.c
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

// TODO: insert other include files here
#include "config.h"
#include "auxfunction.h"
#include "lpc17xx_exti.h"

int main(void) {
	CONFIG_Init();
    volatile static int i = 0 ;
    while(1) { i++ ; }
    return 0 ;
}


void EINT0_IRQHandler(void) {
	EXTI_ClearEXTIFlag(EXTI_EINT0);				// Flag off
	redLedOn();

}

void EINT1_IRQHandler(void) {
	EXTI_ClearEXTIFlag(EXTI_EINT1);				// Flag off
	greenLedOn();

}

void UART0_IRQHandler(void) {

}
