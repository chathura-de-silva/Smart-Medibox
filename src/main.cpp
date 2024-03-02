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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void print_time_now()
{
    display.fillRect(0, 0, 128, 16, BLACK);
    // println(String(days), 0, 0, 2);  // This code makes a flicker effect in the display from left to right since there are multiple statements to be displayed.
    // println(":", 20, 0, 2);
    // println(String(hours), 30, 0, 2);
    // println(":", 50, 0, 2);
    // println(String(minutes), 60, 0, 2);
    // println(":", 80, 0, 2);
    // println(String(seconds), 90, 0, 2);

    // Instead of above lines, the single line below is added to avoid flickering and display time as a whole in once.
    println(timeinfo, "%H:%M:%S", 0, 0, 2);
    display.fillRect(0, 16, 128, 30, BLACK);
    println(timeinfo, "%d %B %Y", 0, 22, 1);
}

void update_time()
{
    if (!getLocalTime(&timeinfo))
    {
        println("Failed to fetch time from server!", 0, 0, 2);
    }
}

void update_time_with_check_alarm()
{
    update_time();
    print_time_now();

    if (alarm_enabled == true)
    {
        for (int i = 0; i < n_alarms; i++)
        {
            // Serial.println("hours : " +  String(timeinfo.tm_hour) + " Alram hours : " + String(alarm_hours[i])); //for debugging.
            if (alarm_triggered[i] == false && alarm_hours[i] == timeinfo.tm_hour && alarm_minutes[i] == timeinfo.tm_min)
            {
                ring_alarm();
                alarm_triggered[i] = true;
            }
        }
    }
}

int wait_for_button_press()
{
    while (true)
    {
        if (digitalRead(PB_UP) == LOW)
        {
            delay(200);
            return PB_UP;
        }
        else if (digitalRead(PB_DOWN) == LOW)
        {
            delay(200);
            return PB_DOWN;
        }
        else if (digitalRead(PB_OK) == LOW)
        {
            delay(200);
            return PB_OK;
        }
        else if (digitalRead(PB_CANCEL) == LOW)
        {
            delay(200);
            return PB_CANCEL;
        }
    }
}

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
