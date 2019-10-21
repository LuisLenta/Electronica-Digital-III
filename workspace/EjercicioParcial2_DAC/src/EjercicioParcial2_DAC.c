/*
===============================================================================
 Name        : EjercicioParcial2_DAC.c
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
#include "lpc17xx_gpio.h"
#include "lpc17xx_dac.h"

#define CFGADDR 			0x10004000UL
#define INPUT 			0
#define OUTPUT 			1


uint32_t volatile const* CFGDATA = (uint32_t*) CFGADDR;

uint32_t maxVal = 0;
uint32_t timeMaxVal = 0;
uint32_t timeIncr = 0;
uint32_t timeDecr = 0;
uint32_t estado = 0;				//Indica en que estado estamos. Si en incremento, mateniendo el valor, o decrementando.
//Estado 0: Incrementando
//Estado 1: Mateniendo valor maximo
//Estado 2: Decrementando
uint32_t valorDAC = 0;
uint32_t maxValueMatch = 1;  			//Indica que hubo un match y que comenzara el estado de mantener el valor.

void config_PINSEL(void);
void config_GPIO(void);
void config_DAC(void);
void config_SYSTICK(void);
void incrYConvertir (void);
void decrYConvertir (void);

void init(void);
// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {

	maxVal = (0xFF & *CFGDATA);								//Maximo valor de la onda
	timeMaxVal = (((0xFF << 8) & *CFGDATA) >> 8);			//Tiempo que dura el valor maximo
	timeIncr = (((0xFF << 16) & *CFGDATA) >> 16);			//Tiempo que dura el incremento
	timeDecr =  (((0xFF << 24) & *CFGDATA) >> 24); 			//Tiempo que dura el decremento

    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
void config_PINSEL (void){
	PINSEL_CFG_Type pinsel;

	pinsel.Pinnum = PINSEL_PIN_26;
	pinsel.Portnum = PINSEL_PORT_0;
	pinsel.Pinmode = PINSEL_PINMODE_TRISTATE;		//DAC no utiliza resistencias
	pinsel.Funcnum = PINSEL_FUNC_2;					//Modo AOUT
	pinsel.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pinsel);
}
void config_GPIO(void){
	GPIO_SetDir(PINSEL_PORT_0, (1<<26), OUTPUT);
}
void config_SYSTICK(void){

	SYSTICK_InternalInit(timeIncr); 			//Comienza el programa con el tiempo de incremento. Como la variable es de 8 bits, nunca superara los 255ms.
												//El SYSTICK soporta hasta 617, que son 24 bits. Ademas, suponemos que el tiempo esta guardado como la cantidad de ms.
	SYSTICK_IntCmd(ENABLE);
	SYSTICK_Cmd(ENABLE);
}
void config_DAC(void){
	DAC_CONVERTER_CFG_Type dac;

	dac.CNT_ENA = 0;
	dac.DBLBUF_ENA = 0;
	dac.DMA_ENA = 0;

	DAC_Init(LPC_DAC);
	DAC_ConfigDAConverterControl(LPC_DAC, &dac);
}
void init(){
	config_PINSEL();
	config_GPIO();
	config_DAC();
	config_SYSTICK();
}
void SysTick_Handler(void){

	switch (estado){
		case 0:
			if(valorDAC < maxVal){
				incrYConvertir();
			}
			else {
				estado = 1;
				maxValueMatch = 1;
			}
			break;
		case 1:
			if(maxValueMatch == 1){  //El tiempo de maxValue esta iniciando
				SYSTICK_Cmd(DISABLE);
				SYSTICK_InternalInit(timeMaxVal);
				SYSTICK_Cmd(ENABLE);
				maxValueMatch = 0; 		//Indica que la proxima vez que ingrese al handler, el tiempo de maxValue habra transcurrido
			}
			else{	//El tiempo ya transcurrio
				estado = 2; //Cambio a estadod de decremento
				SYSTICK_Cmd(DISABLE);
				SYSTICK_InternalInit(timeMaxVal);
				SYSTICK_Cmd(ENABLE);

			}
			break;
		case 2:
			if(valorDAC > 0){
				decrYConvertir();
			}
			else{
				estado = 0; 					//Comienza a incrementar nuevamente
				SYSTICK_Cmd(DISABLE);
				SYSTICK_InternalInit(timeIncr);
				SYSTICK_Cmd(ENABLE);
			}
	}
	SYSTICK_ClearCounterFlag();
}
void incrYConvertir (void){
	valorDAC = valorDAC + 1;
	DAC_UpdateValue(LPC_DAC, valorDAC);
}
void decrYConvertir (void){
	valorDAC = valorDAC - 1 ;
	DAC_UpdateValue(LPC_DAC, valorDAC);

}
