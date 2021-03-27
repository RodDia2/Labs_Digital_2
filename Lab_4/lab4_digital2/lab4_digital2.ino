// Rodrigo Diaz, 18265
// Digital 2, seccion 20
// Lab 4

// primero se asignan los pines predefinidos para el led RGB y los botones
const byte ledVerde = 39;
const byte ledRojo = 30;
const byte ledAzul = 40;
const byte interruptPin = 31;
const byte interruptPin2 = 17;
volatile byte state = LOW;
volatile byte state2 = LOW;
// se definen banderas para cumplir con los requisitos del juego
int bandera = 1;
int bandera2 = 0;
// se crea un contador para cada jugador
int cont = 0;
int cont2 = 0;
// se definen los pines de salida para el jugador 1
int led1 = PB_5;
int led2 = PB_0;
int led3 = PB_1;
int led4 = PE_4;
int led5 = PE_5;
int led6 = PB_4;
int led7 = PA_5;
int led8 = PA_6;
// se definen los pines de salida para el jugador 2
int led9 = 38;
int led10 = 37;
int led11 = 36;
int led12 = 35;
int led13 = 34;
int led14 = 33;
int led15 = 32;
int led16 = 11;
// en el setup se configuran todas las leds como outputs, los botones como inputs 
// y las interrupciones por boton
void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);
  pinMode(led10, OUTPUT);
  pinMode(led11, OUTPUT);
  pinMode(led12, OUTPUT);
  pinMode(led13, OUTPUT);
  pinMode(led14, OUTPUT);
  pinMode(led15, OUTPUT);
  pinMode(led16, OUTPUT);
  
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  // ambas interrupciones suceden cuando el boton se encuentra el low, porque tiene pullup
  // se lleva a las funciones blink y blink2 respectivamente
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, LOW);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), blink2, LOW);
}

void loop() {
  // en el loop solo se tiene dos switch cases, donde se prenderan las leds correspondientes
  // al contador de decadas para cada jugador. Solo se utilizan digital writes.
switch (cont) {
  case 0:
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
//    digitalWrite(led8, LOW);
    break;
  case 1:
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);
    break;
  case 2:
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);
    break;
  case 3:
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);
    break;
  case 4:
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);
    break;
  case 5:
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);
    break;
  case 6:
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, HIGH);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);
    break;
  case 7:
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, HIGH);
    digitalWrite(led8, LOW);
    break;
  case 8:
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, HIGH);
    break;
  }
switch(cont2) {
  case 0:
    digitalWrite(led9, LOW);
    digitalWrite(led10, LOW);
    digitalWrite(led11, LOW);
    digitalWrite(led12, LOW);
    digitalWrite(led13, LOW);
    digitalWrite(led14, LOW);
    digitalWrite(led15, LOW);
 //   digitalWrite(led16, LOW);
    break;
  case 1:
    digitalWrite(led9, HIGH);
    digitalWrite(led10, LOW);
    digitalWrite(led11, LOW);
    digitalWrite(led12, LOW);
    digitalWrite(led13, LOW);
    digitalWrite(led14, LOW);
    digitalWrite(led15, LOW);
    digitalWrite(led16, LOW);
    break;
  case 2:
    digitalWrite(led9, LOW);
    digitalWrite(led10, HIGH);
    digitalWrite(led11, LOW);
    digitalWrite(led12, LOW);
    digitalWrite(led13, LOW);
    digitalWrite(led14, LOW);
    digitalWrite(led15, LOW);
    digitalWrite(led16, LOW);
    break;
  case 3:
    digitalWrite(led9, LOW);
    digitalWrite(led10, LOW);
    digitalWrite(led11, HIGH);
    digitalWrite(led12, LOW);
    digitalWrite(led13, LOW);
    digitalWrite(led14, LOW);
    digitalWrite(led15, LOW);
    digitalWrite(led16, LOW);
    break;
  case 4:
    digitalWrite(led9, LOW);
    digitalWrite(led10, LOW);
    digitalWrite(led11, LOW);
    digitalWrite(led12, HIGH);
    digitalWrite(led13, LOW);
    digitalWrite(led14, LOW);
    digitalWrite(led15, LOW);
    digitalWrite(led16, LOW);
    break;
  case 5:
    digitalWrite(led9, LOW);
    digitalWrite(led10, LOW);
    digitalWrite(led11, LOW);
    digitalWrite(led12, LOW);
    digitalWrite(led13, HIGH);
    digitalWrite(led14, LOW);
    digitalWrite(led15, LOW);
    digitalWrite(led16, LOW);
    break;
  case 6:
    digitalWrite(led9, LOW);
    digitalWrite(led10, LOW);
    digitalWrite(led11, LOW);
    digitalWrite(led12, LOW);
    digitalWrite(led13, LOW);
    digitalWrite(led14, HIGH);
    digitalWrite(led15, LOW);
    digitalWrite(led16, LOW);
    break;
  case 7:
    digitalWrite(led9, LOW);
    digitalWrite(led10, LOW);
    digitalWrite(led11, LOW);
    digitalWrite(led12, LOW);
    digitalWrite(led13, LOW);
    digitalWrite(led14, LOW);
    digitalWrite(led15, HIGH);
    digitalWrite(led16, LOW);
    break;
  case 8:
    digitalWrite(led9, LOW);
    digitalWrite(led10, LOW);
    digitalWrite(led11, LOW);
    digitalWrite(led12, LOW);
    digitalWrite(led13, LOW);
    digitalWrite(led14, LOW);
    digitalWrite(led15, LOW);
    digitalWrite(led16, HIGH);
    break;
  }
}
// en el primer blink, se realiza primero en antirrebote. despues se utiliza la bandera que
// revisa si se inicializo o ya termino un juego previo para poder comenzar el semaforo. 
// de lo contrario, sirve para aumentar uno de los contadores de un jugador. Si el jugador
// llega a 8 en su contador, este se resetea y se regresa a modo semaforo para volver a 
// iniciar un juego.
void blink() {
  delay(50);
  if (digitalRead(interruptPin) == HIGH) {
  if (bandera == 1) {
    digitalWrite(led16, LOW);
    digitalWrite(led8, LOW);
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
  else {
    cont2++;
    if (cont2 > 7) {
      cont2 = 0;
      cont = 0;
      digitalWrite(ledAzul, HIGH);
      digitalWrite(led16, HIGH);
      
      bandera = 1;
      bandera2 = 0;
      }
  }
  
 }
}
// blink2 es una version mas simple de blink porque no se utiliza para activar el semaforo. 
// solamente tiene un antirrebote, y despues aumenta el contador. De igual manera, si el 
// contador del segundo jugador llega a 8, se prende el led indicador y se reinicia el conteo.
// ademas prende la bandera que habilia el semaforo de blink para que el otro jugador no pueda
// seguir jugando
void blink2() {
  delay(50);
  if (digitalRead(interruptPin2) == HIGH) {
  if (bandera2 == 1) {
    cont++;
    if (cont > 7) {
      cont = 0;
      cont2 = 0;
      digitalWrite(ledRojo, HIGH);
      digitalWrite(led8, HIGH);
      bandera = 1;
      bandera2 = 0;
      }
    }
  }
}
