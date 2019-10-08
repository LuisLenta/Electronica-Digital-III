/*
===============================================================================
 Name        : blasco_uart3.c
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

// TODO: insert other include files here
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_uart.h"

// TODO: insert other definitions and declarations here
#define INPUT		0
#define OUTPUT		1
#define TX			(1 << 2)
#define RX			(1 << 3)

void init(void);
void pinselConfig(void);
void gpioConfig(void);
void uartConfig(void);

int main(void) {
	init();
    while(1) { }
    return 0 ;
}

void init(void) {
	pinselConfig();
	gpioConfig();
	uartConfig();
}

void pinselConfig(void) {
	PINSEL_CFG_Type pincfg;

	// Configuración de pin P0.2 (TXD0)
	pincfg.Portnum = PINSEL_PORT_0;					// Puerto 0
	pincfg.Pinnum = PINSEL_PIN_2;					// Pin 2
	pincfg.Pinmode = PINSEL_PINMODE_TRISTATE;		// Sin res.
	pincfg.OpenDrain = PINSEL_PINMODE_NORMAL;		// Sin OD
	pincfg.Funcnum = PINSEL_FUNC_1;					// UART0
	PINSEL_ConfigPin(&pincfg);						// Cargo config.

	// Configuración de pin P0.3 (RXD0)
	pincfg.Pinnum = PINSEL_PIN_3;					// Pin 3
	PINSEL_ConfigPin(&pincfg);						// Cargo config.
	// Es la misma, sólo cambia el n° pin
}

void gpioConfig(void) {
	GPIO_SetDir(PINSEL_PORT_0, RX, INPUT);
	GPIO_SetDir(PINSEL_PORT_0, TX, OUTPUT);
}

void uartConfig(void) {
	UART_CFG_Type uartcfg;

	// Configuración de uart por defecto:
	// (BD = 9600 , 8 data bit , 1 stop bit , None parity)
	UART_ConfigStructInit(&uartcfg);		// Cargo param de la estructura

	// Inicializo la UART
	UART_Init(LPC_UART0, &uartcfg);

	UART_FIFO_CFG_Type fifocfg;

	// Configuración de fifo por defecto:
	// DMA_Mode = DISABLE , Level = TRGLEV0 , RxBuff = ENABLE , State = ENABLE
	UART_FIFOConfigStructInit(&fifocfg);

	// Inicializo la fifo
	UART_FIFOConfig(LPC_UART0, &fifocfg);

	// Habilito la transmisión en pin TX
	UART_TxCmd(LPC_UART0, ENABLE);

	// Activo NVIC
	NVIC_EnableIRQ(UART0_IRQn);
}

void UART0_IRQHandler(void) {
	UART_ReceiveByte(LPC_UART0);	// Recibo el valor
}
