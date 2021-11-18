#include "sleep.hpp"
#include <WiFi.h>
#include <Mokosh.hpp>

void handleSleep(bool showMsg)
{
  mdebugV("Going to sleep");

  mpuSleep();
  tftSleep(false);
  WiFi.mode(WIFI_OFF);
  rtcSleep();
  pinMode(39, GPIO_MODE_INPUT);

  esp_sleep_enable_ext1_wakeup(GPIO_SEL_33 | GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_deep_sleep_disable_rom_logging();
  esp_deep_sleep_start();
}
