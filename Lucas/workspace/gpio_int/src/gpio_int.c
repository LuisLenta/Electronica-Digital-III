/*
===============================================================================
 Name        : gpio_int.c
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

	/* Configuración del pin2[10] en modo GPIO*/
	LPC_PINCON->PINSEL4 &= ~(1 << 20); // SETEO PIN20 EN 0
	LPC_PINCON->PINSEL4 &= ~(1 << 21); // SETEO PIN21 EN 0

	/* Configuración de resistencias internas */
	LPC_PINCON->PINMODE4 &= ~(1 << 20); // POR DEFECTO ES PULL-UP (00)
	LPC_PINCON->PINMODE4 &= ~(1 << 21);

	/* ***************************************************** */
	/*			 Configuración  IO0/2IntEnR 				 */
	/* ***************************************************** */
	// INTERRUPCIÓN POR FLANCO DE SUBIDA EN P0[10]
	LPC_GPIOINT->IO2IntEnF |= (1 << 10);

	/* ***************************************************** */
	/* 					Configuración de NVIC				 */
	/* ***************************************************** */

	/* Configuración de prioridades */
	NVIC_SetPriority(EINT3_IRQn, 1);

	/* Habilitación de interrupción */
	NVIC_EnableIRQ(EINT3_IRQn);

	/* ***************************************************** */
	/* 					Configuración de GPIO				 */
	/* ***************************************************** */

	/* Configuración del led */
	LPC_GPIO0->FIODIR |= (1 << 22); // Pin 22 del puerto 0 como salida
}

void EINT3_IRQHandler(void) {
	// BAJO FLAG DE GPIOINT P0[10]
	LPC_GPIOINT->IO2IntClr |= (1 << 10);

	/* Cambio el estado del pin P0[22] */
	if (LPC_GPIO0->FIOPIN & (1 << 22)) {
		LPC_GPIO0->FIOCLR = (1 << 22); }
	else {
		LPC_GPIO0->FIOSET = (1 << 22); }

}
