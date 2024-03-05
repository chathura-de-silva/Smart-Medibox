#include <map>

// Functions from alarms.cpp
void ring_alarm();
void set_alarm(int alarm);

// Functions from utils.cpp
int wait_for_button_press();
void go_to_menu();
void update_time_with_check_alarm();
void println(String text, int column, int row, int text_size, bool display_now = false, int color = WHITE);
void println(tm timeinfo, char *text, int column, int row, int text_size, bool display_now = false, int color = WHITE);

// FUnctions from temperature.cpp
void check_temp();

// Functions from time.cpp
void set_time_zone();
void update_time();
void print_time_now();
