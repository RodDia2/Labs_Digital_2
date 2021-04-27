//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
 * Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
 * Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
 * Con ayuda de: José Guerra
 * IE3027: Electrónica Digital 2 - 2019
 */
//***************************************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

//#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  

const int P1derecha = PA_7;
const int P1izquierda  = PF_4;
//const int P1agachado = PD_6;
const int P1puno = PF_1;
//const int P1patada = PA_4;

const int P2derecha = PD_7;
const int P2izquierda  = PD_6;
const int P2puno = PE_3;

int x1 = 0;
int x2 = 0;

int posP1 = 0;
int P1_d = 0;
int P1_i = 0;
//int P1_a = 0;
//int P1_pa = 0;
int P1_pu = 0;
int P1_pu_c = 0;

int posP2 = 271;
int P2_d = 0;
int P2_i = 0;
int P2_pu = 0;
int P2_pu_c = 0;
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);

//Movimiento
extern uint8_t jinmov[];
extern uint8_t kazuya[];
//Golpes 
extern uint8_t jinizq[];
extern uint8_t kazuyaizq[];
extern uint8_t jindere[];
extern uint8_t kazuyadere[];
//Patadas
extern uint8_t jinagachado[];
extern uint8_t kazuyaagachado[];
extern uint8_t jinpatada[];
extern uint8_t kazuyapatada[];
//Fondo y portada
extern uint8_t portada[];
extern uint8_t portada2[];
extern uint8_t suelo[];
// muerte 
extern uint8_t jinmuerto[];
extern uint8_t kazuyamuerto[];
// vida
extern uint8_t barravida[];
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  pinMode(P1derecha, INPUT_PULLUP);
  pinMode(P1izquierda, INPUT_PULLUP);
  //pinMode(P1agachado, INPUT_PULLUP);
  pinMode(P1puno, INPUT_PULLUP);
  //pinMode(P1patada, INPUT_PULLUP);
  pinMode(P2derecha, INPUT_PULLUP);
  pinMode(P2izquierda, INPUT_PULLUP);
  pinMode(P2puno, INPUT_PULLUP);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x00);
  FillRect(0, 0, 340, 220, 0x665F);
//LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
    
  //LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  

  for(int x = 0; x <319; x++){ 
    LCD_Bitmap(x, 185, 95, 40, suelo);
    LCD_Bitmap(x, 225, 95, 40, suelo);
    x += 94;
 }
 //234x15
 LCD_Bitmap(43,10,234,15,barravida);
 LCD_Sprite(0, 100, 49,84,jinmov,3,0,0,0 );
 LCD_Sprite(271,100,49,84,kazuya,3,0,1,0);

 x1 = 0;
 x2 = 271;

}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {

//botones
  P1_d = digitalRead(P1derecha);
  P1_i = digitalRead(P1izquierda);
  //P1_a = digitalRead(P1agachado);
  P1_pu = digitalRead(P1puno);
  //P1_pa = digitalRead(P1patada);
  P2_d = digitalRead(P2derecha);
  P2_i = digitalRead(P2izquierda);
  P2_pu = digitalRead(P2puno);
// jin mov derecha
  if (P1_d == LOW) {
    for (int x = x1; x < x1+30; x++) {
      if (x>=260 or x>=x2-55) {
        posP1 = x2-55;
        } else {
          posP1++;
          }
      int anim = (x/25)%3;
      LCD_Sprite(posP1, 100, 49,84,jinmov,3,anim,0,0 );
      V_line(posP1-1,100,84,0x665F);
      
      }
    x1 = posP1;
  }
// jin mov izquierda
  if (P1_i == LOW) {
    for (int x = x1; x > x1-30; x--) {
      if (x<=0) {
        posP1 = 0;
        } else {
          posP1--;
          }
      int anim = (x/25)%3;
      LCD_Sprite(posP1, 100, 49,84,jinmov,3,anim,1,0);
      V_line(posP1+49,100,84,0x665F);
      
      }
    x1 = posP1;
  }
  // kasuya mov derecha
  if (P2_d == LOW) {
    for (int x = x2; x < x2+30; x++) {
      if (x>=270) {
        posP2 = 270;
        } else {
          posP2++;
          }
      int anim5 = (x/25)%3;
      LCD_Sprite(posP2, 100, 49,84,kazuya,3,anim5,0,0 );
      V_line(posP2-1,100,84,0x665F);
      
      }
    x2 = posP2;
  }
// kasuya mov izquierda
  if (P2_i == LOW) {
    for (int x = x2; x > x2-30; x--) {
      if (x<=0 or x<x1+55) {
        posP2 = x1+55;
        } else {
          posP2--;
          }
      int anim5 = (x/25)%3;
      LCD_Sprite(posP2, 100, 49,84,kazuya,3,anim5,1,0);
      V_line(posP2+49,100,84,0x665F);
      
      }
    x2 = posP2;
  }
  // jin agachado
  /*
  if (P1_a == LOW) {
    for(int x = 0; x <=20; x++){
      int anim2 = (x/10)%4;
      LCD_Sprite(posP1,102,56,81,jinagachado,4,anim2,0,0 );
      //V_line(posP1-1,102,81,0x665F); 
      }
    }
    */
  // jin punos
  if (P1_pu == LOW) {
    switch (P1_pu_c) {
      case 0:
        for(int x = 0; x <= 21; x++) {
          int anim3 = (x/10)%2;
          LCD_Sprite(posP1,102,70,82,jindere,2,anim3,0,0 );
          //V_line(posP1-1,102,82,0x665F);
        }
        LCD_Sprite(posP1,102,70,82,jindere,2,0,0,0 );
        FillRect(posP1,101,70,84,0x665F);
        LCD_Sprite(posP1, 100, 49,84,jinmov,3,0,0,0 );
        
        break;
      case 1:
        for(int x = 0; x<= 21; x++) {
          int anim4 = (x/10)%2;
          LCD_Sprite(posP1, 102,66,82,jinizq,2,anim4,0,0 );
          //V_line(posP1-1,102,82,0x665F);
        }
        LCD_Sprite(posP1, 102,66,82,jinizq,2,0,0,0 );
        FillRect(posP1,101,70,84,0x665F);
        LCD_Sprite(posP1, 100, 49,84,jinmov,3,0,0,0 );
        
        break;
    }
    if (P1_pu_c == 0) {
      P1_pu_c = 1;
      } else {
        P1_pu_c = 0;
        }
  }
  // kasuya punos
  if (P2_pu == LOW) {
    switch (P2_pu_c) {
      case 0:
        for(int x = 0; x <= 21; x++) {
          int anim3 = (x/10)%2;
          LCD_Sprite(posP2-20,99,73,84,kazuyadere,2,anim3,1,0 );
          //V_line(posP1-1,102,82,0x665F);
        }
        LCD_Sprite(posP2-20,99,73,84,kazuyadere,2,0,1,0 );
        FillRect(posP2-20,101,70,84,0x665F);
        LCD_Sprite(posP2, 100, 49,84,kazuya,3,0,1,0 );
        
        break;
      case 1:
        for(int x = 0; x<= 21; x++) {
          int anim4 = (x/10)%2;
          LCD_Sprite(posP2-20, 99,68,85,kazuyaizq,2,anim4,1,0 );
          //V_line(posP1-1,102,82,0x665F);
        }
        LCD_Sprite(posP2-20, 99,68,85,kazuyaizq,2,0,1,0 );
        FillRect(posP2-20,101,70,85,0x665F);
        LCD_Sprite(posP2, 100, 49,84,kazuya,3,0,1,0 );
        
        break;
    }
    if (P2_pu_c == 0) {
      P2_pu_c = 1;
      } else {
        P2_pu_c = 0;
        }
  }
  // jin patada 
  /*
  if (P1_pa == LOW) {
    for(int x = 0; x <=100; x++){
      int anim5 = (x/35)%5;
      LCD_Sprite(posP1, 102,84,84,jinpatada,5,anim5,0,0 );
      //V_line(posP1-1,102,84,0x665F); 
    }
  }*/
  //LCD_Sprite(100, 100, 49,84,kazuya,6,0,0,0 );
  //V_line(x-1,100,84,0x665F);
  //V_line( x+49, 100, 84, 0x665F);
  //LCD_Sprite(100, 99,68,85,kazuyaizq,3,0,0,0 );
  //V_line(x-1,99,85,0x665F); 
  //LCD_Sprite(100,99,73,84,kazuyadere,4,0,0,0 );
  //V_line(x-1,99,85,0x665F);
  //LCD_Sprite(100,102,60,84,kazuyaagachado,4,0,0,0 );
  
}

/*  for(int x = 0; x <320-49; x++){
    int anim = (x/35)%6;
    LCD_Sprite(x, 100, 49,84,jinmov,6,anim,0,0 );
    V_line(x-1,100,84,0x665F); 
  }
  for(int x = 320-49; x >0; x--){
    int anim = (x/35)%6;
    LCD_Sprite(x,100,49,84,jinmov,6,anim,1,0);
    V_line( x+49, 100, 84, 0x665F);
  }

  for(int x = 200; x <320-49; x++){
    int anim2 = (x/35)%6;
    LCD_Sprite(x, 100, 49,84,kazuya,6,anim2,0,0 );
    V_line(x-1,100,84,0x665F); 
  }
  for(int x = 320-49; x >0; x--){
    int anim2 = (x/35)%6;
    LCD_Sprite(x,100,49,84,kazuya,6,anim2,1,0);
    V_line( x+49, 100, 84, 0x665F);
  }*/
