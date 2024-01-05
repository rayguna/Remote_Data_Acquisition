#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"

#define DHTPIN 5             // Digital pin connected to the DHT sensor
#define PIN_ANALOG_IN 1      // Analog pin for turbidity sensor
#define LDR_Pin 17           // Analog pin for LDR

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Set your Static IP address
IPAddress local_IP(192, 168, 137, 38);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

void setup() {
  Serial.begin(115200);
  initializeDHTSensor();

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.equals("DHTT")) {
      float temperature = readDHTTemperature();
      Serial.print("DHTT ");
      Serial.println(temperature);
      client.print("DHTT ");
      client.println(temperature);
      
    } else if (input.equals("DHTH")) {
      float humidity = readDHTHumidity();
      Serial.print("DHTH ");
      Serial.println(humidity);
      client.print("DHTH ");
      client.println(humidity);
      
    } else if (input.equals("TUR")) {
      float turbidity = readTurbidity();
      Serial.print("TUR ");
      Serial.println(turbidity);
      client.print("TUR ");
      client.println(turbidity);
      
    } else if (input.equals("LT")) {
      float lightVoltage = readLightVoltage();
      Serial.print("LT ");
      Serial.println(lightVoltage);
      client.print("LT ");
      client.println(lightVoltage);
    }
  }
}

float readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1.0; // Error value
  } else {
    return t;
  }
}

float readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1.0; // Error value
  } else {
    return h;
  }
}

float readTurbidity() {
  int adcVal = analogRead(PIN_ANALOG_IN);
  double voltage = adcVal * (5.0 / 4095.0); // 12-bit ADC resolution
  return voltage;
}

float readLightVoltage() {
  int LDRReading = analogRead(LDR_Pin);
  double voltage = LDRReading * (5.0 / 4095.0); // 12-bit ADC resolution
  return voltage;
}

void initializeDHTSensor() {
  dht.begin();
}
