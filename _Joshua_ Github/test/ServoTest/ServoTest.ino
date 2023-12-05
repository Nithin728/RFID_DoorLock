/**
 * This example demonstrates how to control a continuous rotation servo motor.
 * In this example, the servo spins forward for 1 second, pauses for 1 second,
 * spins backwards for 1 second, and pauses for 1 second in a loop.
 * 
 * This example was written by Cirkit Design LLC.
 */

#include <ESP32Servo.h>

Servo myservo; 

int pos = 0;

void setup() {
  // The servo control wire is connected to Arduino D2 pin.
  myservo.attach(33);
  // Servo is stationary.
  myservo.write(90);

  Serial.begin(115200);
}

void loop() {
  // Servo spins forward at full speed for 1 second.
  myservo.write(180);
  Serial.println("Moving forward");
  delay(250);
  // Servo is stationary for 1 second.
  myservo.write(90);
  delay(250);
  // Servo spins in reverse at full speed for 1 second.
  myservo.write(0);
  Serial.println("Moving backward");
  delay(250);
  // Servo is stationary for 1 second.
  myservo.write(90);
  delay(250);
}