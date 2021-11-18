#define uS_TO_S_FACTOR 1000000ULL

#include "sleep.hpp"
#include <WiFi.h>
#include <Mokosh.hpp>

void handleSleep(bool showMsg)
{
  int minutes = Mokosh::getInstance()->config.getInt("sleep", 0);

  if (minutes == 0)
    mdebugV("Going to sleep");
  else
    mdebugV("Going to sleep, scheduled wakeup in %d minutes", minutes);

  if (showMsg)
  {
    msgBig("Going sleep...");
  }

  mpuSleep();
  tftSleep(false);
  WiFi.mode(WIFI_OFF);
  rtcSleep();
  pinMode(39, GPIO_MODE_INPUT);

  if (minutes != 0)
    esp_sleep_enable_timer_wakeup(minutes * 60 * uS_TO_S_FACTOR);

  esp_sleep_enable_ext1_wakeup(GPIO_SEL_33 | GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_deep_sleep_disable_rom_logging();
  esp_deep_sleep_start();
}
