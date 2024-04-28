#include <Adafruit_SSD1306.h>
// Functions from alarms.cpp
void ring_alarm();
void set_alarm(int alarm);

// Functions from utils.cpp
String formatNumber(int num);
int wait_for_button_press();
void go_to_menu();
void update_time_with_check_alarm();
void println(String text, int column, int row, int text_size, bool display_now = false, int color = WHITE);
void println(tm timeinfo, const char *text, int column, int row, int text_size, bool display_now = false, int color = WHITE);
void show_modal_page(const unsigned char *bitmap, int period, String text, int x_offset);

// FUnctions from temperature.cpp
float check_temp();

// Functions from time.cpp
void set_time_zone();
void update_time();
void print_time_now();

// Functions of preferences.cpp
void load_user_settings();
void save_time_zone();
void save_alarm(int alarm);
void reset_preferences();
void save_is_alarm_enabled();
void save_wifi_credentials(String username, String password);

// Functions from webServer.cpp
void config_wifi();

// Functions from nodeRed.cpp
void setupMQTT();
void brokerConnectMQTT();
void send_mqtt_data(float temperature);