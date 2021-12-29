#include <Arduino.h>
#include <Wire.h>
#include <rom/rtc.h>
#include <WiFi.h>
#include <Mokosh.hpp>
#include "wristband-tft.hpp"
#include "clock.hpp"
#include "pages.hpp"
#include "mpu.hpp"
#include "unattended.hpp"

Mokosh m;
bool unattended = false;

void commandReceived(uint8_t *message, unsigned int length)
{
  char msg[32] = {0};
  for (unsigned int i = 0; i < length; i++)
  {
    msg[i] = message[i];
  }
  msg[length + 1] = 0;

  mdebugD("Command received: %s", msg);

  String s = String(msg);

  uint16_t c = TFT_WHITE;
  if (s.startsWith("e")) {
    c = TFT_BLUE;
  } else if (s.startsWith("w")) {
    c = TFT_YELLOW;
  }

  msgBigAndro(c, s.substring(2).c_str());
  delay(2000);
  forceInitialLoad();
}

void setup()
{
  m.setDebugLevel(DebugLevel::VERBOSE)
      ->setForceWiFiReconnect(false)
      ->setHeartbeat(false)
      ->setIgnoreConnectionErrors(true)
      ->setOta(true)
      ->setRebootOnError(true);

  m.onCommand = commandReceived;

  // ATAKSAK
  // a goddess in Inuit mythology. She is the ruler of the sky,
  // and represents the light in the world that brings joy and
  // happiness to the people.
  m.begin("Ataksak", false);

  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  if (cause == ESP_SLEEP_WAKEUP_TIMER)
  {
    mdebugV("Waking up because of timer, enabling unattended mode");
    unattended = true;
  }

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);

  // update local time from RTC
  mdebugD("Setting up RTC");
  initClock();

  if (!unattended)
  {
    mdebugD("Initializing TFT");
    tftInit();
  }

  btStop();

  mdebugD("Setting up ADC");
  setupADC();

  mdebugD("Setting up MPU to sleep until needed");
  mpuDeepSleep();

  if (!unattended)
  {
    mdebugD("Initializing buttons");
    initButton();
  }

  mdebugD("Initializing battery ADC");
  setupBattery();

  if (unattended)
  {
    mdebugV("Initializing unattended mode");
    unattendedInit();
  }

  if (isCharging())
  {
    mdebugV("Connecting automatically, because charging.");
    initMokosh();
  }

  mdebugV("Go!");
}

void loop()
{
  if (!unattended)
  {
    handleUi();
    updateBatteryChargeStatus();
  }
  else
  {
    unattendedLoop();
  }

  m.loop();
}
