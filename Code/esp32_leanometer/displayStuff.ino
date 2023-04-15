int lsdigit, msdigit;
unsigned int ldr_value;
byte bright_value;

//common anode 7sdu
#ifdef CA_7SDU
const uint8_t  segNumbers[] = {B11000000, //0
                               B11111001, //1
                               B10100100, //2
                               B10110000, //3
                               B10011001, //4
                               B10010010, //5
                               B10000011, //6
                               B11111000, //7
                               B10000000, //8
                               B10011000  //9
                              };
#endif
//common cathode 7sdu
#ifdef CC_7SDU
const uint8_t  segNumbers[] = {B00111111, //0
                               B00000110, //1
                               B01011011, //2
                               B01001111, //3
                               B01100110, //4
                               B01101101, //5
                               B01111100, //6
                               B00000111, //7
                               B01111111, //8
                               B01100111  //9
                              };
#endif

void updateDisp(int angle) {
  lsdigit = segNumbers[angle % 10] ;
  msdigit = segNumbers[(angle / 10) % 10] ;
#ifdef CA_7SDU
  if (msdigit == segNumbers[0]) msdigit = B11111111;
#endif
#ifdef CC_7SDU
  if (msdigit == segNumbers[0]) msdigit = B00000000;
#endif
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
  ldr_value = (analogRead(LDR));
  if (ldr_value <= LDR_MIN_THRES) {
    bright_value = 10;
  }
  else if (ldr_value <= LDR_MAX_THRES) {
    bright_value = map(ldr_value, LDR_MIN_THRES+1, LDR_MAX_THRES, 11, 255);
  }
  else {
    bright_value = 255;
  }
  ledcWrite(PWM_CHAN, bright_value);
//  Serial.print(ldr_value);
//  Serial.print("\t");
//  Serial.println(bright_value);
}
