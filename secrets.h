#ifndef SECRETS_H
#define SECRETS_H

// WiFi credentials
const char* WIFI_SSID = "your_wifi_ssid";
const char* WIFI_PASSWORD = "your_wifi_password";

// Google API credentials
const char* GOOGLE_ACCESS_TOKEN = "your_google_access_token";
const char* GOOGLE_PRIVATE_KEY = "-----BEGIN PRIVATE KEY-----\n"
"your_google_private_key_here\n"
"-----END PRIVATE KEY-----\n";
const char* GOOGLE_CLIENT_CERT = "-----BEGIN CERTIFICATE-----\n"
"your_google_client_cert_here\n"
"-----END CERTIFICATE-----\n";
const char* GOOGLE_CERTIFICATE = "-----BEGIN CERTIFICATE-----\n"
"your_google_certificate_here\n"
"-----END CERTIFICATE-----\n";
const char* GOOGLE_SPEECH_TO_TEXT_API_HOST = "speech.googleapis.com";
const char* GOOGLE_SPEECH_TO_TEXT_API_ENDPOINT = "/v1/speech:recognize";

// ChatGPT API credentials
const char* CHATGPT_API_KEY = "your_chatgpt_api_key";

#endif //SECRETS_H
