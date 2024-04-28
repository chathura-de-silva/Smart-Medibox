#include <PubSubClient.h>
#include <WiFi.h>
#include <Constants.h>
#include <Functions.h>
#include <ArduinoJson.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void brokerConnectMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.println("Attempting MQTT connection...");
        if (mqttClient.connect(MQTT_DEVICE_ID))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(mqttClient.state());
            delay(5000);
        }
    }
}

void dataReceptionCallback(char *topic, byte *payload, unsigned int length)
{

    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    char payloadCharArray[length];
    for (int i = 0; i < length; i++)
    {
        payloadCharArray[i] = (char)payload[i];
    }
}

void send_mqtt_data(float temperature)
{
    JsonDocument data;
    char dataJson[100];
    data["LDR1"] = analogRead(LDR_1);
    data["LDR2"] = analogRead(LDR_2);
    data["Temperature"] = temperature;
    serializeJson(data, dataJson);

    if (!mqttClient.connected())
    {
        brokerConnectMQTT();
    }
    // Publish readings to the mqtt topic.
    mqttClient.publish(DATA_SENDING_TOPIC, dataJson);
}

void setupMQTT()
{
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCallback(dataReceptionCallback);
}