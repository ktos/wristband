#include <Arduino.h>
#include <Wire.h>
#include <rom/rtc.h>
#include <WiFi.h>
#include <Mokosh.hpp>
#include "wristband-tft.hpp"
#include "clock.hpp"
#include "pages.hpp"
#include "mpu.hpp"

Mokosh m;

void setup()
{
  m.setDebugLevel(DebugLevel::VERBOSE)
      ->setForceWiFiReconnect(false)
      ->setHeartbeat(false)
      ->setIgnoreConnectionErrors(true)
      ->setOta(true)
      ->setRebootOnError(true);

  // ATAKSAK
  // a goddess in Inuit mythology. She is the ruler of the sky,
  // and represents the light in the world that brings joy and
  // happiness to the people.
  m.begin("Ataksak", false);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);

  // update local time from RTC
  mdebugV("Setting up RTC");
  initClock();
  tftInit();
  btStop();

  mdebugV("Setting up ADC");
  setupADC();

  mdebugV("Setting up MPU to sleep until needed");
  mpuDeepSleep();

  initButton();

  setupBattery();  
}

void loop()
{
  handleUi();
  updateBatteryChargeStatus();
  m.loop();
}
