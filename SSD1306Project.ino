#include <WiFi.h>
#include <esp_wpa2.h>
#include <HTTPClient.h>

const char* enterpriseSSID = "ONE";
const char* username = "";
const char* password = "";

void connectToEnterpriseWiFi() {
  Serial.println("Connecting to Enterprise Wi-Fi...");

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  esp_wifi_sta_wpa2_ent_set_identity((uint8_t*)username, strlen(username));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t*)username, strlen(username));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t*)password, strlen(password));
  esp_wifi_sta_wpa2_ent_enable();

  WiFi.begin(enterpriseSSID);

  int tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (++tries > 30) {
      Serial.println("\nEnterprise Wi-Fi connection failed!");
      ESP.restart();
    }
  }

  Serial.println("\nConnected to Enterprise Wi-Fi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendHTTPRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    Serial.println("Sending HTTP GET request to http://tableflipped.xyz ...");

    http.begin("https://tableflipped.xyz"); // HTTP
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("HTTP GET response code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println("Response payload:");
      Serial.println(payload);
    } else {
      Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("Wi-Fi not connected. Cannot send HTTP request.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  connectToEnterpriseWiFi();
  sendHTTPRequest();  // Call the function after connection
}

void loop() {
  // Do nothing or implement further logic
}
