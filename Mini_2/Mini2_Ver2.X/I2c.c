/*
 * File:   I2c.c
 * Author: Rodrigo D'iaz, basado en Pablo Mazariegos
 *
 * 
 */

#include <xc.h>
#include "I2c.h"


//---------------[ I2C Routines ]-------------------
//--------------------------------------------------
// funcion para inicializar maestro
void I2C_Master_Init()
{
  SSPCON  = 0x28;
  SSPCON2 = 0x00;
  SSPSTAT = 0x00;
  SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate) - 1;
  SCL_D = 1;
  SDA_D = 1; 
}
// para esperar un dato
void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}
// se inicia el maestro
void I2C_Master_Start()
{
    I2C_Master_Wait();
    SEN = 1;
}
// se inicia la comunicacion
void I2C_Start(char add)
{
    I2C_Master_Wait();
    SEN = 1;
    I2C_Master_Write(add);
}
// se vuelve a encender el maestro
void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();
    RSEN = 1;
}
// se detiene la comunicacion del maestro
void I2C_Master_Stop()
{
    I2C_Master_Wait();
    PEN = 1;
}
// se manda bit de ack
void I2C_ACK(void)
{
	ACKDT = 0;			// 0 -> ACK
    ACKEN = 1;			// Send ACK
    while(ACKEN);
}
// se manda bit de nack
void I2C_NACK(void)
{
	ACKDT = 1;			// 1 -> NACK
	ACKEN = 1;			// Send NACK
    while(ACKEN);
}
// el maestro escribe
unsigned char I2C_Master_Write(unsigned char data)
{
    I2C_Master_Wait();
    SSPBUF = data;
    while(!SSPIF);  // Wait Until Completion
	SSPIF = 0;
    return ACKSTAT;
}
// se lee un byte
unsigned char I2C_Read_Byte(void)
{
    //---[ Receive & Return A Byte ]---
    I2C_Master_Wait();
    RCEN = 1;		  // Enable & Start Reception
	while(!SSPIF);	  // Wait Until Completion
	SSPIF = 0;		  // Clear The Interrupt Flag Bit
    I2C_Master_Wait();
    return SSPBUF;	  // Return The Received Byte
}
// se revisan los bits de ack o nack
unsigned char I2C_Read(unsigned char ACK_NACK)
{  
    //---[ Receive & Return A Byte & Send ACK or NACK ]---
    unsigned char Data;
    RCEN = 1;              
    while(!BF);      
    Data = SSPBUF;           
    if(ACK_NACK==0)
        I2C_ACK();            
    else
        I2C_NACK();     
    while(!SSPIF);                 
    SSPIF=0;   
    return Data;
}
