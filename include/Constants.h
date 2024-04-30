#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESSS 0x3C

#define BUZZER 5
#define LED_1 15
#define PB_CANCEL 34
#define PB_OK 25
#define PB_UP 26
#define PB_DOWN 35
#define DHTPIN 4
#define LDR_1 36
#define LDR_2 39
#define SERVO 33
#define DHTTYPE DHT11 // DHT 11
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET_DST 0 // Daytime offset is not implemented. hence kept zero to have no effect.
#define DEFAULT_UTC_OFFSET_H 5
#define DEFAULT_UTC_OFFSET_M 30
#define AP_SSID "Smart Medibox" // SSID and Password of the Access point created by the ESP32.
#define AP_PASSWORD "abcdefgh"
#define WIFI_CONNECTION_LISTENING_TIMEOUT 10000 // 10 seconds
#define DEFAULT_WIFI_SSID "MSI 8690"
#define DEFAULT_WIFI_PASSWORD "abcdefgh"
#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT 1883
#define MQTT_DEVICE_ID "SmartMedibox-CSE"
#define DATA_SENDING_TOPIC "MEDIBOX_DATA"
#define DATA_RECEPTION_TOPIC "MEDIBOX_MOTOR_DATA"