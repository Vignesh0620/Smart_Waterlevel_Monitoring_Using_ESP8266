#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get an authentication token from the Blynk App.
char auth[] = "Your_Blynk_Authentication_Token";

// Your WiFi credentials.
char ssid[] = "Your_SSID";
char pass[] = "Your_PASSWORD";

// Ultrasonic sensor pins
const int trigPin = D1;  // GPIO pin for triggering the ultrasonic sensor
const int echoPin = D2;  // GPIO pin for receiving the echo signal

// Relay pin to control the motor
const int relayPin = D3; 

// LED pins
const int leds[] = {D4, D5, D6, D7, D8};  // Connect LEDs to these GPIO pins

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);

  // Set up Ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set up Relay and LEDs
  pinMode(relayPin, OUTPUT);
  for (int i = 0; i < 5; i++) {
    pinMode(leds[i], OUTPUT);
  }

  // Connect to Wi-Fi
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  
  // Measure distance using the ultrasonic sensor
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;  // Convert the duration to distance in centimeters

  // Update Blynk app with water level
  int waterLevel = map(distance, 1, 100, 1, 20);
  Blynk.virtualWrite(V0, waterLevel);  // Update Gauge Widget

  // Control LEDs and Relay based on water level
  for (int i = 0; i < 5; i++) {
    if (waterLevel >= (i + 1) * 4) {
      digitalWrite(leds[i], HIGH);  // Turn on LEDs one by one
    } else {
      digitalWrite(leds[i], LOW);  // Turn off LEDs if water level is below their respective stages
    }
  }

  // Check for overflow condition
  if (waterLevel >= 20) {
    digitalWrite(relayPin, LOW);  // Turn off relay to stop the motor
  } else {
    digitalWrite(relayPin, HIGH);  // Turn on relay if water level is below overflow
  }

  delay(1000);  // Add a delay for stability
}
