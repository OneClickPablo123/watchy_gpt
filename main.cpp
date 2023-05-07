#include "secrets.h"
#include "chatgpt.h"
#include "speechtotext.h"
#include "watchy_display.h"

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void loop() {
  String speech = recognize_speech();
  if (speech != "") {
    String response = chatGPT(speech);
    display_on_watch(response);
  }
  delay(1000);
}