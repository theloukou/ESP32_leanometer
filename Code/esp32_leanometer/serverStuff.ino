const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
unsigned long serverCheckTime, otaProgressMillis = 0;

#ifdef SERIAL_DEBUG
void onOTAStart() {
  Serial.println("OTA update started!");
}

void onOTAProgress(size_t current, size_t final) {
  if (millis() - otaProgressMillis > 1000) {
    otaProgressMillis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\r\n", current, final);
  }
}

void onOTAEnd(bool success) {
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
}
#endif

void initWiFi() {
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
#ifdef SERIAL_DEBUG
  Serial.println("AP online!");
  Serial.print("AP IP address: ");
  Serial.println(IP);
#endif
}

void stopServer() {
#ifdef SERIAL_DEBUG
  Serial.println("Server going down!");
#endif
//  server.end();
  WiFi.mode(WIFI_OFF);
}

bool checkServerStop() {
  if ((WiFi.softAPgetStationNum() == 0) && ((millis() - serverCheckTime) >= WIFI_TIMEOUT)) {
    stopServer();
    return true;
  }
  else {
    return false;
  }
}

void startServer() {
  initWiFi();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
#ifdef SERIAL_DEBUG
    Serial.println("Index loading;");
#endif
    request->send_P(200, "text/html", index_html);
  });

  server.on("/calibrateIMU", HTTP_GET, [](AsyncWebServerRequest * request) {
#ifdef SERIAL_DEBUG
    Serial.println("IMU cal triggered");
#endif
    request->send_P(200, "text/plain", "IMU cal triggered");
    serverCalTrig = true;
  });

  server.on("/getRTC", HTTP_GET, [](AsyncWebServerRequest * request) {
    DateTime currentTime = rtc.now();
    unsigned long unixTime = currentTime.unixtime();
    char rtcJson[50];
    sprintf(rtcJson, "{\"unixTime\":%d}", unixTime);

#ifdef SERIAL_DEBUG
    Serial.print("Sending RTC unix time: ");
    Serial.println(rtcJson);
#endif

    request->send_P(200, "application/json", rtcJson);
  });

  AsyncCallbackJsonWebHandler* JsonHandler = new AsyncCallbackJsonWebHandler("/syncRTC", [](AsyncWebServerRequest * request, JsonVariant & json) {
    StaticJsonDocument<200> jsonData;
    jsonData = json.as<JsonObject>();
    String response;
    serializeJson(jsonData, response);
#ifdef SERIAL_DEBUG
    Serial.println(response);
#endif
    rtc.adjust(DateTime(jsonData["Y"], jsonData["M"], jsonData["D"], jsonData["h"], jsonData["m"], jsonData["s"]));
#ifdef SERIAL_DEBUG
    Serial.println("New time set!");
#endif
    delay(100);
    request->send_P(200, "text/plain", "RTC Synced!");
  });

  server.addHandler(JsonHandler);
  ElegantOTA.begin(&server);
#ifdef SERIAL_DEBUG
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);
#endif
  server.begin();
  serverCheckTime = millis();
}
