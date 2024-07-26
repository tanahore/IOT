#include <WiFi.h>
#include <WebServer.h>
#include "ServerRoutes.h"
#include "config.h"
#include "firebase.h"
#include <OneWire.h>
#include <DallasTemperature.h>
// #include <dht.h>
// #include "DHT.h"

#define ONE_WIRE_BUS D0
#define Hum2 A0
#define LightIntensity A1
#define Humidity A3
// #define DHTPIN 26
#define Acidity A2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire); 
// #define Tempp A4

// #define DHTTYPE DHT11

// dht DHT;
// DHT dht(DHTPIN, DHTTYPE);

void setup(void) {
  initSetup();
  setupRoutes(server);
  Serial.println("Dallas Temperature Sensor Example");
  server.begin();  // Start the server
  // dht.begin();
  sensors.begin();
}

void loop(void) {
  if (ssidNew == "") {
    server.handleClient();
    Serial.println("Handling client");
  } else {
    WiFi.mode(WIFI_OFF);
    delay(10);
    
    // DHT.read11(DHTPIN);
    sensors.requestTemperatures();

  // Fetch and print the temperature in Celsius
    float tempt = sensors.getTempCByIndex(0);
    float hum2 = analogRead(Hum2);
    // float tempt = dht.readTemperature(true);
    // float hum = dht.readHumidity();
    int lightIntensity = analogRead(LightIntensity);
    int humidity = analogRead(Humidity);
    float phInput = analogRead(Acidity);
    float ph = (-0.0139*phInput)+7.7851;

    // if (isnan(tempt)) {
    //   Serial.println("Failed to read from DHT sensor!");
    //   return;
    // }

    Serial.print("suhu :");
    Serial.println(tempt);
    Serial.print("hum2 :");
    Serial.println(hum2/10);

    Serial.print("pH off: ");
    Serial.println(ph);

    humidity = map(humidity, 0, 4095, 0, 100);
    humidity = (humidity - 100) * -1; 
    Serial.print("Humidity: ");
    Serial.print(humidity);
    // Serial.print(humidity / 10);
    Serial.println("%");

    Serial.print("Light Intensity: ");
    Serial.println(lightIntensity / 2);

    WiFi.begin(ssidNew.c_str(), passNew.c_str());
    setupFirebase();
    float newph = ph*-1;
    Serial.print("new ph ");
    Serial.println(newph);
    String status = getDeviceStatus();
    if (status == "on") {
      Serial.println("Yeay Terhubung!!!!");
      status = "off";
      if (newph > 1 && newph < 10) {
        updateDeviceInput(newph, hum2/10, 33.5, lightIntensity/2);
        updateDeviceStatus(status);
      }
    }
  }
  delay(1000);
}
