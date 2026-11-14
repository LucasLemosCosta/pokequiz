#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//! Put your password and name
const char *nameWiFe = "";
const char *passwordWiFe = "";

//* API Link
const String httpPokemonLink = "https://pokeapi.co/api/v2/pokemon/";
const String httpPokemonSpeciesLink = "https://pokeapi.co/api/v2/pokemon-species/";

//* Game Settings
const int16_t maxStartPoint = 100;
const int minPokemonRange = 1;
const int maxPokemonRange = 100;

// TODO choose a better name for the comment
//* Game ...
int currentPoints = 0;
const char *guess = "";

//* Keep pokemon info
JsonDocument docPokemon;
JsonDocument docPokemonSpecies;
const char *pokemonName;
unsigned int pokemonId = 0;
const char *pokemonPrimaryType;
const char *pokemonSecondType;
const char *pokemonGeneration;
const char *pokemonColor;

void wifeInit();

void speekWithSerial();
void treatDeserializationError(DeserializationError);

String askApiInfo(String, String);
JsonDocument pickRandomPokemon(const String, JsonDocument, int);
void tipMessage(int counter);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  wifeInit();
  randomSeed(esp_random());
}

void loop()
{
  //* game controllers
  static const char *lastGuess = "";
  static int8_t attemptCounter = 0;
  static int16_t pointsToEarn = 0;
  bool flagCorrectPokemon;

  speekWithSerial();

  //! Start and Change Pokemon
  if (flagCorrectPokemon || pokemonId == 0)
  {

    pointsToEarn = maxStartPoint;

    JsonDocument filterBattleInfo;
    filterBattleInfo["name"] = true;
    filterBattleInfo["id"] = true;
    filterBattleInfo["types"][0]["type"]["name"] = true;
    JsonDocument filterPokedexInfo;
    filterPokedexInfo["color"]["name"] = true;
    filterPokedexInfo["generation"]["name"] = true;

    int indexPokemon = random(minPokemonRange, maxPokemonRange);
    docPokemon = pickRandomPokemon(httpPokemonLink, filterBattleInfo, indexPokemon);
    docPokemonSpecies = pickRandomPokemon(httpPokemonSpeciesLink, filterPokedexInfo, indexPokemon);

    pokemonName = docPokemon["name"] | "Sem Nome";
    pokemonId = docPokemon["id"] | 0;
    pokemonPrimaryType = docPokemon["types"][0]["type"]["name"] | "No primary type";
    pokemonSecondType = docPokemon["types"][1]["type"]["name"] | "No second type";
    pokemonColor = docPokemonSpecies["color"]["name"] | "No color";
    pokemonGeneration = docPokemonSpecies["generation"]["name"] | "No Generetion";

    Serial.println("ESP32 : Pokemon Game");
    Serial.printf(pointsToEarn != 0 ? "Points: %d,\n\r" : 0, pointsToEarn);
    Serial.printf("Attempt: %d, \n\r", attemptCounter);
    Serial.println("Tips: ...");
    tipMessage(attemptCounter);

    Serial.println();
    Serial.print("try enter with a pokemon name: ");

    flagCorrectPokemon = false;
  }

  //! Try a guess
  bool onTryGuess = strcmp(lastGuess, guess) != 0;
  if (onTryGuess)
  {
    bool isCorrect = strcmp(guess, pokemonName) == 0;
    if (isCorrect)
    {
      flagCorrectPokemon = true;
      pokemonId = 0;
      Serial.print("congratulations");
    }
    else
    {
      int lessPoint = maxPokemonRange / 4;
      attemptCounter++;
      pointsToEarn -= lessPoint;

      Serial.println();
      Serial.printf("points to earn: %d \n\r", pointsToEarn);
      Serial.printf("attempt: %d \n\r", attemptCounter);
      Serial.println("Tips: ...");
      tipMessage(attemptCounter);
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
JsonDocument pickRandomPokemon(const String httpLink, JsonDocument filter, int randomIndex)
{
  JsonDocument doc;
  int indexPokedex = randomIndex;
  const char *jsonText = askApiInfo(httpLink, String(indexPokedex)).c_str();

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
void tipMessage(int counter)
{

  Serial.printf(counter >= 0 ? "1- first type: %s, second type: %s \n\r" : "1 - ...\n\r", pokemonPrimaryType, pokemonSecondType);
  Serial.printf(counter >= 1 ? "2 - pokedex number: %d\n\r" : "2 - ...\n\r", pokemonId);
  Serial.printf(counter >= 2 ? "2 - pokedex color: %s\n\r" : "3 - ...\n\r", pokemonColor);
  Serial.printf(counter >= 3 ? "2 - pokedex generetion: %s\n\r" : "4 - ...\n\r", pokemonGeneration);
}