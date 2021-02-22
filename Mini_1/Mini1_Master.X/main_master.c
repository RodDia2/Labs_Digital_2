//*****************************************************************************
/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de implementación de la comunicación SPI 
 * Código Maestro
 * Created on 10 de febrero de 2020, 03:32 PM
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "SPI.h"
#include "LCD.h"
#include "USART.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

uint8_t TEMP = 5;
uint8_t ADC = 0;
uint8_t CONT = 0;
char pantalla[20];
char recibido = 0;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    LCD_Init();
    LCD_clear();
    // el baud rate se pone en 9600
    Set_Baud_Rate();
    
    Init_Transmit();
    Init_Receive();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        // Primero se mandan los strings a la terminal
        USART_Write_String("ADC CONT TEMP \n");
        // se salta a la siguiente linea
        USART_Write(13);
        USART_Write(10);
        // la variable char de pantalla se le asignan los valores de voltaje
        // y del contador
        sprintf(pantalla, "%2d  %2d  %2d", ADC,CONT,TEMP);
        // se muestran los datos, bajo a su nombre en string
        USART_Write_String(pantalla);
       // se vuelve a saltar una linea
        USART_Write(13);
        USART_Write(10);
        // se realiza un clear de la pantalla por seguridad
        LCD_clear();
        // en la primera fila, se despligan los titulos y en la segunda los valores
        LCD_Set_Cursor(1,1);
        LCD_Write_String("ADC  CONT  TEMP");
        LCD_Set_Cursor(2,1);
        LCD_Write_String(pantalla);
        
        __delay_ms(500);
        
       //****************************************************************
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(1);
       CONT = spiRead();
       //PORTB = CONT;
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
      
       __delay_ms(1);
       //***************************************************************
       PORTCbits.RC1 = 0;
       __delay_ms(1);
       
       spiWrite(1);
       ADC = spiRead();
       //PORTD = ADC;
       
       __delay_ms(1);
       PORTCbits.RC1 = 1; 
       __delay_ms(1);
       
       //********************************************************************
       
       PORTCbits.RC0 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(1);
       TEMP = spiRead();
       //PORTB = TEMP;
       
       __delay_ms(1);
       PORTCbits.RC0 = 1;       //Slave Deselect 
      
       __delay_ms(1);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    //TRISC = 0b10000000;
    TRISC0 = 0;
    TRISC1 = 0;
    TRISC2 = 0;
    TRISC7 = 1;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC0 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}
