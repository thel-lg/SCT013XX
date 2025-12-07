#include "SCT013XX.h"

SCT013XX::SCT013XX(int pinoSensor) {
  _pino = pinoSensor;
}

void SCT013XX::begin() {
  pinMode(_pino, INPUT);
  analogReadResolution(12);
}

void SCT013XX::configurar(float vRef, float adcRes, float midRail, float fator, float corte) {
  _vRef = vRef;
  _adcRes = adcRes;
  _midRail = midRail;
  _fator = fator;
  _corte = corte;
}

double SCT013XX::calcular(int amostras) {
  double soma = 0;
  
  for (int i = 0; i < amostras; i++) {
    int leitura = analogRead(_pino);
    
    // Converte para Tensão
    float tensao = (leitura * _vRef) / _adcRes;
    
    // Remove Offset DC
    float tensaoAC = tensao - _midRail;
    
    // Quadrado
    soma += (tensaoAC * tensaoAC);
  }
  
  // Média e Raiz (RMS)
  float rms = sqrt(soma / amostras);
  
  // Multiplica pelo Fator (Amperagem do sensor)
  float corrente = rms * _fator;
  
  // Filtro de Ruído
  if (corrente < _corte) corrente = 0.0;
  
  return corrente;
}