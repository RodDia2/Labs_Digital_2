/* 
 * File:   main.c
 * Author: Rodrigo Díaz
 * Digital 2
 * MINI2
 */

//******************************************************************************
// Importación de librerías
//******************************************************************************
// se incluyen las librerias que se utilizan y las creadas para el lab
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
//#include <pic16f887.h>
//#include "UART.h"
#include "MPU6050.h"
#include "I2C.h"



//******************************************************************************
// Palabra de configuración
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT       // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)



//******************************************************************************
// Variables
//******************************************************************************
// la variable XTAL FREQ es necesaria para que funcionen los delays
//#define _XTAL_FREQ 8000000

// se crean las variables a utilizar con su tamano 

uint8_t contador = 0;


//char char_array[];

//******************************************************************************
// Interrupción
//******************************************************************************

void __interrupt() ISR(){
    

}

//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void setup(void);

//******************************************************************************
// Ciclo principal
//******************************************************************************

void main(void) {
    // 
    //funciones de las librerias creadas para inicializacion 
    setup();
    MPU6050_Init();
    OSCCONbits.IRCF = 0b111;
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    // como es un while(1) siempre se va a repetir este loop.
    while (1) {
        MPU6050_Read();
        PORTDbits.RD2 = !RD2;
        __delay_ms(50);
        //PORTBbits.RB1 = 1;
    }


}

//******************************************************************************
// Configuración
//******************************************************************************

void setup(void) {
    // al final no se utilizaron las interrupciones
//    INTCONbits.PEIE=1;
//    PIE1bits.RCIE=1;
//    PIR1bits.RCIF=0;
//    INTCONbits.GIE=1;
    // Todos los bits utilizados se configuran como salidas, menos los primeros
    // 2 bits del puerto B y el primero del A, debido a que allí estan los push/POT. Ansel y Anselh 
    // se ponen en 1 solamente donde hayan entradas digitales. 
    TRISE = 0;
    PORTE = 0;
    ANSEL = 0b00000000;
    ANSELH = 0;
    TRISB = 0;
    PORTB = 0;
    TRISC = 0b00000000;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
    PORTA = 0;
    TRISA = 0b00000000;
 
}

//******************************************************************************
// Funciones
//******************************************************************************

