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
    display.drawBitmap(0, 0, splashScreen, 128, 64, WHITE); // This is not a modalpage. It is a splash screen. text is embedded to a bitmap.
    display.display();
    delay(1200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(250);
        show_modal_page(wifi,0,"Waiting For Wifi", 18);
    }
    show_modal_page(tick,100,"Wifi Connected!", 20);
    configTime(DEFAULT_UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
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
