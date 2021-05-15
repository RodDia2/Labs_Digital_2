
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

#define led_r GPIO_PIN_1
#define led_v GPIO_PIN_3
#define led_a GPIO_PIN_2

//*******************PROTOTIPOS**************************

void Timer0IntHandler(void);

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
    // interrupcion por timeout
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // establecer ISR
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    // habilitar int en el timer tmr0A
    IntEnable(INT_TIMER0A);
    // volver a habilitar el tmrA
    TimerEnable(TIMER0_BASE, TIMER_A);

    while (1) {
       // if ((TimerValueGet(TIMER0_BASE, TIMER_A)& 0x16)==0) {
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x08);
            SysCtlDelay (5000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
            SysCtlDelay (5000000);
      // }

    }
}
// ***************** FUNCIONES **************************

//*******TMR0 HANDLER******************************
void Timer0IntHandler(void){

}
