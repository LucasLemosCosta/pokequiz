#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//! Put your password and name
const char *nameWiFe = "";
const char *passwordWiFe = "";

const String httpPokemonLink = "https://pokeapi.co/api/v2/pokemon/";
const String httpPokemonSpeciesLink = "https://pokeapi.co/api/v2/pokemon-species/";

//*Game Settigns
JsonDocument docPokemon;
JsonDocument docPokemonSpecies;

bool flagCorrectPokemon = true;
const char *guess = "";
const int16_t maxStartPoint = 100;
const int minPokemonRange = 1;
const int maxPokemonRange = 1024;

void wifeInit();
void speekWithSerial();
void treatDeserializationError(DeserializationError);

String askApiInfo(String, String);
JsonDocument pickRandomPokemon(const String,JsonDocument);

void setup()
{
  Serial.begin(115200);
  Serial.println();

  wifeInit();

  randomSeed(esp_random());
}

void loop()
{
  //* Keep pokemon info
  static unsigned int pokemonId;
  static const char *pokemonName;
  static const char *pokemonPrimaryType;
  static const char *pokemonSecondType;
  static const char *generation;
  static const char *color;
  static const char *flavorText;
  //* game controllers
  static const char *lastGuess = "";
  static int8_t attemptCounter = 0;
  static int16_t points = 0;

  speekWithSerial();

  //! Start and Change Pokemon
  if (flagCorrectPokemon)
  {
    points = 0;

    JsonDocument filter;
    filter["name"] = true;
    filter["id"] = true;
    filter["types"][0]["type"]["name"] = true;

    docPokemon = pickRandomPokemon(httpPokemonLink,filter);

    pokemonName = docPokemon["name"] | "Sem Nome";
    pokemonId = docPokemon["id"] | 0;
    pokemonPrimaryType = docPokemon["types"][0]["type"]["name"] | "No primary type";
    pokemonSecondType = docPokemon["types"][1]["type"]["name"] | "No second type";

    Serial.println("ESP32 : Pokemon Game");
    Serial.printf("Points: %d,\n\r", points);
    Serial.printf("Attempt: %d, \n\r", attemptCounter);

    points = 100;
    Serial.printf("Informations: \n\r1 - Primary type: %s ,Second type: %s \n\r ",
                  pokemonPrimaryType,
                  pokemonSecondType);

    Serial.println();
    Serial.print("try enter with a pokemon name: ");
    flagCorrectPokemon = false;
  }
  //! Try a guess
  if (strcmp(lastGuess, guess) != 0)
  {
    if (strcmp(guess, pokemonName) == 0)
    {
      flagCorrectPokemon = true;
      Serial.print("congratulations");
    }
    else
    {
      attemptCounter++;
      points -= 25;

      Serial.println();
      Serial.printf("points to earn: %d",points);
      Serial.printf("\n\r attempt: %d \n\r", attemptCounter);
      switch (attemptCounter)
      {
        case 1:
           Serial.printf("Informations: \n\r1 - Primary type: %s ,Second type: %s \n\r 2 - Pokedex Number: %d \n\r",
                  pokemonPrimaryType,
                  pokemonSecondType,
                  pokemonId
                );

        break;
        
      }
      Serial.print("try again: ");
    }

    lastGuess = "";
    guess = "";
  }
}

void wifeInit()
{
  WiFi.begin(nameWiFe, passwordWiFe);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.print("\n\r Wife Connected \n\r");
  Serial.println();
}

void speekWithSerial()
{
  static int8_t index = 0;
  static char text[100];

  while (Serial.available() > 0)
  {
    char letter = Serial.read();
    switch (letter)
    {
    case '\r':
      continue;
    case '\n':
      text[index] = '\0';
      guess = text;
      index = 0;
      break;

    default:
      text[index++] = letter;
      break;
    }

    // Serial.print(letter);
  }
}
JsonDocument pickRandomPokemon(const String httpLink,JsonDocument filter)
{
  JsonDocument doc;
  int indexPokedex = random(minPokemonRange, maxPokemonRange);
  const char *jsonText = askApiInfo(httpPokemonLink, String(indexPokedex)).c_str();

  DeserializationError error = deserializeJson(doc, jsonText, DeserializationOption::Filter(filter));
  treatDeserializationError(error);

  return doc;
}

void treatDeserializationError(DeserializationError error)
{
  switch (error.code())
  {
  case DeserializationError::Ok:
    break;
  case DeserializationError::IncompleteInput:
    Serial.println("input is inconplete");
    break;
  case DeserializationError::InvalidInput:
    Serial.println("InvalidInput");
    break;
  case DeserializationError::EmptyInput:
    Serial.println("EmptyInput");
    break;
  case DeserializationError::NoMemory:
    Serial.println("without memory");
    break;
  default:
    Serial.println("unknown error");
    break;
  }
}

String askApiInfo(const String link, String param)
{
  String jsonText;
  HTTPClient http;
  int httpCode;

  http.begin(link + param);
  httpCode = http.GET();

  if (httpCode == 200)
  {
    jsonText = http.getString();
  }

  return jsonText.c_str();
}
