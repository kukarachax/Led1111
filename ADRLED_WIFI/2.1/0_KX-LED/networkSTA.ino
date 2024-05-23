bool WiFiConnectSTA() {
  WiFi.softAPdisconnect();
  WiFi.disconnect();

  WiFi.mode(WIFI_STA);

  getEEPROM_WiFi();
  WiFi.begin(wifiSsid, wifiPassword);

  fillAll(CRGB::Yellow);

  uint8_t TryCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    TryCount++;
    delay(500);

    if (TryCount >= 40) {
      tone(BUZZER_PIN, 1000, 500);
      fillAll(CRGB::Red);
      delay(500);
      return false;
    }
  }

  fillAll(CRGB::Green);
  tone(BUZZER_PIN, 3600, 250);
  delay(500);

  IPAddress localip = WiFi.localIP();
  String ipBuffer = localip.toString();

  Serial.println(ipBuffer);


  udpStart();
  otaStart();
  
  return true; 
}

void otaStart() {
  ArduinoOTA.setHostname(otaName);
  ArduinoOTA.onStart([]() {});
  ArduinoOTA.onEnd([]() { ESP.restart(); });
  ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();
}

void udpStart() {
  udp.begin(udpPort);
}

void preParseUdp() {
  if (!udp.parsePacket()) return;

  String dataWithoutKey;
  char bufferUDP[MAX_UDP_PACKET_SIZE + 1];
  int n = udp.read(bufferUDP, MAX_UDP_PACKET_SIZE);

  for (uint8_t i = 0; i < NETWORK_KEY_SIZE - 1; i++) {
    if (bufferUDP[i] != NETWORK_KEY[i]) return;
  }

  for (int i = NETWORK_KEY_SIZE; i < n; i++) {
    dataWithoutKey += (char) bufferUDP[i];
    if (i == n && bufferUDP[n] != '%') return; 
  }

  if (dataWithoutKey.indexOf("GET_DATA") != -1) {
    getDataResponse();
    return;
  } 
  
  parseDataString(dataWithoutKey);
}

void parseDataString(String data) { 
  Serial.println(data);
  if (data.indexOf("MAX_CURRENT=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    MAX_CURRENT = data.toInt();
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_CURRENT);
  }

  else if (data.indexOf("OnOff=") != -1) {
    data.remove(0, data.indexOf("=")+1);  

    if ((bool) data.toInt()) effectFadeOn();
    else effectFadeOff();
  }

  else if (data.indexOf("Mode=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    Mode = data.toInt();
  }

  else if (data.indexOf("Brightness=") != -1) {
    data.remove(0, data.indexOf("=")+1);  
    Brightness = data.toInt();
  }

  else if (data.indexOf("RainbowLoopSpeed=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    RainbowLoopSpeed = data.toInt();
  }

  else if (data.indexOf("RainbowLoopStep=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    RainbowLoopStep = data.toFloat();
  }
  else if (data.indexOf("RainbowFadeSpeed=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    RainbowFadeSpeed = data.toInt();
  }
  else if (data.indexOf("StrobeRandomColorsSpeed=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    StrobeRandomColorsSpeed = data.toInt();
  }
  else if (data.indexOf("emsLightsSpeed=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    emsLightsSpeed = data.toInt();
  }
  else if (data.indexOf("StaticHueAll=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    StaticHueAll = data.toInt();
  }
  else if (data.indexOf("StaticSatAll=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    StaticSatAll = data.toInt();
  }
  else if (data.indexOf("FireHueGap=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    FireHueGap = data.toInt();
  }
  else if (data.indexOf("FireStep=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    FireStep = data.toInt();
  }
  else if (data.indexOf("FireHueStart=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    FireHueStart = data.toInt();
  }
  else if (data.indexOf("FireMinBrightness=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    FireMinBrightness = data.toInt();
  }
  else if (data.indexOf("FireMinSaturation=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    FireMinSaturation = data.toInt();
  }
  else if (data.indexOf("FireMaxSaturation=") != -1) {
    data.remove(0, data.indexOf("=")+1);
    FireMaxSaturation = data.toInt();
  }
  else if (data.indexOf("Restart") != -1) {
    ESP.restart();
  }
  else if (data.indexOf("ResetAllSettings") != -1) {
    resetEEPROM();
  }
  else if (data.indexOf("ApMode") != -1) {
    WiFiConnectAP();
  }

  else return;
  
  updEEPROM();
  Serial.println(data);
}

void getDataResponse() {
  String replyBuffer = addNetworkKey(replyBuffer);
  replyBuffer += "GET_DATA:";

  replyBuffer += MAX_CURRENT; replyBuffer += ";";
  replyBuffer += OnOff; replyBuffer += ";";
  replyBuffer += Mode; replyBuffer += ";";
  replyBuffer += Brightness; replyBuffer += ";";
  replyBuffer += RainbowLoopSpeed; replyBuffer += ";";
  replyBuffer += RainbowLoopStep; replyBuffer += ";";
  replyBuffer += RainbowFadeSpeed; replyBuffer += ";";
  replyBuffer += StrobeRandomColorsSpeed; replyBuffer += ";";
  replyBuffer += emsLightsSpeed; replyBuffer += ";";
  replyBuffer += StaticHueAll; replyBuffer += ";";
  replyBuffer += StaticSatAll; replyBuffer += ";";
  replyBuffer += FireHueGap; replyBuffer += ";";
  replyBuffer += FireStep; replyBuffer += ";";
  replyBuffer += FireHueStart; replyBuffer += ";";
  replyBuffer += FireMinBrightness; replyBuffer += ";";
  replyBuffer += FireMinSaturation; replyBuffer += ";";
  replyBuffer += FireMaxSaturation; replyBuffer += "%";

  udp.beginPacket(udpSendAddress, udpPort);
  udp.print(replyBuffer.c_str());
  udp.endPacket();
}

String addNetworkKey(String str) {
  for (uint8_t i = 0; i < NETWORK_KEY_SIZE; i++) {
    str += NETWORK_KEY[i];
  }
  return str;
}

void networkSTALoop() {
  preParseUdp();
  ArduinoOTA.handle();
}