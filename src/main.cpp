#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//! Put your password and name
const char *nameWiFe = "";
const char *passwordWiFe = "";

const String httpPokemonLink = "https://pokeapi.co/api/v2/pokemon/";

void wifeInit();
String askApiInfo(String link, String param);
void treatDeserializationError(DeserializationError error);
JsonDocument pickRandomPokemon(int min, int max);

void setup()
{
  Serial.begin(115200);
  Serial.println();

  wifeInit();
  randomSeed(esp_random());

  JsonDocument doc;
  doc = pickRandomPokemon(0,1024);

  char buffer[1000];
  serializeJsonPretty(doc, buffer);
  Serial.print(buffer);
}

void loop()
{
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
