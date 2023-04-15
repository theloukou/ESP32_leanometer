void calCheck() {
  if (calTriggered) {
    buttonTime = millis();
    while (!digitalRead(CAL_BUTTON)) {
      if (millis() - buttonTime > 2000) {
        blinkDisp(5);
        delay(1000);
        IMUcalibration();
        maxAngle=0;
        blinkDisp(5);
      }
    }
    calTriggered = false;
    IMU.resetFIFO();
  }
}

void maxCheck() {
  if (maxTriggered) {
    buttonTime = millis();
    while (!digitalRead(MAX_BUTTON)) {
      if (millis() - buttonTime > 2000) {
        blinkDisp(2);
        Serial.println(maxAngle);
        updateDisp(abs(maxAngle));
        delay(3000);
        blinkDisp(2);
      }
    }
    maxTriggered = false;
    IMU.resetFIFO();
  }
}
