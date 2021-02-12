/* 
 * File:   main.c
 * Author: Rodrigo Díaz
 * Digital 2
 * Lab3
 */

//******************************************************************************
// Importación de librerías
//******************************************************************************
// se incluyen las librerias que se utilizan y las creadas para el lab
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <pic16f887.h>

#include "ADC.h"
#include "LCD.h"


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
#pragma config BOREN = ON      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)



//******************************************************************************
// Variables
//******************************************************************************
// la variable XTAL FREQ es necesaria para que funcionen los delays
#define _XTAL_FREQ 8000000
// se crean las variables a utilizar con su tamano 
uint8_t valor_adc = 0;
uint8_t contador = 0;
float V1 = 0.00;
float V2 = 0.00;
char pantalla[20];



//******************************************************************************
// Interrupción
//******************************************************************************

void __interrupt() ISR(void) {
    /*if (PIR1bits.ADIF == 1) {
        // se pasa el valor del ADRESH a la variable
        valor_adc = ADRESH;
        // se apaga la bandera
        PIR1bits.ADIF = 0;
    }
     */   
}

//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void setup(void);
float LeerADC(uint8_t x);
//******************************************************************************
// Ciclo principal
//******************************************************************************

void main(void) {
    // 
    //librerias creadas
    setup();
    initADC();
    LCD_Init();
    LCD_clear();
   
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    // como es un while(1) siempre se va a repetir este loop.
    while (1) {
        
        V1 = LeerADC(0);
        V2 = LeerADC(1);
        
        sprintf(pantalla, "%1.2f   %1.2f   %d", V1,V2,contador);
        
        LCD_clear();
        LCD_Set_Cursor(1,1);
        LCD_Write_String("V1   V2   CONT");
        LCD_Set_Cursor(2,1);
        LCD_Write_String(pantalla);
        __delay_ms(500);
    }

  
}

//******************************************************************************
// Configuración
//******************************************************************************

void setup(void) {
    // Todos los bits utilizados se configuran como salidas, menos los primeros
    // 2 bits del puerto B y el primero del A, debido a que allí estan los push/POT. Ansel y Anselh 
    // se ponen en 1 solamente donde hayan entradas digitales. 
    TRISE = 0;
    PORTE = 0;
    ANSEL = 0b00000011;
    ANSELH = 0;
    TRISB = 0;
    PORTB = 0;
    TRISC = 0;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
    PORTA = 0;
    TRISA = 0b00000011;
    
    //INTCONbits.PEIE = 1;
    //PIE1bits.ADIE = 1;
    //PIR1bits.ADIF = 0;
    //ADCON0 = 0b01000001;
    //ADCON1 = 0b00000000;
}

//******************************************************************************
// Funciones
//******************************************************************************

float LeerADC(uint8_t x){
    float a = 0.0;
    ADC_Select(x); // se selecciona el bit analogico
    ADCON0bits.ADON=1;//se enciende el enable del ADC
    __delay_us(20);
    ADCON0bits.GO=1;//se comienza la conversion
    while(ADCON0bits.GO_DONE==1){};
    //a = (ADRESH * 5)/255;
    a = ADRESH * 0.0196;
    return a;
}