//Funciones de Read y Write basadas en: Using UART of PIC Microcontroller BY LIGO GEORGE
//extraido de https://electrosome.com/uart-pic-microcontroller-mplab-xc8/

#include "USART.h"

#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 8000000

// solamente se va a comunicar con la terminal, entonces solo se necesita esto
void Set_Baud_Rate(void){
    SPBRG = 12; // establecer en 9600 de baud rate
}
// se habilitan los bits para mandar informacion 
void Init_Transmit(void){
    TXSTAbits.TXEN = 1;
   // TXSTAbits.SYNC = 0;
   // RCSTAbits.SPEN = 1;
    PIR1bits.TXIF = 0;
    PIE1bits.TXIE = 0;
    INTCONbits.GIE = 0;
    INTCONbits.PEIE = 0;
    // se especifican los bits de TX y RX por seguridad
    TRISCbits.TRISC7 = 1;                                   
    TRISCbits.TRISC6 = 0; 
    
}

// se habilitan los bits para recibir informacion
void Init_Receive(void){
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
   // PIE1bits.RCIE = 1;
   // PIR1bits.RCIF = 0;
    //INTCONbits.GIE = 1;
    //INTCONbits.PEIE = 1;
    RCSTAbits.CREN = 1;

}
// estas son las librerias citadas

// la primera espera a que TRMT sea 1 para poder escribir un char
void USART_Write(uint8_t a){
    while(!TRMT);
    TXREG=a;
}
// esta funcion es para mandar datos que tengan un tamano mayor a un char
void USART_Write_String(char *a){
    uint8_t i;
    for(i=0;a[i]!='\0';i++){
        USART_Write(a[i]);
    }
}
// esta funcion espera a que la bandera vuelva a activarse para leer el dato 
// del RCREG. como solo se busca un + o - no se utilizo al final
uint8_t USART_Read(){
  while(!RCIF);
  return RCREG;
} 

