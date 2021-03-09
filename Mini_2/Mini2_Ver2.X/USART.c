/*
 * File:   USART.c
 * Author: betov
 *
 * Created on 22 de febrero de 2021, 07:53 AM
 */



#include <xc.h>
#include "USART.h"


//---------------[ UART Routines ]------------------
//--------------------------------------------------

void UART_TX_Init(void)
{
  BRGH  = 1;   // Set For High-Speed Baud Rate
  SPBRG = 51;  // Set The Baud Rate To Be 19200 bps
  //--[ Enable The Ascynchronous Serial Port ]--
  SYNC = 0;
  SPEN = 1;
  //--[ Set The RX-TX Pins to be in UART mode (not io) ]--
  TX_D = 1;
  RX_D = 1;
  TXEN = 1;  // Enable UART Transmission
}

void UART_Write(unsigned char data)
{
  while(!TRMT);
  TXREG = data;
}

void UART_Write_String(char* buf)
{
    int i=0;
    while(buf[i] != '\0')
        UART_Write(buf[i++]);
}