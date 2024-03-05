#include <Adafruit_SSD1306.h>
#include <DHT.h>

extern Adafruit_SSD1306 display;
extern DHT dhtSensor;

extern bool alarm_enabled;
extern const int n_alarms;
extern int alarm_hours[];
extern int alarm_minutes[];
extern bool alarm_triggered[];
extern int utc_offset;
extern struct tm timeinfo;
extern const unsigned char splashScreen[];
extern const unsigned char tick[];
extern const unsigned char wifi[];
extern const unsigned char waveformAnimation[31][1024];