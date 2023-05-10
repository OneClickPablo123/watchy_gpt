#ifndef SPEECHTOTEXT_H
#define SPEECHTOTEXT_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include "secrets.h"
#include "Arduino.h"
#include <driver/i2s.h>
#include "config.h"

WiFiClientSecure client;
HttpClient http(client, GOOGLE_SPEECH_TO_TEXT_API_HOST, 443);

String speechToText()
{
    // Set up microphone
    i2s_config_t i2sConfig = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = I2S_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64
    };
    i2s_pin_config_t pinConfig = {
        .bck_io_num = MIC_I2S_BCK,
        .ws_io_num = MIC_I2S_WS,
        .data_out_num = -1,
        .data_in_num = MIC_I2S_DI
    };
    i2s_driver_install(I2S_NUM_0, &i2sConfig, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pinConfig);

    //Connect to Google API
    client.setCACert(GOOGLE_CERTIFICATE);
    client.setPrivateKey(GOOGLE_PRIVATE_KEY);

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

    size_t count = 0;
    while (count < SAMPLES_PER_FRAME)
    {
        bytesRead = i2s_read(I2S_NUM_0, samples, SAMPLES_PER_READ * sizeof(int16_t), &bytesWritten, portMAX_DELAY);
        if (bytesRead > 0)
        {
            count += bytesWritten / sizeof(int16_t);
            bytesWritten = http.write((const uint8_t*)samples, bytesRead);
        }
    }

    // Stop recording and clean up
    i2s_stop(I2S_NUM_0);
    i2s_driver_uninstall(I2S_NUM_0);

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
