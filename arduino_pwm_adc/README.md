# Arduino ADC 8-bit + PWM + Onda Quadrada 1 Hz

Descrição rápida:
- Leitura de um sinal analógico em `A0` (ADC 10-bit), convertido para 8-bit.
- Controlo de atuador via PWM em `D9` usando o valor 8-bit.
- Geração de onda quadrada 1 Hz em `D13` (LED onboard por defeito).

Ligações sugeridas:
- Sinal analógico -> `A0`
- Atuador (ou transistor driver) -> `D9` (usar transistor/driver para cargas >20mA)

Notas de segurança:
- Não ligar cargas diretamente ao pino do Arduino sem driver.
- Para motores/solenoides use um transistor + diodo flyback.

Upload:
1. Abrir `arduino_pwm_adc.ino` no Arduino IDE.
2. Selecionar a placa e porta corretas.
3. Fazer upload.
