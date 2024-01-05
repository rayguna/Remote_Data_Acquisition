#include <WiFi.h>
WiFiServer server(80);

const int ledPin = 2; // Define the pin connected to the LED

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  // Connect to Wi-Fi
  WiFi.begin("YourNetworkName", "YourNetworkPassword");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Start the server
  server.begin();
  Serial.println("Server started");

  Serial.println(WiFi.localIP()); // Print the local IP address
}

void loop() {
  WiFiClient client = server.available(); // Check for a new client connection

  if (client) {
    Serial.println("New client connected");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r'); // Read the client's request
        Serial.println(request);

        if (request.indexOf("ON") != -1) {
          digitalWrite(ledPin, HIGH); // Turn on the LED
        } else if (request.indexOf("OFF") != -1) {
          digitalWrite(ledPin, LOW); // Turn off the LED
        }

        client.flush(); // Clear the client buffer
      }
    }
    client.stop(); // Close the connection
    Serial.println("Client disconnected");
  }
}
