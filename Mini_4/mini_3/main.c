

/**
 * main.c
 *  Rodrigo Díaz, 18265
 * Digital II, mini3
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include <string.h>
#include "grlib/grlib.h"
#include "driverlib/pin_map.h"

//*********variables*******************
uint8_t numero = 0;
uint8_t numero2 = 0;
uint8_t p1 = 0;
uint8_t p2 = 0;
uint8_t p3 = 0;
uint8_t p4 = 0;

char send;

uint8_t c1 = 0;
uint8_t c2 = 0;
uint8_t c3 = 0;
uint8_t c4 = 0;

uint8_t b1 = 0;
uint8_t b2 = 0;
uint8_t b3 = 0;
uint8_t b4 = 0;
//**********prototipos******************

//void 7seg(uint8_t numero);
void DatosUart(char *Dat);

//**********codigo principal************
int main(void)
{
    //configuracion del reloj: frecuencia de reloj de 40 MHz porque se uso el PLL
    // se utiliza el oscilador principal
    SysCtlClockSet ( SYSCTL_SYSDIV_5 | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ );

    // en el puerto A se utilizara para los pushbuttons, PINES 2,3,4,5 con weak pull-up
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){}
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // se utilizara el puerto D para las leds azules PINES 0,1,2,3, 6
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}
    // pines del 0 al 7 se configuran como outputs
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6);

    // se utilizara el puerto E para los leds rojos PINES 1,2,3,4
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}
    // pines del 0 al 7 se configuran como outputs
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);

    // CONFIGURACION UART
    SysCtlPeripheralEnable (SYSCTL_PERIPH_UART1);
    //while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1)){}
    // se habilita el puerto D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART4)){}

    // se configuran los bits rx y tx
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);

    // los pines del uart se controlan por perifericos
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
    // se siguen los pasos de la guia
    UARTDisable(UART1_BASE);
    // se setea el uart0 a 115200 baudios, 8 data bits, 1 stop bit y sin paridad
    UARTConfigSetExpClk(UART1_BASE, 16000000, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    // se habilita
    UARTEnable (UART1_BASE);

    // Estados Iniciales
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6, 0xFF);

    //GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0xFF);
    // loop principal
    while (1) {

        // boton 4
        if (!GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5)) {
            SysCtlDelay (500000); // delay 50 ms
            if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5)) {
                if (b4 == 0) {
                    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0xFF);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0x00);
                    p4 = 0;
                    DatosUart("a");
                    b4 = 1;
                }
                else if (b4 == 1) {
                    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0xFF);
                    p4 = 1;
                    DatosUart("b");
                    b4 = 0;
                }
            }
        }

        // boton 3
        if (!GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4)) {
            SysCtlDelay (500000); // delay 50 ms
            if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4)) {
                if (b3 == 0) {
                    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xFF);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
                    p3 = 0;
                    DatosUart("c");
                    b3 = 1;
                }
                else if (b3 == 1) {
                    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xFF);
                    p3 = 1;
                    DatosUart("d");
                    b3 = 0;
                }
            }
        }

        // boton 2
        if (!GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3)) {
            SysCtlDelay (500000); // delay 50 ms
            if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3)) {
                if (b2 == 0) {
                    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0xFF);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0x00);
                    p2 = 0;
                    DatosUart("e");
                    b2 = 1;
                }
                else if (b2 == 1) {
                    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0xFF);
                    p2 = 1;
                    DatosUart("f");
                    b2 = 0;
                }
            }
        }

        // boton 1
        if (!GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2)) {
            SysCtlDelay (500000); // delay 50 ms
            if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2)) {
                if (b1 == 0) {
                    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0xFF);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0x00);
                    p1 = 0;
                    DatosUart("g");
                    b1 = 1;
                }
                else if (b1 == 1) {
                    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0xFF);
                    p1 = 1;
                    DatosUart("h");
                    b1 = 0;
                }
            }
        }


        // se saca el numero de parqueos disponibles
        numero = p1 + p2 + p3 + p4;
/*
        if (send & 0x01) {c1 = 1;}
        else {c1 = 0;}
        if (send & 0x02) {c2 = 1;}
        else {c2 = 0;}
        if (send & 0x04) {c3 = 1;}
        else {c3 = 0;}
        if (send & 0x08) {c4 = 1;}
        else {c4 = 0;}
*/
        numero2 = c1 + c2 + c3 + c4;
/*
        switch(numero2) {
        case 0:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0x77);
            break;
        case 1:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0x14);
            break;
        case 2:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0xB3);
            break;
        case 3:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0xB6);
            break;
        case 4:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0xD4);
            break;
        default:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0x08);
            break;
        }
*/





       // SysCtlDelay (5000000); // DELAY 0.5s
    }
}

void DatosUart(char *Dat){
    while(UARTBusy(UART1_BASE));
    while(*Dat != '\0')
    {
        UARTCharPut(UART1_BASE, *Dat++);
    }
}
