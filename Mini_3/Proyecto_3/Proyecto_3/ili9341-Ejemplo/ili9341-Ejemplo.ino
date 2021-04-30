//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
 * Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
 * Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
 * Con ayuda de: José Guerra
 * IE3027: Electrónica Digital 2 - 2019
 * Rodrigo Díaz 18265
 * Helder Ovalle 18349
 */
//***************************************************************************************************************************************
// se incluyen librerias para la sd y lcd
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

#include <SPI.h>
#include <SD.h>
// crear los files
File root;
File myFile;

//#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  
// se definen los botones a utilizar
const int P1derecha = PA_7;
const int P1izquierda  = PF_4;
//const int P1agachado = PD_6;
const int P1puno = PF_1;
const int P1patada = PC_7;
const int P1agachado = PC_5;


const int P2derecha = PD_7;
const int P2izquierda  = PD_6;
const int P2puno = PE_3;
const int P2patada = PC_6;
const int P2agachado = PC_4;
// variables para posicion
int x1 = 0;
int x2 = 0;
// variables para leer digitalread para cda jugador
int posP1 = 30;
int P1_d = 0;
int P1_i = 0;
int P1_a = 0;
int P1_pa = 0;
int P1_pu = 0;
int P1_pu_c = 0;

int posP2 = 241;
int P2_d = 0;
int P2_i = 0;
int P2_pu = 0;
int P2_pu_c = 0;
int P2_pa = 0;
int P2_a = 0;
// variables para la vida, cobmos, defensa, evitar movimiento, rounds
int vidaP1 = 0;
int vidaP2 = 0;
int vidaP1_index = 0;
int vidaP2_index = 0;
int P1combo = 0;
int P2combo = 0;
int P1def = 0;
int P2def = 0;
int win = 0;
int nomov = 0;
int P1rounds = 0;
int P2rounds = 0;
int P1matchpoint = 0;
int P2matchpoint = 0;
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
unsigned char suelo2[15201]={0};
unsigned char fight[1681]={0};
unsigned char fight2[4929]={0};
unsigned char winpdos[14521]={0};
unsigned char winpuno[1025]={0};
// muerte 
extern uint8_t jinmuerto[];
extern uint8_t kazuyamuerto[];
// vida
extern uint8_t barravida[];
extern uint8_t logo[];
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  pinMode(P1derecha, INPUT_PULLUP);
  pinMode(P1izquierda, INPUT_PULLUP);
  pinMode(P1agachado, INPUT_PULLUP);
  pinMode(P1puno, INPUT_PULLUP);
  pinMode(P1patada, INPUT_PULLUP);
  pinMode(P2derecha, INPUT_PULLUP);
  pinMode(P2izquierda, INPUT_PULLUP);
  pinMode(P2puno, INPUT_PULLUP);
  pinMode(P2patada, INPUT_PULLUP);
  pinMode(P2agachado, INPUT_PULLUP);
  Serial.println("Inicio");

  SPI.setModule(0);
  Serial.print("Initializing SD card...");
//  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
//  // Note that even if it's not used as the CS pin, the hardware SS pin
//  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
//  // or the SD library functions will not work.
  pinMode(PA_3, OUTPUT); // chip select
//  // se revisa el chip select para ver si se realizo la conexion
  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
//  // se abre con la funcion open y se utiliza la funcion de print Directory para saber que archivos hay y su tamano

  
  LCD_Init();
  LCD_Clear(0x00);
  FillRect(0, 0, 340, 220, 0x665F);
//LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
    
  //LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  
// se imprime el suelo
  for(int x = 0; x <319; x++){ 
    LCD_Bitmap(x, 185, 20, 40, suelo);
    LCD_Bitmap(x, 225, 20, 40, suelo);
    x += 19;
 }
 //234x15
 // se imprime la barra de vida y las posiciones iniciales
 LCD_Bitmap(43,10,234,15,barravida);
 LCD_Sprite(30, 100, 49,84,jinmov,3,0,0,0 );
 LCD_Sprite(241,100,49,84,kazuya,3,0,1,0);
// se definen los valores iniciales de variables
 x1 = 30;
 x2 = 241;

 vidaP1 = 100;
 vidaP2 = 100;
 vidaP1_index = 148;
 vidaP2_index = 272;

