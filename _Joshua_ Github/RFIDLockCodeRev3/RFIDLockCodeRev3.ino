
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

Servo myservo; 
int pos = 0;

#define LED_RED 26
#define LED_BLUE 27

#define RST_PIN         22          // Reset pin
#define SS_PIN          5          // Slave select pin
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

const int UID_LENGTH = 4;
const int MAX_UIDS = 10;
byte uids[MAX_UIDS][UID_LENGTH] = {{0}};

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
      myservo.write(180);
      delay(500);
      myservo.write(90);
      delay(2000);

      Serial.println("Scan RFID card to check access");

    //Admin Key detection
    } else if (mfrc522.uid.uidByte[0] == 0x63 && mfrc522.uid.uidByte[1] == 0xDA && mfrc522.uid.uidByte[2] == 0xF7 && mfrc522.uid.uidByte[3] == 0x34) {

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

