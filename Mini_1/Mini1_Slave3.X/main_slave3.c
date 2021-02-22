//*****************************************************************************
/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de implementación de la comunicación SPI 
 * Código Esclavo
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
void __interrupt() isr(void){
   if(SSPIF == 1){
        //PORTD = spiRead();
        spiWrite(valor_adc);
        SSPIF = 0;
    }
   if (PIR1bits.ADIF == 1) {
        // se pasa el valor del ADRESH a la variable
        valor_adc = ((ADRESH * 150)/255);
        // se apaga la bandera
        PIR1bits.ADIF = 0;
    }
   
    
   
}
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    initADC();
    ADC_Select(0);
    ADCON1bits.VCFG0 = 1;
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        adc();
        PORTD = valor_adc;
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
    TRISAbits.TRISA5 = 1;       // Slave Select
   
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}

