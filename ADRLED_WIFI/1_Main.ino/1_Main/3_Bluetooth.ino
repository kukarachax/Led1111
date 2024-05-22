void BT_Send_Read() {
    WiFiClient client = server.available();
    if (!client) return;
    while (!client.available()) {}

    String req = client.readStringUntil('\r');
    client.flush();
    if (req.indexOf("/LedStrip/BTN_1") != -1) {Mode = 1; idex=0; thishue=0; thissat=255; thisstep=10; ihue=0; FastLED.clear(); FastLED.show();}
    else if (req.indexOf("/LedStrip/BTN_2") != -1) {Mode = 2; idex=0; thishue=0; thissat=255; thisstep=10; ihue=0; FastLED.clear(); FastLED.show();}
    else if (req.indexOf("/LedStrip/BTN_3") != -1) {Mode = 3; idex=0; thishue=0; thissat=255; thisstep=10; ihue=0; FastLED.clear(); FastLED.show();}
    else if (req.indexOf("/LedStrip/BTN_C1") != -1) {Mode = 4;}
    else if (req.indexOf("/LedStrip/BTN_C2") != -1) {Mode = 5;}
    else if (req.indexOf("/LedStrip/BTN_C3") != -1) {Mode = 6;}
    else if (req.indexOf("/LedStrip/BTN_RUP") != -1) {colorPicker(true, 1);}
    else if (req.indexOf("/LedStrip/BTN_RDOWN") != -1) {colorPicker(false, 1);}
    else if (req.indexOf("/LedStrip/BTN_GUP") != -1) {colorPicker(true, 2);}
    else if (req.indexOf("/LedStrip/BTN_GDOWN") != -1) {colorPicker(false, 2);}
    else if (req.indexOf("/LedStrip/BTN_BUP") != -1) {colorPicker(true, 3);}
    else if (req.indexOf("/LedStrip/BTN_BDOWN") != -1) {colorPicker(false, 3);}
    else if (req.indexOf("/LedStrip/BTN_BRIGHT_UP") != -1) {BRIGHTNESS = smincr(BRIGHTNESS, 5, 0, 250, true); FastLED.setBrightness(BRIGHTNESS);}
    else if (req.indexOf("/LedStrip/BTN_BRIGHT_DOWN") != -1) {BRIGHTNESS = smincr(BRIGHTNESS, 5, 0, 250, false); FastLED.setBrightness(BRIGHTNESS);}
    else if (req.indexOf("/LedStrip/BTN_LEFT") != -1) {
      switch (Mode) {
        case 1: Speed1 = smincr(Speed1, 1, 0, 100, false); 
        break;
        case 2: Speed2 = smincr(Speed2, 1, 0, 100, false);
        break;
        case 3: Speed3 = smincr(Speed3, 1, 0, 100, false);
      } 
    }
    else if (req.indexOf("/LedStrip/BTN_RIGHT") != -1) {
      switch (Mode) {
        case 1: Speed1 = smincr(Speed1, 1, 0, 100, true); 
        break;
        case 2: Speed2 = smincr(Speed2, 1, 0, 100, true);
        break;
        case 3: Speed3 = smincr(Speed3, 1, 0, 100, true);
      } 
    }
    else if (req.indexOf("/LedStrip/BTN_ONOFF") != -1) {
      if (onoff == true) {
        onoff = false;
        FastLED.clear(); 
        FastLED.show();
        idex=0;  
        thishue=0;  
        thissat=255;  
        thisstep=10;  
        ihue=0;
      }
      else {
        onoff = true;
      }
    }
    else if (req.indexOf("/LedStrip/SLEEP_15") != -1) {SleepTimer.setTimeout(900000); LedTickOn.setTimeout(5000);}
    else if (req.indexOf("/LedStrip/SLEEP_30") != -1) {SleepTimer.setTimeout(1800000); LedTickOn.setTimeout(5000);}
    else if (req.indexOf("/LedStrip/SLEEP_60") != -1) {SleepTimer.setTimeout(3600000); LedTickOn.setTimeout(5000);}
    else if (req.indexOf("/LedStrip/SLEEP_CANCEL") != -1) {SleepTimer.stop();}
    else return;
  client.flush();
  EEPROMSAVE();
}