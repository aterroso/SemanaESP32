#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <FS.h>
#include <SPIFFS.h>

#include <DHT.h>
#include <Adafruit_Sensor.h>

#define pino_DHT 4
#define tipo_DHT DHT11

DHT dht(pino_DHT, tipo_DHT);

const char* ssid = "motog8";
const char* password = "0123456789";

#define led 2

bool flag_led = false;

float temp, umid;

AsyncWebServer server(80);


String processor(const String &aux)
{
  if(aux == "TEMP"){
    temp = dht.readTemperature();
    return String(temp);
  }
  else if(aux == "UMID"){
    umid = dht.readHumidity();
    return String(umid);
  }   
  return String();
}


void setup(){
  
  if(!SPIFFS.begin()){
    Serial.println("Erro inicialização SPIFFS");
    return;
  }
  dht.begin();

  Serial.begin(115200);
  
  pinMode(led, OUTPUT);
  Serial.println(dht.readTemperature());
  Serial.println(dht.readHumidity());
  

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi..");
  }

  Serial.print("Conectado a rede wifi e o IP=");
  Serial.println(WiFi.localIP());

 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html", false, processor);
  });  

  server.on("/estilos.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/estilos.css", "text/css");
  });

  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });

   server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(led, HIGH);
    flag_led = true;
    request->send(SPIFFS, "/index.html", "text/html", false, processor);
  });
  
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(led, LOW);
    flag_led = false;
    request->send(SPIFFS, "/index.html", "text/html", false, processor);
  });

  server.on("/normalTemp.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/normalTemp.png", "image/png");
  });

  server.on("/normalUmid.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/normalUmid.png", "image/png");
  });

 server.begin();
}
void loop()
{}
