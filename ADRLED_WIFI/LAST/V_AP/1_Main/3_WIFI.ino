
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
    FastLED.show();
    if (trys == 15) {
      trys = 0;
      trys1++;
      RedIndicate();
    }
    if (trys1 == 3) {
      for (int i=0;i<NUM_LEDS;i++) {
        leds[i] = CRGB::Blue;
      }
      FastLED.show();
      delay(500);
      FastLED.clear();
      FastLED.show();
      APactivate();
      return;
    }
  }
  GreenIndicate();
  tone(BUZZER_PIN, 3000, 100); 
  Serial.println(WiFi.localIP());
}

void WiFiRead() {
    bool fl = true;

    WiFiClient client = server.available();
    if (!client) return;
    while (!client.available()) {}

    String req = client.readStringUntil('\r');
    if (req.indexOf("/LedStrip/MODE;") != -1) {Mode = cutString(req); FastLED.clear(); FastLED.show();}
    else if (req.indexOf("/LedStrip/BRIGHT;") != -1) {BRIGHTNESS = cutString(req); FastLED.setBrightness(BRIGHTNESS);}
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
        FastLED.clear();
        FastLED.show();
        onoff = false;
      }
      else {
        onoff = true;
      }
    }
    else if (req.indexOf("/LedStrip/SLEEP;") != -1) {SleepTimer.setTimeout(cutString(req)*60000);}
    else if (req.indexOf("/LedStrip/SLEEP_CANCEL") != -1) {SleepTimer.stop();}
    else if (req.indexOf("/GET_DATA") != -1) {
      fl = false;
      Serial.println("GET_DATA");
      client.println("HTTP/1.1 200 OK"); 
      client.println("Content-Type: text/html");
      client.println();
      client.print(Speed1);
      client.print(" ");
      client.print(Speed2); 
      client.print(" ");
      client.print(Speed3);
      client.print(" ");
      client.print(Speed4);
      client.print(" ");
      client.print(HUE1);
      client.print(" ");
      client.print(HUE2);
      client.print(" ");
      client.print(HUE3);
      client.print(" ");
      client.print(HUE4);
      client.print(" ");
      client.print(HUE5);
      client.print(" ");
      client.print(HUE6);
      client.print(" ");
      client.print(SAT1);
      client.print(" ");
      client.print(SAT2);
      client.print(" ");
      client.print(SAT3);
      client.print(" ");
      client.print(SAT4);
      client.print(" ");
      client.print(SAT5);
      client.print(" ");
      client.print(SAT6);
      client.print(" ");
      client.print(BRIGHTNESS); 
      client.print(" ");
      client.print(Mode); 
      client.print(" ");
      client.print(onoff); 
      client.print(" ");
      client.println();
    }
    else if (req.indexOf("/RESTART_AP") != -1){
      fl = false;
      tone(BUZZER_PIN, 500, 500);
      Serial.println("GET_DATA");
      client.println("HTTP/1.1 200 OK"); 
      client.println("Content-Type: text/html");
      client.println();
      client.print("Restart to AP");
      client.println();
      client.stop();
      server.close();
      server.stop();
      APactivate();
      return;
    }
    else {
      fl = false;
      Serial.println("GET_DATA");
      client.println("HTTP/1.1 200 OK"); 
      client.println("Content-Type: text/html");
      client.println();
      client.print("Invalid!");
      client.println();
    }
  if (fl == true) {
      Serial.println("GET_DATA");
      client.println("HTTP/1.1 200 OK"); 
      client.println("Content-Type: text/html");
      client.println();
      client.print("OK!");
      client.println();
    SerialDebug();
  }
  client.stop();
  EEPROMSAVE();
}

int cutString(String inAdd) {
  inAdd.remove(0, inAdd.indexOf(";")+1);
  inAdd.remove(inAdd.indexOf("HTTP/1.1"), inAdd.length());
  int result = inAdd.toInt();
  return result;
}
