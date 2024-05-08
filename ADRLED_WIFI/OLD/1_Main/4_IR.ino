uint32_t IRdata;

void irread() {
  if (!ir.available()) return;
  IRdata = ir.readCommand(), HEX;
  Serial.print("0x");
  Serial.println(IRdata, HEX);
  digitalWrite(LED_INFO, HIGH);
  LedOffTimer.setTimeout(500);
}


void ircheck() {
  if (!ir.available()) return;
  IRdata = ir.readCommand(), HEX;
  if (IRdata == 292984781 || IRdata == 2166136261) return;
  digitalWrite(LED_INFO, HIGH);
  LedOffTimer.setTimeout(500);
    switch (IRdata) {
      case IR_BTN_1: Mode = 1; tone(BUZZER_PIN, 3000, 100); FastLED.clear(); FastLED.show();
        break;
      case IR_BTN_2: Mode = 2; tone(BUZZER_PIN, 3000, 100); FastLED.clear(); FastLED.show();
        break;
      case IR_BTN_3: Mode = 3; tone(BUZZER_PIN, 3000, 100); FastLED.clear(); FastLED.show();
        break;
      case IR_BTN_C1: Mode = 4; tone(BUZZER_PIN, 3000, 100);
        break; 
      case IR_BTN_C2: Mode = 5; tone(BUZZER_PIN, 3000, 100);
        break;
      case IR_BTN_C3: Mode = 6; tone(BUZZER_PIN, 3000, 100);
        break;
      case IR_BTN_C4: Mode = 7; tone(BUZZER_PIN, 3000, 100);
        break;
      case IR_BTN_C5: Mode = 8; tone(BUZZER_PIN, 3000, 100);
        break;
      case IR_BTN_C6: Mode = 9; tone(BUZZER_PIN, 3000, 100);
        break;
      case IR_BTN_HASH: 
        switch (Mode) {
          case 4: 
            if (SAT1 == 255) SAT1 = 0;
            else SAT1 = 255; 
          break;
          case 5: 
            if (SAT2 == 255) SAT2 = 0;
            else SAT2 = 255; 
          break;
          case 6: 
            if (SAT3 == 255) SAT3 = 0;
            else SAT3 = 255; 
          break;
          case 7: 
            if (SAT4 == 255) SAT4 = 0;
            else SAT4 = 255; 
          break;
          case 8: 
            if (SAT5 == 255) SAT5 = 0;
            else SAT5 = 255; 
          break;
          case 9: 
            if (SAT6 == 255) SAT6 = 0;
            else SAT6 = 255; 
          break;
        } 
        break;
      case IR_BTN_UP: tone(BUZZER_PIN, 3000, 100); BRIGHTNESS = smincr(BRIGHTNESS, 5, 0, 255); FastLED.setBrightness(BRIGHTNESS);
        break;
      case IR_BTN_DOWN: tone(BUZZER_PIN, 3000, 100); BRIGHTNESS = smincr(BRIGHTNESS, -5, 0, 255); FastLED.setBrightness(BRIGHTNESS);
        break;
      case IR_BTN_LEFT: 
        tone(BUZZER_PIN, 3000, 100);
        switch (Mode) {
          case 1: Speed1 = smincr(Speed1, 5, 5, 100); 
          break;
          case 2: Speed2 = smincr(Speed2, 5, 5, 100);
          break;
          case 3: Speed3 = smincr(Speed3, 5, 5, 100);
          break;
          case 4: HUE1 = smincr(HUE1, -5, 0, 255); 
          break;
          case 5: HUE2 = smincr(HUE2, -5, 0, 255);
          break;
          case 6: HUE3 = smincr(HUE3, -5, 0, 255);
          break;
          case 7: HUE4 = smincr(HUE4, -5, 0, 255); 
          break;
          case 8: HUE5 = smincr(HUE5, -5, 0, 255);
          break;
          case 9: HUE6 = smincr(HUE6, -5, 0, 255);
          break;
        } 
        break;
      case IR_BTN_RIGHT: 
        tone(BUZZER_PIN, 3000, 100);
        switch (Mode) {
          case 1: Speed1 = smincr(Speed1, -5, 5, 100); 
          break;
          case 2: Speed2 = smincr(Speed2, -5, 5, 100);
          break;
          case 3: Speed3 = smincr(Speed3, -5, 5, 100);
          break;
          case 4: HUE1 = smincr(HUE1, 5, 0, 255); 
          break;
          case 5: HUE2 = smincr(HUE2, 5, 0, 255);
          break;
          case 6: HUE3 = smincr(HUE3, 5, 0, 255);
          break;
          case 7: HUE4 = smincr(HUE4, 5, 0, 255); 
          break;
          case 8: HUE5 = smincr(HUE5, 5, 0, 255);
          break;
          case 9: HUE6 = smincr(HUE6, 5, 0, 255);
          break;
        } 
        break;
      case IR_BTN_ONOFF: 
        tone(BUZZER_PIN, 3000, 100); 
        if (onoff == true) {
          onoff = false;
          FastLED.clear(); 
          FastLED.show();
        }
        else {
          onoff = true;
        }
        break;
      default:
        break;
      }
    EEPROMSAVE();
    #ifdef DEBUG
      SerialDebug();
    #endif
}