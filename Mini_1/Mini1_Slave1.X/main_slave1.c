//*****************************************************************************
/*
 * File:   main.c
 * Author: Rodrigo Díaz, basándose en el código ejemplo de Pablo Mazariegos
 
 * Código Esclavo 1

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

#include "ADC.h"
#include "SPI.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
//uint8_t contador = 0;
uint8_t valor_adc = 0;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
// en las interrupciones se revisa la bandera SSPIF para la comunicación SPI y 
// la bandera ADIF para la conversión ADC.
void __interrupt() isr(void){
    // se revisa la bandera SSPIF
   if(SSPIF == 1){
        //PORTD = spiRead();
       // se escribe un valor al buffer por medio de la función para que se 
       //comience la transmición de datos. 
        spiWrite(valor_adc);
        // la bandera debe apagarse manualmente
        SSPIF = 0;
    }
   // ahora se revisa la bandera de ADIF
   if (PIR1bits.ADIF == 1) {
        // se recibe el valor del ADRESH a la variable
        valor_adc = ADRESH;
        // se apaga la bandera manualmente
        PIR1bits.ADIF = 0;
    }
}
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    // primero se llama al setup para setear los pines
    setup();
    // se inicializa la conversión ADC corrimiento a la izquierda con interrupts
    initADC();
    // solamente se va a utilizar el canal 0 analógico
    ADC_Select(0);
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        // este loop siempre se repite 
        // primero se llama a la función adc para iniciar conversiones
        adc();
        // la variable recibida del adc se despliega en el puerto C para 
        // verificar un buen funcionamiento
        PORTD = valor_adc;

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
    TRISB = 0;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select, como entrada
    // se configura como un slave
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}