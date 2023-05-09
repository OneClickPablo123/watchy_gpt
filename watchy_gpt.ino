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
    void handleButtonPress(uint64_t wakeupBit);
    void drawWatchFace();
};

WatchyChatGPT watchy;

void WatchyChatGPT::handleButtonPress(uint64_t wakeupBit)
{
    if (wakeupBit == UP_BUTTON_MASK)
    {
        // Record audio and convert speech to text
        String text = speechToText();

        // Send text to ChatGPT API and get response
        String response = getChatResponse(text);

        // Show response on the display
        watchy.display.fillScreen(GxEPD_WHITE);
        watchy.display.setCursor(0, 0);
        watchy.display.print(response);
        watchy.display.display();
    }
    else
    {
        // Redraw the watchface on any other button press
        drawWatchFace();
    }
}

void WatchyChatGPT::drawWatchFace()
{
    // Draw your custom watchface here
    watchy.display.fillScreen(GxEPD_WHITE);
    watchy.display.setCursor(0, 0);
    watchy.display.println("My WatchyFace");
    watchy.display.display();
}

void setup()
{
    Serial.begin(9600);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");

    watchy.init();
    drawWatchFace(); // Display the watchface on startup
}

void loop()
{
    watchy.handleButtonPress(watchy.readButtons());
    delay(100);
}
