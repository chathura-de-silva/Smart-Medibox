#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESSS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long timeNow = 0;
unsigned long timeLast = 0;

bool alarm_enabled = true;
int n_alarms = 2;
int alarm_hours[] = {0, 1};
int alarm_minutes[] = {1, 10};
bool alarm_triggered[] = {false, false};

void println(String text, int column, int row, int text_size)
{
    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(column, row);
    display.println(text);
    display.display();
}

void print_time_now()
{
    display.clearDisplay();
    println(String(days), 0, 0, 2);
    println(":", 20, 0, 2);
    println(String(hours), 30, 0, 2);
    println(":", 50, 0, 2);
    println(String(minutes), 60, 0, 2);
    println(":", 80, 0, 2);
    println(String(seconds), 90, 0, 2);
}

void update_time()
{
    timeNow = millis() / 1000; // seconds passed after bootup.
    seconds = timeNow - timeLast;
    if (seconds >= 60)
    {
        minutes++;
        timeLast += 60;
    }
    if (minutes == 60)
    {
        hours++;
        minutes = 0;
    }
    if (hours == 24)
    {
        days++;
        hours = 0;
    }
}

void update_time_with_check_alarm()
{
    update_time();
    print_time_now();
}

void setup()
{
    Serial.begin(9600);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESSS))
    {
        Serial.println(F("SSD1306 Allocation Failed Succesfully!"));
        for (;;)
            ;
    }
    display.display();
    delay(2000);
    display.clearDisplay();
    println("Welcome to Medibox!", 10, 20, 2);
    display.clearDisplay();
}

void loop()
{
    update_time_with_check_alarm();
}
