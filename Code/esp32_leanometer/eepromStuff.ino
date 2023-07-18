void offsetsGet() {
  xAccelOffset = prefs.getShort("xAccelOffset", 0);
  yAccelOffset = prefs.getShort("yAccelOffset", 0);
  zAccelOffset = prefs.getShort("zAccelOffset", 0);
  xGyroOffset = prefs.getShort("xGyroOffset", 0);
  yGyroOffset = prefs.getShort("yGyroOffset", 0);
  zGyroOffset = prefs.getShort("zGyroOffset", 0);
}

void offsetsPut() {
  prefs.putShort("xAccelOffset", IMU.getXAccelOffset());
  prefs.putShort("yAccelOffset", IMU.getYAccelOffset());
  prefs.putShort("zAccelOffset", IMU.getZAccelOffset());
  prefs.putShort("xGyroOffset", IMU.getXGyroOffset());
  prefs.putShort("yGyroOffset", IMU.getYGyroOffset());
  prefs.putShort("zGyroOffset", IMU.getZGyroOffset());
}

void logNumGet(){
  logNum = prefs.getUShort("logNum", 0);
}

void logNumPut(){
  prefs.putUShort("logNum", logNum);
}
