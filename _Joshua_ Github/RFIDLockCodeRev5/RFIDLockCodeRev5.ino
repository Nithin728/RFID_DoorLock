
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <esp_wifi.h>
#include <Firebase_ESP_Client.h>

Servo myservo; 
int pos = 0;

#define LED_RED 26
#define LED_BLUE 27

#define RST_PIN 22          // Reset pin
#define SS_PIN 5          // Slave select pin
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

const int UID_LENGTH = 4;
const int MAX_UIDS = 10;
byte uids[MAX_UIDS][UID_LENGTH] = {{0}};

//Token Generation
#include "addons/TokenHelper.h"
//payload printing info
#include "addons/RTDBhelper.h"

//WIFI credentials
#define WIFI_SSID "here"
#define WIFI_PASSWORD "here"

//Firebase API key
#define API_KEY "here"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "ENTER_YOUR_DATABASE_URL_HERE" 

//Firebase data object
FirebaseData fbdo;
Firebase Auth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup() {
  // Initialize LED's
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  // Initialize RFID reader
  Serial.flush();
  Serial.begin(115200);         // Initialize serial communication
  SPI.begin();                // Initialize SPI bus
  mfrc522.PCD_Init();         // Initialize MFRC522

  // Initialize servo
  myservo.attach(33);
  myservo.write(90);

  //Initialize optical sensor port
  pinMode(32, INPUT);

  //manually entered keys
  uids[0][0] = 0xC1;
  uids[0][1] = 0xD8;
  uids[0][2] = 0xB2;
  uids[0][3] = 0x74;

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

void loop() {
  //If new card is present
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) { 
    Serial.print("UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {     // Print UID
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    
    if (isUIDPresent(mfrc522.uid.uidByte)) {
      digitalWrite(LED_BLUE, 1);
      Serial.println("Access granted");
      Serial.println("Opening door");
      Serial.println(" ");
      delay(500);

      // Open door
      myservo.write(0);
      delay(500);
      myservo.write(90);
      delay(1000);
      digitalWrite(LED_BLUE, 0);
      
      // Wait for proximity sensor the detect that the door is closed
      int sensorValue;
      float voltage = 1;
      int count = 0;

      while (1 == 1) {
        sensorValue = analogRead(32);
        voltage = sensorValue *(3.3 / 4095.0);
        if(voltage >= .95) {
          Serial.println("Waiting for door to shut...");
          count = 0;
        } else {
          Serial.println("Door is shut?");
          count = count + 1;
        }
        delay(1000);
        if(count >= 3) {
          Serial.println("Door is shut!");
          break;
        }
      }
      Serial.println(" ");

      // Close door
      Serial.println("Closing door...");
      Serial.println(" ");
      delay(2000);
      myservo.write(180);
      delay(500);
      myservo.write(90);
      delay(2000);

      Serial.println("Scan RFID card to check access");

    //Admin Key detection
    } else if (mfrc522.uid.uidByte[0] == 0xD1 && mfrc522.uid.uidByte[1] == 0xA4 && mfrc522.uid.uidByte[2] == 0xB3 && mfrc522.uid.uidByte[3] == 0x74) {

      Serial.println("Admin Key present");
      delay(500);
      Serial.println("Scan key to add or remove");

      while(true) {
        //Check if key is present in table alraedy
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) { 
          Serial.print("UID: ");
          for (byte i = 0; i < mfrc522.uid.size; i++) {     // Print UID
            Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
            Serial.print(mfrc522.uid.uidByte[i], HEX);
          }
          Serial.println(" ");

          
          if (mfrc522.uid.uidByte[0] == 0x63 && mfrc522.uid.uidByte[1] == 0xDA && mfrc522.uid.uidByte[2] == 0xF7 && mfrc522.uid.uidByte[3] == 0x34) {
            Serial.println("Please do not scan admin card");
            Serial.println(" ");
            Serial.println("Scan RFID card to check access");
            break;
          //If not present give the key access
          } else if (isUIDPresent(mfrc522.uid.uidByte) != 1) {
            addUID(mfrc522.uid.uidByte);
            digitalWrite(LED_BLUE, 1);
            Serial.println("Tag has been given access!");
            Serial.println(" ");
            delay(1000);
            digitalWrite(LED_BLUE, 0);
            Serial.println("Scan RFID card to check access");
            break;
            //if present remove key's access
          } else {
            removeUID(mfrc522.uid.uidByte);
            digitalWrite(LED_RED, 1);
            Serial.println("Tag has been removed!");
            Serial.println(" ");
            delay(1000);
            digitalWrite(LED_RED, 0);
            Serial.println("Scan RFID card to check access");
            break;

          }

        }
      }

    } 
    else {
      digitalWrite(LED_RED, 1);
      Serial.println("Access denied");
      Serial.println(" ");

      delay(500);
      digitalWrite(LED_RED, 0);

      Serial.println("Scan RFID card to check access");
    }
    delay(2000);    // Wait for 2 seconds before next reading

  }  if else {  //detects if some is trying to leave and opens the door accordingly
    if ( (analogRead(34) * (3.3 / 4095.0)) < 1 ) {
      Serial.println((analogRead(34) * (3.3 / 4095.0)));
      // Open door
      myservo.write(0);
      delay(500);
      myservo.write(90);
      delay(1000);
      digitalWrite(LED_BLUE, 0);
      int sensorValue;
      float voltage;
      int count;

        while (1 == 1) {
        
        sensorValue = analogRead(32);
        voltage = sensorValue *(3.3 / 4095.0);
        if(voltage >= .95) {
          Serial.println("Waiting for door to shut...");
          count = 0;
        } else {
          Serial.println("Door is shut?");
          count = count + 1;
        }
        delay(1000);
        if(count >= 3) {
          Serial.println("Door is shut!");
          break;
        }
      }

            // Close door
      Serial.println("Closing door...");
      Serial.println(" ");
      delay(2000);
      myservo.write(180);
      delay(500);
      myservo.write(90);
      delay(2000);


    } 

  } else {

    if (millis() - lastActivityTime >= UPLOAD_INTERVAL) {
    // Create a sample struct with employee information
    KeyCodes employeeData;
    employeeData.firstName = "John";
    employeeData.lastName = "Doe";
    employeeData.employeeID = 12345;
    employeeData.date = "2023-12-04"; // Replace with the current date

    // Upload data to Firebase
    uploadData(employeeData);

    // Update the last activity time
    lastActivityTime = millis();
  }

  // Add your code to detect activity or perform other tasks

  delay(1000); // Adjust the delay based on your application requirements

  }
}


