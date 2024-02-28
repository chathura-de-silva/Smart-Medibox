#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>

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
#define DHTPIN 4
#define DHTTYPE DHT11   // DHT 11
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET_DST 0   // Daytime offset is not implemented. hence kept zero to have no effect.

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dhtSensor(DHTPIN, DHTTYPE);

int utc_offset = 19800; //default to Sri Lanka's offset.
struct tm timeinfo; // contains time data. pre defined struct type.

bool alarm_enabled = true;
int n_alarms = 3;
int alarm_hours[] = {0, 1,2};
int alarm_minutes[] = {1, 10,20};
bool alarm_triggered[] = {false, false, false};

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
int max_modes = 5;
String modes[] = {"1 - Set Alarm 1", "2 - Set Alarm 2", "3 - Set Alarm 3", "4 - Disable Alarms", "5 - Set Time Zone"};

void println(String text, int column, int row, int text_size)
{
    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(column, row);
    display.println(text);
    display.display();
}

void println(tm timeinfo ,char *text, int column, int row, int text_size)
{
    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(column, row);
    display.println(&timeinfo,text);
    display.display();
}

void print_time_now()
{   display.fillRect(0, 0, 128, 16, BLACK);
    // println(String(days), 0, 0, 2);  // This code makes a flicker effect in the display from left to right since there are multiple statements to be displayed. 
    // println(":", 20, 0, 2);
    // println(String(hours), 30, 0, 2);
    // println(":", 50, 0, 2);
    // println(String(minutes), 60, 0, 2);
    // println(":", 80, 0, 2);
    // println(String(seconds), 90, 0, 2);

    //Instead of above lines, the single line below is added to avoid flickering and display time as a whole in once.
    println(timeinfo,"%H:%M:%S", 0,0,2);
    display.fillRect(0, 16, 128, 30, BLACK);
    println(timeinfo,"%d %B %Y", 0,22,1);
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

void update_time(){
    if (!getLocalTime(&timeinfo)){
        println("Failed to fetch time from server!",0,0,2);
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

void set_alarm(int alarm)
{
    int temp_hour = alarm_hours[alarm];

    while (true)
    {
        display.clearDisplay();
        println("Enter hour: " + String(temp_hour), 0, 0, 2);

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
        println("Enter minute: " + String(temp_minute), 0, 0, 2);

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
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }
    display.clearDisplay();
    /////////////////////////////////////////////
    println("Alarm " + String(alarm) + "is set", 0, 0, 2);
    delay(1000);
}

void set_time_zone()
{
    int temp_offset_hours = utc_offset/3600;
    int temp_offset_minutes = utc_offset/60 - temp_offset_hours*60;

    while (true)
    {
        display.clearDisplay();
        println("Enter UTC offset hours: " + String(temp_offset_hours), 0, 0, 2);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(200);
           temp_offset_hours++;
           if (temp_offset_hours>14){  // 14 hours multiplies by 60.
            temp_offset_hours = -12;   // 12 hours multiplies by 60.
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
            delay(200);   //since the offset is finally a single variable counted in seconds, setting it here globally is unnecessary. It will be set after taking the minutes as well.
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
            utc_offset = temp_offset_hours*3600 + temp_offset_minutes*60;
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }

    }
    configTime(utc_offset, UTC_OFFSET_DST, NTP_SERVER);
    display.clearDisplay();
    println("Time zone is set", 0, 0, 2);
    delay(1000);
}

void run_mode(int mode)
{
    if (mode == 0 || mode == 1 || mode ==2)
    {
        set_alarm(mode - 1); // Notice that the alarm number is equal to the mode number -1.
    }
    else if (mode == 3)
    {
        alarm_enabled = false;
        display.clearDisplay();
        println("Alarms disabled!", 0, 0, 2);
        delay(1000);
    }
    else if (mode  == 4){
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

void check_temp()
{
    float temperature = dhtSensor.readTemperature();
    float humidity = dhtSensor.readHumidity();
    // for temperature
    if (temperature > 35.0)
    {
        display.fillRect(0, 46, 128, 10, BLACK);
        println("TEMP HIGH "+String(temperature)+"C", 0, 46, 1);
    }
    else if (temperature < 25.0)
    {
        display.fillRect(0, 46, 128, 10, BLACK);
        println("TEMP LOW "+String(temperature)+"C", 0, 46, 1);
    }
    // for humidity
    if (humidity > 40.0)
    {
        display.fillRect(0, 56, 128, 8, BLACK);
        println("HUMIDITY HIGH "+String(humidity)+"%", 0, 56, 1);
    }
    else if (humidity < 20.0)
    {
        display.fillRect(0, 56, 128, 8, BLACK);
        println("THUMIDITY LOW "+String(humidity)+"%", 0, 56, 1);
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
