/*
===============================================================================
 Name        : GPIOInterrupt.c
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

// TODO: insert other definitions and declarations here

int main(void) {

	NVIC_EnableIRQ(EINT3_IRQn); //Habilito interrupciono EINT3
	LPC_GPIOINT->IO0IntEnR |= (1<<10); //Habilito
	NVIC_SetPriority(IO0IntEnR, 1);

    // TODO: insert code here

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
void EINT3_IRQHandler(void){

	if((1<<10) & LPC_GPIO0->FIOPIN) LPC_GPIO0->FIOCLR = (1<<22);
	else LPC_GPIO0->FIOSET = (1<<22);

	NVIC_DisableIRQ(IO0IntEnR);
}
