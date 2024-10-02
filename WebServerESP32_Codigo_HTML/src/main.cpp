#include <Arduino.h>

/******************************************* MODO STATION (CONECTA A REDE WIFI) *************************************/

#include <DHT.h>
#include <Adafruit_Sensor.h>

#define pino_DHT 4
#define tipo_DHT DHT11

DHT dht(pino_DHT, tipo_DHT);

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "motog8";  // Enter your SSID here
const char* password = "0123456789";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

void funcao_chama_paginaHtml();

void setup() 
{
  dht.begin();
  Serial.begin(115200);
  Serial.println("Conectando......");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado com sucesso");
  Serial.print("IP do ESP32 eh: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", funcao_chama_paginaHtml);
  server.begin();
  delay(100);
}

void loop() 
{
  server.handleClient();
}

void funcao_chama_paginaHtml() 
{

  float umid = dht.readHumidity();
  float temp = dht.readTemperature();

  String HTML = "<!DOCTYPE html><html>";
  HTML += "<head>";
  HTML += "<meta charset=\"UTF-8\">";
  HTML += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  HTML += "<meta http-equiv=\"refresh\" content=\"5\">";  // Reload a cada 5 segundos
  HTML += "<title>Monitoramento de Temperatura e Umidade</title>";
  HTML += "</head>";
  HTML += "<body><h1><center>Monitoramento de Temperatura e Umidade</center></h1>";
  HTML += "<p><h3>A temperatura atual é de: ";
  HTML += (float)temp;
  HTML += " graus</h3></p>"; 
  HTML += "<p><h3>A umidade relativa do ar é de: ";
  HTML += (float)umid;
  HTML += "%.</h3></p>";
  HTML += "</body></html>";

  server.send(200, "text/html", HTML);
}