/* 
 * File:   main.c
 * Author: Rodrigo Díaz
 * 
 */

//******************************************************************************
// Importación de librerías
//******************************************************************************

#include <xc.h>

//******************************************************************************
// Palabra de configuración
//******************************************************************************

// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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


#define _XTAL_FREQ 8000000

#define LED_Rojo PORTEbits.RE0
#define LED_Amarillo PORTEbits.RE1
#define LED_Verde PORTEbits.RE2

//******************************************************************************
// Variables
//******************************************************************************

char counter = 0;

//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void setup(void);
void Semaforo(void);
void avance(void);
void ganador(void);

//******************************************************************************
// Ciclo principal
//******************************************************************************

void main(void) {

    setup();

    //**************************************************************************
    // Loop principal
    //**************************************************************************

    while (1) {
      
        if (PORTBbits.RB0 == 0) {
            __delay_ms(50);
            if (PORTBbits.RB0 == 1) {
                semaforo();
                avance();
                ganador();
            }
        }
    }

  
}

//******************************************************************************
// Configuración
//******************************************************************************

void setup(void) {
    TRISE = 0;
    PORTE = 0;
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0b00000111;
    PORTB = 0;
    TRISC = 0;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
}

//******************************************************************************
// Funciones
//******************************************************************************

void semaforo(void) {
    PORTC = 0;
    PORTD = 0;
    LED_Rojo = 1;
    __delay_ms(500);
    LED_Rojo = 0;
    LED_Amarillo = 1;
    __delay_ms(500);
    LED_Amarillo = 0;
    LED_Verde = 1;
    __delay_ms(500);
    LED_Verde = 0;
}

void avance(void) {
    
}