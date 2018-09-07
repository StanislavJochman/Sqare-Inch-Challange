#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 12
#define dht_pin 13
#define LDR_pin A0

OneWire oneWire(ONE_WIRE_BUS);
DHT dht(dht_pin, DHTTYPE); 
DallasTemperature sensors(&oneWire);

void setup(void){ 
  dht.begin();
  Serial.begin(115200);
  sensors.begin();

  
}
void loop(){
  sensors.requestTemperatures();
  float C = sensors.getTempCByIndex(0); 
  float h = dht.readHumidity();
  float t = dht.readTemperature();         
  Serial.print("humidity");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("temperature(DHT)");
  Serial.print(t); 
  Serial.print("C  ");
  Serial.print("temperature(DS18B20)");
  Serial.print(C);
  Serial.print("C  ");
  Serial.print("Light");
  Serial.print(map(analogRead(LDR_pin),0,1023,0,100));
  Serial.println("%");
}
