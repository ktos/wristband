#include <Arduino.h>
#include <EasyButton.h>
#include <Mokosh.hpp>
#include "wristband-tft.hpp"
#include "pages/page-clock.hpp"
#include "pages/page-androtime.hpp"
#include "pages/page-battery.hpp"
#include "pages/page-bearing.hpp"
#include "pages/page-temperature.hpp"
#include "pages/page-wifiscan.hpp"
#include "pages/page-matrix.hpp"

void handleUi();
void increasePage();
void showPage();
void initButton();
void handleAction();
void initMokosh(Mokosh* m);