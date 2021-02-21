/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <pic16f887.h>


void initADC(void);

void adc(void);

void ADC_Select (uint8_t a);


#endif	/* ADC_H */