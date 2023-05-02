#include "Definitions.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "Preferences.h"
#include "src/ShiftRegister74HC595.h"
#include "src/I2Cdev.h"
#include "src/MPU6050_6Axis_MotionApps612.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

ShiftRegister74HC595<2> disp(SHIFT_REG_DATA, SHIFT_REG_CLOCK, SHIFT_REG_LATCH);
MPU6050 IMU;
Preferences prefs;

// MPU control/status vars
bool IMUReady = false;  // set true if DMP init was successful
uint8_t IMUdevStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t IMUpacketSize;    // expected DMP packet size (default is 42 bytes)
uint16_t IMUfifoCount;     // count of all bytes currently in FIFO
uint8_t IMUfifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion IMUq;               // [w, x, y, z] quaternion container
VectorFloat IMUgravity;        // [x, y, z]  gravity vector
float IMUypr[3] = {0, 0, 0};   // [yaw, pitch, roll] yaw/pitch/roll container and gravity vector

int xAccelOffset, yAccelOffset, zAccelOffset, xGyroOffset, yGyroOffset, zGyroOffset;
String serialStr;
double angle, maxAngle = 0;
bool calTriggered = false, maxTriggered = false;
unsigned long buttonTime;

void IRAM_ATTR calButton() {
  calTriggered = true;
}

void IRAM_ATTR maxButton() {
  maxTriggered = true;
}

void setup() {
#ifdef CA_7SDU
  disp.setAllHigh();
#endif

#ifdef CC_7SDU
  disp.setAllLow();
#endif

  serialStr.reserve(40);
  Serial.begin(115200);

  pinMode(CAL_BUTTON, INPUT_PULLUP);
  pinMode(BRIGHTNESS_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CAL_BUTTON), calButton, FALLING);

  ledcSetup(PWM_CHAN, PWM_FREQ, PWM_RES);
  ledcAttachPin(BRIGHTNESS_PIN, PWM_CHAN);

  // initialize EEPROM with predefined size
  prefs.begin(PREF_NAMESPACE, false);
  eeprom_get();

  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  Wire.setClock(100000);

  //initialise IMU
  IMUsetup();

  //initialize SD
  sdInit();

  Serial.println("Ready!");

  ledcWrite(PWM_CHAN, 25);

  //reset IMU FIFOs for clean start
  IMU.resetFIFO();
}

void loop() {
  // if IMU programming failed, don't try to do anything
  if (!IMUReady) return;  //halt if MPU is missing

  IMUdata();
  //  Serial.println(angle);
  updateDisp(abs(angle));

  if (abs(angle) > maxAngle) {
    maxAngle = abs(angle);
  }

  brightness();

  calCheck();
}
