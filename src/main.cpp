#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>

// Header files written explicitly for the project.
#include <Constants.h>
#include <Globals.h>
#include <Functions.h>

void setup()
{
    pinMode(BUZZER, OUTPUT);
    pinMode(LED_1, OUTPUT);
    pinMode(PB_CANCEL, INPUT);
    pinMode(PB_DOWN, INPUT);
    pinMode(PB_OK, INPUT);
    pinMode(PB_UP, INPUT);

    dhtSensor.begin();

    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESSS))
    {
        Serial.println(F("SSD1306 Allocation Failed Succesfully!"));
        for (;;)
            ;
    }
    display.clearDisplay();
    display.drawBitmap(0, 0, splashScreen, 128, 64, WHITE);
    display.display();
    delay(1200);
    WiFi.begin("MSI 8690", "abcdefgh", 6);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(250);
        display.clearDisplay();
        display.drawBitmap(0, 0, wifi, 128, 64, WHITE);
        println("Waiting For Wifi", 18, 50, 1,true);
    }
    display.clearDisplay();
    display.drawBitmap(0, 0, tick, 128, 64, WHITE);
    println("Wifi Connected!", 20, 50, 1,true);
    delay(300);
    configTime(utc_offset, UTC_OFFSET_DST, NTP_SERVER);
    display.clearDisplay();
}

void loop()
{
    update_time_with_check_alarm();
    if (digitalRead(PB_OK) == LOW)
    {
        delay(200);
        go_to_menu();
    }
    check_temp();
}
