#include <Watchy.h>
#include <WiFi.h>
#include "speechtotext.h"
#include "chatgpt.h"
#include "secrets.h"

class WatchyChatGPT : public Watchy
{
public:
    WatchyChatGPT() {};

protected:
    void handleButtonPress(uint64_t wakeupBit)
    {
        if (wakeupBit == UP_BUTTON_MASK)
        {
            // Record audio and convert speech to text
            String text = speechToText();

            // Send text to ChatGPT API and get response
            String response = getChatResponse();

            // Show response on the display
            display.fillScreen(GxEPD_WHITE);
            display.setCursor(0, 0);
            display.print(response);
            display.display();
        }
    }
};

WatchyChatGPT watchy;
