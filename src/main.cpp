#include <Arduino.h>
#include <Wire.h>
#include<Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESSS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
    Serial.begin(115200);
    if (! display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESSS)){
        Serial.println(F("SSD1306 Allocation Failed Succesfully!"));
        for(;;);
    }
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println(F("Welcome to Medibox!"));
    display.display();
}

void loop()
{
    delay(10);
}
