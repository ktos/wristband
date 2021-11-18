#include "pages/page-wifiscan.hpp"

void pageWifiScan(bool initialLoad)
{
    if (initialLoad)
    {
        msgInfo("Scan networks", "Press action");
    }
}

void actionWifiScan()
{
    msg("Scanning...", TFT_GREEN);
    char buff[256];

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    int16_t n = WiFi.scanNetworks();

    beginPrintln(TFT_GREEN);
    if (n == 0)
    {
        msgInfo("no networks found");
    }
    else
    {
        for (int i = 0; i < n; ++i)
        {
            sprintf(buff,
                    "[%d]:%s(%d)",
                    i + 1,
                    WiFi.SSID(i).c_str(),
                    WiFi.RSSI(i));
            println(buff);
        }
    }
    WiFi.mode(WIFI_OFF);

    delay(10000);
}
