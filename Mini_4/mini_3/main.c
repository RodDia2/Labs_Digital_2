

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

int send = 0;

uint8_t c1 = 0;
uint8_t c2 = 0;
uint8_t c3 = 0;
uint8_t c4 = 0;
//**********prototipos******************

//void 7seg(uint8_t numero);

//**********codigo principal************
int main(void)
{
    //configuracion del reloj: frecuencia de reloj de 40 MHz porque se uso el PLL
    // se utiliza el oscilador principal
    SysCtlClockSet ( SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ );

    // como solo se tiene el puerto B completo, se usara para el 7 segmentos por facilidad PINES 0-7
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
    // pines del 0 al 7 se configuran como outputs
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

    // en el puerto A se utilizara para los pushbuttons, PINES 2,3,4,5 con weak pull-up
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){}
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

    // se utilizara el puerto C para las leds azules PINES 4,5,6,7
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)){}
    // pines del 0 al 7 se configuran como outputs
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

    // se utilizara el puerto E para los leds rojos PINES 1,2,3,4
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}
    // pines del 0 al 7 se configuran como outputs
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);

    // CONFIGURACION UART
    SysCtlPeripheralEnable (SYSCTL_PERIPH_UART2);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART2)){}
    // se habilita el puerto D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART2)){}
    // se configuran los bits rx y tx
    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);
    // los pines del uart se controlan por perifericos
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    // se siguen los pasos de la guia
    UARTDisable(UART2_BASE);
    // se setea el uart0 a 115200 baudios, 8 data bits, 1 stop bit y sin paridad
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    // se habilita
    UARTEnable (UART2_BASE);

    // loop principal
    while (1) {

        // boton 4
        if ((GPIOPinRead (GPIO_PORTA_BASE,GPIO_PIN_5) & 0x20)==0) {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0x08);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
            p4 = 0;
            send = (send & 0x0E);
        }
        else {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x10);
            p4 = 1;
            send = (send | 0x01);
        }

        // boton 3
        if ((GPIOPinRead (GPIO_PORTA_BASE,GPIO_PIN_4) & 0x10)==0) {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x10);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
            p3 = 0;
            send = (send & 0x0D);
        }
        else {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x20);
            p3 = 1;
            send = (send | 0x02);
        }

        // boton 2

        if ((GPIOPinRead (GPIO_PORTA_BASE,GPIO_PIN_3) & 0x08)==0) {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x04);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
            p2 = 0;
            send = (send & 0x0B);
        }
        else {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x00);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x40);
            p2 = 1;
            send = (send | 0x04);
        }


        // boton 1
        if ((GPIOPinRead (GPIO_PORTA_BASE,GPIO_PIN_2) & 0x04)==0) {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x02);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x0);
            p1 = 0;
            send = (send & 0x07);
        }
        else {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x00);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x80);
            p1 = 1;
            send = (send | 0x08);
        }

        // se saca el numero de parqueos disponibles
        numero = p1 + p2 + p3 + p4;

        if (send & 0x01) {c1 = 1;}
        else {c1 = 0;}
        if (send & 0x02) {c2 = 1;}
        else {c2 = 0;}
        if (send & 0x04) {c3 = 1;}
        else {c3 = 0;}
        if (send & 0x08) {c4 = 1;}
        else {c4 = 0;}

        numero2 = c1 + c2 + c3 + c4;

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

        UARTCharPut(UART2_BASE, send);


        //SysCtlDelay (5000000); // DELAY 0.5s
    }
}
