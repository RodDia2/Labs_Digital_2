// Adafruit IO Digital Output Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-output
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// digital pin 5
#define LED_PIN 2
#define RXD2 16
#define TXD2 17

int incomingByte = 0;
String incomingString = "";
float incomingFloat = 0.0;

// set up the 'digital' feed
AdafruitIO_Feed *AYFeed = io.feed("AY");
AdafruitIO_Feed *Led1Feed = io.feed("Led1");
AdafruitIO_Feed *Piloto1Feed = io.feed("Piloto1");
AdafruitIO_Feed *Led2Feed = io.feed("Led2");
AdafruitIO_Feed *Piloto2Feed = io.feed("Piloto2");

void setup() {
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  
  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  Led1Feed->onMessage(handleMessage);
  Led2Feed->onMessage(handleMessage2);
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  Led1Feed->get();
  Led2Feed->get();

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  if (Serial2.available() > 0) {
    incomingByte = Serial2.read();
   // incomingFloat = Serial2.read();
   // incomingByte = incomingFloat.toInt();
   // incomingString = Serial2.readString();
   // Serial.println(Serial2.readString());
    Serial.print("Valor AY: ");
   // Serial.println(incomingByte);
    Serial.println(incomingByte);
  //  Serial.println(Serial2.read());
    AYFeed->save(incomingByte);
  }

  //Serial2.write((char)10);
  Serial.write((char)2);
  
  delay(3000);
  
}

// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());
  if (data->toString()=="ON") {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(22, HIGH);
    Serial.print("sending -> on1");
    Serial2.write(0X0B);
    Piloto1Feed->save(1);
    }
  if (data->toString()=="OFF") {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(22, LOW);
    Serial.print("sending -> off1");
    Piloto1Feed->save(0);
    }
}
void handleMessage2(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());
  if (data->toString()=="ON") {
    digitalWrite(23, HIGH);
    Serial.print("sending -> on2");
    Serial2.write(0X0A);
    Piloto2Feed->save(1);
    }
  if (data->toString()=="OFF") {
    digitalWrite(23, LOW);
    Serial.print("sending -> off2");
    Piloto2Feed->save(0);
    }
}
