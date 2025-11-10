// Ultrasonic Sensor Data Logger (ESP32 → Linux Server)
// Author: Krishna Gorai
// IIT Madras (BS in Electronic Systems)

// --- Quick info about tech. ---

// This program reads distance data from an HC-SR04 ultrasonic sensor,
// sends it over Wi-Fi to a local server (e.g., a Python TCP server),
// and also indicates distance levels using three LEDs.
//
// Hardware used:
//  - ESP32 Development Board
//  - HC-SR04 Ultrasonic Sensor
//  - Red, Yellow, Green LEDs for distance indication

#include <Arduino.h>
#include <WiFi.h>
#include <Ultrasonic.h>

// --- Wi-Fi Configuration ---
const char* ssid = "_SSID_";           // Replace with your Wi-Fi SSID
const char* password = "_PASSWORD_";   // Replace with your Wi-Fi password
const char* serverIp = "PC_Local_ip";  // Replace with your computer’s IP address
const int serverPort = 8888;           // Port number used by the Python server

// --- Pin Definitions ---
#define TRIG_PIN   4
#define ECHO_PIN   2

#define RED_LED    16
#define GREEN_LED  17
#define YELLOW_LED 18

// --- Miscellaneous ---
const long interval = 500;             // Send data every 0.5 seconds
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);
WiFiClient client;

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected!");
  Serial.print("Device IP: ");
  Serial.println(WiFi.localIP());
  delay(1500);
}

void setup() {
  Serial.begin(115200);

  // Configure LED pins
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  // Turn LEDs off initially
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);

  // Connect to Wi-Fi network
  connectToWiFi();
}

void loop() {
  // Connect (or reconnect) to the TCP server
  if (!client.connected()) {
    Serial.println("Connecting to server...");
    if (client.connect(serverIp, serverPort)) {
      Serial.println("Connected to server successfully.");
    } else {
      Serial.println("Connection failed. Retrying in 5 seconds...");
      delay(5000);
      return;
    }
  }

  // Read distance from the ultrasonic sensor
  long distance = ultrasonic.distanceRead();

  if (distance != -1) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Send distance data to the server
    client.println(String(distance));

    // --- LED Indication Logic ---
    if (distance < 10) {
      // Object is very close → Red LED ON
      digitalWrite(RED_LED, HIGH);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
    } 
    else if (distance <= 30) {
      // Object is in medium range → Yellow LED ON
      digitalWrite(RED_LED, LOW);
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
    } 
    else {
      // Object is far → Green LED ON
      digitalWrite(RED_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
    }
  } 
  else {
    Serial.println("Error reading sensor data. Check connections.");
  }

  delay(interval);
}
