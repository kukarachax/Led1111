
void wificonnect() {
  int trys = 0;
  int trys1 = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
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
      RedIndicate();
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
    if (req.indexOf("/LedStrip/MODE;") != -1) {tone(BUZZER_PIN, 3000, 100); Mode = cutString(req); FastLED.clear(); FastLED.show();}
    else if (req.indexOf("/LedStrip/BRIGHT;") != -1) {tone(BUZZER_PIN, 3000, 100); BRIGHTNESS = cutString(req); FastLED.setBrightness(BRIGHTNESS);}
    else if (req.indexOf("/LedStrip/SPEED;") != -1) {
      tone(BUZZER_PIN, 3000, 100);
      switch (Mode) {
        case 1: Speed1 = cutString(req);
        break;
        case 2: Speed2 = cutString(req);
        break;
        case 3: Speed3 = cutString(req);
        break;
      }
    }
    else if (req.indexOf("/LedStrip/COLOR;") != -1) {
      tone(BUZZER_PIN, 3000, 100);
      switch (Mode) {
      case 4: HUE1 = cutString(req);
        break;
      case 5: HUE2 = cutString(req);
        break;
      case 6: HUE3 = cutString(req);
        break;
      case 7: HUE4 = cutString(req);
        break;
      case 8: HUE5 = cutString(req);
        break;
      case 9: HUE6 = cutString(req);
        break;
      }
    }
    else if (req.indexOf("/LedStrip/SATURATION;") != -1) {
      tone(BUZZER_PIN, 3000, 100);
      switch (Mode) {
      case 4: SAT1 = cutString(req);
        break;
      case 5: SAT2 = cutString(req);
        break;
      case 6: SAT3 = cutString(req);
        break;
      case 7: SAT4 = cutString(req);
        break;
      case 8: SAT5 = cutString(req);
        break;
      case 9: SAT6 = cutString(req);
        break;
      }
    }
    else if (req.indexOf("/LedStrip/ONOFF") != -1) {
      tone(BUZZER_PIN, 3000, 100);
      if (onoff == true) {
        FastLED.clear();
        FastLED.show();
        onoff = false;
      }
      else {
        onoff = true;
      }
    }
    else if (req.indexOf("/LedStrip/SLEEP;") != -1) {tone(BUZZER_PIN, 3000, 100); SleepTimer.setTimeout(cutString(req)*60000);}
    else if (req.indexOf("/LedStrip/SLEEP_CANCEL") != -1) {tone(BUZZER_PIN, 3000, 100); SleepTimer.stop();}
    else if (req.indexOf("/GET_DATA") != -1) {
      String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
      s += (Speed1);
      s += " ";
      s += (Speed2); 
      s += " ";
      s += (Speed3);
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
