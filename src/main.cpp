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
    display.display();
    delay(500);
    WiFi.begin("MSI 8690", "abcdefgh", 6);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(250);
        display.clearDisplay();
        println("Connecting to WIFI", 0, 0, 2);
    }
    display.clearDisplay();
    println("Connected to WIFI", 0, 0, 2);
    delay(1000);

    configTime(utc_offset, UTC_OFFSET_DST, NTP_SERVER);
    display.clearDisplay();
    println("Welcome to Medibox!", 10, 20, 2);
    delay(1000); // added additional delay to display the above message.
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
