//Funciones de Read y Write basadas en: Using UART of PIC Microcontroller BY LIGO GEORGE
//extraido de https://electrosome.com/uart-pic-microcontroller-mplab-xc8/

#include "USART.h"

#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 8000000

void Set_Baud_Rate(void){
    SPBRG = 12; // establecer en 9600 de baud rate
}

void Init_Transmit(void){
    TXSTAbits.TXEN = 1;
   // TXSTAbits.SYNC = 0;
   // RCSTAbits.SPEN = 1;
    PIR1bits.TXIF = 0;
    PIE1bits.TXIE = 0;
    INTCONbits.GIE = 0;
    INTCONbits.PEIE = 0;
    TRISCbits.TRISC7 = 1;                                   //As Prescribed in Datasheet
    TRISCbits.TRISC6 = 0; 
    
}


void Init_Receive(void){
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
   // PIE1bits.RCIE = 1;
   // PIR1bits.RCIF = 0;
    //INTCONbits.GIE = 1;
    //INTCONbits.PEIE = 1;
    RCSTAbits.CREN = 1;

}

void USART_Write(uint8_t a){
    while(!TRMT);
    TXREG=a;
}
void USART_Write_String(char *a){
    uint8_t i;
    for(i=0;a[i]!='\0';i++){
        USART_Write(a[i]);
    }
}
uint8_t USART_Read(){
  while(!RCIF);
  return RCREG;
} 

//char USART_Init(const long int baudrate){
//  unsigned int x;
//  x = (_XTAL_FREQ - baudrate*64)/(baudrate*64);   //SPBRG for Low Baud Rate
//  if(x>255)                                       //If High Baud Rage Required
//  {
//    x = (_XTAL_FREQ - baudrate*16)/(baudrate*16); //SPBRG for High Baud Rate
//    BRGH = 1;                                     //Setting High Baud Rate
//  }
//  if(x<256)
//  {
//    SPBRG = x;                                    //Writing SPBRG Register
//    SYNC = 0;                                     //Setting Asynchronous Mode, ie UART
//    SPEN = 1;                                     //Enables Serial Port
//    TRISC7 = 1;                                   //As Prescribed in Datasheet
//    TRISC6 = 0;                                   //As Prescribed in Datasheet
//    CREN = 1;                                     //Enables Continuous Reception
//    TXEN = 1;                                     //Enables Transmission
//    return 1;                                     //Returns 1 to indicate Successful Completion
//  }
//  return 0;                                       //Returns 0 to indicate UART initialization failed
//}