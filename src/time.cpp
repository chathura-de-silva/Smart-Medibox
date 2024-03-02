#include <Globals.h>
#include <Functions.h>
#include <Constants.h>

int utc_offset = 19800; // default to Sri Lanka's offset.
struct tm timeinfo;     // contains time data. pre defined struct type.

void set_time_zone()
{
    int temp_offset_hours = utc_offset / 3600;
    int temp_offset_minutes = utc_offset / 60 - temp_offset_hours * 60;

    while (true)
    {
        display.clearDisplay();
        println("Enter UTC offset hours: " + String(temp_offset_hours), 0, 0, 2);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(200);
            temp_offset_hours++;
            if (temp_offset_hours > 14)
            {                            // 14 hours multiplies by 60.
                temp_offset_hours = -12; // 12 hours multiplies by 60.
            }
        }
        else if (pressed == PB_DOWN)
        {
            delay(200);
            temp_offset_hours--;
            if (temp_offset_hours < -12)
            {
                temp_offset_hours = 14;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(200); // since the offset is finally a single variable counted in seconds, setting it here globally is unnecessary. It will be set after taking the minutes as well.
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }

    while (true)
    {
        display.clearDisplay();
        println("Enter offset minutes: " + String(temp_offset_minutes), 0, 0, 2);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(200);
            temp_offset_minutes++;
            temp_offset_minutes = temp_offset_minutes % 60;
        }
        else if (pressed == PB_DOWN)
        {
            delay(200);
            temp_offset_minutes--;
            temp_offset_minutes = temp_offset_minutes % 60;
            if (temp_offset_minutes < 0)
            {
                temp_offset_minutes = 59;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(200);
            utc_offset = temp_offset_hours * 3600 + temp_offset_minutes * 60;

            configTime(utc_offset, UTC_OFFSET_DST, NTP_SERVER);
            display.clearDisplay();
            println("Time zone is set", 0, 0, 2);
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