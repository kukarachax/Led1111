
void wificonnect() {
  int trys = 0;
  int trys1 = 0;
  WiFi.mode(WIFI_STA);
  Serial.println("STARTUP WIFI: ");
  Serial.println(EepromGetWiFiSettings(true));
  Serial.println(EepromGetWiFiSettings(false));
  WiFi.begin(EepromGetWiFiSettings(true), EepromGetWiFiSettings(false));
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    trys++;
    delay(500);
    for (int i=0;i<NUM_LEDS;i++) {
      leds[i] = CRGB::Yellow;
    }
    strip.show();
    if (trys == 15) {
      trys = 0;
      trys1++;
      RedIndicate();
    }
    if (trys1 == 3) {
      for (int i=0;i<NUM_LEDS;i++) {
        leds[i] = CRGB::Blue;
      }
      strip.show();
      delay(500);
      strip.clear();
      strip.show();
      APactivate();
      return;
    }
  }
  GreenIndicate();
  tone(BUZZER_PIN, 3000, 100);
  server.begin();
  Serial.println(WiFi.localIP());
}

void WiFiRead() {
    WiFiClient client = server.available();
    if (!client) return;
    while (!client.available()) {}

    String req = client.readStringUntil('\r');
    client.flush();
    if (req.indexOf("/LedStrip/MODE;") != -1) {Mode = cutString(req); strip.clear(); strip.show();}
    else if (req.indexOf("/LedStrip/BRIGHT;") != -1) {BRIGHTNESS = cutString(req); strip.setBrightness(BRIGHTNESS);}
    else if (req.indexOf("/LedStrip/SPEED;") != -1) {
      switch (Mode) {
        case 1: Speed1 = cutString(req);
        break;
        case 2: Speed2 = cutString(req);
        if (Speed2 == 0); Speed2++;
        break;
        case 3: Speed3 = cutString(req);
        break;
        case 4: Speed4 = cutString(req);
        break;
      }
    }
    else if (req.indexOf("/LedStrip/COLOR;") != -1) {
      switch (Mode) {
      case 5: HUE1 = cutString(req);
        break;
      case 6: HUE2 = cutString(req);
        break;
      case 7: HUE3 = cutString(req);
        break;
      case 8: HUE4 = cutString(req);
        break;
      case 9: HUE5 = cutString(req);
        break;
      case 10: HUE6 = cutString(req);
        break;
      }
    }
    else if (req.indexOf("/LedStrip/SATURATION;") != -1) {
      switch (Mode) {
      case 5: SAT1 = cutString(req);
        break;
      case 6: SAT2 = cutString(req);
        break;
      case 7: SAT3 = cutString(req);
        break;
      case 8: SAT4 = cutString(req);
        break;
      case 9: SAT5 = cutString(req);
        break;
      case 10: SAT6 = cutString(req);
        break;
      }
    }
    else if (req.indexOf("/LedStrip/ONOFF") != -1) {
      if (onoff == true) {
        strip.clear();
        strip.show();
        onoff = false;
      }
      else {
        onoff = true;
      }
    }
    else if (req.indexOf("/LedStrip/SLEEP;") != -1) {SleepTimer.setTimeout(cutString(req)*60000);}
    else if (req.indexOf("/LedStrip/SLEEP_CANCEL") != -1) {SleepTimer.stop();}
    else if (req.indexOf("/GET_DATA") != -1) {
      String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
      s += (Speed1);
      s += " ";
      s += (Speed2); 
      s += " ";
      s += (Speed3);
      s += " ";
      s += (Speed4);
      s += " ";
      s += (HUE1);
      s += " ";
      s += (HUE2);
      s += " ";
      s += (HUE3);
      s += " ";
      s += (HUE4);
      s += " ";
      s += (HUE5);
      s += " ";
      s += (HUE6);
      s += " ";
      s += (SAT1);
      s += " ";
      s += (SAT2);
      s += " ";
      s += (SAT3);
      s += " ";
      s += (SAT4);
      s += " ";
      s += (SAT5);
      s += " ";
      s += (SAT6);
      s += " ";
      s += (BRIGHTNESS); 
      s += " ";
      s += (Mode); 
      s += " ";
      s += (onoff); 
      s += " ";
      client.print(s);
    }
    else if (req.indexOf("/RESTART_AP") != -1){
      tone(BUZZER_PIN, 500, 500);
      String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nOK";
      client.print(s);
      client.stop();
      server.close();
      server.stop();
      APactivate();
      return;
    }
    else {
      String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nInvalid!";
      client.print(s);
    }
  client.flush();
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nOK";
  client.print(s);
  EEPROMSAVE();
  SerialDebug();
}

int cutString(String inAdd) {
  inAdd.remove(0, inAdd.indexOf(";")+1);
  inAdd.remove(inAdd.indexOf("HTTP/1.1"), inAdd.length());
  int result = inAdd.toInt();
  return result;
}
