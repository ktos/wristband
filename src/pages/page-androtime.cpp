#include <Arduino.h>
#include <AndroTime.h>
#include "clock.hpp"
#include "ntp.hpp"
#include "wristband-tft.hpp"

unsigned long aclockRefresh = 0;
bool acolon = true;
uint16_t acolonX = 0;
uint8_t oldKlik = 99;
uint8_t oldTay = 99;

void pageAndroClock(bool initialLoad)
{
    andro_time_t current;
    if (initialLoad)
    {
        clearScreen();
        current = AndroTime.calculate(getClockUnixTime());
        displayAndroDate(current);
        acolonX = displayHour(current.naj, current.klik, false);
        oldKlik = current.klik;
        oldTay = current.tay;
        aclockRefresh = millis();
    }
    else if (millis() - aclockRefresh > 1000)
    {
        aclockRefresh = millis();
        current = AndroTime.calculate(getClockUnixTime());
        acolon = !acolon;
        displayColon(acolonX, acolon, false);
        if (oldKlik != current.klik)
        {
            acolonX = displayHour(current.naj, current.klik, false);
        }
        if (oldTay != current.tay)
        {
            displayAndroDate(current);
        }
        oldKlik = current.klik;
        oldTay = current.tay;
    }
}