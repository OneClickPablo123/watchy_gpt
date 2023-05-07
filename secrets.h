#ifndef SECRET_H
#define SECRET_H

//WIFI PASS VARIABLE
const char* WIFI_SSID = "MeinWifiName";
const char* WIFI_PASS = "MeinWifiPasswort";

//CHATGPT VARIABLE
const char* GPT_HOST = "api.openai.com";
const char* GPT_PATH = "/v1/engines/davinci-codex/completions";
const char* GPT_TOKEN = "MeinOpenAIToken";

//GOOGLESPEECH VARIABLE
const char* YANDEX_API = "MeinYandexAPIKey";
const char* YANDEX_HOST = "speech-to-text-demo.api.cloud.yandex.net";
const char* YANDEX_URL = "/stt/v2/longRunningRecognize";


//ROOT CERTIFICAT
const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFZDCCBEygAwIBAgIQCgEBfCNgWtFhDyJwciS4/jANBgkqhkiG9w0BAQsFADBh\n" \
...
"FTfvGTrR\n" \
"-----END CERTIFICATE-----\n";

#endif