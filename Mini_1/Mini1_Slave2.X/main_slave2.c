//*****************************************************************************
/*
 * File:   main.c
 * Author: Rodrigo Díaz, basado en el ejemplo de Pablo Mazariegos

 * Código Esclavo 2

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
#include "INT_B.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
uint8_t contador = 0;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
// se utilizaron las interrupciones de SPI y de on-change en el puerto B
void __interrupt() isr(void){
    // primero se revisa la bandera SSPIF
   if(SSPIF == 1){
        //PORTD = spiRead();
       // se escribe en el buffer para mandar el dato
        spiWrite(contador);
        // se apaga la bandera manualmente
        SSPIF = 0;
    }
   // luego, se revisa la bandera RBIF
   if (INTCONbits.RBIF == 1) {
        // se realiza un antirebote, se sabe que los delays no son recomendables
       // pero se sabe que este slave solo debe ser un contador, entonces 
       // por el momento no debería haber problema. 
        if (PORTBbits.RB0 == 0) {
            __delay_ms(50);
            if (PORTBbits.RB0 == 1) {
                // se incrementa contador con un boton y se apaga la bandera
                contador ++;
                INTCONbits.RBIF = 0;
            }
        }
        // se realiza antirebote para el boton de decremento
        if (PORTBbits.RB1 == 0) {
            __delay_ms(50);
            if (PORTBbits.RB1 == 1) {
                // se decrementa el contador y se apaga la bandera
                contador --;
                INTCONbits.RBIF = 0;
            }
        }
    }
   
}
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    // se realiza el seteo de bits en el setup
    setup();
    // se inicializan los primeros dos bits del puerto B para las interrupciones
    // al cambio.
    initIntB(0);
    initIntB(1);
    
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        // se manda el valor del contador al puerto D donde están los LEDS
        PORTD = contador;
//       if (PORTDbits.RD0 == 0){
//            __delay_ms(100);
//            if(PORTDbits.RD0==1){
//            contador ++;
//            PORTB = contador;
//            //spiWrite(PORTB);
//            }            
//        }
        
        //__delay_ms(250);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    // Todos los bits utilizados se configuran como salidas, menos los primeros
    // 2 bits del puerto B, debido a que allí estan los push. Ansel y Anselh 
    // se ponen en 1 solamente donde hayan entradas digitales. 
    ANSEL = 0b00000000;
    ANSELH = 0;
    
    TRISA = 0b00000000;
    TRISB = 0b00000011;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select, como entrada
   // se configura como slave
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}
