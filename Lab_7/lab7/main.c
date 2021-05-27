
/**
 * main.c
 * Rodrigo Díaz, 18265
 * Digital II, lab7
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

// ********variables***********

uint32_t ui32Period;
uint32_t ui32Status;

uint8_t flag_r = 0;
uint8_t flag_v = 0;
uint8_t flag_a = 0;
char color='z';
char colorpre='z';
int cambio = true;

#define led_r GPIO_PIN_1
#define led_v GPIO_PIN_3
#define led_a GPIO_PIN_2

//*******************PROTOTIPOS**************************

void Timer0IntHandler(void);
void UARTIntHandler(void);

//*********************CODIGO PRINCIPAL******************************************
int main(void)
{
    //configuracion del reloj: frecuencia de reloj de 40 MHz porque se uso el PLL
    // se utiliza el oscilador principal
    SysCtlClockSet ( SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ );
    //se habilitan los perifericos en el puerto F, RCGCGPIO.5 = 1
    SysCtlPeripheralEnable ( SYSCTL_PERIPH_GPIOF );
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    //configuracion de los LEDs como salidas: rojo, azul y verde en ese orden
    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE,GPIO_PIN_1);
    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE, GPIO_PIN_3);

    // CONFIG TIMER
    // Se habilita el reloj para el temporizador
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // se siguen los pasos de la guia en el datasheet
    SysCtlDelay (5);
    TimerDisable(TIMER0_BASE, TIMER_A|TIMER_B);
    // se configura el TMR0 como periódico, este es de 32 bits
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // Se calcula el período para el temporizador (1 seg)
    ui32Period = (SysCtlClockGet()) / 2;
    // Establecer el periodo del temporizador
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);
    // Se habilita el Timer
    TimerEnable(TIMER0_BASE, TIMER_A|TIMER_B);
    // interrupcion TMR0
    IntMasterEnable();
    // interrupcion por timeout
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // establecer ISR
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    // habilitar int en el timer tmr0A
    IntEnable(INT_TIMER0A);
    // volver a habilitar el tmrA
    TimerEnable(TIMER0_BASE, TIMER_A);

    // CONFIG UART
    // se habilita el uart0
    SysCtlPeripheralEnable (SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){}
    // se habilita el puerto a
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){}
    // se configuran los bits rx y tx
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    // se habilitan todas las interrupciones
    //IntMasterEnable();
    // los pines del uart se controlan por perifericos
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // se siguen los pasos de la guia
    UARTDisable(UART0_BASE);
    // se setea el uart0 a 115200 baudios, 8 data bits, 1 stop bit y sin paridad
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    // se habilita la interrupcion uart0
    IntEnable (INT_UART0);
    // se habilitan las interrupciones del uart0 solamente cuando se reciben datos
    UARTIntEnable (UART0_BASE, UART_INT_RX | UART_INT_RT);
    UARTEnable (UART0_BASE);

    // se setea la prioridad de las interrupciones al uart, prueba
    //IntPrioritySet(INT_UART0, 0x0);
    //IntRegister(INT_UART0, UARTIntHandler);
    //UARTFIFOEnable(UART0_BASE);
    //UARTFIFOLevelSet(UART0_BASE,UART_FIFO_TX1_8,UART_FIFO_RX1_8);

    while (1) {
       // if ((TimerValueGet(TIMER0_BASE, TIMER_A)& 0x16)==0) {
          //  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x08);
          //  SysCtlDelay (5000000);
          //  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
          // SysCtlDelay (5000000);
      // }

    }
}
// ***************** FUNCIONES **************************

//*******TMR0 HANDLER******************************
void Timer0IntHandler(void){
    // se realiza un clear de la bandera interrupcion
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // se va a realizar un titileo con una variable alternante entre true y false.
    if (cambio){
        // cuando cambio es true, apaga el rgb
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0);
    } else {
        // si es false, despliega el valor que se le mando por uart y guarda el valor previo
        switch(color) {
            case 'r':
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x02);
                colorpre='r';
                break;
            case 'g':
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x08);
                colorpre='g';
                break;
            case 'b':
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x04);
                colorpre='b';
                break;
            case 'n':
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0);
                colorpre='n';
                break;
         }

       }
    cambio = !cambio;

}


//*******UART HANDLER***************************
void UARTIntHandler(void){
    // obtener el status
    ui32Status = UARTIntStatus(UART0_BASE, true);
    // hacerle clear el interrupt
    UARTIntClear(UART0_BASE, ui32Status);
    // leer un dato del UART
    while(UARTCharsAvail(UART0_BASE)){
        color = UARTCharGet(UART0_BASE);
        UARTCharPutNonBlocking(UART0_BASE,color);
        // si se repite el color, parar de titilar
        if(color==colorpre){
            color='n';
        }
    }

}
