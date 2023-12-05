#include<WiFi.h>
#include<Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "Dell Smarat TV"
#define WIFI_PASSWORD "123456789"
#define API_KEY "AIzaSyAVZ1DQ92AXrKU9biQ3KGg6DQuDkjbkUKc"
#define DATABASE_URL "https://rfiddoorlock2-default-rtdb.firebaseio.com/"

#define LED_PIN 22
#define SEN_PIN 32

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print("."); delay(3000);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config, &auth, "", "")){
    Serial.println("signUP OK");
    signupOK = true;
  } else {

  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(LED_PIN, OUTPUT);
  pinMode(SEN_PIN, INPUT);
  
}

void loop() {
  if(Firebase.ready() && sinupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // ------------------------------- store sensor data
    senData = analogueRead(SEN_PIN);
    voltage = (float)analogReadMilliVolts(LDR_PIN)/1000;
    if(Firebase.RTDB.setInt(&fbdo, "Sensor/sen_data", senData)) {
      Serial.println(); Serial.print(senData);
      Serial.print(" - Successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    } else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }

}
