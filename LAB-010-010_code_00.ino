#include <ESP8266WiFi.h>

// Pin Definitions
const int redLedPin = D6;
const int greenLedPin = D5;
const int redButtonPin = D7;
const int blackButtonPin = D8;
const int relayPin = D2;

// Variables
int relayState = 0;

void setup() {
  // Initialize pins
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redButtonPin, INPUT_PULLUP);
  pinMode(blackButtonPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);

  // Initialize serial communication
  Serial.begin(115200);
}

void loop() {
  // Read the state of the red button
  int redButtonState = digitalRead(redButtonPin);

  // Check if the red button is pressed
  if (redButtonState == HIGH) {
    // Toggle the relay state
    relayState = !relayState;
    digitalWrite(relayPin, relayState);

    // Turn on the green LED for 2 seconds if the relay is off (0)
    if (relayState == 0) {
      digitalWrite(greenLedPin, HIGH);
      delay(2000);
      digitalWrite(greenLedPin, LOW);
    }
    // Turn on the red LED for 2 seconds if the relay is on (1)
    else {
      digitalWrite(redLedPin, HIGH);
      delay(2000);
      digitalWrite(redLedPin, LOW);
    }
  }

  // Check if the black button is pressed
  if (digitalRead(blackButtonPin) == HIGH) {
    // Check the relay state and turn on the corresponding LED
    if (relayState == 0) {
      digitalWrite(greenLedPin, HIGH);
    } else {
      digitalWrite(redLedPin, HIGH);
    }
    // Keep the LED on as long as the black button is pressed
    while (digitalRead(blackButtonPin) == HIGH) {
      delay(100);
    }
    // Turn off the LED
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, LOW);
  }

  // Handle serial communication
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.toLowerCase();

    if (command == "status") {
      Serial.println("B" + String(relayState));
    } else if (command == "status_blink") {
      // Blink the corresponding LED for 15 seconds
      int blinkDuration = 300; // .3 seconds
      int ledPin = (relayState == 0) ? greenLedPin : redLedPin;
      for (int i=0; i <= 18; i++){
        digitalWrite(ledPin, HIGH);
        delay(blinkDuration);
        digitalWrite(ledPin, LOW);
        delay(blinkDuration);
      }
      
    } else if (command == "relay b0") {
      relayState = 0;
      digitalWrite(relayPin, relayState);
    } else if (command == "relay b1") {
      relayState = 1;
      digitalWrite(relayPin, relayState);
    } else if (command == "help") {
      Serial.println("HELP, Commands:");
      Serial.println(" - 'status' : returns 'b1' or 'b2 connection;");
      Serial.println(" - 'status_blink' : blinks LED of the connection;");
      Serial.println(" - 'relay b0' : connect b0;");
      Serial.println(" - 'relay b1' : connect b1;");
    } else {
      Serial.println("Invalid command");
    }
  }
}
