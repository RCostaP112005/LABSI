/*
  arduino_pwm_adc.ino
  - Leitura ADC e conversão para 8 bits
  - Controlo de atuador por PWM (8-bit)
  - Geração de onda quadrada 1 Hz

  Hardware: Arduino Uno (ou compatível)
*/

const int analogPin = A0;   // entrada analógica
const int pwmPin = 9;      // saída PWM (0-255)
const int squarePin = 13;  // saída onda quadrada 1 Hz (LED onboard por defeito)

unsigned long previousMillis = 0;
const unsigned long interval = 500; // 500 ms -> toggle -> 1 Hz
int squareState = LOW;

void setup() {
  pinMode(pwmPin, OUTPUT);
  pinMode(squarePin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // 1) Aquisição ADC e conversão para 8 bits
  int raw = analogRead(analogPin); // 0..1023 (10-bit)
  uint8_t value8 = raw >> 2;      // escala para 0..255 (8-bit)

  // 2) Controlo do atuador por PWM
  // Use driver/transistor se for carregar indutiva ou corrente elevada
  analogWrite(pwmPin, value8); // pwmPin recebe 0..255

  // 3) Gerar onda quadrada 1 Hz (não-bloqueante)
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    squareState = !squareState;
    digitalWrite(squarePin, squareState);
  }

  // Debug via Serial
  Serial.print("ADC10:"); Serial.print(raw);
  Serial.print("  ADC8:"); Serial.print(value8);
  Serial.print("  PWM:"); Serial.println(value8);

  delay(10);
}
