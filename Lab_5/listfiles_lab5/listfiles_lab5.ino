/*
  Listfiles
 
 This example shows how print out the files in a 
 directory on a SD card 
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 2 Feb 2014
 by Scott Fitzgerald
 
 This example code is in the public domain.

 */
#include <SPI.h>
#include <SD.h>
// crear los files
File root;
File myFile;
// se crea una variable para seleccionar las opciones
int seleccion = 0;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  SPI.setModule(0);


  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(PA_3, OUTPUT); // chip select
  // se revisa el chip select para ver si se realizo la conexion
  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  // se abre con la funcion open y se utiliza la funcion de print Directory para saber que archivos hay y su tamano
  root = SD.open("/");

  printDirectory(root, 0);

  Serial.println("done!");
}

void loop()
{
 // en el loop se revisa si se recibe un dato en el puerto serial y se revisa si es alguna de las opciones disponibles
if (Serial.available()>0){
  // se guarda el dato
  seleccion = Serial.read();
  // si es igual a 1
  if (seleccion == '1'){
    // se abre el archivo
      myFile = SD.open("kirby.txt");
      // se revisa que se pueda acceder el archivo
      if (myFile) {
      Serial.println("kirby.txt:");

      // read from the file until there's nothing else in it:
      while (myFile.available()) {
        Serial.write(myFile.read());
      }
    // close the file:
      myFile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
    }
    // se repite el mismo proceso para 1 pero para 2 y 3
  if (seleccion == '2'){
      myFile = SD.open("rayquaza.txt");
      if (myFile) {
      Serial.println("rayquaza.txt:");

      // read from the file until there's nothing else in it:
      while (myFile.available()) {
        Serial.write(myFile.read());
      }
    // close the file:
      myFile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
    }
  if (seleccion == '3'){
      myFile = SD.open("pikachu.txt");
      if (myFile) {
      Serial.println("pikachu.txt:");

      // read from the file until there's nothing else in it:
      while (myFile.available()) {
        Serial.write(myFile.read());
      }
    // close the file:
      myFile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
    }
    // esta ultima opcion es simplemente volver a desplegar el menu
  if (seleccion == '4'){
     root = SD.open("/");

     printDirectory(root, 0);

     Serial.println("done!");
    }
  }
}

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
