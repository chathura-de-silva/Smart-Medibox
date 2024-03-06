#include <Globals.h>
#include <Functions.h>
#include <Constants.h>

bool alarm_enabled = true;
const int n_alarms = 3;
int alarm_hours[] = {0, 1, 2};
int alarm_minutes[] = {1, 10, 20};
bool alarm_triggered[] = {false, false, false};

const int n_notes = 8;
const int C = 262;
const int D = 294;
const int E = 330;
const int F = 349;
const int G = 392;
const int A = 440;
const int B = 494;
const int C_H = 523;
const int notes[] = {C, D, E, F, G, A, B, C_H};

void ring_alarm()
{
    display.clearDisplay();
    println("MEDICINE TIME!", 0, 0, 2, true);
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

void set_alarm(int alarm)
{
    int temp_hour = alarm_hours[alarm];

    while (true)
    {
        display.clearDisplay();
        println("Enter hour: " + String(temp_hour), 0, 0, 2, true);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(200);
            temp_hour++;
            temp_hour = temp_hour % 24;
        }
        else if (pressed == PB_DOWN)
        {
            delay(200);
            temp_hour--;
            temp_hour = temp_hour % 24;
            if (temp_hour < 0)
            {
                temp_hour = 23;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(200);
            alarm_hours[alarm] = temp_hour;
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }

    int temp_minute = alarm_minutes[alarm];

    while (true)
    {
        display.clearDisplay();
        println("Enter minute: " + String(temp_minute), 0, 0, 2, true);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(200);
            temp_minute++;
            temp_minute = temp_minute % 60;
        }
        else if (pressed == PB_DOWN)
        {
            delay(200);
            temp_minute--;
            temp_minute = temp_minute % 60;
            if (temp_minute < 0)
            {
                temp_minute = 59;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(200);
            alarm_minutes[alarm] = temp_minute;

            display.clearDisplay();
            println("Alarm " + String(alarm + 1) + " is set", 0, 0, 2, true);
            delay(1000);
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }
}