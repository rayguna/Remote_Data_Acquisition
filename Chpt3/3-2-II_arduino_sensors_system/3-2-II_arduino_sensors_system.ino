#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include "GravityTDS.h"
#include "DHT.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress staticIP(XXX, XXX, YYY, YYY);
IPAddress gateway(XXX, XXX, 1, 1);
IPAddress subnet(255, 255, 255, 0);
EthernetServer server(80);

#define TdsSensorPin A1
GravityTDS gravityTds;

#define sensorPower 7
#define sensorPin A0

#define DHT11PIN 5
#define DHTTYPE DHT11
DHT dht = DHT(DHT11PIN, DHTTYPE);

void setup() {
  Ethernet.begin(mac, staticIP, gateway, subnet);
  server.begin();
  Serial.begin(115200);
  setupTDS();
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);
  dht.begin();
}

void loop() {
  EthernetClient client = server.available();

  if (client) {
    while (client.connected()) {
      if (client.available()) {
        String command = client.readStringUntil('\n');
        command.trim();

        if (command == "TDS") {
          calculateAndSendTDS(client);
        } else if (command == "WL") {
          sendWaterLevel(client);
        } else if (command == "DHTT") {
          sendTemperature(client);
        } else if (command == "DHTH") {
          sendHumidity(client);
        }
        delay(100);
      }
    }
    client.stop();
  }
}

void setupTDS() {
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);
  gravityTds.setAdcRange(1024);
  gravityTds.begin();
}

void calculateAndSendTDS(EthernetClient &client) {
  float temperature = dht.readTemperature();
  gravityTds.setTemperature(temperature);
  gravityTds.update();
  int tdsValue = gravityTds.getTdsValue();
  Serial.print("TDS ");
  Serial.print(tdsValue);
  Serial.println(" ppm");
  client.print("TDS ");
  client.print(tdsValue);
  client.println(" ppm");
}

void sendWaterLevel(EthernetClient &client) {
  int waterLevel = readSensor(); // Read water level each time the command is received
  Serial.print("WL ");
  Serial.println(waterLevel);
  client.print("WL ");
  client.print(waterLevel);
  client.println();
}

void sendTemperature(EthernetClient &client) {
  float temperature = dht.readTemperature();
  Serial.print("DHTT ");
  Serial.println(temperature);
  client.print("DHTT ");
  client.println(temperature);
}

void sendHumidity(EthernetClient &client) {
  float humidity = dht.readHumidity();
  Serial.print("DHTH ");
  Serial.println(humidity);
  client.print("DHTH ");
  client.println(humidity);
}

int readSensor() {
  digitalWrite(sensorPower, HIGH);
  delay(10);
  int val = analogRead(sensorPin);
  digitalWrite(sensorPower, LOW);
  return val;
}
