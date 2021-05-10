/**
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/gpio.c"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.c"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"


int main(void)
{
    //configuracion del reloj: frecuencia de reloj de 40 MHz porque se uso el PLL
    // se utiliza el oscilador principal
    SysCtlClockSet ( SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ );
    //se habilitan los perifericos en el puerto F, RCGCGPIO.5 = 1
    SysCtlPeripheralEnable ( SYSCTL_PERIPH_GPIOF );
    // TABLA parte 2:
    //Para configurar los timers:GPTMCTL, GPTMCFG, GPTMTnMR, GPTMTnILR, GPTMIMR, GPTMRIS,GPTMICR
    //Para configurar UART: RCGCUART, RCGCGPIO, GPIOPCTL, UARTIBRD, UARTFBRD, UARTLCRH
    //Para configurar USB: RCGCUSB, RCGCGPIO, GPIOPCTL, USBGPCS
    //Para configurar PWM: RCGC0, RCGC2, GPIOAFSEL, GPIOPCTL, PWM0CTL, PWM0GENA, PWM0GENB, PWM0LOAD, PWM0CMPA,PWM0CMPB, PWM0CTL,PWMENABLE
    //Para configurar ADC: RCGCADC, RCGCGPIO, GPIODEN, GPIOAMSEL, ADCSSPRI

    //configuracion de los LEDs como salidas: rojo, azul y verde en ese orden
    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE,GPIO_PIN_1);
    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE, GPIO_PIN_3);
    // TABLA parte 3:
    // se encontro la funcion de SysCtlDelay, donde el unico parametro es la duracion del delay en nanosegundos

    //configuracion del boton 1 como input ==> weak pull up
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE , GPIO_PIN_4 );
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4, GPIO_STRENGTH_4MA , GPIO_PIN_TYPE_STD_WPU );

    while(1) {
        // el valor de 8 enciende el pin 3 (verde)
        // el valor de 2 enciende el pin 1 (rojo)
        // el valor de 10 (A) enciende los pines 1 y 3 (verde + rojo = amarillo) para colores de sintesis
        if ((GPIOPinRead (GPIO_PORTF_BASE,GPIO_PIN_4) & 0x16)==0) {
            while ((GPIOPinRead (GPIO_PORTF_BASE,GPIO_PIN_4) & 0x16)==0) { }

            // verde
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x08);
            SysCtlDelay (20000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
            SysCtlDelay (5000000);
            // verde parpadeante
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x08);
            SysCtlDelay (5000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
            SysCtlDelay (5000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x08);
            SysCtlDelay (5000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
            SysCtlDelay (5000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x08);
            SysCtlDelay (5000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
            SysCtlDelay (5000000);
            // amarillo
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x0A);
            SysCtlDelay (20000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
            SysCtlDelay (5000000);
            // rojo
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x02);
            SysCtlDelay (20000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
            SysCtlDelay (5000000);

        }


    }

}
