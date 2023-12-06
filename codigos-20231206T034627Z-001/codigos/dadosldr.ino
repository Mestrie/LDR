#define LIGHT_SENSOR_PIN 36 // ESP32 pin GIOP36 (ADC0)


void setup() {
  // inicialize a comunicação serial em 9600 bits por segundo:
  Serial.begin(9600);
}


void loop() {
  // lê a entrada no pino analógico (valor entre 0 e 4095)
  int analogValue = analogRead(LIGHT_SENSOR_PIN);


  Serial.print("Analog Value = ");
  Serial.print(analogValue);   // a leitura analógica bruta


  // Teremos alguns limiares, determinados qualitativamente
  if (analogValue < 40) {
    Serial.println(" => Dark");
  } else if (analogValue < 800) {
    Serial.println(" => Dim");
  } else if (analogValue < 2000) {
    Serial.println(" => Light");
  } else if (analogValue < 3200) {
    Serial.println(" => Bright");
  } else {
    Serial.println(" => Very bright");
  }


  delay(500);
}

