#ifndef SPEECHTOTEXT
#define SPEECHTOTEXT

#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

String speechToText() {
  WiFiClientSecure client;
  client.setCACert(root_ca);

  const String host = YANDEX_HOST;
  const String url = YANDEX_URL;
  const String auth_token = YANDEX_API;

  const String boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";

  const String body =
      "--" + boundary + "\r\n"
      "Content-Disposition: form-data; name=\"config\"\r\n"
      "Content-Type: application/json\r\n"
      "\r\n"
      "{"
      "  \"specification\": {"
      "    \"languageCode\": \"de-DE\","
      "    \"profanityFilter\": false,"
      "    \"model\": \"general\""
      "  }"
      "}\r\n"
      "--" + boundary + "\r\n"
      "Content-Disposition: form-data; name=\"audioContent\"; filename=\"audio.wav\"\r\n"
      "Content-Type: audio/wav\r\n"
      "\r\n";

  HTTPClient http;
  http.begin(client, host, 443, url);

  http.addHeader("Authorization", "Bearer " + auth_token);
  http.addHeader("Transfer-Encoding", "chunked");
  http.addHeader("Content-Type", "multipart/form-data; boundary=" + boundary);
  http.addHeader("User-Agent", "Mozilla/5.0");

  http.sendRequest("POST", "");

  http.print(body);

  // Read audio file from microphone
  AudioInI2S* mic = new AudioInI2S(i2s_bck_pin, i2s_ws_pin, i2s_data_pin);
  AudioInI2S& i2s_mic = *mic;
  AudioInQueue queue;
  AudioOutputI2S i2s_output(0, 1);
  AudioConnection patchCord1(i2s_mic, 0, queue, 0);
  AudioConnection patchCord2(i2s_mic, 1, queue, 1);
  AudioConnection patchCord3(queue, 0, i2s_output, 0);
  AudioConnection patchCord4(queue, 0, i2s_output, 1);

  // Wait for the microphone to settle
  delay(500);

  // Start recording audio
  i2s_output.begin();
  AudioStream::resumeAll();

  // Wait for recording to finish
  delay(10000);

  // Stop recording audio
  AudioStream::stopAll();
  i2s_output.end();

  http.print("\r\n--" + boundary + "--\r\n");
  http.endRequest();

  // Get response
  String response = http.getString();
  http.end();

  // Parse response
  const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + 220;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, response);
  const char* text = doc["result"]["chunks"][0]["alternatives"][0]["text"];
  return String(text);
}

#endif