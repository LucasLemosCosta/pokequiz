#include <Arduino.h>
#include <WiFi.h>


const char* nameWiFe = "Bento & Thor";
const char* passwordWiFe = "Lucasfilho68";

void setup()
{
  Serial.begin(115200);
  
  WiFi.begin(nameWiFe,passwordWiFe);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.print("\n\r Wife Connected");
}

void loop()
{
}
