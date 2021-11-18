#include "unattended.hpp"
#include <Mokosh.hpp>
#include <ArduinoJson.hpp>
#include "sleep.hpp"
#include "battery.hpp"
#include "mpu.hpp"

unsigned long max_timeout = 30000;
unsigned long timeout = 0;

Mokosh *mokosh;

void publishStatus()
{
    float voltage = getVoltage();
    uint8_t percentage = calcPercentage(voltage);

    StaticJsonDocument<128> sjd;
    sjd["batteryVoltage"] = voltage;
    sjd["batteryPercentage"] = percentage;

    mdebugD("Battery voltage is %f (%d%%)", voltage, percentage);

    String output;
    serializeJson(sjd, output);

    String topic = String(mokosh->debug_topic) + "/status";

    mokosh->publish(topic.c_str(), output);
}

void publishTempReadings()
{
    initMPU();
    float t = getTemperature();

    StaticJsonDocument<128> sjd;
    sjd["temperature"] = t;

    mdebugD("MPU temperature is %f*C", t);

    String output;
    serializeJson(sjd, output);

    mokosh->publish("readings/mpu", output);

    mpuDeepSleep();
}

void unattendedInit()
{
    mokosh = Mokosh::getInstance();

    mdebugD("Setting up Wi-Fi connection");
    mokosh->setupWiFiClient();
    mokosh->connectWifi();
    mokosh->setupRemoteDebug();
    mokosh->setupMqttClient();
    mokosh->hello();
    mokosh->setupOta();

    if (!mokosh->isWifiConnected())
    {
        mdebugW("Wi-Fi is not connected, going back to sleep.");
        handleSleep(false);
    }

    mdebugV("Publishing unattended status");
    publishStatus();
    mokosh->publish(mokosh->heartbeat_topic, millis());
    publishTempReadings();

    mdebugV("Finished, waiting for %d ms for OTA or commands", max_timeout);
    timeout = millis();
}

void unattendedLoop()
{
    if (millis() - timeout > max_timeout)
    {
        handleSleep(false);
    }
}