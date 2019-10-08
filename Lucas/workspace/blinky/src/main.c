/*
 ===============================================================================
 Name        : blinky.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h" //Directiva de preprocesador para incluir un archivo
#endif

#include <cr_section_macros.h>

void retardo(void); //prototipo de la función de retardo. Se declara como
//de tipo void porque no retorna ningún resultado.

int main(void) { //función main. Cuerpo principal del código.
	uint32_t verClk;
	verClk = SystemCoreClock; //se verifica la frecuencia del cpu.
	LPC_GPIO0->FIODIR |= (1 << 22); //define al pin 22 del Puerto 0 como de salida (en "1").
	while (1) { //lazo que se repite indefinidamente porque su condición es verdadera siempre
		if (LPC_GPIO2->FIOPIN & (1 << 10)) {
			LPC_GPIO0->FIOSET = (1 << 22); //pone en alto al pin 22, encendiendo el led.
			retardo(); //invoca a la función retardo().
			LPC_GPIO0->FIOCLR = (1 << 22); //pone en cero al pin 22, apagando el led.
			retardo(); //invoca a la función retardo().
		} else {
			LPC_GPIO0->FIOSET = (1 << 22); //pone en alto al pin 22, encendiendo el led.
			retardo(); //invoca a la función retardo().
			retardo();
			retardo();
			LPC_GPIO0->FIOCLR = (1 << 22); //pone en cero al pin 22, apagando el led.
			retardo(); //invoca a la función retardo().
			retardo();
			retardo();
		}
	} // fin del while
	return 0;
} // fin del main

void retardo(void) { //prototipo de la función retardo.
	uint32_t conta, ver; //declara variable entera conta y ver.
	ver = sizeof(conta); //se verifica el tamaño, en bytes, de la varible conta
	for (conta = 0; conta < 1000000; conta++) {
	} //lazo for de retardo
	return; // fin de la función retardo y regreso a main
}
