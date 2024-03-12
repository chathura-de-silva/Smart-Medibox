#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Preferences.h>

extern Adafruit_SSD1306 display;
extern DHT dhtSensor;
extern Preferences preferences;

extern bool alarm_enabled;
extern const int n_alarms;
extern int alarm_hours[];
extern int alarm_minutes[];
extern bool alarm_triggered[];
extern struct tm timeinfo;
extern int utc_offset; 
extern int temp_offset_hours;
extern int temp_offset_minutes;
//images
extern const unsigned char splashScreen[];
extern const unsigned char tick[];
extern const unsigned char wifi[];
extern const unsigned char waveformAnimation[31][1024];
extern const unsigned char time_zone [];
extern const unsigned char alarm_ring [];
extern const unsigned char alarm_disable [];
extern const unsigned char medicine_time [];
extern const unsigned char reset [];
