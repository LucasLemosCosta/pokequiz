#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//! Put your password and name
const char *nameWiFe = "";
const char *passwordWiFe = "";

const String httpPokemonLink = "https://pokeapi.co/api/v2/pokemon/";

JsonDocument doc;
bool flagCorrectPokemon = true;
const char *guess = "";

void wifeInit();
String askApiInfo(String link, String param);
void treatDeserializationError(DeserializationError error);
JsonDocument pickRandomPokemon(int min, int max);
void speekWithSerial();

void setup()
{
  Serial.begin(115200);
  Serial.println();

  wifeInit();

  randomSeed(esp_random());
}

void loop()
{
  static unsigned int pokemonId;

  static const char *pokemonName;
  static const char *pokemonPrimaryType;
  static const char *pokemonSecondType;
  static const char *lastGuess = "";

  speekWithSerial();
  if (strcmp(lastGuess, guess) != 0)
  {
    if (strcmp(guess, pokemonName) == 0)
    {
      flagCorrectPokemon = true;
    }
    Serial.println(strcmp(guess, pokemonName) == 0 ? "congratulations" : "miss the shot");
    Serial.printf("\n\r name : %s \n\r", pokemonName);
    Serial.printf("\n\r guess: %s \n\r", guess);

    lastGuess = "";
    guess = "";
  }

  if (flagCorrectPokemon)
  {
    doc = pickRandomPokemon(0, 1024);
    pokemonName = doc["name"] | "Sem Nome";
    pokemonId = doc["id"] | 0;
    pokemonPrimaryType = doc["types"][0]["type"]["name"] | "No primary type";
    pokemonSecondType = doc["types"][1]["type"]["name"] | "No second type";

    Serial.printf("\n\r Name pokemon: %s, Pokedex number: %u\n\r Primary type: %s ,Second type: %s \n\r ",
                  pokemonName,
                  pokemonId,
                  pokemonPrimaryType,
                  pokemonSecondType);

    flagCorrectPokemon = false;
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
      Serial.printf("\n\r guess serial: %s \n\r", guess);
      Serial.printf("\n\r text: %s \n\r", text);

      break;

    default:
      text[index++] = letter;
      break;
    }

    // Serial.print(letter);
  }
}
JsonDocument pickRandomPokemon(int min, int max)
{
  JsonDocument doc;
  int indexPokedex = random(min, max);
  const char *jsonText = askApiInfo(httpPokemonLink, String(indexPokedex)).c_str();

  JsonDocument filter;
  filter["name"] = true;
  filter["id"] = true;
  filter["types"][0]["type"]["name"] = true;

  DeserializationError error = deserializeJson(doc, jsonText, DeserializationOption::Filter(filter));
  treatDeserializationError(error);

  return doc;
}

void treatDeserializationError(DeserializationError error)
{
  switch (error.code())
  {
  case DeserializationError::Ok:
    Serial.println("without error");
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
