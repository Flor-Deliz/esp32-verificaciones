#include <WiFi.h>

const char* ssid = "Deliz";
const char* password = "23456789";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a WiFi!");
}

void loop() {}
