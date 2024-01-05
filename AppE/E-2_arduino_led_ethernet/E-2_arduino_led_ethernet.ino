#include <SPI.h>
#include <Ethernet.h>

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// IP address settings
IPAddress ip(XXX, XXX, XXX, XXX);
EthernetServer server(80);

const int ledPin = 13; // Use pin 13 for the LED

void setup() {
  // Start the Ethernet connection and the server
  Ethernet.begin(mac, ip);
  server.begin();
  
  // Initialize LED pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(9600); // Start serial communication
}

void loop() {
  EthernetClient client = server.available();
  
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char command = client.read();
        if (command == '1') {
          digitalWrite(ledPin, HIGH); // Turn LED on
          Serial.println("Received command to turn LED ON");
        } else if (command == '0') {
          digitalWrite(ledPin, LOW); // Turn LED off
          Serial.println("Received command to turn LED OFF");
        }
        delay(100); // Delay to process command
        client.println(command); // Send back the received command as acknowledgment
      }
    }
    client.stop();
  }
}
