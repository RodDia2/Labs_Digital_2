
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USART_H
#define	USART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>  

void Set_Baud_Rate(void);

void Init_Transmit(void);

void Init_Receive(void);

void USART_Write(uint8_t a);

void USART_Write_String(char *a);

uint8_t USART_Read(void);

//char USART_Init(const long int baudrate);
#endif	/* USART_H */

