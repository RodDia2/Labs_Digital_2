
#include <xc.h>
#include <stdint.h>

#include "LCD.h"
#define _XTAL_FREQ 4000000

//funcion para imprimir un caracter
void LCD_Write_Char(char a){
    RS = 1;             // => RS = 1
    LCD_Port(a);
    EN = 1;
    __delay_us(40);
    EN = 0;
    __delay_us(10);
}
// desplegar el valor en el puerto
void LCD_Port(char a){
    PORTD = a;
    //if(x & 1){D0=1;}else{D0=0;}
    //if(x & 2){D1=1;}else{D1=0;}
    //if(x & 4){D2=1;}else{D2=0;}
    //if(x & 8){D3=1;}else{D3=0;}
    //if(x & 16){D4=1;}else{D4=0;}
    //if(x & 32){D5=1;}else{D5=0;}
    //if(x & 64){D6=1;}else{D6=0;}
    //if(x & 128){D7=1;}else{D7=0;}

}
//funcion para enviar datos a la LCD
// los comandos se pueden encontrar en el datasheet
void LCD_Command(char a){
    RS=0;
    LCD_Port(a);
    EN=1;
    __delay_ms(4);
    EN=0;
    __delay_ms(2);
}
//limpiar LCD 
void LCD_clear(void){
    LCD_Command(0);
    LCD_Command(1);
}
//funcion para iniciar la LCD, basandose en el diagrama de flujo del datasheet
void LCD_Init(){
    RS=0;
    EN=0;
    LCD_Port(0x00);
    __delay_ms(20);
    LCD_Command(0x30);
    __delay_ms(5);
    LCD_Command(0x30);
    __delay_ms(11);
    LCD_Command(0x30);
    LCD_Command(0x38);
    LCD_Command(0x0C);
    LCD_Command(0x01);
    LCD_Command(0x06);
   
}
//Funcion para configurar el cursor con fila y columna (x,y))
void LCD_Set_Cursor(char x,char y){
	char a;
	if(x == 1)
	{
        a = 0x80 + y;
		LCD_Command(a);
	}
	else if(x == 2)
	{
		a = 0xC0 + y;
		LCD_Command(a);
	}
}
//funcion para enviar un string, compuesto de multiples char
void LCD_Write_String(char *a){
	int i;
	for(i=0;a[i]!='\0';i++)
	   LCD_Write_Char(a[i]);
}


