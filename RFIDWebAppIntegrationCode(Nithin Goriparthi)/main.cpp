// Libraries
#include <SPI.h> // SPI library for communication with RFID reader
#include <MFRC522.h> // Library for handling RFID
#include <LittleFS.h> // File system library for handling files
#include <SD.h> // Library for SD card functionality
#include <WiFi.h> // WiFi library for ESP32
#include <Update.h> // Library for updating firmware
#include <Arduino.h> // Base Arduino library
#include <FirebaseESP32.h> // Firebase library for ESP32

// Firebase token and RTDB helper functions
#include "addons/TokenHelper.h" // Handles token generation for Firebase
#include "addons/RTDBHelper.h" // Helper functions for Firebase Realtime Database

// Constants for RFID reader pins
#define SS_PIN 5 // Slave Select pin
#define RST_PIN 4 // Reset pin


// WiFi credentials
#define WIFI_SSID "<enter WIFI>"
#define WIFI_PASSWORD "<enter password>"

// Firebase project API Key
#define API_KEY "<enter API Key>"

// Firebase Realtime Database URL */
#define FIREBASE_HOST "https://rfiddoorlock-e0d8c-default-rtdb.firebaseio.com/"

// Firebase user credentials
#define USER_EMAIL "<enter email>"
#define USER_PASSWORD "<enter password>"

// Firebase objects for data and authentication
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to store hexidecimal keycode
String hexString = "";

// IP address parameters
const int ipaddress[4] = {103, 97, 67, 25};

// Variables for RFID
byte nuidPICC[4] = {0, 0, 0, 0}; // Array to store the NUID of the RFID card
MFRC522::MIFARE_Key key; // MIFARE key for RFID
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN); // Create MFRC522 instance


void setup() {
 // Initialize serial communication at 115200 baud rate
 Serial.begin(115200);

 //Assign the project and user sign in credentials
 config.host = FIREBASE_HOST;
 config.api_key = API_KEY;
 auth.user.email = USER_EMAIL;
 auth.user.password = USER_PASSWORD;

 //Setup Wifi connection
 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 Serial.print("Connecting to Wi-Fi");
 while (WiFi.status() != WL_CONNECTED){
   Serial.print(".");
   delay(300);
 }
 Serial.println();
 Serial.print("Connected with IP: ");
 Serial.println(WiFi.localIP());
 Serial.println();
 Serial.print("Wifi connected");

 // Initialize the library with the Firebase authen and config
 String hostStr = String(config.host.c_str());
 Serial.println("Config Host: " + String(hostStr));
 String api_keyStr = String(config.api_key.c_str());
 Serial.println("Config API Key: " + String(api_keyStr));
 String authEmailStr = String(auth.user.email.c_str());
 Serial.println("Auth Email: " + String(authEmailStr));
 String authPasswordStr = String(auth.user.password.c_str()); 
 Serial.println("Auth Password: " + String(authPasswordStr));
 Serial.println("Firebase.begin(&config, &auth starting)");
 Firebase.begin(&config, &auth);
 Serial.println("Firebase.begin(&config, &auth worked");

 // This option allows get and delete functions (PUT and DELETE HTTP requests) works for
 // device connected behind the Firewall that allows only GET and POST requests.
 Firebase.enableClassicRequest(fbdo, true);

 // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
 // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
 fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
//  Serial.println("Test2");

 // Limit the size of response payload to be collected in FirebaseData
 fbdo.setResponseSize(2048);
//  Serial.println("Test3");

 /* Assign the callback function for the long running token generation task */
 config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
//  Serial.println("Test5");

 //Wifi reconnection
 Firebase.reconnectWiFi(true);

  // Initialize RFID reader
 Serial.println(F("Initialize System"));
 Serial.println("Approximate your card to the reader...");
 SPI.begin(); //SPI bus
 rfid.PCD_Init(); //MFRC522 reader

 Serial.print(F("Reader :"));
 rfid.PCD_DumpVersionToSerial();

 // Initialize time for timestamping
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
}

// Function to get formatted current time
String getFormattedTime() {
  time_t now = time(nullptr);
  struct tm timeinfo;
  if (!gmtime_r(&now, &timeinfo)) {
    Serial.println("Failed to obtain time");
    return "";
  }
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

/**
 Helper routine to dump a byte array as dec values to Serial.
*/
// void printDec(byte *buffer, byte bufferSize) {
//  for (byte i = 0; i < bufferSize; i++) {
//    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
//    Serial.print(buffer[i], DEC);
//  }
// }

//Function to print a byte array as hex values to Serial
void printHex(byte *buffer, byte bufferSize) {
  hexString = ""; // Initialize an empty String to store the hex number

  for (byte i = 0; i < bufferSize; i++) {
    // Convert each byte to a two-character uppercase hex string
    if (buffer[i] < 0x10) {
      hexString.concat("0"); // Add leading zero for formatting
    }
    hexString.concat(String(buffer[i], HEX)); // Append the hex value to the String
    hexString.toUpperCase();


    // Add a space after each byte, except the last one
    if (i < bufferSize - 1) {
      hexString.concat(" ");
    }

    // Print each byte in hex format to Serial for debugging
    // Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    // Serial.print(buffer[i], HEX);
 }

  // Now hexString contains the complete hex number in the desired format
  Serial.println(); // New line after printing hex values
  Serial.println("Hex String: " + hexString); // Print the complete hex number
  Serial.println("RFID=" + String(hexString.length()));
}

//Function to read RFID card
void readRFID(void ) { /* function readRFID */
  ////Read RFID card

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }

  Serial.print(F("RFID In hex: "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  Serial.println("RFID=" + String(hexString.length()));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


void loop() {
  Serial.println("Read rfid executed");
  readRFID();
  delay(3000);
  if (Firebase.ready()) {
    Serial.println("Entered if statement");
    delay(1000);

    if (Firebase.getString(fbdo, "/users/sxHEN70UALWZiBs9AFEKn9Frvns1/keycodes/-Nkqw5urjFih1rakNGtq/content")) {
      String firebaseData = fbdo.stringData();
      firebaseData.trim(); // Trim whitespace
      firebaseData.toUpperCase(); // Convert to upper case

      printHex(rfid.uid.uidByte, rfid.uid.size);
      hexString.trim(); // Trim whitespace
      hexString.toUpperCase(); // Convert to upper case

      Serial.println("From RTDB: " + firebaseData);
      Serial.println("From RFID: " + hexString);
      Serial.println("Lengths: RTDB=" + String(firebaseData.length()) + ", RFID=" + String(hexString.length()));
      String currentTime = getFormattedTime();

      if (strcmp(firebaseData.c_str(), hexString.c_str()) == 0) {
        Serial.println("Authorize access");
        Firebase.setString(fbdo, "/accessStatus", "Access Granted");
        Firebase.setString(fbdo, "/accessGrantedTime", currentTime);
      } else {
        Serial.println("Access Denied");
        Firebase.setString(fbdo, "/accessStatus", "Access Denied");
        Firebase.setString(fbdo, "/accessDeniedTime", currentTime);
      }
    } else {
      Serial.println("Failed to retrieve data: " + fbdo.errorReason());
    }
  }
  delay(1000);
  Serial.println("After if statement");
}



 





