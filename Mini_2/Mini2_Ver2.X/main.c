/*
 * File:   main.c
 * Author: Rodrigo Díaz, basado en el código de Khaled Magdy
 * Extraído de https://deepbluembedded.com/mpu6050-with-microchip-pic-accelerometer-gyroscope-interfacing-with-pic/
 * Creado lunes 8 de marzo del 2021
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
#include "MPU.h"

//******************************************************************************
// Variables
//******************************************************************************
#define _XTAL_FREQ 8000000
// se crean las variables que se van a utilizar 
uint8_t recibido = 0;
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
    // se realiza el seteo de los puertos y tris
    setup();
    // se inicializa la comunicacion uart tanto para mandar como para recibir
    UART_TX_Init();  
    // se inicializa la configuración del sensor acelerómetro
    MPU6050_Init();
    // se configura el oscilador
    setOsc();
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1)
	{
        RD2 = !RD2;  // Blink LED, para verificar que se ejecuta el loop
        MPU6050_Read(); // se lee in valor y se manda por usart
        if (RCIF==1){ // la bandera RCIF se levanta aunque no se utilice interrupciones
            recibido = USART_Read(); // se guarda el valor recibido de usart en una variable
            switch(recibido) { // se crea un switch para saber cual led encender o apagar
                case 0x01: PORTDbits.RD1 = 1;PORTCbits.RC2 = 0;
                case 0x02: PORTDbits.RD1 = 0;PORTCbits.RC2 = 0;
                case 0x04: PORTDbits.RD0 = 1;PORTCbits.RC2 = 0;
                case 0x08: PORTDbits.RD0 = 0;PORTCbits.RC2 = 0;
                default: PORTCbits.RC2 = 1;
            }
        }
        // se tiene el delay recomendado para el sensor
        __delay_ms(50);
    }
    return;
}

//******************************************************************************
// Configuración
//******************************************************************************
void setup (void) {
    // para los puertos, todos se inician en 0. Para los tris, los 0 simbolizan 
    // entradas y los 1 salidas. En este caso solo se tiene entrada en C7 por USART.
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
    // se cambia el oscilador a 8MHz del default de 4 
    OSCCONbits.IRCF = 0b111; // 8MHz
}

