/*
===============================================================================
 Name        : Clase3-Practico.c
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
#define uint32_t sysTickCount 10000000;

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

/*int main(void) {
	/**
	 * Config led and systick
	 */
	/*/LPC_GPIO0->FIODIR |= (1 << 22); //Seteo el pin 22 del puerto 0 como salida, para prender y apagar LED rojo
	LPC_GPIO0->FIOCLR = 1<<22; //Hago que arranque en bajo
	SysTick_Config(10000000);  //Inicio el systick

	/**
	 * Config pin where is conected the button and the external interrupt
	 */
	//LPC_PINCON->PINSEL4
    // Force the counter to be placed into memory
  /*  volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }

    return 0 ;
}*/
void SysTick_Handler(void){
	static int i = 0;
	i++;
	//if(i < countPressBtn && i < 100){
	if(i < 100){
		LPC_GPIO0->FIOSET = 1<<22; // Pongo en alto la salida
	}
	else{
		LPC_GPIO0->FIOCLR = 1<<22;
	}
}
