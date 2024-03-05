#include <Globals.h>
#include <Functions.h>
#include <Constants.h>

int current_mode = 0;
const int max_modes = 5;
const String modes[] = {"Set Alarm 1", "Set Alarm 2", "Set Alarm 3", "Disable Alarms", "Set Time Zone"};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void println(String text, int column, int row, int text_size, bool display_now, int color)
{
    display.setTextSize(text_size);
    display.setTextColor(color);
    display.setCursor(column, row);
    display.println(text);
    if (display_now)
    {
        display.display();
    }
}

void println(tm timeinfo, char *text, int column, int row, int text_size, bool display_now, int color)
{
    display.setTextSize(text_size);
    display.setTextColor(color);
    display.setCursor(column, row);
    display.println(&timeinfo, text);
    if (display_now)
    {
        display.display();
    }
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
        println("Alarms disabled!", 0, 0, 2, true);
        delay(1000);
    }
    else if (mode == 4)
    {
        set_time_zone();
    }
}

void display_menu(int active_mode){
    int row = 0; 
    const int padding_top = 4;
    int page_number = 1+active_mode/5;

    display.clearDisplay();
    
    for(int i = 5*(page_number-1) ;i<5*page_number ;i++){  //Only 5 menu items displayed at a time in the display.
        if (i == active_mode){
            display.fillRect(0,row,128,14,WHITE);
            println(modes[i],0,row+padding_top,1, false, BLACK );
        }
        else{
            println(modes[i],0,row+padding_top,1, false, WHITE );
        }
        row+=12;
    }
    display.display();
}

void go_to_menu()
{
    while (digitalRead(PB_CANCEL) == HIGH)
    {
        display_menu(current_mode);

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
