/*
===============================================================================
 Name        : blasco_tp1.c
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
#define SIZE 8

static int j = 100;
uint32_t array[SIZE];
uint32_t right = 1;

void config(void);
void arrayLoad(uint32_t index);
void portLoad(uint32_t index);

int main(void) {

    // TODO: insert code here
	for (int i = 0 ; i < SIZE ; i++) {
		uint32_t aux = 23;
		array[i] = (1 << (aux - i));
	}
	config();

    volatile static int i = 0 ;
    while(1) { i++; }
    return 0 ;
}

void config(void) {
	/* ***************************************************** */
	/* 			Configuración Pin Connect Block 			 */
	/* ***************************************************** */

	/* Configuración pin p2[10] como EINT0 */
	LPC_PINCON->PINSEL4 |= (1 << 20); // BIT 20 EN 1
	LPC_PINCON->PINSEL4 &= ~(1 << 21); // BIT 21 EN 0

	/* Configuración pin p2[8] como GPIO */
	LPC_PINCON->PINSEL4 &= ~(1 << 16); // BIT 16 EN 0
	LPC_PINCON->PINSEL4 &= ~(1 << 17); // BIT 17 EN 0

	/* Configuración de resistencias internas */
	LPC_PINCON->PINMODE4 &= ~(1 << 20); // PULL-UP DEFAULT (00)
	LPC_PINCON->PINMODE4 &= ~(1 << 21);
	LPC_PINCON->PINMODE4 &= ~(1 << 16);
	LPC_PINCON->PINMODE4 &= ~(1 << 17);

	/* ***************************************************** */
	/*			 Configuración System Control				 */
	/* ***************************************************** */

	/* Configuración de modo de int (EINT0) */
	LPC_SC->EXTMODE |= (1 << 0); // INT POR FLANCO

	/* Configuración de tipo de flanco (EINT0) */
	LPC_SC->EXTMODE = (1 << 0); // INT POR FLANCO DE SUBIDA

	/* ***************************************************** */
	/*			 Configuración  IO0/2IntEnR 				 */
	/* ***************************************************** */

	/* Interrupción por flanco de subida (GPIOINT) */
	LPC_GPIOINT->IO2IntEnR |= (1 << 8);

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
	/* 					Configuración SysTick				 */
	/* ***************************************************** */

	SysTick_Config(ticks);

	/* ***************************************************** */
	/* 					Configuración de GPIO				 */
	/* ***************************************************** */

	/* Configuración byte bajo de GPIO0 como salida */
	LPC_GPIO0->FIODIR0 = 255;
	/* Por defecto vienen configurados con resistencias PULL-UP */
}

void EINT3_IRQHandler(void) {
	right = ~right;
	LPC_GPIOINT->IO2IntClr |= (1 << 8); // BAJO FLAG DE GPIOINT P2[8]
}

void EINT0_IRQHandler(void) {
	j = j * 2;
	if (j/100 > SIZE) { j = 1; }
	LPC_SC->EXTINT=1; // BAJO FLAG DE EINT0 P2[10]
}

void SysTick_Handler(void) {
	static uint32_t index = 0;
	static int i = 100;
	i--;

	if (i == 0) {
		portLoad(index);
		i = 100 * j;
	}
}

void arrayLoad(uint32_t index) {
	LPC_GPIO0->FIOPIN2 = ((array[index] >> 16) && 0xFF);
}

void portLoad(uint32_t index) {
	switch(right) {
		case 1: // Recorro hacia la derecha
			arrayLoad(index++);
			if (index == SIZE) { index = 0; }
			break;
		case 0: // Recorro hacia la izquierda
			arrayLoad(index--);
			if (index == 0) { index = SIZE - 1; }
			break;
		default:
			break;
	}
}
