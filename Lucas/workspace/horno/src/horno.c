/*
===============================================================================
 Name        : horno.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : 	Considere que en la dirección 0x10000025 se encuentra un dato
 	 	 	 	tipo punto flotante ( float ) que tiene el valor correspondiente
 	 	 	 	a la temperatura deseada. Para tal fin se cuenta con la
 	 	 	 	termocupla, cuya curva de respuestas puede observar en la
 	 	 	 	gráfica anterior. Se solicita:
				1-Proponga un circuito para activar led de control mediante un
					pin de la LPC1769 teniendo en cuenta que Vf=3.5V e If=15mA
					(valores requeridos para el funcionamiento del LED).
				2-Realice y comente el software necesario que permita el control
				 	 de la temperatura del horno, verificando su temperatura
				 	  cada 1 segundo, la termocupla antes mencionada está
				 	  conectada al ADC de la LPC1769, almacene los últimos 100
				 	  datos de temperatura en un arreglo del tipo de dato que
					  usted cocedera más conveniente.
				3-Una vez que el horno supere temperatura que fue establecida
					en la dirección 0x10000025 se deberá apaga la resistencia,
					y al descender un 10% dicha temperatura se encenderá
					nuevamente.
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "lpc17xx_adc.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"

// TODO: insert other definitions and declarations here
#define INPUT		0
#define OUTPUT		1
#define PERCENT		10
#define SIZE		100
#define MAXTEMP		160
#define MAXRES		4096
#define clock		200000
#define LED			(1 << 22)
#define CHANNEL0	(1 << 23)
#define	ADDRDATO	0x10000025

void init(void);
void adcConfig(void);
void gpioConfig(void);
void pinselConfig(void);
void systickConfig(void);

uint32_t compare(uint32_t);
uint32_t convert(uint16_t);
void calculateAVG(void);
void ledOn(void);
void ledOff(void);

static uint32_t AVG = 0;
static uint32_t array[SIZE];
float volatile *const tRef = (float*) ADDRDATO;

int main(void) {
	init();
    volatile static int i = 0 ;
    while(1) { i++ ; }
    return 0 ;
}

void init(void) {
	adcConfig();
	gpioConfig();
	pinselConfig();
	systickConfig();
}

void adcConfig(void) {
	ADC_Init(LPC_ADC, clock);						// Inicio y config. clock
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);	// Activo ch0
	ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, ENABLE);	// Activo int. ch0
	ADC_BurstCmd(LPC_ADC, DISABLE);					// Desactivo Burst

	NVIC_EnableIRQ(ADC_IRQn);						// Activo int. por ADC
}

void gpioConfig(void) {
	GPIO_SetDir(PINSEL_PORT_0, LED, OUTPUT);		// Out -> Led
	GPIO_SetDir(PINSEL_PORT_0, CHANNEL0, INPUT);	// In  -> ADC
}

void pinselConfig(void) {
	PINSEL_CFG_Type pin_cfg;

	// Configuración pin P0.22 (LED)
	pin_cfg.Portnum = PINSEL_PORT_0;				// Puerto 0
	pin_cfg.Pinnum = PINSEL_PIN_22;					// Pin 22
	pin_cfg.Pinmode = PINSEL_PINMODE_PULLUP;		// Res. Pull-up
	pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;		// Sin OpenDrain
	pin_cfg.Funcnum = PINSEL_FUNC_0;				// GPIO

	PINSEL_ConfigPin(&pin_cfg);						// Cargo config.

	// Configuración pin P0.23 (ADC)
	pin_cfg.Portnum = PINSEL_PORT_0;				// Puerto 0
	pin_cfg.Pinnum = PINSEL_PIN_23;					// Pin 23
	pin_cfg.Pinmode = PINSEL_PINMODE_TRISTATE;		// Sin res.
	pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;		// Sin OpenDrain
	pin_cfg.Funcnum = PINSEL_FUNC_1;				// ADC

	PINSEL_ConfigPin(&pin_cfg);						// Cargo config.
}

void systickConfig(void) {
	SYSTICK_InternalInit(10);						// Cuenta 10ms
	SYSTICK_IntCmd(ENABLE);							// Activo int. SysTick
	SYSTICK_Cmd(ENABLE);							// Activo la cuenta
}

void SysTick_Handler(void) {
	SYSTICK_ClearCounterFlag();
	static int i = 100;
	i--;
	if (i == 0) {
		i = 100;
		ADC_StartCmd(LPC_ADC, ADC_START_NOW);		// Inicio conversión
		calculateAVG();								// Calculo el prom
	}
}

void ADC_IRQHandler(void) {
	static int i = SIZE - 1;
	uint32_t value = convert(ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0));
	compare(value) == 1 ? ledOn() : ledOff() ;

	array[i] = value;
	i == 0 ? (i = SIZE - 1) : i-- ;
}

uint32_t convert(uint16_t value) {
	return value * MAXTEMP / MAXRES;
}

uint32_t compare(uint32_t value) {
	return (value < (*tRef - (*tRef/PERCENT))) ? 1 : 0 ;
}

void calculateAVG(void) {
	uint32_t aux;
	for (int i = SIZE ; i > 0 ; i--)
		aux = aux + array[i-1];
	AVG = aux / SIZE;
}

void ledOn(void) {
	FIO_ClearValue(PINSEL_PORT_0, LED);
}

void ledOff(void) {
	FIO_SetValue(PINSEL_PORT_0, LED);
}
