#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "YOUR SSID HERE";       //Glöm inte fylla i SSID o Password
const char* password = "YOUR PSK HERE";
const int sensorPin = A0;

ESP8266WebServer server(80);

void sendMessage() {
 // server.send(200, "text/plain; charset=UTF-8", "Termometer");    //Bortkommenterad för att ersättas av Temp: *actual temp*
    }   

void handleNotFound() {
  String message = "File not found\n\n";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for(uint8_t i=0; i<server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  
}

void setup() {
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
  MDNS.begin("esp8266");
  server.on("/", sendMessage);

  
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Server up!");
  pinMode(sensorPin, INPUT);
  
}

void loop() {
  server.handleClient();
  int sensorVal = analogRead(sensorPin);

  Serial.print("Sensor Value: ");
  Serial.println(sensorVal);

  float voltage = (sensorVal/1024.0);   //Eftersom maxVolt är 1.0v på Featherns Analog port behövs inte * 5.0/3.0 

  Serial.print("Volts: ");
  Serial.println(voltage);

  float temperature = (voltage - .5) * 100;

  Serial.print("Degrees C: ");
  Serial.println(temperature);
  Serial.println ( WiFi.localIP() );
  delay(1000);
  
  String TempPrint = "Temp: " + String(temperature);      //Konverterat temperature till en sträng och printar ut den
  
  server.send(200, "text/plain", TempPrint);
}
