const int Analogpin = 34;
int ADCValue = 0;
int voltageValue = 0;

void setup() {
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

  ADCValue = analogRead(Analogpin);
  Serial.println("ADC Value = ");
  Serial.println(ADCValue);
  delay(1000);

  voltageValue = (ADCValue * 3.3) / (4095);
  Serial.println("Voltage = ");
  Serial.print(voltageValue);
  Serial.println("volts");
  delay(1000);

}
