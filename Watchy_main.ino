#include "Watchy.h"
#include "secrets.h"
#include "chatgpt.h"
#include "speechtotext.h"

void setup() {
  Watchy::init();
  Watchy::rtc.begin();
  Watchy::WiFiOn();
  Watchy::connectWiFi(WIFI_SSID, WIFI_PASS);
}

void loop() {
 Watchy::showWatchFace();
  if (digitalRead(WATCHY_BUTTON) == LOW) {
    string speech = speechToText(text)
    string response = chatGPT(speech)
    display_on_watch(response);
    
  } else {
    Watchy::showWatchFace();
  }
  delay(100);
}
