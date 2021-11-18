#include "ota.hpp"

void onOtaStarted()
{
    msgBig("OTA");
}

void onOtaProgress(unsigned int progress, unsigned int total)
{
    int percentage = (progress / (total / 100));
    drawProgressBar(10, 30, 120, 15, percentage, TFT_WHITE, TFT_GREENYELLOW);
}