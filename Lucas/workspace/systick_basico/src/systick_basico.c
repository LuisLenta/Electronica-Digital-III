/*
===============================================================================
 Name        : systick_basico.c
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

// TODO: insert other definitions and declarations here
#define ticks 1000000

int main(void) {
	LPC_GPIO0->FIODIR |= (1 << 22); //Pin 22 del Puerto 0 como de salida (en "1").
	SysTick_Config(ticks);

    while(1) { }
    return 0 ;
}

void SysTick_Handler(void) {
	static int i = 100;
	i--;

	if (i == 0) {
		i = 100;
		if ((1 << 22) & LPC_GPIO0->FIOPIN) {
			LPC_GPIO0->FIOCLR = (1 << 22);
		} else {
			LPC_GPIO0->FIOSET = (1 << 22);
		}
	}
}
