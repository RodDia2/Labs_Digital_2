
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

uint32_t ui32Period;

int main(void)
{
    //configuracion del reloj: frecuencia de reloj de 40 MHz porque se uso el PLL
    // se utiliza el oscilador principal
    SysCtlClockSet ( SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ );
    //se habilitan los perifericos en el puerto F, RCGCGPIO.5 = 1
    SysCtlPeripheralEnable ( SYSCTL_PERIPH_GPIOF );
    //configuracion de los LEDs como salidas: rojo, azul y verde en ese orden
    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE,GPIO_PIN_1);
    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE, GPIO_PIN_3);
    // se configura el TMR0 como periódico, este es de 32 bits
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // Se habilita el reloj para el temporizador
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // Se calcula el período para el temporizador (1 seg)
    ui32Period = (SysCtlClockGet()) / 2;
    // Establecer el periodo del temporizador
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);
    // Se habilita el Timer
    TimerEnable(TIMER0_BASE, TIMER_A);

}
