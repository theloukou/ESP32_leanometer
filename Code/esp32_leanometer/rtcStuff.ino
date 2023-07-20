void rtcInit() {
  if (! rtc.begin()) {
#ifdef SERIAL_DEBUG
    Serial.println("Couldn't find RTC");
#endif
  }

  if (!rtc.initialized() || rtc.lostPower()) {
#ifdef SERIAL_DEBUG
    Serial.println("RTC is NOT initialized, setting to preset. Please set date and time.");
#endif
    rtc.adjust(DateTime(2000, 1, 01, 12, 0, 0));
  }
}

unsigned long rtcGetUnixTime() {
  DateTime currentTime = rtc.now();
  return currentTime.unixtime();
}
