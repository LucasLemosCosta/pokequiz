#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//! Put your password and name
const char *nameWiFe = "";
const char *passwordWiFe = "";

const String httpLink = "https://pokeapi.co/api/v2/pokemon/";

void wifeInit();
String askApiInfo(String link,String param);

void setup()
{
  Serial.begin(115200);
  wifeInit();

  JsonDocument doc;
  const char* info = askApiInfo(httpLink,"1").c_str();
  DeserializationError error = deserializeJson(doc,info);
  char buffer[1000];
  serializeJsonPretty(doc,buffer);
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

  Serial.print("\n\r Wife Connected");
}

String askApiInfo(const String link,String param)
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