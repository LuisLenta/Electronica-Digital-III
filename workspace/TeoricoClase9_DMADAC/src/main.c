/*
===============================================================================
 Name        : TeoricoClase9_DMADAC.c
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
#include "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"

#define RAMPA 				100
#define EXTRA				42
#define NUMERO_MUESTRAS		142
#define FRECUENCIA 			100
#define CLOCK_DAC 			25000000  	//CLK divided by 4

void configDAC(void);
void configGPDMA(void);
void configPINSEL(void);
void init (void);
void configSYSTICK(void);
void crearLista (void);

uint32_t TABLA_DAC[NUMERO_MUESTRAS];
int main(void) {

	crearLista();
	init();

	volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
void configDAC (void) {
	DAC_CONVERTER_CFG_Type dac;

	dac.CNT_ENA = SET; 				//Time out counter ENABLE
	dac.DMA_ENA = SET; 				//DMA access ENABLE

	DAC_ConfigDAConverterControl(LPC_DAC, &dac);

	uint32_t tmp = CLOCK_DAC / (FRECUENCIA * NUMERO_MUESTRAS);
	DAC_SetDMATimeOut(LPC_DAC, tmp);								//Tiempo en el cual DMA actualiza valor a convertir en DAC

	DAC_Init(LPC_DAC);
}
void configGPDMA(void){
	GPDMA_Channel_CFG_Type  channelGPDMA;

	channelGPDMA.ChannelNum = 0;
	channelGPDMA.TransferSize = NUMERO_MUESTRAS;
	channelGPDMA.SrcMemAddr = (uint32_t) (TABLA_DAC);
	channelGPDMA.TransferType = GPDMA_TRANSFERTYPE_M2P;
	channelGPDMA.DstConn = GPDMA_CONN_DAC;
	channelGPDMA.DMALLI = 0;

	GPDMA_Init();
	GPDMA_Setup(&channelGPDMA);
	GPDMA_ChannelCmd(0, ENABLE);

}

void configPINSEL (void) {
	PINSEL_CFG_Type pinsel;
	//Config P0.26 para salida analogica del DAC.
	pinsel.Pinnum = PINSEL_PIN_26;
	pinsel.Portnum = PINSEL_PORT_0;
	pinsel.Funcnum = PINSEL_FUNC_2; 	//AOUT para DAC
	pinsel.Pinmode = PINSEL_PINMODE_TRISTATE;
	pinsel.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pinsel);

}
void init(void){
	configPINSEL();
	configGPDMA();
	configDAC();
	configSYSTICK();
}
void crearLista(void){

	TABLA_DAC[0] = 0;
	for (int i = 1 ; i < RAMPA; i++)
			TABLA_DAC[i] = TABLA_DAC[i - 1] + 10;		// Cargo los valores de la rampa
	for (int i = RAMPA ; i < NUMERO_MUESTRAS ; i++)
			TABLA_DAC[i] = 0;
}
void configSYSTICK(void){
	SYSTICK_InternalInit(10);
	SYSTICK_IntCmd(ENABLE);
	SYSTICK_Cmd(ENABLE);
}
void SysTick_Handler(void){
	SYSTICK_ClearCounterFlag();
	IntStatus prueba = GPDMA_IntGetStatus(GPDMA_STAT_ENABLED_CH, 0);
	IntStatus prueba2 = GPDMA_IntGetStatus(GPDMA_STAT_INT, 0);
	IntStatus prueba3 = GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 0);
}
