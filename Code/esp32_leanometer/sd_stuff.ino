bool sdPresent = false;
File activeLogFile;
#define LOG_PATH "/logs/0001.txt"

void sdSetupCheck() {
  if (!digitalRead(SD_DET)) {
    sdInit();
  }
#ifdef SERIAL_DEBUG
  else {
    Serial.println("No SD card attached");
  }
#endif
}

void sdInit() {
  //  if (!SD.begin(SD_SS, SPI, 8000000)) {
  if (!SD.begin()) {
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
  sdLogFileClose(&activeLogFile);
  SD.end();
  Serial.println("SD card de-initialized!");
}

void sdDetection() {
  if (sdDetectionTriggered) {
    sdDetectionTriggered = false;
    sdPresent = digitalRead(SD_DET);
    if (sdPresent) {
      sdDeint();
      !sdPresent;
#ifdef SERIAL_DEBUG
      Serial.println("SD card removed!");
#endif
    }
    else {
      !sdPresent;
#ifdef SERIAL_DEBUG
      Serial.println("SD card inserted!!");
#endif
      sdInit();
      sdLogFileOpen(&activeLogFile);
    }
  }
}

int8_t sdLogFileOpen(File* file) {
  *file = SD.open(LOG_PATH, FILE_WRITE);

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
  file->close();
}

void sdLogFileWrite(File* file, double data) {
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  char buf[100];
  //  int8_t bufLen = sprintf(buf, "%u,%.2f,%.2f,%.2f,%d,%d,%d\n", random(0,1000),IMUypr[0],IMUypr[1],IMUypr[2],IMUaaWorld.x,IMUaaWorld.y,IMUaaWorld.z);
  int8_t bufLen = sprintf(buf, "%u,%.2f,%.2f,%.2f,%d,%d,%d\n", random(0, 1000), IMUypr[0], IMUypr[1], IMUypr[2], IMUaaReal.x, IMUaaReal.y, IMUaaReal.z);
  //  file->write(buf, bufLen);
  file->print(buf);

  //  static uint8_t buf[512];
  //  uint32_t start = millis();
  //  uint32_t end = start;
  //  start = millis();
  //  for (int i = 0; i < 2048; i++) {
  //    file->write(buf, 512);
  //  }
  //  end = millis() - start;
  //  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
}

void sdHandleLogs() {
  if (!sdPresent) {
    sdLogFileWrite(&activeLogFile, angle);
  }
}
