/*
===============================================================================
 Name        : contador.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : Mediante 2 display 7 segmentos se necesita visualizar una cuenta
 	 	 	   que va desde 0 hasta 99 en decimal. Cuenta esta que se
 	 	 	   incrementara por la llegada de una interrupción externa, al
 	 	 	   llegar a 99, en la cuenta siguiente el contador vuelve a cero y
 	 	 	   continua contando. Utilice System tick timer para realizar las
 	 	 	   temporizaciones necesarias, el Cclock es de 100Mhz. Escriba la
 	 	 	   interrupción de SysTick, la interrupción externa, la configuración
 	 	 	   de SysTick.
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "lpc17xx_systick.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_gpio.h"

// TODO: insert other definitions and declarations here
#define INPUT		0
#define OUTPUT		1
#define TIME		16
#define DISP1		(1 << 8)
#define DISP2		(1 << 9)
#define EXTI0_BTN	(1 << 10)

void systickConfig(void);
void pinselConfig(void);
void extiConfig(void);
void gpioConfig(void);
void init(void);

uint8_t conver7seg(uint32_t);
void disp1On(void);
void disp2On(void);

uint32_t volatile unidad;
uint32_t volatile decena;
uint8_t display[10] = { 0x03F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int main(void) {
	init();
	volatile static int i = 0 ;
    while(1) { i++ ; }
    return 0 ;
}

void init(void) {
	systickConfig();
	pinselConfig();
	extiConfig();
	gpioConfig();
	unidad = display[0];
	decena = display[0];
}

void systickConfig(void) {
	SYSTICK_InternalInit(TIME);				// Inicializo con clk int y 16ms
	SYSTICK_IntCmd(ENABLE);					// Activo int.
	SYSTICK_Cmd(ENABLE);					// Activo la cuenta
}

void pinselConfig(void) {
	PINSEL_CFG_Type pin_cfg;

	// Configuración de los pines 9:0 del puerto 0
	// P0.[7:0]-> displays; P0.[9:8] -> mpx displays
	for (int i = 9 ; i >= 0 ; i--) {
		pin_cfg.Portnum = PINSEL_PORT_0;			// Puerto 0
		pin_cfg.Pinnum = PINSEL_PIN_0 + i;			// Pin 0 + i
		pin_cfg.Pinmode	= PINSEL_PINMODE_PULLUP;	// Res. Pull-Up
		pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL; 	// Sin OpenDrain
		pin_cfg.Funcnum = PINSEL_FUNC_0;			// GPIO
		PINSEL_ConfigPin(&pin_cfg);					// Cargo config.
	}

	// Configuración pin P2.10 (EINT0)
	pin_cfg.Portnum = PINSEL_PORT_2;				// Puerto 2
	pin_cfg.Pinnum = PINSEL_PIN_10;					// Pin 10
	pin_cfg.Pinmode = PINSEL_PINMODE_PULLUP;		// Res. Pull-Up
	pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;		// Sin OpenDrain
	pin_cfg.Funcnum = PINSEL_FUNC_1;				// EINT0
	PINSEL_ConfigPin(&pin_cfg);						// Cargo config.
}

void extiConfig(void) {
	EXTI_InitTypeDef exti_cfg;

	// Configuración int. externa (EINT0)
	exti_cfg.EXTI_Line = EXTI_EINT0;				// EINT0
	exti_cfg.EXTI_Mode = EXTI_MODE_LEVEL_SENSITIVE;	// Por nivel bajo
	exti_cfg.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
	EXTI_Config(&exti_cfg);							// Cargo config.
	NVIC_EnableIRQ(EINT0_IRQn);						// Activo int.
}

void gpioConfig(void) {
	// Pines para los displays como salida
	for (int i = 9 ; i >= 0 ; i--) {
		GPIO_SetDir(PINSEL_PORT_0, (1 << i) ,OUTPUT);
	}
	// Pulsador EINT0 como entrada
	GPIO_SetDir(PINSEL_PORT_2,EXTI0_BTN,INPUT);
}

void SysTick_Handler(void) {
	SYSTICK_ClearCounterFlag();						// Bajo flag
	if (GPIO_ReadValue(PINSEL_PORT_0) & DISP1) {	// Disp 1 off
		LPC_GPIO0->FIOPIN0 = display[unidad]; 		// Cargo unidad en P0
		disp1On();
	} else {
		LPC_GPIO0->FIOPIN0 = display[decena]; 		// Cargo decena en P0
		disp2On();
	}
}

void EINT0_IRQHandler(void) {
	EXTI_ClearEXTIFlag(EXTI_EINT0);				// Bajo flag
	if (unidad < 9) {
		unidad = unidad + 1;					// Incremento unidad
	} else {
		unidad = 0;								// Reseteo cuenta
		if (decena < 9) {
			decena = decena + 1;				// Incremento decena
		} else {
			decena = 0 ;						// Reseteo cuenta
		}
	}
}

void disp1On(void) {
	GPIO_SetValue(PINSEL_PORT_0, DISP2);		//Se apaga por alto
	GPIO_ClearValue(PINSEL_PORT_0, DISP1);		//Se prende por bajo
}

void disp2On(void) {
	GPIO_SetValue(PINSEL_PORT_0, DISP1);		//Se apaga por alto
	GPIO_ClearValue(PINSEL_PORT_0, DISP2);		//Se prende por bajo
}














