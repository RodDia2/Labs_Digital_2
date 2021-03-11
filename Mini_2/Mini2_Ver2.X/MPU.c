/*
 * File:   MPU.c
 * Author: Rodrigo Díaz, basado en el código de Khaled Magdy
 * Extraído de https://deepbluembedded.com/mpu6050-with-microchip-pic-accelerometer-gyroscope-interfacing-with-pic/
 * Creado lunes 8 de marzo del 2021
 */


#include <xc.h>
#include "I2c.h"
#include "MPU.h"

#include "USART.h"  // for debugging serial terminal
#include <stdio.h>

//-------------[ MPU6050 Routines ]------------------
//---------------------------------------------------

void MPU6050_Init()
{
  // Power-Up Delay & I2C_Init
  __delay_ms(100);
  I2C_Master_Init();
 
  // Setting The Sample (Data) Rate
  // sample rate = gyro. out rate/(1+SMPLRT_DIV)
  I2C_Start(0xD0);
  I2C_Master_Write(SMPLRT_DIV);
  I2C_Master_Write(0x07);
  I2C_Master_Stop();
 
  // Setting The Clock Source
  // se toma el eje x como referencia para tener mas estabilidad
  I2C_Start(0xD0);
  I2C_Master_Write(PWR_MGMT_1);
  I2C_Master_Write(0x01);
  I2C_Master_Stop();
 
  // Configure The DLPF
  // setear el low pass filter, opciones de 0 a 6, mientras menor el bandwidth, mayor el delay
  I2C_Start(0xD0);
  I2C_Master_Write(CONFIG);
  I2C_Master_Write(0x04);
  I2C_Master_Stop();
 
  // Configure The ACCEL (FSR= +-2g)
  // se configura el rango total acelerometro, al minimo
  I2C_Start(0xD0);
  I2C_Master_Write(ACCEL_CONFIG);
  I2C_Master_Write(0x00);
  I2C_Master_Stop();
 
  // Configure The GYRO (FSR= +-2000d/s)
  // se configura el rango del giroscopio, al max
  I2C_Start(0xD0);
  I2C_Master_Write(GYRO_CONFIG);
  I2C_Master_Write(0x18);
  I2C_Master_Stop();
 
  // Enable Data Ready Interrupts
  // se habilita la interrupcion, se puede revisar el registro WHO_AM_I para obtener valor booleano si esta funcionando bien
  I2C_Start(0xD0);
  I2C_Master_Write(INT_ENABLE);
  I2C_Master_Write(0x01);
  I2C_Master_Stop();
}
// en esta funcion, se leen todos los datos.
void MPU6050_Read()
{ 
  char buffer[40]; // esta variable contiene el string que se va a mandar
  int Ax,Ay,Az,T,Gx,Gy,Gz; //se crean variables para guardar las lecturas
  int AY  = 0;
  // Prepare For Reading, Starting From ACCEL_XOUT_H
  I2C_Start(0xD0);
  I2C_Master_Write(ACCEL_XOUT_H);
  I2C_Master_Stop();
  I2C_Start(0xD1);
  Ax = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Ay = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Az = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  T  = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Gx = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Gy = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Gz = ((int)I2C_Read(0)<<8) | (int)I2C_Read(1);
  I2C_Master_Stop();
  // se mapea un valor a 0-255 para mostrar en el puerto B y verificar funcionamiento
  PORTB = (Ay+16384)/128;
  // se le asigna el mismo valor a una variable para mandarla
  AY = (Ay+16384)/128;
  //UART_Write_String((Ay+16384)/128);
  //UART_Write_String();
  //sprintf(buffer,"3.2%f",AY);
  //UART_Write_String(buffer);
// se manda un decimal
  sprintf(buffer,"%d",AY);
  UART_Write_String(buffer);
// 
//  sprintf(buffer," Az = %d    ",Az);
//  UART_Write_String(buffer);
// 
//  sprintf(buffer," T = %d  ",T);
//  UART_Write_String(buffer);
// 
//  sprintf(buffer," Gx = %d    ",Gx);
//  UART_Write_String(buffer);
// 
//  sprintf(buffer," Gy = %d    ",Gy);
//  UART_Write_String(buffer);
// 
//  sprintf(buffer," Gz = %d\r\n",Gz);
//  UART_Write_String(buffer);
}
