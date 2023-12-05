const int CNY70_PIN = 32;  //32 exit proxy 34 door
const int ledPin = 26;

void setup() {
  pinMode(CNY70_PIN, INPUT);
  Serial.begin(115200);
  pinMode (ledPin, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(CNY70_PIN);
  float voltage = sensorValue * (3.3 / 4095.0);  // convert analog reading to voltage
  Serial.print("Voltage: ");
  Serial.println(voltage, 2);  // print voltage with 2 decimal places

  
  if (voltage < 0.25) {
    Serial.println("CNY70 sensor voltage is below .27 volt!");
    digitalWrite (ledPin, HIGH);	// turn on the LED
    // do something when voltage is below 1 volt
  } else {
      digitalWrite (ledPin, LOW);	// turn off the LED
  }
  

  delay(1000);  // wait for 1 second before taking another reading
}