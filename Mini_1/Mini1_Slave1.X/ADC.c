# include "ADC.h"

#define _XTAL_FREQ 8000000

void initADC(void){
    //INTCONbits.GIE = 1;
    
    // se realiza el setup para utilizar interrupciones y apagar la bandera
    // se tiene un corrimiento hacia la izquierda para obtener los valores 
    // mas significativos.
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    ADCON0 = 0b01000001;
    ADCON1 = 0b00000000;
}

void adc(void) {
    __delay_us(8);
    ADCON0bits.GO_DONE = 1;
}
