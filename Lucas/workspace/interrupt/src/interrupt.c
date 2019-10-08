/*
===============================================================================
 Name        : interrupt.c
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
void config(void);

int main(void) {
	config();
    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}

void config(void) {
	/* ***************************************************** */
	/* Configuración Pin Connect Block - PINSEL4[21:20] = 01 */
	/* ***************************************************** */

	/* Configuración del pin2[10] en modo EINT0*/
	LPC_PINCON->PINSEL4 |= (1 << 20); // SETEO PIN20 EN 1
	LPC_PINCON->PINSEL4 &= ~(1 << 21); // SETEO PIN21 EN 0

	/* Configuración de resistencias internas */
	LPC_PINCON->PINMODE4 &= ~(1 << 20); // POR DEFECTO ES PULL-UP (00)
	LPC_PINCON->PINMODE4 &= ~(1 << 21);

	/* ***************************************************** */
	/*			 Configuración System Control				 */
	/* ***************************************************** */
	/* Flag de interrupción */
	LPC_SC->EXTINT;

	/* Configuración de modo interrupción */
	LPC_SC->EXTMODE |= (1 << 0); // SETEO EL MODO COMO FLANCO EXTMODE[0]

	/* Configuración de tipo de flanco */
	LPC_SC->EXTPOLAR |= (1 << 0); // SETEO COMO FLANCO DE SUBIDA EXTPOLAR[0]

	/* ***************************************************** */
	/* 					Configuración de NVIC				 */
	/* ***************************************************** */

	/* Configuración de prioridades */
	NVIC_SetPriority(EINT0_IRQn, 1);

	/* Habilitación de interrupción */
	NVIC_EnableIRQ(EINT0_IRQn);

	/* ***************************************************** */
	/* 					Configuración de GPIO				 */
	/* ***************************************************** */

	/* Configuración del led */
	LPC_GPIO0->FIODIR |= (1 << 22); // Pin 22 del puerto 0 como salida
}

void EINT0_IRQHandler(void) {
	/* Cambio el estado del pin P0[22] */
	if (LPC_GPIO0->FIOPIN & (1 << 22)) {
		LPC_GPIO0->FIOCLR = (1 << 22); }
	else {
		LPC_GPIO0->FIOSET = (1 << 22); }

	/* Bajo el flag de interrupción por EINT0 */
	LPC_SC->EXTINT = 1 << 0;
}
