# Pokémon Quiz with ESP32

Um jogo de adivinhação de Pokémon desenvolvido em C++ para ESP32. O projeto utiliza a PokéAPI para obter informações dos Pokémon e apresenta dicas progressivas ao jogador por meio do Monitor Serial.

## Sobre o projeto

O Pokémon Quiz é um projeto de IoT que integra programação embarcada, conexão Wi-Fi e consumo de uma API REST.

A cada rodada, o sistema seleciona um Pokémon aleatoriamente e consulta a PokéAPI para obter informações utilizadas durante o jogo. O objetivo é descobrir qual Pokémon foi selecionado utilizando as dicas apresentadas.

## Funcionalidades

- Seleção aleatória de Pokémon.
- Consulta de informações por meio da PokéAPI.
- Exibição de dicas progressivas:
  - Tipo do Pokémon.
  - Número na Pokédex.
  - Cor.
  - Geração.
- Recebimento de respostas pelo Monitor Serial.
- Sistema de pontuação.
- Contagem de tentativas.
- Encerramento da rodada após acertar ou esgotar os pontos disponíveis.

## Tecnologias utilizadas

| Tecnologia | Aplicação |
|---|---|
| C++ | Linguagem de programação |
| Arduino Framework | Desenvolvimento para ESP32 |
| ESP32-S3 | Plataforma embarcada |
| PlatformIO | Gerenciamento e compilação do projeto |
| ArduinoJson 7 | Processamento das respostas JSON |
| Wi-Fi | Conexão com a internet |
| HTTPClient | Requisições HTTP |
| PokéAPI | Fonte de dados dos Pokémon |

## Hardware e requisitos

- Placa ESP32-S3 compatível com a configuração do projeto.
- Computador com Visual Studio Code.
- Extensão PlatformIO IDE.
- Rede Wi-Fi com acesso à internet.

## Como executar

### 1. Clonar o projeto

```bash
git clone https://github.com/LucasLemosCosta/pokequiz.git
```

Entre na pasta:

```bash
cd pokequiz
```

### 2. Configurar a conexão Wi-Fi

No arquivo `src/main.cpp`, localize as configurações de rede:

```cpp
const char *nameWiFe = "SEU_WIFI";
const char *passwordWiFe = "SUA_SENHA";
```

Substitua os valores pelas credenciais da sua rede.

**Importante:** não publique suas credenciais no GitHub. Para uma versão pública, o ideal é separar essas configurações em um arquivo local excluído pelo `.gitignore`.

### 3. Compilar o projeto

Com o PlatformIO instalado, execute:

```bash
pio run
```

### 4. Enviar para a ESP32

Conecte a placa ao computador e execute:

```bash
pio run -t upload
```

### 5. Abrir o Monitor Serial

```bash
pio device monitor
```

A comunicação serial está configurada para **115200 baud**.

## Como jogar

1. Ligue a ESP32 e aguarde a conexão Wi-Fi.
2. O jogo selecionará um Pokémon aleatoriamente.
3. Leia a primeira dica apresentada no Monitor Serial.
4. Digite seu palpite e pressione Enter.
5. Caso erre, novas dicas serão disponibilizadas.
6. Tente descobrir o Pokémon antes que seus pontos acabem.

O jogador começa cada rodada com 100 pontos. As respostas incorretas reduzem a pontuação disponível.

## Estrutura do projeto

```text
pokequiz/
├── include/
├── lib/
├── src/
│   └── main.cpp
├── test/
├── platformio.ini
└── .gitignore
```

## API utilizada

O projeto utiliza a [PokéAPI](https://pokeapi.co/) para consultar informações dos Pokémon.

São utilizados dois endpoints principais:

- `/api/v2/pokemon/`: informações gerais do Pokémon.
- `/api/v2/pokemon-species/`: informações da espécie, como cor e geração.

As respostas são recebidas em JSON e processadas com a biblioteca ArduinoJson.

## Status do projeto

Concluído.

## Autor

**Lucas Lemos Costa**

- [GitHub](https://github.com/LucasLemosCosta)
