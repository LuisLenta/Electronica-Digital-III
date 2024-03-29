/*
===============================================================================
 Name        : EjercicioParcial_2Display.c
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
#include "lpc17xx_exti.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_gpio.h"

#define INPUT 			0
#define OUTPUT 			1
#define DISP1			(1 << 7)	// Cátodo común
#define DISP2			(1 << 8)	// Cátodo común


void init(void);
void configEXTI(void);
void configPINSEL(void);
void configSYSTICK(void);
void configGPIO(void);
void disp1_ON(void);
void disp2_ON(void);
uint32_t volatile descena;
uint32_t volatile unidad;
uint8_t display[10] = { 0x03F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67 };

int main(void) {

	init();
    volatile static int i = 0 ;
    while(1) {
        i++ ;
    }
    return 0 ;
}
void init(void){
	configPINSEL();
	configGPIO();
	configEXTI();
	configSYSTICK();
}
void configEXTI(void){

	EXTI_InitTypeDef exti;
	//Config INT externa  EINT0  (P2.10) por nivel bajo.
	exti.EXTI_Line = EXTI_EINT0;
	exti.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	exti.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;

	EXTI_Config(&exti);
	NVIC_EnableIRQ(EINT0_IRQn);
}
void configPINSEL(void){

	PINSEL_CFG_Type pinsel;
	//Config Pin 2.10 como entrada para EINT0
	pinsel.Pinnum = PINSEL_PIN_10;
	pinsel.Portnum = PINSEL_PORT_2;
	pinsel.Funcnum = PINSEL_FUNC_1;
	pinsel.Pinmode = PINSEL_PINMODE_PULLUP;
	pinsel.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pinsel);

	//Config pines 2.[0-6] como salida para display 7 segmento
	//Config pin 2.[7-8] como salida para mpx display

	for(int i=0; i<9; i++){

		pinsel.Pinnum = PINSEL_PIN_0 + i;
		pinsel.Portnum = PINSEL_PORT_2;
		pinsel.Pinmode = PINSEL_PINMODE_PULLUP;
		pinsel.Funcnum = PINSEL_FUNC_0;
		pinsel.OpenDrain = PINSEL_PINMODE_NORMAL;

		PINSEL_ConfigPin(&pinsel);
	}
}
void configSYSTICK(void){

	SYSTICK_InternalInit(16); //16 ms
	SYSTICK_IntCmd(ENABLE);
	SYSTICK_Cmd(ENABLE);
}
void configGPIO(void){
	//Pines para display
	for(int i=0; i<10; i++){
		GPIO_SetDir(PINSEL_PORT_2, (1<<i), OUTPUT);
	}
	//Pin para EINT0
	GPIO_SetDir(PINSEL_PORT_2, (1<<10), INPUT);
}
void SysTick_Handler(void){
	SYSTICK_ClearCounterFlag();
	uint32_t aux = GPIO_ReadValue(PINSEL_PORT_2);
	if (GPIO_ReadValue(PINSEL_PORT_2) & DISP1){
		disp1_ON();
	}
	else {
		disp2_ON();
	}
}
void EINT0_IRQHandler(void){


	if (unidad < 9) {
		unidad = unidad + 1;
	}
	else {
		unidad = 0;
		if (descena < 9) {
			descena = descena + 1;
		}
		else {
			descena = 0;
		}
	}
	for (int i=0; i<1000000; i++){

	}
	EXTI_ClearEXTIFlag(EXTI_EINT0);
}
void disp1_ON(void){
	LPC_GPIO2->FIOPIN0 = display[unidad];
	GPIO_SetValue(PINSEL_PORT_2, DISP2);					//Se apaga por alto
	GPIO_ClearValue(PINSEL_PORT_2, DISP1);					//Se prende por bajo
}
void disp2_ON(void){
	LPC_GPIO2->FIOPIN0 = display[descena];
	GPIO_SetValue(PINSEL_PORT_2, DISP1);					//Se apaga por alto
	GPIO_ClearValue(PINSEL_PORT_2, DISP2);					//Se prende por bajo
}
