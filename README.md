Este projeto é um carrinho autônomo desenvolvido inicialmente para entreter minha gata (que, ironicamente, não se interessou muito 😅). Apesar disso, o projeto se tornou uma ótima oportunidade para integrar sensores, controle de movimento, gerenciamento de energia e modelagem 3D em um sistema embarcado completo.

---

##  Visão Geral

O carrinho utiliza um ESP32 que permanece em deep sleep até que o sensor PIR (HC-SR501) detecte movimento. Quando acorda, ele realiza uma varredura usando um VL53L0X (sensor laser Time-of-Flight) montado em um servo SG90.

O sensor mede as distâncias para a esquerda, centro e direita, permitindo que o carrinho escolha a direção mais livre de obstáculos.

Optei pelo sensor laser em vez do ultrassônico para evitar possíveis desconfortos auditivos à minha gata, já que gatos são muito sensíveis a sons de alta frequência.

---

##  Funcionalidades Principais

- Detecção de movimento via PIR para acordar o ESP32  
- Varredura angular usando VL53L0X + servo SG90  
- Leitura de distâncias: esquerda, centro e direita  
- Lógica de desvio baseada no menor valor detectado  
- Controle de dois motores DC com ponte H L298N  
- Operação com uma única bateria Li-ion 3.7V, carregada com TP4056 e elevada com MT3608  
- Chassi projetado no Fusion360 e impresso em 3D  

---

## Código

Todo o firmware foi escrito em C++, incluindo:

- controle do servo  
- leitura do VL53L0X  
- normalização e processamento dos dados  
- tomada de decisão de trajetória  
- rotinas de deep sleep para economia de energia  

## Desafios e Decisões de Projeto

- Operar todo o sistema de forma estável com apenas uma célula Li-ion, evitando picos de corrente que poderiam acionar o BMS  
- Balancear consumo, autonomia e desempenho dos módulos (motores, sensores e ESP32)  
- Integrar eletrônica e mecânica usando módulos separados de forma compacta  

---

## Próximos Passos

- Criar uma PCB customizada para substituir os módulos e organizar o hardware  
- Implementar uma lógica de power-up sequencial para reduzir picos de corrente  
- Adicionar comportamentos mais avançados e padrões de movimento  

---

## Componentes Utilizados

- ESP32  
- VL53L0X (sensor laser ToF)  
- PIR HC-SR501  
- Servo SG90  
- Ponte H L298N  
- Motores DC com caixa de redução  
- Bateria Li-ion 3,7V (1000 mAh)  
- TP4056 (carregamento)
- MT3608 (conversor boost)
- Chave liga/desliga  
- Chassi impresso em 3D  


## Pinagem
Pino 13: Servo motor

Pino 27: IN1

Pino 26: IN2

Pino 25: IN3

Pino 33: IN4

Pino 14: ENA

Pino 12: ENB

Pino 4: PIR
