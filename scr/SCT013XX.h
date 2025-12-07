#ifndef SCT013XX_H
#define SCT013XX_H

#include <Arduino.h>

class SCT013XX {
  public:
    // Construtor
    SCT013XX(int pinoSensor);

    // Inicializa o ADC
    void begin();

    // Configura a calibração
    // fator: Coloque a amperagem do seu sensor (30, 50, 100)
    void configurar(float vRef, float adcRes, float midRail, float fator, float corte);

    // Lê a corrente RMS
    double calcular(int amostras);

  private:
    int _pino;
    float _vRef = 3.3;
    float _adcRes = 4095.0;
    float _midRail = 1.65;
    float _fator = 50.0;
    float _corte = 0.5;
};

#endif