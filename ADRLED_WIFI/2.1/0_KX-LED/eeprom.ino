void getEEPROM_WiFi() {
  uint8_t len;

  EEPROM.get(1, len);
  for (uint8_t i = 0; i < len; i++) {
    wifiSsid += (char) EEPROM.read(i+3);
  }

  EEPROM.get(2, len);
  for (uint8_t i = 0; i < len; i++) {
    wifiPassword += (char) EEPROM.read(i+36);
  }

  Serial.println(wifiSsid);
  Serial.println(wifiPassword);
}

void updEEPROM_WiFi(String UPDssid, String UPDpass) {
  EEPROM.put(1, (uint8_t)UPDssid.length());
  EEPROM.put(2, (uint8_t)UPDpass.length());

  for (uint8_t i = 0; i < UPDssid.length(); i++) {
    EEPROM.put(i+3, UPDssid[i]);
    Serial.print(UPDssid[i]);
  }
  Serial.println();
  for (uint8_t i = 0; i < UPDpass.length(); i++) {
    EEPROM.put(i+36, UPDpass[i]);
    Serial.print(UPDpass[i]);
  }
  Serial.println();

  EEPROM.commit();
}

void resetEEPROM() {
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, NULL);
  }
  EEPROM.commit();
  delay(1000);

  StrobeAll(CRGB::Red, CRGB::Black, 500, 3);

  ESP.restart();
}

void updEEPROM() {
  EEPROM.write(0, 123);

  EEPROM.put(100, FireMaxSaturation);
  EEPROM.put(104, FireMinSaturation);
  EEPROM.put(108, FireMinBrightness);
  EEPROM.put(110, FireHueStart);
  EEPROM.put(112, FireStep);
  EEPROM.put(116, FireHueGap);
  EEPROM.put(120, StaticSatAll);
  EEPROM.put(124, StaticHueAll);
  EEPROM.put(128, emsLightsSpeed);
  EEPROM.put(132, StrobeRandomColorsSpeed);
  EEPROM.put(136, RainbowFadeSpeed);
  EEPROM.put(140, RainbowLoopStep);
  EEPROM.put(144, RainbowLoopSpeed);
  EEPROM.put(148, Brightness);
  EEPROM.put(152, Mode);
  EEPROM.put(156, OnOff);
  EEPROM.put(160, MAX_CURRENT);
  EEPROM.put(164, rusLightsSpeed);

  EEPROM.commit();
}

void getEEPROM() {
  EEPROM.get(100, FireMaxSaturation);
  EEPROM.get(104, FireMinSaturation);
  EEPROM.get(108, FireMinBrightness);
  EEPROM.get(110, FireHueStart);
  EEPROM.get(112, FireStep);
  EEPROM.get(116, FireHueGap);
  EEPROM.get(120, StaticSatAll);
  EEPROM.get(124, StaticHueAll);
  EEPROM.get(128, emsLightsSpeed);
  EEPROM.get(132, StrobeRandomColorsSpeed);
  EEPROM.get(136, RainbowFadeSpeed);
  EEPROM.get(140, RainbowLoopStep);
  EEPROM.get(144, RainbowLoopSpeed);
  EEPROM.get(148, Brightness);
  EEPROM.get(152, Mode);
  EEPROM.get(156, OnOff);
  EEPROM.get(160, MAX_CURRENT);
  EEPROM.get(164, rusLightsSpeed);
}