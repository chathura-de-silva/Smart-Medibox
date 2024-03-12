#include <Globals.h>
#include <Functions.h>
#include <Constants.h>

int current_mode = 0;
const int max_modes = 6;
const String modes[] = {"Set Alarm 1", "Set Alarm 2", "Set Alarm 3", "Disable Alarms", "Set Time Zone", "Reset Settings"};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String formatNumber(int num) {  //formats a given number to have two digits if its between 0 and 9.
    if (num >= 0 && num <= 9) {
        return "0" + String(num); // Prepend 0 if num is a single digit
    } else {
        return String(num); // No need to modify if num is already two or more digits
    }
}

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

void println(tm timeinfo, const char *text, int column, int row, int text_size, bool display_now, int color)
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
        save_is_alarm_enabled();
        show_modal_page(alarm_disable,1000,"Alarms Disabled!", 20);
    }
    else if (mode == 4)
    {
        set_time_zone();
    }
    else if (mode==5){
       reset_preferences();
    }
}

void display_menu(int active_mode){
    int row = 0; 
    const int padding_top = 4;
    int page_number = 1+active_mode/5;

    display.clearDisplay();
    
    for(int i = 5*(page_number-1) ;i<min(5*page_number,max_modes) ;i++){  //Only 5 menu items displayed at a time in the display.
        if (i == active_mode){
            display.fillRoundRect(0,row,128,14,2,WHITE);
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
            delay(50);
            current_mode++;
            current_mode = current_mode % max_modes;
        }
        else if (pressed == PB_DOWN)
        {
            delay(50);
            current_mode--;
            current_mode = current_mode % max_modes;
            if (current_mode < 0)
            {
                current_mode = max_modes - 1;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(50);
            run_mode(current_mode);
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(50);
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
            delay(100);
            return PB_UP;
        }
        else if (digitalRead(PB_DOWN) == LOW)
        {
            delay(100);
            return PB_DOWN;
        }
        else if (digitalRead(PB_OK) == LOW)
        {
            delay(100);
            return PB_OK;
        }
        else if (digitalRead(PB_CANCEL) == LOW)
        {
            delay(100);
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

void show_modal_page(const unsigned char* bitmap,int period, String text, int x_offset){ //This function is responsible for showing a provided bitmap above a given text, in full screen.
    display.clearDisplay();
    display.drawBitmap(0, 0, bitmap, 128, 64, WHITE);
    println(text, x_offset, 50, 1, true);
    delay(period);
}