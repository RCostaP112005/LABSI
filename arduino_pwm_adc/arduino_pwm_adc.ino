/*
  arduino_pwm_adc.ino
  - Leitura ADC e conversão para 8 bits
  - Controlo de atuador por PWM (8-bit)
  - Geração de onda quadrada 1 Hz

  Implementação sem usar funções da biblioteca Arduino.
  Usa acesso direto aos registos AVR do ATmega328P.

  Hardware: Arduino Uno (ou compatível)
*/

#include <avr/io.h>
#include <avr/interrupt.h>

const uint8_t analogChannel = 0; // ADC0 -> A0
const uint8_t pwmPin = PB1;      // D9 = PB1, saída PWM do Timer1
const uint8_t squarePin = PB5;   // D13 = PB5, LED onboard

volatile uint16_t timer0Ticks = 0;

void adc_init(void) {
  // ADC habilitado, prescaler 128 para 125 kHz em 16 MHz
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  ADMUX = _BV(REFS0); // referência AVcc, canal ADC0
}

uint16_t adc_read_10bit(uint8_t channel) {
  ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
  ADCSRA |= _BV(ADSC);

  while (ADCSRA & _BV(ADSC)) {
    // espera até a conversão terminar
  }

  return ADC;
}

void pwm_init(void) {
  DDRB |= _BV(pwmPin); // saída PWM em PB1 (D9)

  // Timer1 em Fast PWM 8-bit, canal A, não invertido
  TCCR1A = _BV(COM1A1) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(CS10); // sem prescaler
  OCR1A = 0;
}

void timer0_init_1ms(void) {
  // Timer0 em CTC com prescaler 64 -> tick de ~1 ms em 16 MHz
  TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS01) | _BV(CS00);
  OCR0A = 249;
  TIMSK0 |= _BV(OCIE0A);
}

ISR(TIMER0_COMPA_vect) {
  timer0Ticks++;

  // Toggle de 1 Hz: 500 ms em HIGH + 500 ms em LOW
  if ((timer0Ticks % 500) == 0) {
    PORTB ^= _BV(squarePin);
  }
}

void setup(void) {
  DDRB |= _BV(pwmPin) | _BV(squarePin);
  PORTB &= ~_BV(pwmPin);
  PORTB &= ~_BV(squarePin);

  adc_init();
  pwm_init();
  timer0_init_1ms();

  sei();
}

void loop(void) {
  uint16_t raw = adc_read_10bit(analogChannel);
  uint8_t value8 = (uint8_t)(raw >> 2); // 0..1023 -> 0..255

  OCR1A = value8;
}
