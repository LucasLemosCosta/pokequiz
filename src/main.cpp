#include <Arduino.h>
#include <WiFi.h>

//! Put your password and name 
const char* nameWiFe = "******";
const char* passwordWiFe = "******";

void wifeInit();

void setup()
{
  Serial.begin(115200);
  wifeInit();

}

void loop()
{
}

void wifeInit()
{
  WiFi.begin(nameWiFe,passwordWiFe);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  
  Serial.print("\n\r Wife Connected");
  
}