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

### 2. Exemplo Básico (Leitura de Corrente)

```cpp
#include <SCT013XX.h>

#define PINO_SENSOR 35 

SCT013XX sensor(PINO_SENSOR);

void setup() {
  Serial.begin(115200);
  sensor.begin();

  // Configuração: (Vref, ADC_Bits, MidRail_Volts, Fator_Calibracao, Noise_Gate)
  // Use o "Sketch de Calibração" abaixo para encontrar o valor exato do MidRail
  sensor.configurar(3.3, 4095.0, 1.5492, 50.0, 0.5);
}

void loop() {
  // Lê 2000 amostras
  double amperagem = sensor.calcular(2000);
  
  Serial.print("Corrente: ");
  Serial.print(amperagem);
  Serial.println(" A");
  
  delay(500);
}
````

-----

## 🔧 Ferramenta de Calibração

Use este código para descobrir a tensão exata do seu "Zero Virtual" (MidRail) antes de configurar o código principal.

**Passo a passo:**

1.  Carregue este código no ESP32.
2.  Mantenha o sensor conectado, mas **sem passar nenhum fio/carga dentro dele**.
3.  Abra o Serial Monitor. O valor que aparecer é o seu `MidRail`.

<!-- end list -->

```cpp
// Sketch para descobrir o Zero Virtual (MidRail)
#define PINO_SENSOR 35 

void setup() {
  Serial.begin(115200);
  pinMode(PINO_SENSOR, INPUT);
}

void loop() {
  long soma = 0;
  // Tira uma média de 5000 leituras para estabilidade
  for(int i=0; i<5000; i++){
    soma += analogRead(PINO_SENSOR);
  }
  float mediaADC = soma / 5000.0;
  
  // Converte para tensão (Considerando ESP32 3.3V e 12 bits)
  float voltagemZero = mediaADC * (3.3 / 4095.0);

  Serial.print("Leitura ADC: ");
  Serial.print(mediaADC);
  Serial.print(" | MidRail (Use este valor): ");
  Serial.println(voltagemZero, 4); // 4 casas decimais para precisão
  
  delay(1000);
}
```

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

