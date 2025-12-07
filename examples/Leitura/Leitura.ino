#include <SCT013XX.h>

#define PINO_SENSOR 35 

// Instancia a biblioteca com o nome literal SCT013XX
SCT013XX sensor(PINO_SENSOR);

void setup() {
  Serial.begin(115200);
  
  sensor.begin();

  // CONFIGURAÇÃO:
  // 1. Tensão ESP: 3.3V
  // 2. Resolução: 4095
  // 3. Zero Virtual: 1.5492 (Ajuste conforme seu hardware)
  // 4. Fator: 50.0 (Se seu sensor for de 100A, coloque 100.0)
  // 5. Corte: 1.0A
  sensor.configurar(3.3, 4095.0, 1.5492, 50.0, 1.0);
}

void loop() {
  double amp = sensor.calcular(2000);
  Serial.print("Amperes: ");
  Serial.println(amp);
  delay(500);
}