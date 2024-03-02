#include <Globals.h>
#include <Functions.h>
#include <Constants.h>

DHT dhtSensor(DHTPIN, DHTTYPE);

void check_temp()
{
    float temperature = dhtSensor.readTemperature();
    float humidity = dhtSensor.readHumidity();
    // for temperature
    if (temperature > 35.0)
    {
        display.fillRect(0, 46, 128, 10, BLACK);
        println("TEMP HIGH " + String(temperature) + "C", 0, 46, 1);
    }
    else if (temperature < 25.0)
    {
        display.fillRect(0, 46, 128, 10, BLACK);
        println("TEMP LOW " + String(temperature) + "C", 0, 46, 1);
    }
    // for humidity
    if (humidity > 40.0)
    {
        display.fillRect(0, 56, 128, 8, BLACK);
        println("HUMIDITY HIGH " + String(humidity) + "%", 0, 56, 1);
    }
    else if (humidity < 20.0)
    {
        display.fillRect(0, 56, 128, 8, BLACK);
        println("THUMIDITY LOW " + String(humidity) + "%", 0, 56, 1);
    }
}