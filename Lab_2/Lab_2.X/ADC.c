# include "ADC.h"

void initADC(void){
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    ADCON0 = 0b01000001;
    ADCON1 = 0b00000000;
}