#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>


#define sleep_time 300
#define DHTTYPE DHT11
#define ONE_WIRE_BUS 12
#define dht_pin 13
#define LDR_pin A0

#define data1 t_ds
#define data2 h_dht
#define data3 l
#define data4 p_bmp
#define data5 a_bmp
#define data6 s_bmp


OneWire oneWire(ONE_WIRE_BUS);
WiFiServer TelnetServer(8266);
DHT dht(dht_pin, DHTTYPE); 
DallasTemperature sensors(&oneWire);
Adafruit_BMP085 bmp;

const char* ssid = "ssid";
const char* password = "password";

const String devid = "v2829F2D7DCB9358";
const String website = "http://api.pushingbox.com/pushingbox?devid="+devid;
String new_website = "";

void setup(void){
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
  TelnetServer.begin();
  dht.begin();
  Serial.begin(115200);
  sensors.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
      {
        delay(1000);
        Serial.print(".");
      }
  ArduinoOTA.onStart([]() {
  Serial.println("OTA Start");
  });
  ArduinoOTA.onEnd([]() {
  Serial.println("OTA End");
  Serial.println("Rebooting...");
  });
  pinMode(2,OUTPUT);
  ArduinoOTA.onStart([]() {
  Serial.println("OTA Start");
  });
  ArduinoOTA.onEnd([]() {
  Serial.println("OTA End");
  Serial.println("Rebooting...");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
  Serial.printf("Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
  else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  sensors.begin();
  
}
void loop(){
  ArduinoOTA.handle();
  digitalWrite(2,HIGH);
  sensors.requestTemperatures();
  float t_ds = sensors.getTempCByIndex(0); 
  float h_dht = dht.readHumidity();
  float t_dht = dht.readTemperature();
  int l = map(analogRead(LDR_pin),0,1023,0,100);
  float t_bmp = bmp.readTemperature();
  float p_bmp = bmp.readPressure();
  float a_bmp = bmp.readAltitude();
  float s_bmp = bmp.readSealevelPressure();

  if (t_ds!=-127&&!isnan(h_dht))
      {
        new_website = website+"&Data1="+String(data1)+"&Data2="+String(data2)+"&Data3="+String(data3)+"&Data4="+String(data4)+"&Data5="+String(data5)+"&Data6="+String(data6);
        Serial.println(new_website);
        if (WiFi.status() == WL_CONNECTED)
          {
            HTTPClient http;
            http.begin(new_website);
            int httpCode = http.GET();
            if (httpCode > 0)
              {
                String payload = http.getString();
              }
            http.end();
            delay(sleep_time*1000);
          }

      }
}
