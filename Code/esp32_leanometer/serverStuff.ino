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

  // Initialize SPIFFS
  //  if (!SPIFFS.begin(true)) {
  //#ifdef SERIAL_DEBUG
  //    Serial.println("An Error has occurred while mounting SPIFFS");
  //#endif
  //    return;
  //  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
#ifdef SERIAL_DEBUG
    Serial.println("Index loading;");
#endif
    request->send_P(200, "text/html", index_html);
  });

  AsyncCallbackJsonWebHandler* JsonHandler = new AsyncCallbackJsonWebHandler("/rtc", [](AsyncWebServerRequest * request, JsonVariant & json) {
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
