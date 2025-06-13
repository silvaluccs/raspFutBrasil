# raspFutBrasil

O **raspFutBrasil** é um sistema embarcado desenvolvido com **RP2040 (Raspberry Pi Pico)** e **Spring Boot** que permite monitorar partidas em tempo real do Campeonato Brasileiro - Séries A e B. O sistema foi idealizado para ser simples, portátil e eficiente, podendo ser utilizado em painéis físicos, displays ou aplicações locais que exibem dados atualizados dos jogos.

---

## 🚀 Tecnologias Utilizadas

### Backend
- **Java 17**
- **Spring Boot**
- **Maven**
- **REST API**
- **Docker**
- **Mosquitto**

### Dispositivo Embarcado
- **RP2040 (Raspberry Pi Pico)**
- Comunicação via **MQTT** com o backend
- Integração com display, LEDs 

---

## 📡 Funcionalidades

- 🕒 Consulta de partidas em tempo real da Série A e B do Brasileirão
- ⚽ Exibição de dados como: times, placar, tempo de jogo e status da partida
- 🔧 API REST para consulta e integração com outros sistemas

---

## 📁 Estrutura do Projeto
raspFutBrasil-main/
│
├── backend/ # Projeto Spring Boot (Java)
│ ├── src/main/java/ # Código-fonte da API
│ ├── pom.xml # Dependências Maven
│ └── Dockerfile # Containerização
│
├── docker-compose.yml # Orquestração dos containers
└── README.md # Este arquivo
