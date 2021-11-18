#include "pages/page-battery.hpp"

void pageBattery(bool initialLoad)
{
  if (initialLoad)
  {    
    float voltage = getVoltage();
    drawBattery(voltage, calcPercentage(voltage), isCharging());
  }
}
