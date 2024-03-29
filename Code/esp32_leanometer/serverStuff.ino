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

  //  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
  //#ifdef SERIAL_DEBUG
  //    Serial.println("Index loading;");
  //#endif
  //    request->send_P(200, "text/html", index_html);
  //  });

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
#ifdef SERIAL_DEBUG
    Serial.println("An Error has occurred while mounting SPIFFS");
#endif
    return;
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
#ifdef SERIAL_DEBUG
    Serial.println("Index loading;");
#endif
    request->send(SPIFFS, "/index_html");
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/styles.css", "text/css");
  });

  AsyncElegantOTA.begin(&server);
  server.begin();
}
