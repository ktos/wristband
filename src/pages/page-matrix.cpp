#include <DigitalRainAnim.h>
#include "pages/page-matrix.hpp"
#include "wristband-tft.hpp"

DigitalRainAnim digitalRainAnim = DigitalRainAnim();

void pageMatrix(bool initialLoad)
{
    if (initialLoad)
    {
        digitalRainAnim.init(getTft());
    }

    digitalRainAnim.loop();
}