#include <Globals.h>
#include <Functions.h>
#include <Constants.h>

DHT dhtSensor(DHTPIN, DHTTYPE);

int animationFrameCounter = 30;

void check_temp()
{
    float temperature = dhtSensor.readTemperature();
    float humidity = dhtSensor.readHumidity();
    bool temp_in_range = false;

    // Check temperature
    if (temperature > 32.0)
    {
        println("TEMP HIGH " + String(temperature) + "C", 14, 46, 1);
    }
    else if (temperature < 26.0)
    {
        println("TEMP LOW " + String(temperature) + "C", 18, 46, 1);
    }
    else
    {
        temp_in_range = true;
    }

    // Check humidity
    if (humidity > 80.0)
    {
        println("HUMIDITY HIGH " + String(humidity) + "%", 4, 56, 1);
    }
    else if (humidity < 60.0)
    {
        println("HUMIDITY LOW " + String(humidity) + "%", 8, 56, 1);
    }
    else if (temp_in_range)
    {
        display.drawBitmap(0, 20, waveformAnimation[animationFrameCounter], 128, 64, WHITE);
        animationFrameCounter--;
        if (animationFrameCounter < 0)
        {
            animationFrameCounter = 30;
        }
    }
    display.display();
}
