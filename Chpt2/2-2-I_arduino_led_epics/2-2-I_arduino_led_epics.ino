const int pin13 = LED_BUILTIN;
     
       void setup() {
      pinMode(LED_BUILTIN, OUTPUT);
     
      Serial.begin(115200);
      //Serial.println("Enter Value (ON = 1 and OFF = 0):");
      //Serial.print("Input:");  
      }
     
      void loop() {
      if (Serial.available() > 0) {
               char comdata = char(Serial.read());
            if (comdata == '1') {
               //Serial.println("LED is ON");
               digitalWrite(pin13, HIGH);
            }
            else if (comdata == '0') {
               //Serial.println("LED is OFF");
               digitalWrite (pin13, LOW);
            }
         }
      }
