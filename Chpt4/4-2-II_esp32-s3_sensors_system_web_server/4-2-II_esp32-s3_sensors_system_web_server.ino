#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#define DHTPIN 5     // Digital pin connected to the DHT sensor
#define PIN_ANALOG_IN 1 // Analog pin for turbidity sensor
#define LDR_Pin 17 // Analog pin for LDR

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);

float readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return -1.0; // Error value
  } else {
    Serial.println(t);
    return t;
  }
}

float readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1.0; // Error value
  } else {
    Serial.println(h);
    return h;
  }
}

float readTurbidity() {
  int adcVal = analogRead(PIN_ANALOG_IN);
  double voltage = adcVal * (5.0 / 4095.0); // Assuming 12-bit ADC resolution
  Serial.printf("Turbidity Sensor Output (V): %d, \t Voltage: %.2fV\r\n", adcVal, voltage);
  return voltage;
}

float readLightVoltage() {
  int LDRReading = analogRead(LDR_Pin);
  double voltage = LDRReading * (5.0 / 4095.0); // Assuming 12-bit ADC resolution
  Serial.printf("LDR Reading: %d, \t Voltage: %.2fV\r\n", LDRReading, voltage);
  return voltage;
}

void initializeDHTSensor() {
  dht.begin();
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    /* CSS styling */
  </style>
</head>
<body>
  <h2>ESP32: Humidity, Temperature, Turbidity, and Light Server</h2>
  <p>
    <i class="fas fa-thermometer-quarter" style="color:#FF0000;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <span class="units">&deg;C</span>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#BF40BF;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <span class="units">&percnt;</span>
  </p>
  <p>
    <i class="fas fa-water" style="color:#008080;"></i>
    <span class="dht-labels">Turbidity</span>
    <span id="turbidity">%TURBIDITY%</span>
    <span class="units">V</span>
  </p>
  <p>
    <i class="far fa-lightbulb" style="color:#FFFF00;"></i>
    <span class="dht-labels">Light</span>
    <span id="light">%LIGHT%</span>
    <span class="units">V</span>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("turbidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/turbidity", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("light").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/light", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

String processor(const String& var){
  if(var == "TEMPERATURE"){
    return String(readDHTTemperature());
  }
  else if(var == "HUMIDITY"){
    return String(readDHTHumidity());
  }
  else if(var == "TURBIDITY"){
    return String(readTurbidity());
  }
  else if(var == "LIGHT"){
    return String(readLightVoltage());
  }
  return String();
}

void setup(){
  Serial.begin(115200);
  initializeDHTSensor();
  

  const char* ssid = "REPLACE_WITH_YOUR_SSID";
  const char* password = "REPLACE_WITH_YOUR_PASSWORD";
  
  
  WiFi.begin(ssid, password);

  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(readDHTTemperature()).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(readDHTHumidity()).c_str());
  });
  server.on("/turbidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(readTurbidity()).c_str());
  });
  server.on("/light", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(readLightVoltage()).c_str());
  });

  server.begin();
}

void loop(){
  // Empty loop for handling server requests
}
