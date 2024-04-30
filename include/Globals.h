#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Preferences.h>
#include <Servo.h>

extern Adafruit_SSD1306 display;
extern DHT dhtSensor;
extern Preferences preferences;
extern Servo servo_motor;

extern bool alarm_enabled;
extern const int n_alarms;
extern int alarm_hours[];
extern int alarm_minutes[];
extern bool alarm_triggered[];
extern struct tm timeinfo;
extern int temp_offset_hours;
extern int temp_offset_minutes;
extern String wifi_username;
extern String wifi_password;
extern int servo_angle;
// images
extern const unsigned char splashScreen[];
extern const unsigned char tick[];
extern const unsigned char wifi[];
extern const unsigned char waveformAnimation[31][1024];
extern const unsigned char time_zone[];
extern const unsigned char alarm_ring[];
extern const unsigned char alarm_disable[];
extern const unsigned char medicine_time[];
extern const unsigned char reset[];
extern const unsigned char wifi_fail [];
extern const unsigned char wifi_config [];