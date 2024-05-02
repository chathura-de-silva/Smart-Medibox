#include <PubSubClient.h>
#include <WiFi.h>
#include <Constants.h>
#include <Functions.h>
#include <ArduinoJson.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);
JsonDocument data; /*"data" is a global variable that stores the data to be sent to the mqtt broker.
                   Keeping this global helps to compare with retained last sent values inside it and
                   reduce the frequency of MQTT requests sent, via avoiding sending the same values again and again.*/

void brokerConnectMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.println("Attempting MQTT connection...");
        if (mqttClient.connect(MQTT_DEVICE_ID))
        {
            Serial.println("connected");
            mqttClient.subscribe(DATA_RECEPTION_TOPIC);
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(mqttClient.state());
            delay(5000);
        }
    }
    mqttClient.loop();
}

void dataReceptionCallback(char *topic, byte *message, unsigned int length)
{

    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    String degree;
    for (int i = 0; i < length; i++)
    {
        degree += (char)message[i]; // Adjust this code if you are sending something more than just the degree of rotation of the servo motor.
    }
    Serial.println(degree); // for debugging
    if (strcmp(topic, DATA_RECEPTION_TOPIC) == 0)
    {
        turn_servo_motor(degree.toInt());
    }
    else
    {
        Serial.println("Invalid command");
    }
}

float getLuminance(float sensorReading)
{
    float lux;
    if (LINEAR_MAPPING)
    {
        // Serial.println("Linear Mapping");// for debugging.
        lux = sensorReading/FINITE_INFINITY;
        if (lux >= FINITE_INFINITY)
            return 1;
        return lux;
    }
    else
    {
        // Serial.println("Non-Linear Mapping"); // for debugging.
        float voltage = sensorReading / 4096.0 * VCC;
        float resistance = RESISTOR * voltage / (VCC - voltage);
        // Change below lines according to the LDR used. This depends on LDR Characteristics.
        lux = pow(RL10 * 1000 * pow(10, GAMMA) / resistance, (1 / GAMMA));
        if (lux >= FINITE_INFINITY)
            return 1;
        return lux / FINITE_INFINITY;
    }
}

void send_mqtt_data(float temperature)
{
    char dataJson[100];

    float ldr1 = getLuminance(analogRead(LDR_1));
    float ldr2 = getLuminance(analogRead(LDR_2));

    if (fabs(data["LDR1"].as<float>() - ldr1) >= EPSILON || fabs(data["LDR2"].as<float>() - ldr2) >= EPSILON || fabs(data["Temperature"].as<float>() - temperature) >= TEMPSILON)
    {
        data["LDR1"] = ldr1;
        data["LDR2"] = ldr2;
        data["Temperature"] = temperature;

        serializeJson(data, dataJson);

        Serial.println(dataJson);

        if (!mqttClient.connected())
        {
            brokerConnectMQTT();
        }
        // Publish readings to the mqtt topic.
        mqttClient.publish(DATA_SENDING_TOPIC, dataJson);
    }
}

void setupMQTT()
{
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCallback(dataReceptionCallback);
}