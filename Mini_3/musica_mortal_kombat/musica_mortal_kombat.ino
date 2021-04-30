// Variables A1-A5 are reserved on Arduino, So I renamed my Ax notes to AAx.
// Rights to the song obviously don't belong to me. blah blah, legal words, blah blah.

#include <stdint.h>
#include <stdbool.h>
#include <Wire.h>

#include "wiring_private.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
 
const int C2 = 65;
const int Db2 = 69;
const int D2 = 73;
const int Eb2 = 78;
const int E2 = 82;
const int F2 = 87;
const int Gb2 = 93;
const int G2 = 98;
const int Ab2 = 104;
const int AA2 = 110;
const int Bb2 = 117;
const int B2 = 123;
const int C3 = 131;
const int Db3 = 139;
const int D3 = 147;
const int Eb3 = 156;
const int E3 = 165;
const int F3 = 175;
const int Gb3 = 185;
const int G3 = 196;
const int Ab3 = 208;
const int AA3 = 220;
const int Bb3 = 233;
const int B3 = 247;
const int C4 = 262;
const int Db4 = 277;
const int D4 = 294;
const int Eb4 = 311;
const int E4 = 330;
const int F4 = 349;
const int Gb4 = 370;
const int G4 = 392;
const int Ab4 = 415;
const int AA4 = 440;
const int Bb4 = 466;
const int B4 = 494;
const int C5 = 523;
const int Db5 = 554;
const int D5 = 587;
const int Eb5 = 622;
const int E5 = 659;
const int F5 = 698;
const int Gb5 = 740;
const int G5 = 784;
const int Ab5 = 831;
const int AA5 = 880;
const int Bb5 = 932;
const int B5 = 988;
const int C6 = 1047;
const int Db6 = 1109;
const int D6 = 1175;
const int Eb6 = 1245;
const int E6 = 1319;
const int F6 = 1397;
const int Gb6 = 1480;
const int G6 = 1568;
const int Ab6 = 1661;
const int AA6 = 1760;
const int Bb6 = 1865;
const int B6 = 1976;

 
const byte buzzerPin = PF_2;
const int ledPin1 = PF_3;
const int ledPin2 = PB_3;

const int note_cc = 261;
#define note_dd 294
#define note_ee 329
#define note_ff 349
const int note_g = 391;
#define note_gS 415
#define note_a 440
#define note_aS 455
#define note_b 493
#define note_cH 523
#define note_cSH 554
#define note_dH 587
#define note_dSH 622
#define note_eH 659
#define note_fH 698
#define note_fSH 740
#define note_gH 784
#define note_gSH 830
#define note_aH 880

#define note_bH 988
#define note_cHH 1047
#define note_dHH 1175
#define note_eHH 1318
 
int counter = 0;

const byte pushcambio = PA_5;
int cambio = 0;
int counter2 = 1;
const int ledsalida = PA_7;
 
void setup()
{
  Serial.begin(9600);
  //Setup pin modes
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(pushcambio, INPUT_PULLUP);
  pinMode(ledsalida, OUTPUT);
//  configureTimer1A();
  //attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
}
 
void loop()
// al encender suena tekken y despues se loopea en mortal kombat porque suena mejor
{
if (cambio == 0) {
    tekken();
    cambio ++;
}
  firstSection();
  firstSection();
  secondSection();
  thirdSection();
  thirdSection();
   
}
 
void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
 
 
  //Play different LED depending on value of 'counter'
  if(counter % 2 == 0)
  {
    digitalWrite(ledPin1, HIGH);
    delay(duration);
    digitalWrite(ledPin1, LOW);
  }else
  {
    digitalWrite(ledPin2, HIGH);
    delay(duration);
    digitalWrite(ledPin2, LOW);
  }
 
  //Stop tone on buzzerPin
  noTone(buzzerPin);
 
  delay(60);
 
  //Increment counter
  counter++;
}
 
void firstSection()
{
  beep(AA3, 200);
  beep(AA3, 200);
  beep(C4, 200);
  beep(AA3, 200);
  beep(D4, 200);
  beep(AA3, 200);
  beep(E4, 200);
  beep(D4, 200);
  beep(C4, 200);
  beep(C4, 200);
  beep(E4, 200);
  beep(C4, 200);
  beep(G4, 200);
  beep(C4, 200);
  beep(E4, 200);
  beep(C4, 200);
  beep(G3, 200);
  beep(G3, 200);
  beep(B3, 200);
  beep(G3, 200);
  beep(C4, 200);
  beep(G3, 200);
  beep(D4, 200);
  beep(C4, 200);
  beep(F3, 200);
  beep(F3, 200);
  beep(AA3, 200);
  beep(F3, 200);
  beep(C4, 200);
  beep(F3, 200);
  beep(C4, 200);
  beep(B3, 200);
  
}
void secondSection()
{
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(G3, 200);
  beep(C4, 200);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(G3, 200);
  beep(E3, 200);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(G3, 200);
  beep(C4, 200);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 200);
  beep(AA3, 75);
  beep(AA3, 325);
  beep(AA3, 450);
}

void thirdSection()
{
  beep(AA3, 75);
  beep(E4, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 75);
  beep(G3, 200);
  beep(AA3, 75);
  beep(E4, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 75);
  beep(G3, 200);
  beep(AA3, 75);
  beep(E4, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 75);
  beep(G3, 200);
  beep(AA3, 75);
  beep(E4, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(G3, 75);
  beep(G3, 200);
  beep(G3, 75);
  beep(AA3, 200);
  beep(AA3, 450);
}

void tekken() {
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_eHH, 85);

  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_eHH, 85);

  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_eHH, 85);

  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_eHH, 85);
//////////////////////////
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_dHH, 85);

  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_dHH, 85);

  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_dHH, 85);

  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_dHH, 85);
/////////////////
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_bH, 85);
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_bH, 85);
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_bH, 85);
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_bH, 85);
  //////////
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_cHH, 85);
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_cHH, 85);
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_cHH, 85);
  
  ////////
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_eHH, 85);

  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_eHH, 85);

  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_eHH, 85);

  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_eHH, 85);
  ////////////////////
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_dHH, 85);

  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_dHH, 85);
  ///////////////////
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_bH, 85);
  beep(note_g, 85);
  beep(note_b, 85);
  beep(note_eH, 85);
  beep(note_fH, 85);
  beep(note_gH, 85);
  beep(note_bH, 85);
  }