// pruebas bajar vida 
 //FillRect(48,14,100,6,0xFFFF);
 //FillRect(172,14,100,6,0xFFFF);
 // pruebas rounds
/*
 FillRect(130,24,10,10,0xFFFF);
 FillRect(110,24,10,10,0xFFFF);
 FillRect(180,24,10,10,0xFFFF);
 FillRect(200,24,10,10,0xFFFF);
*/
 //open_SD_bitmap(suelo2,15201,"suelo.txt");
 //LCD_Bitmap(80, 100, 95, 40, suelo2);
 //open_SD_bitmap(fight,1681,"fight.txt");
 //LCD_Bitmap(200, 100, 35, 12, fight);
//********************************************************** 
// se cargan los bitmaps de la sd  a la RAM
 open_SD_bitmap(fight2,4929,"fight2.txt");
 LCD_Bitmap(120, 50, 77, 16, fight2);
 delay(1500);
 FillRect(120, 50, 77, 16, 0x665F);
 
 open_SD_bitmap(winpdos,14521,"winpdos.txt");
 //LCD_Bitmap(100, 45, 110, 33, winpdos);
 open_SD_bitmap(winpuno,1025,"winpuno.txt");
 //LCD_Bitmap(100, 45, 110, 33, winpdos);
 //LCD_Bitmap(154, 62, 16, 16, winpuno);
}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  // variable para que no se muevan durante el reinicio de round
  nomov = 0;
 // LCD_Bitmap(100,100,194,81,logo);
//se leen los botones y se guardan en variables*************************************************************************************************
  P1_d = digitalRead(P1derecha);
  P1_i = digitalRead(P1izquierda);
  P1_a = digitalRead(P1agachado);
  P1_pu = digitalRead(P1puno);
  P1_pa = digitalRead(P1patada);
  P2_d = digitalRead(P2derecha);
  P2_i = digitalRead(P2izquierda);
  P2_pu = digitalRead(P2puno);
  P2_pa = digitalRead(P2patada);
  P2_a = digitalRead(P2agachado);
// Se revisa si se esta bloqueando ************************************************************************
  if (P1_a == LOW) {
    P1def = 1;
    }
  if (P1_d == LOW or P1_i == LOW or P1_pu == LOW or P1_pa ==LOW) {
    P1def = 0;
    }
    
  if (P2_a == LOW) {
    P2def = 1;
    }
  if (P2_d == LOW or P2_i == LOW or P2_pu == LOW or P2_pa ==LOW) {
    P2def = 0;
    }
// Se revisa si se gano el juego **********************************************************************************
  if (win == 1) {
    // se deshabilitan todos los movimientos
    P1_d = HIGH;
    P1_i = HIGH;
    P1_a = HIGH;
    P1_pu = HIGH;
    P1_pa = HIGH;
    P2_d = HIGH;
    P2_i = HIGH;
    P2_pu = HIGH;
    P2_pa = HIGH;
    P2_a = HIGH;
    P1_d = digitalRead(P1derecha);
    P2_i = digitalRead(P2izquierda);
    nomov = 1;
 //   FillRect(100, 45, 110, 33, 0x665F);
//    if (P1rounds == 2 and P2rounds<2) {
//      LCD_Bitmap(100, 45, 110, 33, winpdos);
//      LCD_Bitmap(194, 62, 16, 16, winpuno);
//      delay(100);
//    }
//    if (P2rounds == 2 and P1rounds<2) {
//      LCD_Bitmap(100, 45, 110, 33, winpdos);
//      delay(100);
//    }
    //*********************************************************
    // solamente si ambos jugadores presionan el boton correspondiente se reinicia
    if (P2_i == LOW and P1_d == LOW) {
      // se incrementa el conteo de rounds
      if (vidaP1 == 0) {
        P2rounds++;
        }
      if (vidaP2 == 0) {
        P1rounds++;
        }
        // si un jugador llega a los 3 rounds, se imprime el texto de ganador
        // adicionalmente, se imprime en la SD quien fue el ganador para llevar el conteo de victorias
      if (P1rounds == 3) {
        LCD_Bitmap(100, 45, 110, 33, winpdos);
        LCD_Bitmap(194, 62, 16, 16, winpuno);
        delay(2000);
        FillRect(100, 45, 110, 33, 0x665F);
        Victory_SD(1);
        }
      if (P2rounds == 3) {
        LCD_Bitmap(100, 45, 110, 33, winpdos);
        delay(2000);
        FillRect(100, 45, 110, 33, 0x665F);
        Victory_SD(2);
        }
      //FillRect(100, 45, 110, 33, 0x665F);
      // win es 0 porque se reinicia
      win = 0;
      nomov = 0;
      // se repiten los prints del setup............
      FillRect(0, 0, 340, 220, 0x665F);
      for(int x = 0; x <319; x++){ 
        LCD_Bitmap(x, 185, 20, 40, suelo);
        LCD_Bitmap(x, 225, 20, 40, suelo);
        x += 19;
      }
 //234x15
      LCD_Bitmap(43,10,234,15,barravida);
      LCD_Sprite(0, 100, 49,84,jinmov,3,0,0,0 );
      LCD_Sprite(271,100,49,84,kazuya,3,0,1,0);

      x1 = 0;
      x2 = 260;
      posP1 = x1;
      posP2 = x2;
      P1combo = 0;
      P2combo = 0;
      P1_pu_c = 0;
      P2_pu_c = 0;

      vidaP1 = 100;
      vidaP2 = 100;

      LCD_Bitmap(120, 50, 77, 16, fight2);
      delay(1500);
      FillRect(120, 50, 77, 16, 0x665F);
      }
    }
