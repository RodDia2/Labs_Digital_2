/*
 * File:   main.c
 * Author: Rodrigo Díaz
 *
 * Creado martes 9 de marzo del 2021
 */

//******************************************************************************
// Palabra de configuración
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
// Importación de librerías
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdint.h>

//#include "config.h"
#include "USART.h"


//******************************************************************************
// Variables
//******************************************************************************
#define _XTAL_FREQ 8000000

char pantalla[20];
char buffer[40];
char recibido = 0;
uint8_t contador = 0;
uint8_t contador2 = 1;
//******************************************************************************
//Portotipos de funciones
//******************************************************************************
void setup(void);
void setOsc(void);
//******************************************************************************
// COdigo de interrupcion
//******************************************************************************

//******************************************************************************
// Ciclo principal
//******************************************************************************
void main(void){
    setup();
    UART_TX_Init();  
    setOsc();
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1)
	{
        RD2 = !RD2;  // Blink LED
        UART_Write_String("V1     V2   CONT");
        UART_Write(13);
        UART_Write(10);
        sprintf(pantalla, "%1.2f   %3d %3d", 5.2,contador2,contador);
//        // se muestran los datos, bajo a su nombre en string
        UART_Write_String(pantalla);
//       // se vuelve a saltar una linea
        UART_Write(13);
        UART_Write(10);
//        
        sprintf(buffer,"%d",5);
        UART_Write_String(buffer);
        UART_Write_String("1");
       // se vuelve a saltar una linea
        UART_Write(13);
        UART_Write(10);
        if(RCIF==1){
            // se tranfiere el dato recibido en USART a la variable recibido
            // despues se revisa si es + o - para modificar el contador
            //while (!RCIF);
            recibido = USART_Read();  
          //  recibido = RCREG;
            if(recibido > 0) {
                PORTDbits.RD0 = 1;
            }
            if(recibido == 0) {
                PORTCbits.RC2 = 1;
            }
            else {
                PORTCbits.RC1 = 1;
            }
            
            if(recibido == '+'){
                contador2++;
            } 
            if(recibido == '-'){
                contador2--;
            }
            
        }
        
        contador++;
       // PORTB = contador2;
        __delay_ms(500);
    }
    return;
}

//******************************************************************************
// Configuración
//******************************************************************************
void setup (void) {
    ANSEL = 0b00000000;
    ANSELH = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;  
    PORTD = 0;
    PORTE = 0;
    
    TRISA = 0b00000000;
    TRISB = 0;
    TRISC = 0b10000000;
    TRISD = 0;
    TRISE = 0;
}

void setOsc(void){
    OSCCONbits.IRCF = 0b111; // 8MHz
}

