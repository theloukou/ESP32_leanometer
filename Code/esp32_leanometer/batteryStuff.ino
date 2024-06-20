uint32_t getBat() {
  uint32_t vBatRaw = analogReadMilliVolts(VBAT_PIN);
  uint32_t vBat = (vBatRaw * 4.969);
#ifdef SERIAL_DEBUG
  Serial.printf("vBat raw/calc: %d / %d \r\n", vBatRaw, vBat);
#endif
  return vBat;
}