// jin mov derecha
// se revisa que no borre al otro jugador
  if (P1_d == LOW) {
    if (nomov == 0) {
    for (int x = x1; x < x1+30; x++) {
      if (x>=260 or x>=x2-65) {
        posP1 = x2-65;
        } else {
          posP1++;
          }
      int anim = (x/25)%3;
      LCD_Sprite(posP1, 100, 49,84,jinmov,3,anim,0,0 );
      V_line(posP1-1,100,84,0x665F);
      
      }
    x1 = posP1;
  }
  }
// jin mov izquierda
// se revisa qeu no se salga de la pantalla
  if (P1_i == LOW) {
    for (int x = x1; x > x1-30; x--) {
      if (x<=30) {
        posP1 = 30;
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
  // se revisa que no se salga de la pantalla
  if (P2_d == LOW) {
    for (int x = x2; x < x2+30; x++) {
      if (x>=241) {
        posP2 = 241;
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
// se revisa que no choque con el jugador 
  if (P2_i == LOW) {
    if (nomov == 0) {
    for (int x = x2; x > x2-30; x--) {
      if (x<=0 or x<x1+65) {
        posP2 = x1+65;
        } else {
          posP2--;
          }
      int anim5 = (x/25)%3;
      LCD_Sprite(posP2, 100, 49,84,kazuya,3,anim5,1,0);
      V_line(posP2+49,100,84,0x665F);
      
      }
    x2 = posP2;
  }
  }
  // jin agachado***************************************************************************************************
  // para los agachados solamente se ejecuta el for de animacion
  if (P1_a == LOW) {
    for(int x = 0; x <=19; x++){
      int anim2 = (x/10)%2;
      LCD_Sprite(posP1,102,56,81,jinagachado,2,anim2,0,0 );
      //V_line(posP1-1,102,81,0x665F); 
    }
    FillRect(posP1+52,151,10,30,0x665F);
    FillRect(posP1+5,90,30,15,0x665F);
  }
  // kazuya agachado fachero facherito
   if (P2_a == LOW) {
    for(int x = 0; x <=19; x++){
      int anim2 = (x/10)%2;
      LCD_Sprite(posP2,102,60,84,kazuyaagachado,2,anim2,1,0 );
      //V_line(posP1-1,102,81,0x665F); 
    }
    FillRect(posP2+5,90,30,15,0x665F);
  }
    
  // jin punos*************************************************************************************************************
  // se alterna entre derecha e izquierda
  // si golpea primero derehca y despues izquieda, se efectua un combo
  // se revisa que esten suficientemente cerca
  // si esta agachado no toma dano
  if (P1_pu == LOW) {
    switch (P1_pu_c) {
      case 0:
        for(int x = 0; x <= 21; x++) {
          int anim3 = (x/10)%2;
          LCD_Sprite(posP1+5,102,70,82,jindere,2,anim3,0,0 );
          //V_line(posP1-1,102,82,0x665F);
        }
        LCD_Sprite(posP1+5,102,70,82,jindere,2,0,0,0 );
        FillRect(posP1+5,101,70,84,0x665F);
        LCD_Sprite(posP1, 100, 49,84,jinmov,3,0,0,0 );
        P1combo = 0;
        if (x2-x1<75){
          if (P2def == 0) {
            vidaP2 = vidaP2 - 10;
            P1combo = 1;
          } 
        }
      break;
      case 1:
        for(int x = 0; x<= 21; x++) {
          int anim4 = (x/10)%2;
          LCD_Sprite(posP1+5, 102,66,82,jinizq,2,anim4,0,0 );
          //V_line(posP1-1,102,82,0x665F);
        }
        LCD_Sprite(posP1+5, 102,66,82,jinizq,2,0,0,0 );
        FillRect(posP1+5,101,70,84,0x665F);
        LCD_Sprite(posP1, 100, 49,84,jinmov,3,0,0,0 );
        if (x2-x1<75) {
          if (P2def == 0) {
            if (P1combo == 1) {
              vidaP2 = vidaP2 - 20;
              P1combo = 2;
            } else {
              vidaP2 = vidaP2 -10;
              P1combo = 0;
              }
          } else {
            P1combo = 0;
            }
        } else {
            P1combo = 0;
            }
       break;
    }
    if (P1_pu_c == 0) {
      P1_pu_c = 1;
      } else {
        P1_pu_c = 0;
        }
  }
  // kasuya punos
  // lo mismo que para jin
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
        P2combo = 0;
        if (x2-x1<75) {
          if (P1def == 0) {
            vidaP1 = vidaP1 - 10;
            P2combo = 1;
          }
        }      
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
        if (x2-x1<75) {
          if (P1def == 0) {
            if (P2combo == 1) {
              vidaP1 = vidaP1 - 20;
              P2combo = 2;
            } else {
              vidaP1 = vidaP1 - 10;
              P2combo = 0;
              }
          } else {
            P2combo = 0;
            }
         } else {
          P2combo = 0;
          }
       break;
    }
    if (P2_pu_c == 0) {
      P2_pu_c = 1;
      } else {
        P2_pu_c = 0;
        }
  }
  // jin patada *******************************************************************************************************************************
  // se revisa si se tiene combo para terminar el combo perfect, de lo contrario es normal
  // se revia que esten suficientemente cerca 
  // si esta agachado, toma menos dano
  if (P1_pa == LOW) {
    for(int x = 0; x <=21; x++){
      int anim5 = (x/10)%2;
      LCD_Sprite(posP1-10, 101,84,84,jinpatada,2,anim5,0,0 );
      //V_line(posP1-1,102,84,0x665F); 
    }
    FillRect(posP1,101,75,84,0x665F);
    FillRect(posP1-20,101,20,84,0x665F);
    LCD_Sprite(posP1, 100, 49,84,jinmov,3,0,0,0 );
    if (x2-x1<75) {
      if (P2def == 0) {
        if (P1combo == 2) {
          vidaP2 = vidaP2 - 70;
        } else {
          vidaP2 = vidaP2 - 25;
          }
      } else {
        vidaP2 = vidaP2 - 15;
        }
    }
    P1combo = 0;
  }
  // kazuya patada
  // lo mismo que para jin
  if (P2_pa == LOW) {
    for(int x = 0; x <=21; x++){
      int anim5 = (x/10)%2;
      LCD_Sprite(posP2-20, 93,72,92,kazuyapatada,2,anim5,1,0 );
      //V_line(posP1-1,102,84,0x665F); 
    }
    FillRect(posP2-20,100,20,84,0x665F);
    LCD_Sprite(posP2, 100, 49,84,kazuya,3,0,1,0 );
    if (x2-x1<75) {
      if (P1def == 0) {
        if (P2combo == 2) {
          vidaP1 = vidaP1 - 70;
        } else {
          vidaP1 = vidaP1 - 25;
          }
      } else {
        vidaP1 = vidaP1 -15;
        }
    }
    P2combo = 0;
  }
  //******************************************************************************************
  // se revisa que alguno de los jugadores haya muerto, y se indica que ya alguien gano un round
  if (vidaP2 <= 0) {
    vidaP2 = 0;
    win = 1;
    FillRect(18,14,15,15,0xFF40);
    //P1rounds++;
    }
  if (vidaP1 <= 0) {
    vidaP1 = 0;
    win = 1;
    FillRect(292,14,15,15,0xFF40);
    //P2rounds++;
    }
  // se pinta de blanco la barra de vida dependiendo de cuanta vida tiene cada jugador
  FillRect(172+vidaP2,14,(100-vidaP2),6,0xFFFF);
  FillRect(48,14,(100-vidaP1),6,0xFFFF);

// conteo de rondas*****************************************************************
// se imprimen cuadrados amarillos dependiendo de las rounds que ganaron 
// al superar las 3 rounds ganadas, se reinicia el conteo. 
  switch(P1rounds) {
    case 0:
    FillRect(130,24,10,10,0x665F);
    FillRect(110,24,10,10,0x665F);
    break;
    case 1:
    FillRect(130,24,10,10,0xFF40);
    break;
    case 2:
    FillRect(130,24,10,10,0xFF40);
    FillRect(110,24,10,10,0xFF40);
    break;
    case 3:
    P1rounds = 0;
    P2rounds = 0;
    break;
    default:
    P1rounds = 0; 
    P2rounds = 0;
    break;
    }
    // se repite para P2
  switch(P2rounds) {
    case 0:
    FillRect(180,24,10,10,0x665F);
    FillRect(200,24,10,10,0x665F);
    break;
    case 1:
    FillRect(180,24,10,10,0xFF40);
    break;
    case 2:
    FillRect(180,24,10,10,0xFF40);
    FillRect(200,24,10,10,0xFF40);
    break;
    case 3:
    P2rounds = 0;
    P1rounds = 0;
    break;
    default:
    P2rounds = 0;
    P2rounds = 0;
    break;
    }

}



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
//*********************************************
// FUNCION PARA LEER VALORES ASCII DE LA SD
//*********************************************
void open_SD_bitmap(unsigned char Bitmap_SD[], unsigned long Size_bitmap, char* filename) {
  File myFile = SD.open(filename);     // ABRIR EL ARCHIVO 
  unsigned long i = 0;            
  char Bitmap_SD_HEX[] = {0, 0};          //SE HACE ARREGLO DE DOS NUM, PARA CADA UNA DE LAS POSICIONES
  int Pos_1, Pos_2;                     //VARIABLES DE LAS POSICIONES
  if (myFile) {                 
    do {
      Bitmap_SD_HEX[0] = myFile.read(); //LEE
      Pos_1 = ACII_to_HEX(Bitmap_SD_HEX);       //TRANSOFRMA
      Bitmap_SD_HEX[0] = myFile.read(); //LEE
      Pos_2 = ACII_to_HEX(Bitmap_SD_HEX);       //TRANSFORMA
      Bitmap_SD[i] = (Pos_1 << 4) | (Pos_2 & 0xF);  //SE CONCATENA CONCATENA
      i++;                        
    } while (i < (Size_bitmap + 1));
  }
  myFile.close();                       
}

//*********************************************
// FUNCION PARA PASAR INFO DE SD A VALORES HEXADECIMALES
//*********************************************
int ACII_to_HEX(char *puntero) {
  int i = 0;
  for (;;) {
    char num = *puntero;
    if (num >= '0' && num <= '9') {
      i *= 16;
      i += num - '0';
    }
    else if (num >= 'a' && num <= 'f') {
      i *= 16;
      i += (num - 'a') + 10;
    }
    else break;
    puntero++;
  }
  return i;
}
//**********************************************************
// Funcion para escribir en la SD
//**********************************************************
void Victory_SD(int jugador) {
  if (jugador == 1) { 
    File dataFile = SD.open("test.txt", FILE_WRITE);
    if (dataFile) {
      Serial.println("Writing...");
      dataFile.println("P1 WINS");
      dataFile.close();
      Serial.println("Done");
    }
    else {
      Serial.println("error opening WINNER.txt");
    }

  }
  if (jugador == 2) { 
    File dataFile = SD.open("test.txt", FILE_WRITE);
    if (dataFile) {
      Serial.println("Writing...");
      dataFile.println("P2 WINS");
      dataFile.close();
      Serial.println("Done");
    }
    else {
      Serial.println("error opening WINNER.txt");
    }

  }
}
