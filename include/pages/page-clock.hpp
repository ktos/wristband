#include <Arduino.h>
#include "clock.hpp"
#include "ntp.hpp"
#include "wristband-tft.hpp"

void pageClock(bool initialLoad);
void pageRtc(bool initialLoad);
void actionClock();