#include <Globals.h>
#include <Functions.h>
#include <Constants.h>

bool alarm_enabled;
const int n_alarms = 3;
int alarm_hours[n_alarms];
int alarm_minutes[n_alarms];
bool alarm_triggered[n_alarms];

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
        // Turning the LED ON
    digitalWrite(LED_1, HIGH);
    show_modal_page(medicine_time ,200,"It's Medicine Time!", 8);
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
    int temp_minute = alarm_minutes[alarm];

    while (true)
    {
        display.clearDisplay();
        display.fillRoundRect(23, 12, 39, 34, 4, WHITE);
        println(formatNumber(temp_hour), 26, 18, 3,false, BLACK);
        println(formatNumber(temp_minute), 76, 18, 3,true, WHITE);
        println(":", 60, 18, 3,true, WHITE);
        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(50);
            temp_hour++;
            temp_hour = temp_hour % 24;
        }
        else if (pressed == PB_DOWN)
        {
            delay(50);
            temp_hour--;
            temp_hour = temp_hour % 24;
            if (temp_hour < 0)
            {
                temp_hour = 23;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(50);
            alarm_hours[alarm] = temp_hour;
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(50);
            return;
        }
    }

    while (true)
    {
        display.fillRoundRect(23, 12, 39, 34, 4, BLACK);
        println(formatNumber(temp_hour), 26, 18, 3,false, WHITE); 
        //Above two lines removes the white background around the hour setting state.(i.e. inverts the hour part of the display back.)
        display.fillRoundRect(73, 12, 39, 34, 4, WHITE);
        println(formatNumber(temp_minute), 76, 18, 3,true, BLACK);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(50);
            temp_minute++;
            temp_minute = temp_minute % 60;
        }
        else if (pressed == PB_DOWN)
        {
            delay(50);
            temp_minute--;
            temp_minute = temp_minute % 60;
            if (temp_minute < 0)
            {
                temp_minute = 59;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(50);
            alarm_minutes[alarm] = temp_minute;
            if (!alarm_enabled){
                alarm_enabled = true;
                save_is_alarm_enabled();
                for (int i = 0; i < n_alarms; i++)
                {
                        alarm_triggered[i] = true;  
                }
                alarm_triggered[alarm] = false;
            }

            show_modal_page(alarm_ring,1000,"Alarm set to "+formatNumber(temp_hour)+":"+formatNumber(temp_minute),10);
            save_alarm(alarm);
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(50);
            break;
        }
    }
}