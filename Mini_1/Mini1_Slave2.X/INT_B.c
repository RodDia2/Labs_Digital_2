
#include "INT_B.h"
#include <stdint.h>
#include <pic16f887.h>

// esta función habilita las interrupciones por cambio en el puerto B y 
// habilita el bit deseado para que se habilite la interrupción
void initIntB(uint8_t select) {
    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    switch (select) {
        case 0: IOCBbits.IOCB0 = 1;
        case 1: IOCBbits.IOCB1 = 1;
        case 2: IOCBbits.IOCB2 = 1;
        case 3: IOCBbits.IOCB3 = 1;
        case 4: IOCBbits.IOCB4 = 1;
        case 5: IOCBbits.IOCB5 = 1;
        case 6: IOCBbits.IOCB6 = 1;
        case 7: IOCBbits.IOCB7 = 1;
        default: IOCBbits.IOCB0 = 1;
    }
}
