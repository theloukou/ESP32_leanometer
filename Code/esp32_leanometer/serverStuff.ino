void initWiFi() {
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
#ifdef SERIAL_DEBUG
  Serial.println("AP online!");
  Serial.print("AP IP address: ");
  Serial.println(IP);
#endif

AsyncElegantOTA.begin(&server);
server.begin();
}
