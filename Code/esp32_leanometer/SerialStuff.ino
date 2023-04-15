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
}
