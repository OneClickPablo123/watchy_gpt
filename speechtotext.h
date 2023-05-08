#ifndef SPEECHTOTEXT_H
#define SPEECHTOTEXT_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include "secrets.h"
#include "Arduino.h"
#include <Adafruit_I2S.h>
#include "config.h"

WiFiClientSecure client;
HttpClient http(client, GOOGLE_SPEECH_TO_TEXT_API_HOST, 443);

String speechToText()
{
    // Set up microphone
    Adafruit_I2S i2s(MIC_I2S_BCK, MIC_I2S_WS, MIC_I2S_DO, MIC_I2S_DI);

    //Connect to Google API
    client.setCACert(GOOGLE_CERTIFICATE);
    client.setPrivateKey(GOOGLE_PRIVATE_KEY);
    client.setCertificate(GOOGLE_CLIENT_CERT);

    String authString = "Bearer " + String(GOOGLE_ACCESS_TOKEN);
    http.begin(GOOGLE_SPEECH_TO_TEXT_API_ENDPOINT);
    http.addHeader("Content-Type", "audio/wav");
    http.addHeader("Authorization", authString);
    // Set up request headers
    http.addHeader("Content-Type", "audio/wav");
    http.addHeader("Content-Encoding", "gzip");
    http.addHeader("User-Agent", "SpeechToTextESP32");

    // Record audio
    int16_t samples[SAMPLES_PER_FRAME];
    size_t bytesRead = 0;
    size_t bytesWritten = 0;

    i2s.begin(I2S_SAMPLE_RATE);
    size_t count = 0;
    while (count < SAMPLES_PER_FRAME)
    {
        bytesRead = i2s.read(samples, SAMPLES_PER_READ);
        if (bytesRead > 0)
        {
            bytesWritten = http.write((const uint8_t*)samples, bytesRead);
            count += bytesWritten / 2;
        }
    }
    i2s.end();

    // Send request
    http.endRequest();

    // Check response
    int httpCode = http.responseStatusCode();
    if (httpCode != 200)
    {
        Serial.printf("Error sending request, HTTP status code: %d\n", httpCode);
        return "";
    }

    // Parse response
    String response = http.responseBody();
    JSONVar json = JSON.parse(response);

    String text = "";
    if (JSON.typeof(json) == "object")
    {
        text = JSON.parse(response)["results"][0]["alternatives"][0]["transcript"];
    }

    return text;
}

#endif
