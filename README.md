# Biblioteca SCT013XX para ESP32

Esta biblioteca foi desenvolvida para resolver problemas críticos de leitura de corrente AC com sensores **SCT-013** (não invasivos) em microcontroladores **ESP32**, focando especialmente em circuitos com divisores de tensão personalizados onde o "Zero Virtual" (Offset DC) não é exato.

---

## ⚠️ O Problema (Motivação)

A maioria das bibliotecas padrão (como EmonLib) foi projetada para Arduino UNO (5V) e assume cenários ideais que não se aplicam ao ESP32:

* **Offset DC Incorreto:** Em circuitos de 3.3V, o ponto médio teórico é 1.65V. Porém, resistores reais têm tolerância, fazendo o zero real ser 1.54V, 1.58V, etc. Isso gera leituras falsas.
* **Sensores com Tensão de Saída:** Alguns modelos (ex: SCT-013-050 1V) já possuem resistor de carga interno, o que confunde bibliotecas que esperam apenas corrente.
* **Ruído:** O ADC do ESP32 não é linear, gerando leituras "fantasmas" (0.2A, 0.5A) mesmo com o motor desligado.

---

## ✅ A Solução (Funcionalidades)

Esta biblioteca implementa um algoritmo RMS (*Root Mean Square*) otimizado que permite controle total sobre a matemática da leitura:

1.  **Ajuste Fino do Zero (MidRail):** Você define exatamente qual é a tensão DC do seu circuito (ex: `1.5492V`) em vez de usar um valor fixo.
2.  **Fator de Calibração Universal:** Permite ajustar a leitura final para bater com um Alicate Amperímetro de referência.
3.  **Noise Gate (Corte):** Define um valor mínimo (ex: `1.0A`). Qualquer leitura abaixo disso é considerada ruído e zerada via software.

---

## 🚀 Como Usar

### 1. Instalação
1.  Baixe este repositório clicando em **Code > Download ZIP**.
2.  Na IDE do Arduino, vá em: **Sketch -> Incluir Biblioteca -> Adicionar biblioteca .ZIP**.
3.  Selecione o arquivo baixado.

### 2. Exemplo Básico

```cpp
#include <SCT013XX.h>

// Defina o pino analógico onde o sensor está ligado
#define PINO_SENSOR 35 

// Cria o objeto da biblioteca
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
````

-----

## 🛠️ Guia de Calibração

Para obter precisão máxima, siga estes passos com um multímetro e um alicate amperímetro:

### Passo 1: Ajuste do Zero (MidRail)

  * Ligue o ESP32 **sem carga** no sensor.
  * Meça com um multímetro a tensão DC no pino de entrada do ESP32.
  * Coloque esse valor exato no **3º parâmetro** do `configurar` (ex: `1.5492`).

### Passo 2: Ajuste do Fator

  * Ligue uma carga constante (ex: um motor ou secador).
  * Meça a corrente real com um **Alicate Amperímetro**.
  * Compare com o valor do Serial Monitor.
      * Se o Serial mostrar **menos**, **AUMENTE** o Fator (4º parâmetro).
      * Se o Serial mostrar **mais**, **DIMINUA** o Fator.

### Passo 3: Ajuste do Corte (Noise Gate)

  * Desligue a carga.
  * Se o monitor mostrar valores como `0.12A` ou `0.30A` (ruído), defina o **5º parâmetro** para um valor logo acima (ex: `0.5` ou `1.0`).

-----

## 📋 Compatibilidade

| Placa | Tensão (Vref) | Resolução (ADC) | Nota |
| :--- | :---: | :---: | :--- |
| **ESP32** | 3.3V | 4095 | ✅ Recomendado |
| Arduino Uno | 5.0V | 1023 | Funciona (ajustar parâmetros) |
| Arduino Mega | 5.0V | 1023 | Funciona (ajustar parâmetros) |

### Sensores Suportados:

Esta biblioteca suporta qualquer sensor da família SCT-013. Abaixo os valores típicos de calibração inicial:

  * **SCT-013-000** (100A/50mA) -\> *Requer resistor de carga externo (Burden)*
  * **SCT-013-005** (5A/1V)
  * **SCT-013-010** (10A/1V)
  * **SCT-013-020** (20A/1V)
  * **SCT-013-030** (30A/1V)
  * **SCT-013-050** (50A/1V)
  * **SCT-013-060** (60A/1V)
  * **SCT-013-100** (100A/1V)

-----

## 🎓 Autoria

> Desenvolvido por **Lucas Santos Gama** como parte do Trabalho de Conclusão de Curso (TCC) em Eletroeletrônica - **SENAI Mariano Ferraz** (2025).

```
```
