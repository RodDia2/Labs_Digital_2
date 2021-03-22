// Rodrigo Diaz, 18265
// Digital 2, seccion 20
// Lab 4

const byte ledVerde = 39;
const byte ledRojo = 30;
const byte ledAzul = 40;
const byte interruptPin = 31;
const byte interruptPin2 = 17;
volatile byte state = LOW;
volatile byte state2 = LOW;

int bandera = 1;
int bandera2 = 0;
int cont = 0;
int cont2 = 0;

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, LOW);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), blink2, RISING);
}

void loop() {
//switch (cont) {
//  case 0:
  
//  }
}

void blink() {
  delay(50);
  if (digitalRead(interruptPin) == HIGH) {
  if (bandera == 1) {
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledAzul, LOW);
    digitalWrite(ledVerde, LOW);
    delay(50);
    digitalWrite(ledRojo, HIGH);
    delay(1000);
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledAzul, HIGH);
    delay(1000);
    digitalWrite(ledAzul, LOW);
    digitalWrite(ledVerde, HIGH);
    delay(1000);
    digitalWrite(ledVerde, LOW);
    bandera2 = 1;
    bandera = 0;
  }
  if (bandera == 0) {
    cont2++;
    if (cont2 > 8) {
      cont2 = 0;
      digitalWrite(ledRojo, HIGH);
      bandera = 1;
      }
  }
 }
}

void blink2() {
  delay(50);
  if (digitalRead(interruptPin2) == HIGH) {
  if (bandera2 == 1) {
    cont++;
    if (cont > 8) {
      cont = 0;
      digitalWrite(ledAzul, HIGH);
      bandera = 1;
      }
    }
  }
}
