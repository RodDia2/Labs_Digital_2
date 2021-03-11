/*
 * File:   USART.c
 * Author: Rodrigo Díaz, basado en el código de Pablo Mazariegos
 *
 * Creado lunes 8 de marzo del 2021
 */

#include <xc.h>
#include "USART.h"
#include <stdint.h>
#define _XTAL_FREQ 8000000

//---------------[ UART Routines ]------------------
//--------------------------------------------------

// se inicializa para tanto mandar como recibir, pero sin interrupciones
void UART_TX_Init(void)
{
  BRGH  = 0;   // Set For High-Speed Baud Rate
  SPBRG = 12;  // Set The Baud Rate To Be 9600 bps
  //--[ Enable The Ascynchronous Serial Port ]--
  SYNC = 0;
  SPEN = 1;
  //--[ Set The RX-TX Pins to be in UART mode (not io) ]--
  TX_D = 0;
  RX_D = 1;
  TXEN = 1;  // Enable UART Transmission
  
  RCSTAbits.CREN = 1; // recibe
}

// funcion para escribir un char, se manda al TXREG
void UART_Write(unsigned char data)
{
  while(!TRMT);
  TXREG = data;
}
// funcion para escribir un conjunto de chars
void UART_Write_String(char* buf)
{
    int i=0;
    while(buf[i] != '\0')
        UART_Write(buf[i++]);
}
// funcion para leer el RCREG
uint8_t USART_Read(){
  while(!RCIF);
  return RCREG;
} 
