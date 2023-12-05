#include <Arduino.h>
#include <esp_wifi.h>
#include <Firebase_ESP_Client.h>

#define LED_PIN 4

//Token Generation
#include "addons/TokenHelper.h"
//payload printing info
#include "addons/RTDBhelper.h"

//WIFI credentials
#define WIFI_SSID "*redacted*"
#define WIFI_PASSWORD "*redacted*"

//Firebase API key
#define API_KEY "here"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "ENTER_YOUR_DATABASE_URL_HERE" 

// Define the time interval for uploading data (10 minutes)
#define UPLOAD_INTERVAL 600000 // in milliseconds

// Define the struct for employee information
struct KeyCodes {
  String firstName;
  String lastName;
  int employeeID;
  String date;
};

//Firebase data object
FirebaseData fbdo;
Firebase Auth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.pring("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}