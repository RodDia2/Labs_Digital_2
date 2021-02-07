# include "TMR0.h"

void initTMR0(void){ //Interrupciones cada 2,5 ms
  // se configuran los bits para tener interrupciones en el TMR0
  INTCONbits.GIE = 1;
  INTCONbits.PEIE = 1;
  INTCONbits.T0IE = 1;
  INTCONbits.T0IF = 0;
  // se configura un prescaler de 64
  OPTION_REG = 0b10000101;
  // con la calculadora proporcionada, se calcula el valor del TMR0
  TMR0 = 176;
}
