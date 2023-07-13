bool sdPresent = false;

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
  SD.end();
  Serial.println("Sd card de-initialized!");
}

void sdDetection() {
  if (sdDetectionTriggered) {
    sdDetectionTriggered = false;
    sdPresent = digitalRead(SD_DET);
    if (sdPresent) {
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
    }
  }
}
