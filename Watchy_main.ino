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
  Watchy::showMenu();
}

void menuCallback(int index) {
  switch (index) {
    case 0: {
      // Chat with GPT
      Watchy::drawMenu("Chat with GPT", "Press back to exit");
      Watchy::display.show();
      String message = "";
      String response = "";
      while (1) {
        response = chatGPT(message);
        if (response == "") break;
        Watchy::drawChatMessage(response, true);
        Watchy::display.show();
        message = Watchy::getChatInput();
        if (message == "") break;
        Watchy::drawChatMessage(message, false);
        Watchy::display.show();
      }
      break;
    }
    case 1: {
      // Convert Speech to Text
      Watchy::drawMenu("Speech to Text", "Press back to exit");
      Watchy::display.show();
      String text = googglespeech(YANDEX_API, YANDEX_HOST, YANDEX_URL);
      Watchy::drawSpeechToTextResult(text);
      Watchy::display.show();
      break;
    }
    case 2: {
      // Add more options as needed
      break;
    }
    default:
      break;
  }
}