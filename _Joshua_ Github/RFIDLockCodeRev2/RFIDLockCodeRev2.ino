
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

Servo myservo; 
int pos = 0;

#define RST_PIN         22          // Reset pin
#define SS_PIN          5          // Slave select pin
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

void setup() {
  Serial.flush();
  Serial.begin(9600);         // Initialize serial communication
  SPI.begin();                // Initialize SPI bus
  mfrc522.PCD_Init();         // Initialize MFRC522
  Serial.println("Scan RFID card to check access");

  // The servo control wire is connected to Arduino D2 pin.
  myservo.attach(33);
  // Servo is stationary.
  myservo.write(90);

  pinMode(32, INPUT);
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {    // If new card present
    Serial.print("UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {     // Print UID
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    
    if (mfrc522.uid.uidByte[0] == 0x63 && mfrc522.uid.uidByte[1] == 0xDA && mfrc522.uid.uidByte[2] == 0xF7 && mfrc522.uid.uidByte[3] == 0x34) {
      Serial.println("Access granted");
      Serial.println("Opening door");
      Serial.println(" ");
      delay(500);

      // Open door
      myservo.write(180);
      delay(1000);
      myservo.write(90);
      delay(1000);
      
      // Wait for proximity sensor the detect that the door is closed
      int sensorValue;
      float voltage = 1;
      int count = 0;

      while (1 == 1) {
        sensorValue = analogRead(32);
        voltage = sensorValue *(3.3 / 4095.0);
        if(voltage >= .23) {
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
      myservo.write(0);
      delay(1000);
      myservo.write(90);
      delay(2000);

      Serial.println("Scan RFID card to check access");

    } else {
      Serial.println("Access denied");
      Serial.println(" ");
    }
    delay(2000);    // Wait for 2 seconds before next reading
  }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






/*
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         22          // Reset pin
#define SS_PIN          5          // Slave select pin
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

byte whitelist[4][4] = { 0 };   // Initialize whitelist array to store up to 4 tags

void setup() {
  Serial.begin(9600);         // Initialize serial communication
  SPI.begin();                // Initialize SPI bus
  mfrc522.PCD_Init();         // Initialize MFRC522
  Serial.println("Scan RFID card to check access");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {    // If new card present
    Serial.print("UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {     // Print UID
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    
    if (mfrc522.uid.uidByte[0] == 0xB8 && mfrc522.uid.uidByte[1] == 0x3D && mfrc522.uid.uidByte[2] == 0xA3 && mfrc522.uid.uidByte[3] == 0x89) {
      Serial.println("Add next scanned tag to whitelist");
      byte index = 0;
      while (index < 4) {
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
          for (byte i = 0; i < mfrc522.uid.size; i++) {
            whitelist[index][i] = mfrc522.uid.uidByte[i];
            delay(4000);
          }
          Serial.print("Tag added to whitelist: ");
          for (byte i = 0; i < mfrc522.uid.size; i++) {
            Serial.print(whitelist[index][i] < 0x10 ? " 0" : " ");
            Serial.print(whitelist[index][i], HEX);
          }
          Serial.println();
          index++;
        }
      }
      Serial.println("Whitelist is full");
    } else {
      boolean isAuthorized = false;
      for (byte i = 0; i < 4; i++) {
        if (whitelist[i][0] == mfrc522.uid.uidByte[0] && whitelist[i][1] == mfrc522.uid.uidByte[1] && whitelist[i][2] == mfrc522.uid.uidByte[2] && whitelist[i][3] == mfrc522.uid.uidByte[3]) {
          isAuthorized = true;
          break;
        }
      }
      if (isAuthorized) {
        Serial.println("Access granted");
      } else {
        Serial.println("Access denied");
      }
    }
    delay(2000);    // Wait for 2 seconds before next reading
  }
}
*/