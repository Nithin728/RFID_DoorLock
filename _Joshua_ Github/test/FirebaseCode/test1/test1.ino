// Key: BGdm0pXTH3LSkpFsXVzefybzs2MBYH5cmB6c8ChLT6FAQtGYykaz_ote6ehZvm6NcR9KSKQDOyTFU5cjTzFkQzI

#include <Arduino.h>
#include <WiFi.h>
// Firebase library for ESP8266/ESP32
#include <FirebaseESP32.h> 
//#include <Firebase_ESP_Client.h>

// Replace with your Firebase project and database details
#define FIREBASE_HOST "https://console.firebase.google.com/u/1/project/rfiddoorlock-2f62e/database/rfiddoorlock-2f62e-default-rtdb/data"
// Replace with your Firebase API key
#define FIREBASE_AUTH "BGdm0pXTH3LSkpFsXVzefybzs2MBYH5cmB6c8ChLT6FAQtGYykaz_ote6ehZvm6NcR9KSKQDOyTFU5cjTzFkQzI" 

// GPIO pin connected to the LED
#define LED_PIN 26 

FirebaseData firebaseData;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin("Dell Smart TV", "123456789");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Read the LED state from Firebase
  if (Firebase.getInt(firebaseData, "/ledState")) {
    int ledState = firebaseData.intData();
    if (ledState == 1) {
      digitalWrite(LED_PIN, HIGH); // Turn on the LED
    } else {
      digitalWrite(LED_PIN, LOW); // Turn off the LED
    }
  } else {
    Serial.println("Firebase read failed");
  }

  // Wait for a while before checking again
  delay(1000);
}

