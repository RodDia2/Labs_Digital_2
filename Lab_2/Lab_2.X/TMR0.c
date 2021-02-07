# include "TMR0.h"

void initTMR0(void){ //Interrupciones cada 5 ms
  
  INTCONbits.GIE = 1;
  INTCONbits.PEIE = 1;
  INTCONbits.T0IE = 1;
  INTCONbits.T0IF = 0;
  
  OPTION_REG = 0b10000101;
  
  TMR0 = 176;
}
