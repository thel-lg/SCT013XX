Biblioteca SCT013XX para ESP32

Uma biblioteca Arduino leve e eficiente projetada especificamente para ler sensores de corrente não-invasivos SCT-013 (qualquer amperagem) utilizando microcontroladores ESP32.

Desenvolvida para corrigir problemas comuns de leitura RMS e Offset DC em circuitos personalizados com divisores de tensão.

⚠️ O Problema (Por que usar esta biblioteca?)

A maioria das bibliotecas padrão (como EmonLib) foi projetada para Arduino UNO (5V) e assume que você está usando um sensor de saída de corrente com um resistor de carga externo calculado para aquele cenário.

Ao utilizar sensores SCT-013 com saída de tensão embutida (ex: 1V) ou ao montar circuitos para ESP32 (3.3V), surgem problemas graves:

Offset DC Incorreto: O "zero" da onda nem sempre é 1.65V (metade de 3.3V). Resistores reais têm tolerância, deslocando o ponto médio e gerando leituras falsas.

Imprecisão do ADC: O ADC do ESP32 não é linear nas pontas.

Ruído: Leituras "fantasmas" de 0.2A ou 0.5A aparecem mesmo com o motor desligado.

✅ A Solução

Esta biblioteca implementa um algoritmo RMS (Root Mean Square) otimizado que permite:

Ajuste Fino do Zero (MidRail): Você define exatamente qual é a tensão DC do seu circuito (ex: 1.5492V) em vez de usar um valor teórico.

Fator de Calibração Universal: Permite ajustar a leitura final para bater com um Alicate Amperímetro de referência.

Noise Gate (Corte): Define um valor mínimo (ex: 1.0A). Qualquer leitura abaixo disso é considerada ruído e zerada via software.

🚀 Como Usar

1. Instalação

Baixe este repositório como arquivo .ZIP (Botão verde "Code" -> "Download ZIP").

Na IDE do Arduino, vá em: Sketch -> Incluir Biblioteca -> Adicionar biblioteca .ZIP.

Selecione o arquivo baixado.

2. Exemplo Básico

#include <SCT013XX.h>

// Defina o pino analógico onde o sensor está ligado
#define PINO_SENSOR 35 

// Cria o objeto
SCT013XX sensor(PINO_SENSOR);

void setup() {
  Serial.begin(115200);
  
  sensor.begin();

  // --- CONFIGURAÇÃO DE CALIBRAÇÃO (O Pulo do Gato) ---
  // Parâmetros: (Vref, Resolução ADC, Zero Virtual, Fator, Corte Ruído)
  
  // Exemplo para ESP32 (3.3V) e sensor de 50A:
  // Ajuste o FATOR (50.0) e o ZERO (1.5492) conforme seu hardware!
  sensor.configurar(3.3, 4095.0, 1.5492, 50.0, 1.0);
}

void loop() {
  // Lê 2000 amostras para calcular a média RMS
  double amperagem = sensor.calcular(2000);
  
  Serial.print("Corrente: ");
  Serial.print(amperagem);
  Serial.println(" A");
  
  delay(500);
}


🛠️ Guia de Calibração

Para obter precisão máxima, siga estes passos com um multímetro e um alicate amperímetro:

Ajuste do Zero (MidRail):

Ligue o ESP32.

Não ligue nenhuma carga no sensor (ou desconecte o sensor do pino).

Meça com um multímetro a tensão DC no pino de entrada do ESP32 (onde o sensor liga).

Coloque esse valor exato no 3º parâmetro do configurar (ex: 1.5492).

Ajuste do Fator:

Ligue uma carga constante (ex: um motor, secador de cabelo ou aquecedor).

Meça a corrente real com um Alicate Amperímetro.

Compare com o valor do Serial Monitor.

Se o Serial mostrar menos, AUMENTE o Fator (4º parâmetro).

Se o Serial mostrar mais, DIMINUA o Fator.

Dica: Para sensores de 30A, comece com 30.0. Para 50A, comece com 50.0.

Ajuste do Corte (Noise Gate):

Desligue a carga.

Se o monitor mostrar valores como 0.12A ou 0.30A (ruído), defina o 5º parâmetro para um valor logo acima (ex: 0.5 ou 1.0).

📋 Compatibilidade

Placas: Otimizado para ESP32 (qualquer modelo com ADC de 12 bits). Funciona em Arduino Uno/Mega ajustando Vref para 5.0 e ADC para 1023.

Sensores: SCT-013-030, SCT-013-050, SCT-013-100 (versões com saída de tensão 1V ou corrente com resistor de carga).

Autoria

Desenvolvido por Lucas Santos Gama como parte do Trabalho de Conclusão de Curso (TCC) em Eletroeletrônica - SENAI Mariano Ferraz (2025).
