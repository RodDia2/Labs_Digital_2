//*****************************************************************************
/*
 * File:   main.c
 * Author: Rodrigo Díaz, basado en el ejemplo de Pablo Mazariegos
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
// variables a utilizar, V1 es float porque todavía no se utiliza suficiente 
// program memory para que sea un problema. el char de pantalla debe ser de 20
// caracteres para poder guardar el string de los diferentes valores. 
uint8_t TEMP = 5;
uint8_t ADC = 0;
uint8_t CONT = 0;
char pantalla[20];
char recibido = 0;
float V1 = 0.0;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    // primero se setean los bits en el setup
    setup();
    // se inicializa la pantalla LCD y se limpia
    LCD_Init();
    LCD_clear();
    // el baud rate se pone en 9600
    Set_Baud_Rate();
    // se inicializa la comunicación UART
    Init_Transmit();
    Init_Receive();
    #ifndef RS
    #define RS RE0
    #endif 

    #ifndef EN
    #define EN RE1
    #endif 
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        // Este loop siempre se repite
        // Primero se mandan los strings a la terminal
        USART_Write_String("ADC  CONT  TEMP \n");
        // se salta a la siguiente linea
        USART_Write(13);
        USART_Write(10);
        // la variable char de pantalla se le asignan los valores de voltaje ADC,
        // del contador y de la temperatura. Pero, primero se convierte el valor
        // de 0-255 que recibe del ADC y lo convierte a un voltaje de 0-5 V.
        V1 = ADC*0.0196;
        sprintf(pantalla, "%1.2fV %3d  %3dC", V1,CONT,TEMP);
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
        // se realiza un delay para poder visualizar mejor los valores en la terminal
        // y dar tiempo a la lcd
        __delay_ms(500);
        
       //****************************************************************
        // Esta es la rutina de slave selects. El slave con el que se desea 
        // comunicar, se debe setear su bit respectivo a 0. Como el maestro solo
        // recibe datos, se manda un 1 y de allí se guarda el valor del buffer
        // de recepción en una variable local.
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
    // Todos los bits utilizados se configuran como salidas, menos 
    // el bit 7 del puerto C para UART. Ansel y Anselh 
    // se ponen en 1 solamente donde hayan entradas analogicas.
    ANSEL = 0;
    ANSELH = 0;
    //TRISC = 0b10000000;
    TRISC0 = 0;
    TRISC1 = 0;
    TRISC2 = 0;
    TRISC7 = 1;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    // no se utilizaron las interrupciones para SPI
    // se setean los bits de slave select como entradas. 
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC0 = 1;
    // se configura como el maestro, con frecuencia de Fosc/4, que idle sea low
    // y que se transmita en rising edge
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}
