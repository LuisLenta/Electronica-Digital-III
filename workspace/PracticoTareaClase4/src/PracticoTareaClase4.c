/*
===============================================================================
 Name        : PracticoTareaClase4.c
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
#define loadTimer 1000000

typedef struct {
	   uint32_t Byte;
	    struct {
	        uint16_t ByteL;
	        uint16_t ByteH;
	    };
	    struct {
	        uint8_t  Byte0;
	        uint8_t  Byte1;
	        uint8_t  Byte2;
	        uint8_t  Byte3;
	    };
}Byte_TypeDef;


//Variables
Byte_TypeDef array[8];
int btnPress;
int arrayPosition;
int sentidoRecorrido;
//Funciones
void config(void);
void loadArray(void);
void loopArray(void);


// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {

	btnPress = 100;
	arrayPosition = 7;
	sentidoRecorrido = 1;  //Por defecto el recorrido es mayor posicion en el arreglo a menor
	config();
	loadArray();

	//Cargo SysTick
	SysTick_Config(loadTimer);

	//NVIC config
	NVIC_EnableIRQ(EINT3_IRQn);
	NVIC_EnableIRQ(EINT0_IRQn);

    // TODO: insert code here


    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
/**
 * Configuraciones iniciales de todos los puertos
 */
void config(void){

	//Config como salida P0.0 a P0.8
	LPC_GPIO0->FIODIR0 = 255;

	//Config EINT0
	LPC_PINCON->PINSEL4 |= (1<<20);
	LPC_PINCON->PINSEL4 &= ~(1<<21);

	//Config GPIOINT, esto no es necesario porque lo hace por defecto
	LPC_PINCON->PINSEL4 &= ~(1<<16);
	LPC_PINCON->PINSEL4 &= ~(1<<17);

	//Pull-up config, esto no es necesario por que lo hace por defecto luego de un reset
	LPC_PINCON->PINMODE4 &= ~(1<<20);
	LPC_PINCON->PINMODE4 &= ~(1<<21);
	LPC_PINCON->PINMODE4 &= ~(1<<16);
	LPC_PINCON->PINMODE4 &= ~(1<<17);

	//Config interrupt
	LPC_SC->EXTMODE |= (1<<0);  //Por flanco
	LPC_SC->EXTPOLAR |= (1<<0); //De subida
	LPC_GPIOINT->IO2IntEnR |= (1<<8);  //Subida en GPIOINT


}
/*
 * Carga un arreglo de 8 numeros de 32 bits.
 * Mueve un 1 desde el bit mas significativo al menos significativo a medida que carga el arreglo.
 */
void loadArray(void){

	uint8_t j= 1;
	for(int i=7;  i>=0; i--){
		array[i].Byte2 = j & 0xff;
		j = j*2;
	}

}
/*
 * De acuerdo al sentido de recorrido, mueve una posicion del arreglo
 */
void loopArray(){
	if(sentidoRecorrido == 1){
		arrayPosition--;
		if(arrayPosition == -1)
			arrayPosition = 7;
	}
	else if(sentidoRecorrido == 2){
		arrayPosition++;
		if(arrayPosition == 8){
			arrayPosition = 0;
		}
	}
}

void EINT0_IRQHandler(void){

	if(btnPress == 800){
		btnPress = 100;
		return;
	}

	btnPress = btnPress * 2;
	// BAJO FLAG DE GPIOINT P2[10]
	LPC_SC->EXTINT |= (1<<0);
}
void EINT3_IRQHandler(void){

	if(sentidoRecorrido == 1)
		sentidoRecorrido = 2;
	else if(sentidoRecorrido == 2)
		sentidoRecorrido = 1;

	// BAJO FLAG DE GPIOINT P2[8]
	LPC_GPIOINT->IO2IntClr |= (1 << 8);
}
void SysTick_Handler (void){
	static int i = 100;
	i--;


	if(i==0){
		//Posiciono el array
			loopArray();
		//Muevo bit de arreglo a el puerto
		LPC_GPIO0->FIOPIN0 = array[arrayPosition].Byte2;

		//Controlo que velocidad esta seleccionada por el boton
		switch(btnPress){
			case 100: {
				i = 100;
				break;
			}
			case 200: {
				i = 200;
				break;
			}
			case 400: {
				i = 400;
				break;
			}
			case 800: {
				i = 800;
				break;
			}
		}
	}
}
