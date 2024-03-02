#include <Globals.h>
#include <Functions.h>
#include <Constants.h>

int current_mode = 0;
const int max_modes = 5;
const String modes[] = {"Set Alarm 1", "Set Alarm 2", "Set Alarm 3", "Disable Alarms", "Set Time Zone"};

void println(String text, int column, int row, int text_size)
{
    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(column, row);
    display.println(text);
    display.display();
}

void println(tm timeinfo, char *text, int column, int row, int text_size)
{
    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(column, row);
    display.println(&timeinfo, text);
    display.display();
}

void run_mode(int mode)
{
    if (mode == 0 || mode == 1 || mode == 2)
    {
        set_alarm(mode); // Notice that the alarm number is equal to the mode number -1.
    }
    else if (mode == 3)
    {
        alarm_enabled = false;
        display.clearDisplay();
        println("Alarms disabled!", 0, 0, 2);
        delay(1000);
    }
    else if (mode == 4)
    {
        set_time_zone();
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
        else if (pressed == PB_DOWN)
        {
            delay(200);
            current_mode--;
            current_mode = current_mode % max_modes;
            if (current_mode < 0)
            {
                current_mode = max_modes - 1;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(200);
            run_mode(current_mode);
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }
}