/*
===============================================================================
 Name        : sanchez_tp1.c
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
static int j = 0;
static int i = 100;

void config(void);
void test(void);
void error(void);

int main(void) {
	// Rutina de configuración de Puertos e Interrupciones
	config();
	// Rutina de testeo de leds
	test();

	// Terminado el test habilito int. Externa
	NVIC_EnableIRQ(EINT0_IRQn);

    while(1) { }
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
	NVIC_SetPriority(EINT0_IRQn, 10);

	/* ***************************************************** */
	/* 					Configuración de GPIO				 */
	/* ***************************************************** */

	/* Configuración del led ROJO */
	LPC_GPIO0->FIODIR |= (1 << 22); // Pin 22 del puerto 0 como salida
	/* Configuración del led AZUL */
	LPC_GPIO3->FIODIR |= (1 << 26); // Pin 26 del puerto 3 como salida
	/* Confiugración del led VERDE */
	LPC_GPIO3->FIODIR |= (1 << 25); // Pin 25 del puerto 3 como salida
}

void test(void) {
	/* Rutina de testeo de leds */

	SysTick_Config(ticks); // Activo Systick

	LPC_GPIO0->FIOSET |= (1 << 22); // Enciendo LED ROJO
	while(LPC_GPIO0->FIOPIN & (1 << 22)) {} // Espero que se apague el led

	LPC_GPIO3->FIOSET |= (1 << 26); // Enciendo LED AZUL
	while(LPC_GPIO3->FIOPIN & (1 << 26)) {} // Espero que se apague el led

	LPC_GPIO3->FIOSET |= (1 << 25); // Enciendo LED VERDE
	while(LPC_GPIO3->FIOPIN & (1 << 25)) {} // Espero que se apague el led
}

void error(void) {
	/* Rutina de error (parpadeo de leds) */
	NVIC_DisableIRQ(EINT0_IRQn); 	// Desactivo int. Externas
	LPC_GPIO0->FIOSET |= (1 << 22); // Enciendo LED ROJO
	while(LPC_GPIO0->FIOPIN & (1 << 22)) {} // Espero que se apague el led
}

void EINT0_IRQHandler(void) {
	// Chequeo el estado de j para saber el estado
	switch(j) {
		// Inicio la cuenta de prueba
		case 0:
			LPC_GPIO0->FIOSET |= (1 << 22); // Enciendo LED ROJO
			LPC_GPIO3->FIOSET |= (1 << 26); // Enciendo LED AZUL
			LPC_GPIO3->FIOSET |= (1 << 25); // Enciendo LED VERDE
			i = 500; 						// Aumento el límite de cuenta
			j++;
			break;
		case 1:
			LPC_GPIO3->FIOSET |= (1 << 25); // Enciendo LED VERDE
			j++;
			break;
		case 2:
			LPC_GPIO3->FIOSET |= (1 << 26); // Enciendo LED VERDE
			j++;
			break;
		case 3:
			LPC_GPIO0->FIOSET |= (1 << 22); // Enciendo LED ROJO
			j++;
			break;
		case 4:
			for (int k = 0 ; k < 2 ; i++) { // Parpadeo 2 veces
				LPC_GPIO0->FIOSET |= (1 << 22); // Enciendo LED ROJO
				LPC_GPIO3->FIOSET |= (1 << 26); // Enciendo LED AZUL
				LPC_GPIO3->FIOSET |= (1 << 25); // Enciendo LED VERDE
				while(LPC_GPIO0->FIOPIN & (1 << 22)) {} // Espero que se apague el led
			}
			j++;
			break;
		default:
			break;
	}

	/* Bajo el flag de interrupción por EINT0 */
	LPC_SC->EXTINT = 1 << 0;
}

void SysTick_Handler(void) {
	i--;

	if (i == 0) {
		i = 100;
		LPC_GPIO0->FIOCLR = (1 << 22);
		LPC_GPIO3->FIOCLR = (1 << 26);
		LPC_GPIO3->FIOCLR = (1 << 25);
		if (j==1) { error(); }
	}
}
