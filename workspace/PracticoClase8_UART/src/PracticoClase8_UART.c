/*
===============================================================================
 Name        : PracticoClase8_UART.c
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
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

void configPINSEL(void);
void configUART(void);
void configSYSTICK(void);

int main(void) {

    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
void configPINSEL(void){

	PINSEL_CFG_Type pin;

	pin.Portnum = PINSEL_PORT_0;
	pin.Pinnum = PINSEL_PIN_0;
	pin.Funcnum = PINSEL_FUNC_2;
	pin.OpenDrain = PINSEL_PINMODE_NORMAL;
	pin.Pinmode = PINSEL_PINMODE_TRISTATE;

	PINSEL_ConfigPin(pin);
}
void configUART(void){
	UART_CFG_Type uart;        //Estructura de UART
	UART_FIFO_CFG_Type fifo;	//Estructura de FIFO de UART

	UART_ConfigStructInit(UART_InitStruct);		//Configuro estructura UART
	UART_Init(LPC_UART3, uart);					//Inicio UART
	UART_FIFOConfigStructInit(fifo);			//Configuro structura FIFO
	UART_FIFOConfig(LPC_UART3, fifo);			//Inicio FIFO
	UART_TxCmd(LPC_UART3, ENABLE);
}
void configSYSTICK(void){
	SYSTICK_InternalInit(100);
	SYSTICK_IntCmd(ENABLE);
	SYSTICK_Cmd(ENABLE);
}
void SysTick_Handler(void){
	static int i=100;
	i--;

	if(i==0){
		i=100;
		UART_SendByte(LPC_UART3,0x41);
	}
}
