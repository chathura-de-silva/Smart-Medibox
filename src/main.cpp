#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESSS 0x3C

#define BUZZER 5
#define LED_1 15
#define PB_CANCEL 34
#define PB_OK 32
#define PB_UP 33
#define PB_DOWN 35

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

int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int notes[] = {C, D, E, F, G, A, B, C_H};

int current_mode = 0;
int max_modes = 4;
String modes[] = {"1 - Set Time", "2 - Set Alarm 1", "3 - Set Alarm 2", "4 - Disable Alarms"};

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

void ring_alarm()
{
    display.clearDisplay();
    println("MEDICINE TIME!", 0, 0, 2);
    // Turning the LED ON
    digitalWrite(LED_1, HIGH);
    // Ringing the buzzer

    bool break_happened = false;

    while (digitalRead(PB_CANCEL) == HIGH && break_happened == false)
    {
        for (int i = 0; i < n_notes; i++)
        {
            if (digitalRead(PB_CANCEL) == LOW)
            {
                delay(200); // to prevent bouncing of buzzer.
                break_happened = true;
                break;
            }
            tone(BUZZER, notes[i]);
            delay(500);
            noTone(BUZZER);
            delay(2);
        }
    }
    digitalWrite(LED_1, LOW);
    display.clearDisplay();
}

void update_time_with_check_alarm()
{
    update_time();
    print_time_now();

    if (alarm_enabled == true)
    {
        for (int i = 0; i < n_alarms; i++)
        {
            if (alarm_triggered[i] == false && alarm_hours[i] == hours && alarm_minutes[i] == minutes)
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
        update_time();
    }
}

void go_to_menu()
{
    while (digitalRead(PB_CANCEL) == HIGH)
    {
        display.clearDisplay();
        println(modes[current_mode], 0, 0, 2);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(200);
            current_mode++;
            current_mode = current_mode % max_modes;
        }
        else if (pressed == PB_DOWN){
            delay(200);
            current_mode--;
            current_mode = current_mode % max_modes;
            if (current_mode < 0){
                current_mode = max_modes-1;
            }
        }
        else if (pressed == PB_OK){
            delay(200);
            // run_mode(current_mode);
            Serial.println(current_mode); //Note that the 
        }
        else if (pressed == PB_CANCEL){
            delay(200);
            break;
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
}
