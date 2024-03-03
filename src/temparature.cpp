#include <Globals.h>
#include <Functions.h>
#include <Constants.h>

DHT dhtSensor(DHTPIN, DHTTYPE);

int animationFrameCounter = 30;
// Flag to track if conditions for shwoing image were previously met
bool conditionsMetPreviously = false;

void check_temp()
{
    float temperature = dhtSensor.readTemperature();
    float humidity = dhtSensor.readHumidity();

    // Check if conditions for white square are currently met
    bool conditionsMetNow = (temperature >= 26.0 && temperature <= 32.0 && humidity >= 60.0 && humidity <= 80.0); // becomes true if temperature and humidity both are within limits. i.e. when the OK image is supposed to be started displaying.

    // Reset display if conditions are different from the previous iteration
    if (conditionsMetNow != conditionsMetPreviously)
    {
        display.fillRect(0, 30, 128, 34, BLACK);
        conditionsMetPreviously = conditionsMetNow;
    }

    // Check temperature
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

    // Check humidity
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

    // Display the OK image if conditions are currently met
    if (conditionsMetNow)
    {
        display.fillRect(0, 30, 128, 34, BLACK);
        display.drawBitmap(0, 20, waveformAnimation[animationFrameCounter], 128, 64, WHITE);
        animationFrameCounter--;
        if (animationFrameCounter < 0)
        {
            animationFrameCounter = 30;
        }
    }
}
