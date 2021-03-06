/* 
 * File:   main.c
 * Author: Rodrigo D�az
 * Digital 2
 * Lab3
 */

//******************************************************************************
// Importaci�n de librer�as
//******************************************************************************
// se incluyen las librerias que se utilizan y las creadas para el lab
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
//#include <pic16f887.h>

#include "ADC.h"
#include "LCD.h"
#include "USART.h"


//******************************************************************************
// Palabra de configuraci�n
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



//******************************************************************************
// Variables
//******************************************************************************
// la variable XTAL FREQ es necesaria para que funcionen los delays
#define _XTAL_FREQ 8000000
// se crean las variables a utilizar con su tamano 
uint8_t valor_adc = 0;
uint8_t contador = 0;
char recibido = 0;
float V1 = 0.0;
float V2 = 0.0;
char pantalla[20];

//char char_array[];

//******************************************************************************
// Interrupci�n
//******************************************************************************

void __interrupt() ISR(){
    
//    if(RCIF==1){
//        
//        //while (!RCIF);
//        recibido = RCREG;  
//        if(recibido == '+'){
//            contador++;
//        } 
//        if(recibido == '-'){
//            contador--;
//        }
//        else {
//            contador = 255;
//        }
//        //contador++;
//    }   
//    contador++;
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
    //funciones de las librerias creadas para inicializacion 
    setup();
    initADC();
    LCD_Init();
    LCD_clear();
    // el baud rate se pone en 9600
    Set_Baud_Rate();
    // se encienden los bits para USART tanto transmicion como recepcion 
    Init_Transmit();
    
    Init_Receive();
    
    
   
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    // como es un while(1) siempre se va a repetir este loop.
    while (1) {
        // se manda a llamar a la funcion que mapea los datos del potenciometro
        // a los 5V
        V1 = LeerADC(0);
        V2 = LeerADC(1);
        // Primero se mandan los strings a la terminal
        USART_Write_String("V1     V2   CONT \n");
        // se salta a la siguiente linea
        USART_Write(13);
        USART_Write(10);
        // la variable char de pantalla se le asignan los valores de voltaje
        // y del contador
        sprintf(pantalla, "%1.2f   %1.2f %3d", V1,V2,contador);
        // se muestran los datos, bajo a su nombre en string
        USART_Write_String(pantalla);
       // se vuelve a saltar una linea
        USART_Write(13);
        USART_Write(10);
        // se realiza un clear de la pantalla por seguridad
        LCD_clear();
        // en la primera fila, se despligan los titulos y en la segunda los valores
        LCD_Set_Cursor(1,1);
        LCD_Write_String("V1   V2    CONT");
        LCD_Set_Cursor(2,1);
        LCD_Write_String(pantalla);    
        // la bandera RCIF se activa aunque no hayan interrupciones
        if(RCIF==1){
            // se tranfiere el dato recibido en USART a la variable recibido
            // despues se revisa si es + o - para modificar el contador
            //while (!RCIF);
            recibido = RCREG;  
            if(recibido == '+'){
                contador++;
            } 
            if(recibido == '-'){
                contador--;
            }
            
        }   
        // finalmente hay un delay para que se logren distinguir los datos 
        // desplegados en la terminal
        __delay_ms(500);
       
        
    }


}

//******************************************************************************
// Configuraci�n
//******************************************************************************

void setup(void) {
    // al final no se utilizaron las interrupciones
//    INTCONbits.PEIE=1;
//    PIE1bits.RCIE=1;
//    PIR1bits.RCIF=0;
//    INTCONbits.GIE=1;
    // Todos los bits utilizados se configuran como salidas, menos los primeros
    // 2 bits del puerto B y el primero del A, debido a que all� estan los push/POT. Ansel y Anselh 
    // se ponen en 1 solamente donde hayan entradas digitales. 
    TRISE = 0;
    PORTE = 0;
    ANSEL = 0b00000011;
    ANSELH = 0;
    TRISB = 0;
    PORTB = 0;
    TRISC = 0b10000000;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
    PORTA = 0;
    TRISA = 0b00000011;
 
}

//******************************************************************************
// Funciones
//******************************************************************************

// esta funcion selecciona un puerto para leer una entrada ADC y recibe el dato
// despues, mapea el valor de 0-255 a un valor de 0-5V de tipo float. 
float LeerADC(uint8_t x){
    float a = 0.0;
    ADC_Select(x); // se selecciona el bit analogico
    ADCON0bits.ADON=1;//se enciende el enable del ADC
    __delay_us(20);
    ADCON0bits.GO=1;//se comienza la conversion
    while(ADCON0bits.GO_DONE==1){};
    //a = (ADRESH * 5)/255;
    // poner el el valor de 0.0196 mostro tener mas decimales 
    a = ADRESH * 0.0196;
    return a;
}