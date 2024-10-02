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

#include <SPIFFS.h>

void funcao_chama_paginaHtml();
void func_imgTemp();
void func_imgUmid();

void setup() 
{
  dht.begin();
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
  Serial.println("Falha ao montar o SPIFFS");
  return;
  }
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
  server.on("/normalTemp.png", func_imgTemp);
  server.on("/normalUmid.png", func_imgUmid);

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
  HTML += "<center><img src=\"/normalTemp.png\" alt=\"Imagem\" style=\"width:40px;height:80px;\"></center>"; 
  HTML += "<p><h3>A umidade relativa do ar é de: ";
  HTML += (float)umid;
  HTML += "%.</h3></p>";
  HTML += "<center><img src=\"/normalUmid.png\" alt=\"Imagem\" style=\"width:40px;height:80px;\"></center>";
  HTML += "</body></html>";
  server.send(200, "text/html", HTML);
}

void func_imgTemp() {
  File file = SPIFFS.open("/normalTemp.png", "r");
  if (!file) {
    Serial.println("Falha ao abrir a imagem");
    return;
  }
  server.streamFile(file, "image/png");
  file.close();
}

void func_imgUmid() {
  File file = SPIFFS.open("/normalUmid.png", "r");
  if (!file) {
    Serial.println("Falha ao abrir a imagem");
    return;
  }
  server.streamFile(file, "image/png");
  file.close();
}