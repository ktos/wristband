#include <Arduino.h>
#include <EasyButton.h>
#include <Mokosh.hpp>
#include <vector>
#include "ota.hpp"
#include "sleep.hpp"
#include "pages.hpp"

int8_t page = 0;
EasyButton tp_button(TP_PIN_PIN, 80, true, false);
uint32_t time_out = millis();
uint16_t max_time_out = 15000;
bool handlingAction = false;
bool initialLoad = true;
bool sleepy = true; // princess in a demonic castle

void initMokosh()
{
  Mokosh *mokosh = Mokosh::getInstance();

  mdebugV("Setting up Wi-Fi connection");
  mokosh->setupWiFiClient();
  mokosh->connectWifi();
  mokosh->setupRemoteDebug();
  mokosh->setupMqttClient();
  mokosh->hello();

  mokosh->otaEvents.onStart = onOtaStarted;
  mokosh->otaEvents.onProgress = onOtaProgress;
  mokosh->setupOta();  
}

void actionMokoshConnect()
{
  if (sleepy)
  {
    msgInfo("Setting up Mokosh", "Not sleepy");
    initMokosh();
    msgSuccess("Finished");

    sleepy = false;
  }
  else
  {
    msgInfo("Sleepy...", "Mokosh disabled");
    sleepy = true;
  }

  delay(2000);
}

void initButton()
{
  pinMode(TP_PWR_PIN, PULLUP);
  digitalWrite(TP_PWR_PIN, HIGH);
  tp_button.begin();
  tp_button.onPressedFor(1000, handleAction);
  tp_button.onPressed(increasePage);
  page = 0;
  showPage();
}

void handleUi()
{
  if (millis() - time_out > max_time_out && !handlingAction && sleepy)
  {
    handleSleep();
  }
  else
  {
    tp_button.read();
    if (!handlingAction)
    {
      showPage();
    }
  }
}

void forceInitialLoad() 
{
  initialLoad = true;
  time_out = millis();
}

void increasePage()
{
  time_out = millis();
  page++;
  initialLoad = true;
}

void startPage()
{
  time_out = millis();
  page = 0;
  initialLoad = true;
}

void showPage()
{
  switch (page)
  {
  case 0:
    max_time_out = 8000;
    pageClock(initialLoad);
    break;
  case 1:
    max_time_out = 15000;
    pageAndroClock(initialLoad);
    break;
  case 2:
    max_time_out = 15000;
    pageBattery(initialLoad);
    break;
  // case 3:
  //   max_time_out = 60000;
  //   pageBearing(initialLoad);
  //   break;
  // case 4:
  //   max_time_out = 30000;
  //   pageTemperature(initialLoad);
  //   break;
  // case 5:
  //   max_time_out = 30000;
  //   pageWifiScan(initialLoad);
  //   break;
  // case 6:
  //   max_time_out = 30000;
  //   pageMatrix(initialLoad);
  //   break;
  case 3:
    if (sleepy && !isCharging())
    {
      handleSleep();
    }
    else
    {
      startPage();
      return;
    }
    break;
  }

  if (isCharging())
    max_time_out *= 2;

  initialLoad = false;
}

void handleAction()
{
  handlingAction = true;
  time_out = millis();
  switch (page)
  {
  case 0:
    actionMokoshConnect();
    break;
  case 1:
    actionClock();
    break;
  case 3:
    actionBearing();
    break;
  case 5:
    actionWifiScan();
    break;
  }

  time_out = millis();
  handlingAction = false;
  initialLoad = true;
}
