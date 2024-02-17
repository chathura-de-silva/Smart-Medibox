#include <Arduino.h>

#define LED 13

void setup()
{
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
  Serial.println("Inital Check  ESP32");
}

void loop()
{
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}
