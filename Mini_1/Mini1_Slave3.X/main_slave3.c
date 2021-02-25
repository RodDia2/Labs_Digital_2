//*****************************************************************************
/*
 * File:   main.c
 * Author: Rodrigo Díaz, basado en el ejemplo de Pablo Mazariegos

 * Código Esclavo 3

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
#include <pic16f887.h>


#include "SPI.h"
#include "ADC.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
uint8_t contador = 0;
uint8_t valor_adc = 0;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
// se utilizaron las interrupciones del SPI y de conversión ADC
void __interrupt() isr(void){
    // primero se revisa la bandera SSPIF 
   if(SSPIF == 1){
        //PORTD = spiRead();
       // se escribe el valor en el buffer para comenzar transmición
        spiWrite(valor_adc);
        // se apaga la bandera manualmente
        SSPIF = 0;
    }
   // luego se revisa la bandera ADIF para ver si ya termino una conversión
   if (PIR1bits.ADIF == 1) {
        // se pasa el valor del ADRESH a la variable de valor_adc, pero se 
       // realiza la conversión respectiva para el valor de voltaje de referencia
       // manual que se estableció físicamente.
        valor_adc = ((ADRESH * 150)/255);
        // se apaga la bandera
        PIR1bits.ADIF = 0;
    }
   
    
   
}
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    // se realiza el seteo de bits en el setup
    setup();
    // se inicializa el ADC con interrupciones
    initADC();
    // solamente se va a utilizar el canal 0 analogico
    ADC_Select(0);
    // el voltaje de referencia se setea en el bit RA3, para cambiar los valores
    // de voltaje de 0-5 a 0-1.5 .
    ADCON1bits.VCFG0 = 1;
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    // este loop siempre se repite
    while(1){
        // se llama a la función que inicializa la conversión
        adc();
        // se manda el valor al puerto para las primeras pruebas
        //PORTD = valor_adc;
        
        // luego se realiza el samáforo. Si la temperatura es menor a 25 se 
        // enciende RD2 si está entre 25 y 36 se enciende sólo el RD1 y si es 
        // mayor a 36 solo se prende el RD0
        if (valor_adc < 25) {
            PORTDbits.RD2 = 1;
            PORTDbits.RD1 = 0;
            PORTDbits.RD0 = 0;
                  
        } else if (25 <= valor_adc && valor_adc < 36) {
            PORTDbits.RD2 = 0;
            PORTDbits.RD1 = 1;
            PORTDbits.RD0 = 0;
        } else if (valor_adc >= 36) {
            PORTDbits.RD2 = 0;
            PORTDbits.RD1 = 0;
            PORTDbits.RD0 = 1;
        }

    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    // Todos los bits utilizados se configuran como salidas, menos 
    // el primero del A, debido a que allí esta el POT. Ansel y Anselh 
    // se ponen en 1 solamente donde hayan entradas analogicas.
    ANSEL = 0b00000001;
    ANSELH = 0;
    
    TRISA = 0b00000001;
    TRISB = 0b00000000;
    TRISC = 0;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    
    //ADCON1bits.VCFG0 = 1;
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select, como entrada
    // se configura como esclavo
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}

