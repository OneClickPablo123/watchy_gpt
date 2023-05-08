
#ifndef CHATGPT_H
#define CHATGPT_H

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include "speechtotext.h"

const String GPT_API_ENDPOINT = "https://api.openai.com/v1/engines/" + String(OPENAI_API_ENGINE) + "/completions";

String getChatResponse(String userText) {
    String result = "";

    // Build the request body for the GPT API
    StaticJsonDocument<512> requestBody;
    requestBody["prompt"] = speechToText();
    requestBody["max_tokens"] = 50;
    requestBody["temperature"] = 0.5;

    String requestBodyString;
    serializeJson(requestBody, requestBodyString);

    // Send the request to the GPT API
    HTTPClient httpClient;
    httpClient.begin(GPT_API_ENDPOINT);
    httpClient.addHeader("Content-Type", "application/json");
    httpClient.addHeader("Authorization", "Bearer " + String(CHATGPT_API_KEY));
    int httpCode = httpClient.POST(requestBodyString);

    // Parse the response from the GPT API
    if (httpCode > 0) {
        String responseBody = httpClient.getString();
        StaticJsonDocument<1024> responseJson;
        deserializeJson(responseJson, responseBody);
        result = responseJson["choices"][0]["text"].as<String>();
    }

    httpClient.end();
    return result;
}

#endif
