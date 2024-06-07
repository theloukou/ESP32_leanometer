void initWiFi() {
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
#ifdef SERIAL_DEBUG
  Serial.println("AP online!");
  Serial.print("AP IP address: ");
  Serial.println(IP);
#endif
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
  AsyncElegantOTA.begin(&server);
  server.begin();
}
