/*
===============================================================================
 Name        : adc-sanchez.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "../Drivers/inc/lpc17xx_adc.h"
#include "../Drivers/inc/lpc17xx_pinsel.h"
#include "../Drivers/inc/lpc17xx_gpio.h"
#include "core_cm3.h"
#endif

#include <cr_section_macros.h>

#define rojo (1<<22)
#define azul (1<<26)
#define verde (1<<25)

volatile uint32_t dataADC;
volatile static int toggle = 0;
volatile static int contSeg = 0;

void prenderVerde(void);
void prenderAzul(void);
void prenderRojo(void);
void configGPIO(void);
void configADC(void);
void execTermometro(void);
void parpadeo(void);

int main(void) {
	configADC();
	configGPIO();
	SysTick_Config(1000000);
	while (1) {
		execTermometro();
	}
	return 0 ;
}

void prenderRojo(void) {
	GPIO_SetValue(3, azul);
	GPIO_SetValue(3, verde);
	GPIO_ClearValue(0, rojo);
	if (contSeg == 3)
		parpadeo();
}

void prenderAzul(void) {
	GPIO_SetValue(0, rojo);
	GPIO_SetValue(3, verde);
	GPIO_ClearValue(3, azul);
}

void prenderVerde (void) {
	GPIO_SetValue(0, rojo);
	GPIO_SetValue(3, azul);
	GPIO_ClearValue(3, verde);
}


void configGPIO(void) {
	GPIO_SetDir(0, rojo, 1);
	GPIO_SetDir(3, azul, 1);
	GPIO_SetDir(3, verde, 1);
	GPIO_SetValue(0, rojo);
	GPIO_SetValue(3, azul);
	GPIO_SetValue(3, verde);
}

void configADC(void) {
	PINSEL_CFG_Type pin;
	pin.Funcnum = PINSEL_FUNC_1;
	pin.Portnum = PINSEL_PORT_0;
	pin.Pinnum = PINSEL_PIN_23;
	PINSEL_ConfigPin(&pin);

	ADC_Init(LPC_ADC, 100000);
	ADC_BurstCmd(LPC_ADC, 1);
	ADC_ChannelCmd(LPC_ADC, 0, ENABLE);
	ADC_StartCmd(LPC_ADC, ADC_START_CONTINUOUS);
}

void execTermometro(void) {
	while (ADC_ChannelGetStatus(LPC_ADC, 0, 1));
	dataADC = ADC_ChannelGetData(LPC_ADC, 0)*100/4096;
	if ((dataADC<60)&&(dataADC>0))
		prenderVerde();
	else if ((dataADC>60) && (dataADC<80))
		prenderAzul();
	else if (dataADC>80)
		prenderRojo();
}

void SysTick_Handler(void) {
	volatile static int vSysTick=100;
	vSysTick--;
	if (vSysTick==0) {
		toggle = !toggle;
		if (dataADC>80)
			contSeg++;
		if ((contSeg>0)&&(!(dataADC>80))&&(contSeg<3))
			contSeg = 0;
		vSysTick = 100;
	}
}

void parpadeo (void) {
	while (1) {
		if (toggle)
			GPIO_ClearValue(0, rojo);
		else
			GPIO_SetValue(0, rojo);
	}
}
