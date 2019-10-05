/*
===============================================================================
 Name        : TeoricoTareaClase4.c
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
// TODO: insert other include files here

// TODO: insert other definitions and declarations here
void retardo(void);
int main(void) {

	/**************************************************************************
	 **								 Configuraciones                         **
	 * ************************************************************************/

	/*****Config de puertos******************************/
	//Seteo P2.10 en modo EINT0, para BUZZER
	LPC_PINCON->PINSEL4 |= (1<<20);
	LPC_PINCON->PINSEL4 &= ~(1<<21);

	//Seteo P2.8 en modo GPIO, esto no es necesario por que lo hace por defecto luego de un reset
	LPC_PINCON->PINSEL4 &= ~(1<<16);
	LPC_PINCON->PINSEL4 &= ~(1<<17);

	//Pull-up config, esto no es necesario por que lo hace por defecto luego de un reset
	LPC_PINCON->PINMODE4 &= ~(1<<20);
	LPC_PINCON->PINMODE4 &= ~(1<<21);
	LPC_PINCON->PINMODE4 &= ~(1<<16);
	LPC_PINCON->PINMODE4 &= ~(1<<17);

	//Seteo P0.22 y P2.7 como salida.
	LPC_GPIO0->FIODIR |= (1<<22);  //LED
	LPC_GPIO2->FIODIR |= (1<<7);   //BUZZER

	/*****************************************************/
	/******Configuracion del modo interrupcion******/

	//Congig EINT0
	LPC_SC->EXTMODE |= (1<<0); //Interrupcion por flanco.
	LPC_SC->EXTPOLAR |= (1<<0); //Flanco de subida.

	//Config GPIO INT
	LPC_GPIOINT->IO2IntEnR	|= (1<<8);

	/**********************************************/
	//NVIC config
	NVIC_SetPriority(EINT0_IRQn,3); // BUZZER
	NVIC_SetPriority(EINT3_IRQn, 6); //LED
	NVIC_EnableIRQ(EINT3_IRQn);
	NVIC_EnableIRQ(EINT0_IRQn);


    // Enter an infinite loop, just incrementing a counter
    while(1) {

    }
    return 0 ;
}
void EINT0_IRQHandler(void){
	// BAJO FLAG DE GPIOINT P2[10]
		LPC_SC->EXTINT |= (1<<0);
	//Enciendo el LED
		LPC_GPIO2->FIOSET |= (1 << 7);
		retardo();
	//Apago el LED
		LPC_GPIO2->FIOCLR |= (1 << 7);
}
void EINT3_IRQHandler(void){
	// BAJO FLAG DE GPIOINT P2[8]
	LPC_GPIOINT->IO2IntClr |= (1 << 8);
		//Enciendo el LED
			LPC_GPIO0->FIOSET |= (1 << 22);
			retardo();
		//Apago el LED
			LPC_GPIO0->FIOCLR |= (1 << 22);
}
void retardo(void) {
	uint32_t conta;
	for (conta = 0; conta < 50000000; conta++) {
	}

}
