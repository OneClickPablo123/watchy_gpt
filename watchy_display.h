#ifndef WATCHY_DISPLAY_H
#define WATCHY_DISPLAY_H

#include <Watchy.h>

void display_on_watch(String text) {
  Watchy::display.fillScreen(GxEPD_WHITE);
  Watchy::display.setCursor(0, 0);
  Watchy::display.print(text);
  Watchy::display.display(true);
}

#endif