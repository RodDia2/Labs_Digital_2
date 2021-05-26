/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server
  Basándose en los ejemplos de:
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
//definimos los pines de UART 2
#define RXD0 3
#define TXD0 1
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "ARRIS-EB62";  // Enter your SSID here
const char* password = "2PM7H7600913";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

char datain[1];
uint8_t parqueo1 = 0;
uint8_t parqueo2 = 0;
uint8_t parqueo3 = 0;
uint8_t parqueo4 = 0;
uint8_t disponibles = 0;
String datain2 = "";

int a = 0;
char b;



//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200, SERIAL_8N1, RXD0, TXD0);
  //Serial2.begin(115200, SERIAL_8N1, RXD0, TXD0);
 
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8

  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);

  //pines 7seg
  pinMode(36, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(19, OUTPUT);
  //7seg apagado
  digitalWrite(36, LOW);//
  digitalWrite(39, LOW);//
  digitalWrite(34, LOW);//
  digitalWrite(35, LOW);//
  digitalWrite(23, LOW);//
  digitalWrite(22, LOW);//
  digitalWrite(21, LOW);//
  digitalWrite(19, LOW);//
 
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  if (Serial.available() > 0) {
    b=Serial.read();
    Serial.println(b);
    //delay(500);
    if (b == 'a'){
      parqueo1 = 1;
    }
    else if (b == 'b'){
      parqueo1 = 0;
    }
    if (b == 'c'){
      parqueo2 = 1;
    }
    else if (b == 'd'){
      parqueo2 = 0;
    }
    if (b == 'e'){
      parqueo3 = 1;
    }
    else if (b == 'f'){
      parqueo3 = 0;
    }
    if (b == 'g'){
      parqueo4 = 1;
    }
    else if (b == 'h'){
      parqueo4 = 0;
    }
    Serial.println(parqueo1);
  }
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  //LED1status = LOW;
  Serial.println("Updating Data...");
  //  parqueo1 = 0;
  //  parqueo2 = 1;
  //  parqueo3 = 1;
  //  parqueo4 = 1;
  disponibles = 4 - (parqueo1 + parqueo2 + parqueo3 + parqueo4);
  server.send(200, "text/html", SendHTML2(parqueo1, parqueo2, parqueo3, parqueo4, disponibles));
}

//************************************************************************************************
// Procesador de HTML
//************************************************************************************************

String SendHTML2(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4, uint8_t disponible) {
  String pagina = "<!doctype html>\n";
  pagina += "<html lang=en>\n";
  pagina += "<head>\n";
  pagina += "<meta charset=utf-8>\n";
  pagina += "<meta name=viewport content=\"width=device-width, initial-scale=1\">\n";
  pagina += "<link href=https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css rel=stylesheet integrity=sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x crossorigin=anonymous>\n";
  pagina += "<title>Parqueos</title>\n";
  pagina += "</head>\n";
  pagina += "<body>\n";
  pagina += "<h1>Parqueo-Matic &#127915  &#128653  &#128663</h1>\n";
  pagina += "<table class=\"table table-light table-sm text-center table-bordered\">\n";
  pagina += "<caption>Control de parqueos: Primer Nivel</caption>\n";
  pagina += "<thead class=table-dark>\n";
  pagina += "<tr>\n";
  pagina += "<th scope=col># Parqueo &#128664</th>\n";
  pagina += "<th scope=col>Estado &#128655</th>\n";
  pagina += "</tr>\n";
  pagina += "</thead>\n";
  pagina += "<tbody>\n";
  pagina += "<tr>\n";

  // 1 significa ocupado y 0 libre

  if (p1 == 1)
  { pagina += "<th scope=row>1</th>\n";
    pagina += "<td class=table-danger>Ocupado &#128308</td>\n";
    pagina += "</tr>\n";
    pagina += "<tr>\n";
  }
  else
  { pagina += "<th scope=row>1</th>\n";
    pagina += "<td class=table-success>Disponible &#128309</td>\n";
    pagina += "</tr>\n";
    pagina += "<tr>\n";
  }

  if (p2 == 1)
  { pagina += "<th scope=row>2</th>\n";
    pagina += "<td class=table-danger>Ocupado &#128308</td>\n";
    pagina += "</tr>\n";
    pagina += "<tr>\n";
  }
  else
  { pagina += "<th scope=row>2</th>\n";
    pagina += "<td class=table-success>Disponible &#128309</td>\n";
    pagina += "</tr>\n";
    pagina += "<tr>\n";
  }

  if (p3 == 1)
  { pagina += "<th scope=row>3</th>\n";
    pagina += "<td class=table-danger>Ocupado &#128308</td>\n";
    pagina += "</tr>\n";
    pagina += "<tr>\n";
  }
  else
  { pagina += "<th scope=row>3</th>\n";
    pagina += "<td class=table-success>Disponible &#128309</td>\n";
    pagina += "</tr>\n";
    pagina += "<tr>\n";
  }

  if (p4 == 1)
  { pagina += "<th scope=row>4</th>\n";
    pagina += "<td class=table-danger>Ocupado &#128308</td>\n";
    pagina += "</tr>\n";
    pagina += "<tr>\n";
  }
  else
  { pagina += "<th scope=row>4</th>\n";
    pagina += "<td class=table-success>Disponible &#128309</td>\n";
    pagina += "</tr>\n";
    pagina += "<tr>\n";
  }

  pagina += "</tbody>\n";
  pagina += "</tfoot>\n";
  pagina += "<tr>\n";
  pagina += "<th class=table-active scope=row>Total Disponible</th>\n";
  pagina += "<td class=table-info>";
  pagina += (int)disponible;
  pagina += "</td>\n";
  pagina += "</tr>\n";
  pagina += "</tfoot>\n";
  pagina += "</table>\n";
  pagina += "<script>function timedRefresh(a){setTimeout(\"location.reload(true);\",a)}window.onload=timedRefresh(1000);</script>\n";
  pagina += "<script src=https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js integrity=sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4 crossorigin=anonymous></script>\n";
  pagina += "<script src=https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js integrity=sha384-IQsoLXl5PILFhosVNubq5LC7Qb9DXgDA9i+tQ8Zj3iwWAwPtgFTxbJ8NT4GN1R8p crossorigin=anonymous></script>\n";
  pagina += "<script src=https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.min.js integrity=sha384-Atwg2Pkwv9vp0ygtn1JAojH0nYbwNJLPhwyoVbhoPwBhjQPR5VtM2+xf0Uwh9KtT crossorigin=anonymous></script>\n";
  pagina += "</body>\n";
  pagina += "</html>";
  return pagina;
}
String SendHTML(uint8_t led1stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP32 Web Server &#128664</h1>\n";
  ptr += "<h3>Ejemplo de Web Server</h3>\n";

  if (led1stat)
  {
    ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
