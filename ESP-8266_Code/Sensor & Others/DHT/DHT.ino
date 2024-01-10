#include "DHTesp.h"
DHTesp dht;
#define DHTTYPE DHT11
float currentTemp;
float currentHumidity;
const int dhtPing = 0;

void setup() {
  pinMode (dhtPing, INPUT);
  dht.setup(dhtPing, DHTesp::DHT11);
  Serial.begin(115200);
  Serial.setTimeout(100);
}

void loop() {
  
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  
  Serial.print("Temperature : ");
  Serial.println(temperature);
  Serial.print("Humidity : ");
  Serial.println(humidity);
  delay(1000);
}
