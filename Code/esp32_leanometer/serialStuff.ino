//local Serial vars
unsigned long serTime;

//incoming serial data routine
void serialEvent() {
  //reset serial input string
  serialStr = "";

  //wait a bit for whole command to come through
  serTime = millis();
  while (millis() - serTime < 100) {}   //100 msec

  //read serial command string
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    serialStr += inChar;
  }

  if (serialStr == "calibrate") {
    IMUcalibration();
  }
  else if (serialStr == "reset log filename") {
    logNum = 0;
    logNumPut();
  }
  // Adjust time. rtc:YYYY.MM.DD.hh.mm.ss:end
  else if (serialStr.startsWith("rtc:") && (serialStr.endsWith(":end"))) {
    rtc.adjust(DateTime(serialStr.substring(4, 8).toInt(),
                        serialStr.substring(9, 11).toInt(),
                        serialStr.substring(12, 14).toInt(),
                        serialStr.substring(15, 17).toInt(),
                        serialStr.substring(18, 20).toInt(),
                        serialStr.substring(21, 23).toInt()));
#ifdef SERIAL_DEBUG
    Serial.println("New time set!");
#endif
  }
#ifdef SERIAL_DEBUG
  else if (serialStr == "get unix time") {
    Serial.println(rtcGetUnixTime());
  }
  else if (serialStr == "get log filename") {
    logNumGet();
    Serial.printf("Current log filename: %05u.txt\r\n", logNum);
  }
  else {
    Serial.println("Unknown command!");
  }
#endif
}
