/*  
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_H
#define	LCD_H

#include <xc.h> // include processor files - each processor file is guarded.  

//#ifndef __LCD_H_
//#define	__LCD_H_

#ifndef RS
#define RS RE0
#endif 

#ifndef EN
#define EN RE1
#endif 

#ifndef D0
#define D0 RB0 
#endif 
#ifndef D1
#define D1 RB1 
#endif 
#ifndef D2
#define D2 RB2 
#endif 
#ifndef D3
#define D3 RB3 
#endif 
#ifndef D4
#define D4 RB4 
#endif 
#ifndef D5
#define D5 RB5 
#endif 
#ifndef D6
#define D6 RB6 
#endif 
#ifndef D7
#define D7 RB7 
#endif 

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void LCD_Init(void);
void LCD_Write_Char(char a);
void LCD_Command(char a);
void LCD_Port(char a);
void LCD_clear(void);
void LCD_Set_Cursor(char x,char y);
void LCD_Write_String(char *a);
//void Lcd_Shift_Right(void);
//void Lcd_Shift_Left(void);

#endif	/* LCD_H */

