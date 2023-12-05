#include <ESP32Servo.h>

Servo myservo;  // create servo object to control a servo

void setup() {
  Serial.begin(115200);  // initialize serial communication at 9600 bits per second
  while (!Serial);     // wait for serial monitor to open

  myservo.attach(33);   // The servo control wire is connected to Arduino D2 pin.
  myservo.write(90);   // Servo is stationary.
}

void loop() {
  if (Serial.available() > 0) {
    int input = Serial.read();  // read the input from the serial monitor

    if (input == '1') {         // Servo spins forward at full speed for 1 second.
      Serial.println("moving forward");
      myservo.write(180);
      delay(700);
    }
    else if (input == '2') {    // Servo spins in reverse at full speed for 1 second.
      Serial.println("moving backward");
      myservo.write(0);
      delay(700);
    }
    else if (input == '3') {    // Servo spins in reverse at full speed for 1 second.
      Serial.println("moving forward");
      myservo.write(180);
      delay(1000);
    }
    else if (input == '4') {    // Servo spins in reverse at full speed for 1 second.
      Serial.println("moving backward");
      myservo.write(0);
      delay(1000);
    }

    myservo.write(90);          // Servo is stationary for 1 second.
    delay(500);
  }
}
