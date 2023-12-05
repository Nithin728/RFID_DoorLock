/***
 * LED Blinking
 */


const int ledPin = 27;
void setup() {
  // setup pin 5 as a digital output pin
  pinMode (ledPin, OUTPUT);
}
void loop() {
  digitalWrite (ledPin, HIGH);	// turn on the LED
  Serial.println("LED ON");
  delay(500);	// wait for half a second or 500 milliseconds
  digitalWrite (ledPin, LOW);	// turn off the LED
  Serial.println("LED OFF");
  delay(500);	// wait for half a second or 500 milliseconds
}