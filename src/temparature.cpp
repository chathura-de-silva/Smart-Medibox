#include <Globals.h>
#include <Functions.h>
#include <Constants.h>

DHT dhtSensor(DHTPIN, DHTTYPE);

void check_temp()
{
    float temperature = dhtSensor.readTemperature();
    float humidity = dhtSensor.readHumidity();
    // for temperature
    if (temperature > 32.0)
    {
        display.fillRect(0, 46, 128, 10, BLACK);
        println("TEMP HIGH " + String(temperature) + "C", 14, 46, 1);
    }
    else if (temperature < 26.0)
    {
        display.fillRect(0, 46, 128, 10, BLACK);
        println("TEMP LOW " + String(temperature) + "C", 18, 46, 1);
    }
    // for humidity
    if (humidity > 80.0)
    {
        display.fillRect(0, 56, 128, 8, BLACK);
        println("HUMIDITY HIGH " + String(humidity) + "%", 4, 56, 1);
    }
    else if (humidity < 60.0)
    {
        display.fillRect(0, 56, 128, 8, BLACK);
        println("HUMIDITY LOW " + String(humidity) + "%", 8, 56, 1);
    }
}