#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <Servo.h>

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
    pinMode(LDR_1, INPUT);
    pinMode(LDR_2, INPUT);

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
    load_user_settings(); // loads data using preferences object. defaults assigned if not stored in preferences.
    delay(1000);
    WiFi.begin(wifi_username, wifi_password, 6);

    unsigned long currentMillis = millis();
    unsigned long previousMillis = currentMillis;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(250);
        show_modal_page(wifi, 0, "Waiting For Wifi", 18);
        currentMillis = millis();
        if (currentMillis - previousMillis >= WIFI_CONNECTION_LISTENING_TIMEOUT) // IF the limit gets exceeded, then the ESP32 will restart after getting wifi reconfigured.
        {
            // Serial.println("Failed to connect.");
            show_modal_page(wifi_fail, 1000, "Wifi Failure!", 26);
            config_wifi(); // This function is defined in webServer.cpp
            ESP.restart();
        }
    }
    show_modal_page(tick, 100, "Wifi Connected!", 20);

    setupMQTT();
    brokerConnectMQTT();

    configTime(temp_offset_hours * 3600 + temp_offset_minutes * 60, UTC_OFFSET_DST, NTP_SERVER);
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
    float temperature = check_temp();
    send_mqtt_data(temperature);
}
