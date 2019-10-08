/*
===============================================================================
 Name        : sanchez_tp4.c
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
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"

#define RAMPA				100
#define EXTRA				42
#define NUMERO_MUESTRAS		142
#define FRECUENCIA			100
#define CLOCK_DAC_MHZ		25 //CCLK divided by 4

void init(void);
void dacConfig(void);
void gpdmaConfig(void);
void pinselConfig(void);

void crearLista(void);

uint32_t TABLA_DAC[NUMERO_MUESTRAS];
uint32_t DMA_SIZE = RAMPA + EXTRA;

int main(void) {
	init();
    volatile static int i = 0 ;
    while(1) { i++ ; }
    return 0 ;
}

void init(void) {
	dacConfig();
	gpdmaConfig();
	pinselConfig();
	systickConfig();
	crearLista();
}

void dacConfig(void) {
	DAC_CONVERTER_CFG_Type DAC_Struct;			// Estructura DAC

	DAC_Struct.CNT_ENA =SET;					//Activa el modo timeout
	DAC_Struct.DMA_ENA = SET;					//Activa el modo DMA
	DAC_Init(LPC_DAC);

	//Calculamos el tiempo de actualizacion de muestra
	uint32_t tmp = (CLOCK_DAC_MHZ*1000000)/(FRECUENCIA*NUMERO_MUESTRAS);
	DAC_SetDMATimeOut(LPC_DAC,tmp);						//Fija el timeout, separacion entre muestras
	DAC_ConfigDAConverterControl(LPC_DAC, &DAC_Struct);	//Configura el DAC
}

void gpdmaConfig(void) {
	GPDMA_LLI_Type DMA_LLI_Struct;				// Estructura Linked List

	//Configuracion de la lista del DMA
	DMA_LLI_Struct.SrcAddr= (uint32_t)TABLA_DAC;		//Direccion de los datos fuente
	DMA_LLI_Struct.DstAddr= (uint32_t)&(LPC_DAC->DACR);	//Destino: DAC
	DMA_LLI_Struct.NextLLI= (uint32_t)&DMA_LLI_Struct;	//Solo un juego de datos
	DMA_LLI_Struct.Control= DMA_SIZE
							| (2<<18) 					//Fuente: 32bits
							| (2<<21) 					//Destino: 32bit
							| (1<<26) 					//Incremento automático de la fuente
							;

	GPDMA_Channel_CFG_Type GPDMACfg;			// Estructura de configuracion del DMA
	GPDMA_Init();								//Inicializa el modulo DMA

	GPDMACfg.ChannelNum = 0;						//Canal 0
	GPDMACfg.SrcMemAddr = (uint32_t)(TABLA_DAC);	//Origen
	GPDMACfg.DstMemAddr = 0;						//Destino = es un periferico, no es memoria
	GPDMACfg.TransferSize = DMA_SIZE;				//Tamaño de la transferencia
	GPDMACfg.TransferWidth = 0;						//No usado
	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2P;	//Tipo de transferencia = Memory 2 Peripheral
	GPDMACfg.SrcConn = 0;							//La fuente es memoria => no connection
	GPDMACfg.DstConn = GPDMA_CONN_DAC;				//Destino : conexión al DAC
	GPDMACfg.DMALLI = (uint32_t)&DMA_LLI_Struct;	//Lista de enlace del DMA

	GPDMA_Setup(&GPDMACfg);						//Configura el DMA
	GPDMA_ChannelCmd(0, ENABLE);				//Enciende el modulo DMA
}

void pinselConfig(void) {
	PINSEL_CFG_Type PinCfg;
	PinCfg.Portnum = PINSEL_PORT_0;				// Puerto 0
	PinCfg.Pinnum = PINSEL_PIN_26;				// Pin 26
	PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;		// Pull-Up
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;	// Sin OpenDrain
	PinCfg.Funcnum = PINSEL_FUNC_2;				// Func. AOUT
	PINSEL_ConfigPin(&PinCfg);					// Cargo config
}

void crearLista(void) {
	for (int i = 1 ; i < RAMPA; i++)
		TABLA_DAC[i] = TABLA_DAC[i-1] + 10;		// Cargo los valores de la rampa
	for (int i = RAMPA ; i < NUMERO_MUESTRAS ; i++)
		TABLA_DAC[i] = 0;						// Cargo 0 en los 3ms restantes
}
