#ifndef CHATGPT
#define CHATGPT

#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "Watchy_main.ino"

String chatGPT(String prompt) {
  WiFiClientSecure client;
  client.setCACert(root_ca);

  HTTPClient http;
  http.begin(client, "https://" + chat_api_host + chat_api_path);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + chat_api_token);

  // Build JSON request payload
  DynamicJsonDocument payload(2048);
  payload["prompt"] = prompt;
  payload["temperature"] = 0.7;
  payload["max_tokens"] = 50;
  payload["stop"] = "\n";

  String payload_str;
  serializeJson(payload, payload_str);
  Serial.println(payload_str);

  int httpCode = http.POST(payload_str);

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println(response);

      DynamicJsonDocument json_resp(4096);
      deserializeJson(json_resp, response);
      String choices_0_text = json_resp["choices"][0]["text"];
      return choices_0_text;
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
  } else {
    Serial.printf("[HTTP] Unable to connect\n");
  }

  http.end();
  return "";
}

#endif