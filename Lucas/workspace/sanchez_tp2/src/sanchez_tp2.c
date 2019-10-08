/*
===============================================================================
 Name        : sanchez_tp2.c
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
void retardo(void);

int main(void) {
	// Rutina de configuración de Puertos e Interrupciones
	config();

    while(1) { }
    return 0 ;
}

void config(void) {
	/* ***************************************************** */
	/* 			Configuración Pin Connect Block 			 */
	/* ***************************************************** */

	/* Configuración del pin2[10] en modo EINT0*/
	LPC_PINCON->PINSEL4 |= (1 << 20); // SETEO PIN20 EN 1
	LPC_PINCON->PINSEL4 &= ~(1 << 21); // SETEO PIN21 EN 0

	/* Configuración del pin2[8] en modo GPIO */
	LPC_PINCON->PINSEL4 &= ~(1 << 16); // SETEO BIT16 EN 0
	LPC_PINCON->PINSEL4 &= ~(1 << 17); // SETEO BIT16 EN 0

	/* Configuración de resistencias internas */
	LPC_PINCON->PINMODE4 &= ~(1 << 20); // POR DEFECTO ES PULL-UP (00)
	LPC_PINCON->PINMODE4 &= ~(1 << 21);
	LPC_PINCON->PINMODE4 &= ~(1 << 16);
	LPC_PINCON->PINMODE4 &= ~(1 << 17);

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
	/*			 Configuración  IO0/2IntEnR 				 */
	/* ***************************************************** */

	// INTERRUPCIÓN POR FLANCO DE BAJADA EN P2[8]
	LPC_GPIOINT->IO2IntEnF |= (1 << 8);

	/* ***************************************************** */
	/* 					Configuración de NVIC				 */
	/* ***************************************************** */

	/* Configuración de prioridades */
	NVIC_SetPriority(EINT0_IRQn, 3); // BUZZER
	NVIC_SetPriority(EINT3_IRQn, 6); // LED

	/* Habilitación de interrupción */
	NVIC_EnableIRQ(EINT0_IRQn);
	NVIC_EnableIRQ(EINT3_IRQn);

	/* ***************************************************** */
	/* 					Configuración de GPIO				 */
	/* ***************************************************** */

	/* Configuración del led ROJO */
	LPC_GPIO0->FIODIR |= (1 << 22); // Pin 22 del puerto 0 como salida
	/* Configuración de BUZZER */
	LPC_GPIO2->FIODIR |= (1 << 7); // Pin 7 del puerto 2 como salida

	/* ***************************************************** */
	/* 					Config. Estado Inicial				 */
	/* ***************************************************** */
	LPC_GPIO0->FIOSET = (1 << 22); // Apago led -> P0[22]
	LPC_GPIO2->FIOSET = (1 << 7); // Apago led -> P2[7]
	LPC_GPIO2->FIOSET = (1 << 8); // Apago pin -> P2[8]
}

void EINT3_IRQHandler(void) {
	LPC_GPIOINT->IO2IntClr |= (1 << 8); // BAJO FLAG DE GPIOINT P2[8]

	LPC_GPIO2->FIOCLR = (1 << 7); // Enciendo led -> P2[7]
	retardo();
	LPC_GPIO2->FIOSET = (1 << 7); // Apago led -> P2[7]
}

void EINT0_IRQHandler(void) {
	LPC_SC->EXTINT = 1 << 0; // BAJO FLAG DE EINT0 P2[10]

	LPC_GPIO0->FIOCLR = (1 << 22); // Enciendo led -> P0[22]
	retardo();
	LPC_GPIO0->FIOSET = (1 << 22); // Apago led -> P0[22]
}

void retardo(void) {
	for (int i = 0 ; i < 10000000 ; i++) {}
}