//Golpes de los jugadores  
//Kazuya
//Izquierda
/*for(int x = 0; x <320-68; x++){
    int anim3 = (x/35)%3;
    LCD_Sprite(x, 99,68,85,kazuyaizq,3,anim3,0,0 );
    V_line(x-1,99,85,0x665F); 
  }
  for(int x = 320-68; x >0; x--){
    int anim3 = (x/35)%3;
    LCD_Sprite(x,99,68,85,kazuyaizq,3,anim3,1,0);
    V_line( x+68,99,85, 0x665F);
  }
//Derecha
for(int x = 0; x <320-73; x++){
    int anim4 = (x/35)%4;
    LCD_Sprite(x,99,73,84,kazuyadere,4,anim4,0,0 );
    V_line(x-1,99,85,0x665F); 
  }
  for(int x = 320-73; x >0; x--){
    int anim4 = (x/35)%4;
    LCD_Sprite(x,99,73,84,kazuyadere,4,anim4,1,0);
    V_line( x+73,99,84, 0x665F);
  }*/
//Jin
//Izquierda
/*for(int x = 0; x <320-66; x++){
    int anim5 = (x/35)%3;
    LCD_Sprite(x, 102,66,82,jinizq,3,anim5,0,0 );
    V_line(x-1,102,82,0x665F); 
  }
  for(int x = 320-66; x >0; x--){
    int anim5 = (x/35)%3;
    LCD_Sprite(x,102,66,82,jinizq,3,anim5,1,0);
    V_line( x+66,102,82, 0x665F);
  }
//Derecha
for(int x = 0; x <320-70; x++){
    int anim6 = (x/35)%3;
    LCD_Sprite(x,102,70,82,jindere,4,anim6,0,0 );
    V_line(x-1,102,82,0x665F); 
  }
  for(int x = 320-70; x >0; x--){
    int anim6 = (x/35)%3;
    LCD_Sprite(x,102,70,82,jindere,4,anim6,1,0);
    V_line( x+70,102,82, 0x665F);
  }
  */
//Patadas de los jugadores 
//Kazuya
//Patada
/*
  for(int x = 0; x <320-72; x++){
    int anim7 = (x/35)%5;
    LCD_Sprite(x, 90,72,92,kazuyapatada,5,anim7,0,0 );
    V_line(x-1,90,92,0x665F); 
  }
  for(int x = 320-72; x >0; x--){
    int anim7 = (x/35)%5;
    LCD_Sprite(x,90,72,92,kazuyapatada,5,anim7,1,0);
    V_line( x+72,90,92, 0x665F);
  }
//Agahcado
  for(int x = 0; x <320-60; x++){
    int anim8 = (x/35)%4;
    LCD_Sprite(x,102,60,84,kazuyaagachado,4,anim8,0,0 );
    V_line(x-1,102,84,0x665F); 
  }
  for(int x = 320-60; x >0; x--){
    int anim8 = (x/35)%4;
    LCD_Sprite(x,102,60,84,kazuyaagachado,4,anim8,1,0);
    V_line( x+60,102,84, 0x665F);
  }
//Jin
//Patada
  for(int x = 0; x <320-84; x++){
    int anim9 = (x/35)%5;
    LCD_Sprite(x, 102,84,84,jinpatada,5,anim9,0,0 );
    V_line(x-1,102,84,0x665F); 
  }
  for(int x = 320-84; x >0; x--){
    int anim9 = (x/35)%5;
    LCD_Sprite(x,102,84,84,jinpatada,5,anim9,1,0);
    V_line( x+84,102,84, 0x665F);
  }
//Agahcado
for(int x = 0; x <320-56; x++){
    int anim10 = (x/35)%4;
    LCD_Sprite(x,102,56,81,jinagachado,4,anim10,0,0 );
    V_line(x-1,102,81,0x665F); 
  }
  for(int x = 320-56; x >0; x--){
    int anim10 = (x/35)%4;
    LCD_Sprite(x,102,56,81,jinagachado,4,anim10,1,0);
    V_line( x+56,102,81, 0x665F);
  }
}
*/
// muertes 
// jin
/*
for(int x = 0; x <320-90; x++){
    int anim11 = (x/35)%3;
    LCD_Sprite(x,108,90,75,jinmuerto,3,anim11,0,0 );
    V_line(x-1,108,76,0x665F); 
  }
  for(int x = 320-90; x >0; x--){
    int anim11 = (x/35)%3;
    LCD_Sprite(x,108,90,75,jinmuerto,3,anim11,1,0);
    V_line( x+90,108,76, 0x665F);
  }

// kasuya
for(int x = 0; x <320-90; x++){
    int anim12 = (x/35)%3;
    LCD_Sprite(x,104,90,81,kazuyamuerto,3,anim12,0,0 );
    V_line(x-1,103,81,0x665F); 
  }
  for(int x = 320-90; x >0; x--){
    int anim12 = (x/35)%3;
    LCD_Sprite(x,104,90,81,kazuyamuerto,3,anim12,1,0);
    V_line( x+90,103,81, 0x665F);
  }
}
*/

//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
/*void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
*/

void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      
      //LCD_DATA(bitmap[k]);    
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}
