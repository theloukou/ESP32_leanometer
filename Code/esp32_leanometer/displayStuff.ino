int lsdigit, msdigit;
unsigned int ldrValue;
uint8_t brightValue, angleOffset, brightnessCount=0;
bool dispFlip=0;

//segment format: B x x x x x x x x
//                  P G F E D C B A
//
//                  A
//                F G B
//                E   C
//                  D   P


//{normal orientation, flipped orientation}
//common anode 7sdu
#ifdef CA_7SDU
const uint8_t  segNumbers[10][2] = {{B11000000, 0xFF}, //0
                                   {B11111001, 0xFF}, //1
                                   {B10100100, 0xFF}, //2
                                   {B10110000, 0xFF}, //3
                                   {B10011001, 0xFF}, //4
                                   {B10010010, 0xFF}, //5
                                   {B10000011, 0xFF}, //6
                                   {B11111000, 0xFF}, //7
                                   {B10000000, 0xFF}, //8
                                   {B10011000, 0xFF} //9
                                  };
#endif
//common cathode 7sdu
#ifdef CC_7SDU
const uint8_t  segNumbers[10][2] = {{B00111111, B00111111}, //0
                                   {B00000110, B00110000}, //1
                                   {B01011011, B01011011}, //2
                                   {B01001111, B01111001}, //3
                                   {B01100110, B01110100}, //4
                                   {B01101101, B01101101}, //5
                                   {B01111100, B01100111}, //6
                                   {B00000111, B00111000}, //7
                                   {B01111111, B01111111}, //8
                                   {B01100111, B01111100}  //9
                                  };
#endif

void orientateDisp(){
  int16_t ax, ay, az;
  IMU.getAcceleration(&ax, &ay, &az);
  
  if (az > 0){
    dispFlip = 0;
    angleOffset = 0;
  }
  else{
    dispFlip = 1;
    angleOffset = 180;
#ifdef SERIAL_DEBUG
    Serial.printf("Ax,Ay,Az: %d,%d,%d \r\n", ax, ay, az);
    Serial.printf("Display flip: %d \r\n", dispFlip);
#endif
  }
}

void batToDisp(){
  uint32_t vBat = getBat();
  uint16_t vBatInt = vBat /1000 ;
  uint16_t vBatdec = (vBat % 1000) / 10 ;
#ifdef SERIAL_DEBUG
  Serial.printf("int-dec: %d-%d \r\n", vBatInt, vBatdec);
#endif
  splitDigits(vBatInt);
  setDot(&lsdigit);
  updateDisp();
  delay(1500);
  splitDigits(vBatdec);
  updateDisp();
  delay(1500);
}

void angleToDisp(int angle){
  int fAngle = abs(angle - angleOffset);
  splitDigits(fAngle);
#ifdef CA_7SDU
  if (msdigit == segNumbers[0][dispFlip]) msdigit = B11111111;
#endif
#ifdef CC_7SDU
  if (msdigit == segNumbers[0][dispFlip]) msdigit = B00000000;
#endif
  updateDisp();
}

void setDot(int* digit){
  #ifdef CA_7SDU
  *digit &= B01111111;
#endif
#ifdef CC_7SDU
  *digit |= B10000000;
#endif
}

void splitDigits(int twoDigits){
  lsdigit = segNumbers[twoDigits % 10][dispFlip] ;
  msdigit = segNumbers[(twoDigits / 10) % 10][dispFlip] ;
}

void updateDisp() {
  uint8_t numberToPrint[] = {msdigit, lsdigit};
  disp.setAll(numberToPrint);
}

//Blink
void blinkDisp(char times) {
  for (int i = 0; i < times; i++) {
#ifdef CA_7SDU
    disp.setAllLow(); // set all pins Low (off)
    delay(50);
    disp.setAllHigh(); // set all pins High (on)
    delay(50);
#endif

#ifdef CC_7SDU
    disp.setAllHigh(); // set all pins Low (off)
    delay(50);
    disp.setAllLow(); // set all pins High (on)
    delay(50);
#endif
  }
}

void brightness() {
  if (brightnessCount == 0 ){
    ldrValue = (analogRead(LDR));
    if (ldrValue <= LDR_MIN_THRES) {
      brightValue = 10;
    }
    else if (ldrValue <= LDR_MAX_THRES) {
      brightValue = map(ldrValue, LDR_MIN_THRES+1, LDR_MAX_THRES, 11, 255);
    }
    else {
      brightValue = 255;
    }
    ledcWrite(PWM_CHAN, brightValue);
  }
  brightnessCount++;
}
