//void eeprom_get() {
//  int eeAddress = 0;
//  EEPROM.get(eeAddress, xAccelOffset);
//  eeAddress += sizeof(int16_t);
//  EEPROM.get(eeAddress, yAccelOffset);
//  eeAddress += sizeof(int16_t);
//  EEPROM.get(eeAddress, zAccelOffset);
//  eeAddress += sizeof(int16_t);
//  EEPROM.get(eeAddress, xGyroOffset);
//  eeAddress += sizeof(int16_t);
//  EEPROM.get(eeAddress, yGyroOffset);
//  eeAddress += sizeof(int16_t);
//  EEPROM.get(eeAddress, zGyroOffset);
//}
//
//void eeprom_put() {
//  int eeAddress = 0;
//  EEPROM.put(eeAddress, IMU.getXAccelOffset());
//  eeAddress += sizeof(int16_t);
//  EEPROM.put(eeAddress, IMU.getYAccelOffset());
//  eeAddress += sizeof(int16_t);
//  EEPROM.put(eeAddress, IMU.getZAccelOffset());
//  eeAddress += sizeof(int16_t);
//  EEPROM.put(eeAddress, IMU.getXGyroOffset());
//  eeAddress += sizeof(int16_t);
//  EEPROM.put(eeAddress, IMU.getYGyroOffset());
//  eeAddress += sizeof(int16_t);
//  EEPROM.put(eeAddress, IMU.getZGyroOffset());
//}

void eeprom_get() {
  xAccelOffset = prefs.getShort("xAccelOffset", 0);
  yAccelOffset = prefs.getShort("yAccelOffset", 0);
  zAccelOffset = prefs.getShort("zAccelOffset", 0);
  xGyroOffset = prefs.getShort("xGyroOffset", 0);
  yGyroOffset = prefs.getShort("yGyroOffset", 0);
  zGyroOffset = prefs.getShort("zGyroOffset", 0);
}

void eeprom_put() {
  prefs.putShort("xAccelOffset", IMU.getXAccelOffset());
  prefs.putShort("yAccelOffset", IMU.getYAccelOffset());
  prefs.putShort("zAccelOffset", IMU.getZAccelOffset());
  prefs.putShort("xGyroOffset", IMU.getXGyroOffset());
  prefs.putShort("yGyroOffset", IMU.getYGyroOffset());
  prefs.putShort("zGyroOffset", IMU.getZGyroOffset());
}
