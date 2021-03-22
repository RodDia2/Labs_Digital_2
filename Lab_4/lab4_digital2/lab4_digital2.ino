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
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), blink2, RISING);
}

void loop() {

}

void blink() {
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
}

void blink2() {
  if (bandera2 == 1) {
    cont++;
    if (cont > 8) {
      cont = 0;
      digitalWrite(ledAzul, HIGH);
      }
    }
}
