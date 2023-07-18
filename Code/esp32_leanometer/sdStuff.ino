bool sdPresent = false;
File activeLogFile;
#define TEST_PATH "/logs/abcde.txt"

void sdSetupCheck() {
  sdPresent = !digitalRead(SD_DET);
  if (sdPresent) {
    sdInit();
  }
#ifdef SERIAL_DEBUG
  else {
    Serial.println("No SD card attached");
  }
#endif
}

void sdInit() {
  if (!SD.begin(SD_SS, SPI, 20000000)) {
    //  if (!SD.begin()) {
#ifdef SERIAL_DEBUG
    Serial.println("SD card Mount Failed");
#endif
    return;
  }
  uint8_t cardType = SD.cardType();

#ifdef SERIAL_DEBUG
  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
#endif
}

void sdDeint() {
  SD.end();
#ifdef SERIAL_DEBUG
  Serial.println("SD card de-initialized!");
#endif
}

void sdDetection() {
  if (sdDetectionTriggered) {
    sdDetectionTriggered = false;
    sdPresent = !digitalRead(SD_DET);
    if (!sdPresent) {
      sdLogFileClose(&activeLogFile);
      sdDeint();
#ifdef SERIAL_DEBUG
      Serial.println("SD card removed!");
#endif
    }
    else {
#ifdef SERIAL_DEBUG
      Serial.println("SD card inserted!!");
#endif
      sdInit();
      //      sdTest();
      sdLogFileOpen(&activeLogFile);
    }
  }
}

int8_t sdLogFileOpen(File* file) {
  logNumGet();
  char logPath[50];
  sprintf(logPath, "%s%05u.csv", LOG_PATH, logNum);

#ifdef SERIAL_DEBUG
  Serial.printf("Starting new log file in %s\r\n", logPath);
#endif
  *file = SD.open(logPath, FILE_WRITE);

  if (!file) {
#ifdef SERIAL_DEBUG
    Serial.println("Failed to open file for writing");
#endif
    return -1;
  }
  else {
    file->print("timestamp,y,p,r,gX,gY,gZ\n");
    return 0;
  }
}

void sdLogFileClose(File* file) {
#ifdef SERIAL_DEBUG
  Serial.printf("Total log size written: %u Bytes\r\n", file->size());
#endif
  file->close();
  logNum++;
  logNumPut();
}

void sdLogFileWrite(File* file) {
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  char buf[100];
  //  int8_t bufLen = sprintf(buf, "%u,%.2f,%.2f,%.2f,%d,%d,%d\n", random(0,1000),IMUypr[0],IMUypr[1],IMUypr[2],IMUaaWorld.x,IMUaaWorld.y,IMUaaWorld.z);
  int8_t bufLen = sprintf(buf, "%u,%.2f,%.2f,%.2f,%d,%d,%d\n", millis(), IMUypr[0], IMUypr[1], IMUypr[2], IMUaaReal.x, IMUaaReal.y, IMUaaReal.z);
  file->print(buf);
}

void sdHandleLogs() {
  if (sdPresent) {
    sdLogFileWrite(&activeLogFile);
  }
}

void sdTest() {
  static uint8_t buf[512];
  File testFile = SD.open("/test.txt", FILE_WRITE);
  if (!testFile) {
    Serial.println("Failed to open file for writing");
    return;
  }

  uint32_t start = millis();
  uint32_t end = start;
  start = millis();
  for (uint16_t i = 0; i < 2048; i++) {
    testFile.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  testFile.close();
}