// Function to add a new UID to the table
void addUID(byte newUID[]) {
  for (int i = 0; i < MAX_UIDS; i++) {
    if (memcmp(uids[i], newUID, UID_LENGTH) == 0) {
      // UID already exists in table
      return;
    }
    if (memcmp(uids[i], "\0\0\0\0", UID_LENGTH) == 0) {
      // Found an empty slot in the table
      memcpy(uids[i], newUID, UID_LENGTH);
      return;
    }
  }
}

// Function to remove a UID from the table
void removeUID(byte UID[]) {
  for (int i = 0; i < MAX_UIDS; i++) {
    if (memcmp(uids[i], UID, UID_LENGTH) == 0) {
      // Found the UID in the table
      memset(uids[i], 0, UID_LENGTH);
      return;
    }
  }
}

// Function to detect if a UID is present in the table
bool isUIDPresent(byte UID[]) {
  for (int i = 0; i < MAX_UIDS; i++) {
    if (memcmp(uids[i], UID, UID_LENGTH) == 0) {
      // Found the UID in the table
      return true;
    }
  }
  return false;
}

void blinkLED(unsigned long duration, int pin) {
  unsigned long pMillis = 0;
  boolean ledState = LOW;
  pinMode(pin, OUTPUT);

  while(millis() < pMillis + duration) {
    if (millis() - pMillis >- 500) {
      pMillis = millis();
      ledState = !ledState;
      digitalWrite(pin, ledState);
    }
  }
  digitalWrite(pin, LOW);
}

// Global variable to store the last activity timestamp
unsigned long lastActivityTime = 0;

// Function to upload data to Firebase
void uploadData(const KeyCodes& data) {
  FirebaseData firebaseData;
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  // Set the database path where you want to store the data
  String databasePath = "/employeeData";
  
  // Push the data to Firebase
  Firebase.pushInt(firebaseData, databasePath + "/employeeID", data.employeeID);
  Firebase.pushString(firebaseData, databasePath + "/firstName", data.firstName);
  Firebase.pushString(firebaseData, databasePath + "/lastName", data.lastName);
  Firebase.pushString(firebaseData, databasePath + "/date", data.date);

  if (firebaseData.httpCode() == 200) {
    Serial.println("Data uploaded successfully");
  } else {
    Serial.println("Error uploading data to Firebase");
    Serial.println(firebaseData.errorReason());
  }

  Firebase.end();
}

