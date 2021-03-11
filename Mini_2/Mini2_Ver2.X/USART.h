/* 
 * File:   
 * Author: Rodrigo Díaz, basado en el código de Pablo Mazariegos
 * Comments:
 * Revision history: 
 */
#define _XTAL_FREQ  8000000
#include <stdint.h>

#define BaudRate    9600
// se utilizan estos nombres para activar los tris de TX y RX
#define TX_D        TRISC6
#define RX_D        TRISC7

//-----------[ Functions' Prototypes ]--------------

//---[ UART Routines ]---
// se crean funciones para inicializar, escribir y leer
void UART_TX_Init(void);
void UART_Write(unsigned char);
void UART_Write_String(char*);
uint8_t USART_Read(void);
