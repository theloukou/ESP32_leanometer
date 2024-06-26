// Default libs
#include "FS.h"
#include "SD.h"
#include "SPIFFS.h"
#include "SPI.h"
#include "Preferences.h"
#include "WiFi.h"
#include "esp32/rom/rtc.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

// Libs/files in sketch folder
#include "Definitions.h"
#include "src/ShiftRegister74HC595.h"
#include "src/I2Cdev.h"
#include "src/MPU6050_6Axis_MotionApps612.h"
#include "src/ElegantOTA.h"

// Libs that need to be installed in IDE
#include "RTClib.h"
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"

bool serverStopped = false;
AsyncWebServer server(80);

ShiftRegister74HC595<2> disp(SHIFT_REG_DATA, SHIFT_REG_CLOCK, SHIFT_REG_LATCH);
MPU6050 IMU(0x69);
RTC_PCF8523 rtc;
Preferences prefs;

// MPU control/status vars
bool IMUReady = false;      // set true if DMP init was successful
uint8_t IMUdevStatus;       // return status after each device operation (0 = success, !0 = error)
uint16_t IMUpacketSize;     // expected DMP packet size (default is 42 bytes)
uint16_t IMUfifoCount;      // count of all bytes currently in FIFO
uint8_t IMUfifoBuffer[64];  // FIFO storage buffer

// orientation/motion vars
Quaternion IMUq;              // [w, x, y, z] quaternion container
VectorInt16 IMUaa;            // [x, y, z] accel sensor measurements
VectorInt16 IMUgy;            // [x, y, z] gyro sensor measurements
VectorInt16 IMUaaReal;        // [x, y, z] gravity-free accel sensor measurements
VectorInt16 IMUaaWorld;       // [x, y, z] world-frame accel sensor measurements
VectorFloat IMUgravity;       // [x, y, z] gravity vector
float IMUypr[3] = {0, 0, 0};  // [yaw, pitch, roll] yaw/pitch/roll container and gravity vector

int xAccelOffset, yAccelOffset, zAccelOffset, xGyroOffset, yGyroOffset, zGyroOffset;
String serialStr;
double angle, maxAngle = 0;
volatile bool userButtonTriggered = false, bootButtonTriggered = false, sdDetectionTriggered = false, IMUintTriggered = false, serverCalTrig = false;
unsigned long buttonTime;
unsigned int logNum;

// ISR functions
void IRAM_ATTR userButtonISR() {
  userButtonTriggered = true;
}

void IRAM_ATTR bootButtonISR() {
  bootButtonTriggered = true;
}

void IRAM_ATTR sdDetectionISR() {
  sdDetectionTriggered = true;
}

void IRAM_ATTR IMUdataReadyISR() {
  IMUintTriggered = true;
}

void setup() {
#ifdef CA_7SDU
  disp.setAllHigh();
#endif

#ifdef CC_7SDU
  disp.setAllLow();
#endif

#ifdef SERIAL_DEBUG
  serialStr.reserve(40);
  Serial.begin(115200);
#endif

  analogReadResolution(10);
  pinMode(USER_BUTTON, INPUT_PULLUP);
  pinMode(BRIGHTNESS_PIN, OUTPUT);
  pinMode(SD_DET, INPUT_PULLUP);
  pinMode(IMU_INT, INPUT);
  attachInterrupt(digitalPinToInterrupt(USER_BUTTON), userButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BOOT_BUTTON), bootButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(SD_DET), sdDetectionISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IMU_INT), IMUdataReadyISR, RISING);
  analogReadResolution(10);

  ledcSetup(PWM_CHAN, PWM_FREQ, PWM_RES);
  ledcAttachPin(BRIGHTNESS_PIN, PWM_CHAN);

  // initialize EEPROM with predefined size
  prefs.begin(PREF_NAMESPACE, false);
  offsetsGet();

  // Setup wireless coms
  btStop();
  startServer();

  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  Wire.setClock(I2C_FREQ);

  //initialise RTC
  rtcInit();

  //initialise IMU
  IMUsetup();

  //set display orientation
  //  for (byte index=0;index<100;index++){
  //    IMUdata();
  //  }
  //  orientateDisp();

  //check for SD card
  sdSetupCheck();

#ifdef SERIAL_DEBUG
  Serial.println("Ready!");
#endif

  //show vBat
  brightness();
  batToDisp();

  //reset IMU FIFOs for clean start
  IMU.resetFIFO();
}

void loop() {
  if (!serverStopped) {
    ElegantOTA.loop();
    serverStopped = checkServerStop();
  }
  
  // if IMU programming failed, don't try to do anything
  if (!IMUReady) {
    return;  //halt if MPU is missing
  }

  if (IMUintTriggered) {
    IMUintTriggered = false;
    IMUdata();
    IMUangles();
    //    IMUgforces();
    angleToDisp(angle);
    //    sdHandleLogs();
  }

  if (abs(angle) > maxAngle) {
    maxAngle = abs(angle);
  }

  brightness();
  calCheck();
  sdDetection();
}
