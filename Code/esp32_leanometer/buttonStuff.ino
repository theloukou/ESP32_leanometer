void calCheck() {
  if (userButtonTriggered) {
    buttonTime = millis();
    while (!digitalRead(USER_BUTTON)) {
      if (millis() - buttonTime > 2000) {
        blinkDisp(5);
        delay(1000);
        IMUcalibration();
        maxAngle = 0;
        blinkDisp(5);
      }
    }
    userButtonTriggered = false;
    IMU.resetFIFO();
  }
  else if (serverCalTrig) {
    blinkDisp(5);
    delay(1000);
    IMUcalibration();
    maxAngle = 0;
    blinkDisp(5);
    serverCalTrig = false;
  }
}
