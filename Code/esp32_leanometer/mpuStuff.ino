void IMUsetup() {
  // initialize device
  IMU.initialize();

  // verify connection
#ifdef SERIAL_DEBUG
  Serial.println(IMU.testConnection() ? F("IMU connection successful") : F("IMU connection failed"));
#endif

  // load and configure the DMP
  IMUdevStatus = IMU.dmpInitialize();

  // set IMU offsets
  IMU.setXAccelOffset(xAccelOffset);
  IMU.setYAccelOffset(yAccelOffset);
  IMU.setZAccelOffset(zAccelOffset);
  IMU.setXGyroOffset(xGyroOffset);
  IMU.setYGyroOffset(yGyroOffset);
  IMU.setZGyroOffset(zGyroOffset);
  IMU.PrintActiveOffsets();

  // make sure it worked (returns 0 if so)
  if (IMUdevStatus == 0) {
    // turn on the DMP, now that it's ready
    IMU.setDMPEnabled(true);

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
#ifdef SERIAL_DEBUG
    Serial.println(F("IMU DMP ready!"));
#endif
    IMUReady = true;

    // get expected DMP packet size for later comparison
    IMUpacketSize = IMU.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
#ifdef SERIAL_DEBUG
    Serial.print(F("IMU DMP Initialization failed (code "));
    Serial.print(IMUdevStatus);
    Serial.println(F(")"));
#endif
  }
}

void IMUcalibration() {
#ifdef SERIAL_DEBUG
  Serial.println("Calibrating");
#endif
  delay(3000);
  IMU.CalibrateAccel(6);
  IMU.CalibrateGyro(6);
#ifdef SERIAL_DEBUG
  IMU.PrintActiveOffsets();
#endif
  eepromPut();
}

//get IMU data
void IMUdata() {
  // get current IMU FIFO count
  IMUfifoCount = IMU.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if (IMUfifoCount >= 1024) {
    // reset so we can continue cleanly
    IMU.resetFIFO();
#ifdef SERIAL_DEBUG
    Serial.println(F("IMU FIFO overflow!"));
#endif
  }
  // otherwise, check for DMP data ready interrupt (this should happen frequently)
  else if (IMUfifoCount >= 42) {
    // read a packet from FIFO
    IMU.getFIFOBytes(IMUfifoBuffer, IMUpacketSize);
  }
}

//calculate needed angles
void IMUangles() {
  IMUdata();

  // calculate Euler angles in degrees
  IMU.dmpGetQuaternion(&IMUq, IMUfifoBuffer);
  IMU.dmpGetGravity(&IMUgravity, &IMUq);
  IMU.dmpGetYawPitchRoll(IMUypr, &IMUq, &IMUgravity);

  //get desired used angles
  angle = IMUypr[2] * 180 / M_PI;
}

//calculate g-forces
void IMUgforces() {
  IMUdata();

  // display initial world-frame acceleration, adjusted to remove gravity and rotated based on known orientation from quaternion
  IMU.dmpGetQuaternion(&IMUq, IMUfifoBuffer);
  IMU.dmpGetAccel(&IMUaa, IMUfifoBuffer);
  IMU.dmpGetGravity(&IMUgravity, &IMUq);
  IMU.dmpGetLinearAccel(&IMUaaReal, &IMUaa, &IMUgravity);
  IMU.dmpGetLinearAccelInWorld(&IMUaaWorld, &IMUaaReal, &IMUq);
  Serial.print("aworld\t");
  Serial.print(IMUaaWorld.x);
  Serial.print("\t");
  Serial.print(IMUaaWorld.y);
  Serial.print("\t");
  Serial.println(IMUaaWorld.z);
}
