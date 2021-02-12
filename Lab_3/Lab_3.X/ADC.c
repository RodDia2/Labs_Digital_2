#include <xc.h> 
#include <stdint.h> 
#include "ADC.h"
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

void ADC_Select (uint8_t a) {
    switch(a){
        case 0: ADCON0bits.CHS=0; break;
        case 1: ADCON0bits.CHS=1; break;
        case 2: ADCON0bits.CHS=2; break;
        case 3: ADCON0bits.CHS=3; break;
        case 4: ADCON0bits.CHS=4; break;
        case 5: ADCON0bits.CHS=5; break;
        case 6: ADCON0bits.CHS=6; break;
        case 7: ADCON0bits.CHS=7; break;
        case 8: ADCON0bits.CHS=8; break;
        case 9: ADCON0bits.CHS=9; break;
        case 10: ADCON0bits.CHS=10; break;
        case 11: ADCON0bits.CHS=11; break;
        case 12: ADCON0bits.CHS=12; break;
        case 13: ADCON0bits.CHS=13; break;
        default: ADCON0bits.CHS=0; break;
    }
}
