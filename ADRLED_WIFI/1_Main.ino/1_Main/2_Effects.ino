int TOP_INDEX = int(NUM_LEDS / 2);
int idex=0;
int thishue=0;
int thissat=255;
int thisstep=10;
int ihue=0;
int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {
    iN = ( i + TOP_INDEX ) % NUM_LEDS;
  }
  return iN;
}

void RBlights() {
    idex++;
    if (idex >= NUM_LEDS) {
      idex = 0;
    }
    int idexR = idex;
    int idexB = antipodal_index(idexR);
    int thathue = (thishue + 160) % 255;
    leds[idexR] = CHSV(thishue, thissat, 255);
    leds[idexB] = CHSV(thathue, thissat, 255);
    FastLED.show();
    delay(Speed1);
}

void rainbow_loop() {                        //-m3-LOOP HSV RAINBOW
  idex++;
  ihue = ihue + thisstep;
  if (idex >= NUM_LEDS) {
    idex = 0;
  }
  if (ihue > 255) {
    ihue = 0;
  }
  leds[idex] = CHSV(ihue, thissat, 255);
  FastLED.show();
  delay(Speed2);
}

void rainbow_fade() {                         //-m2-FADE ALL LEDS THROUGH HSV RAINBOW
  ihue++;
  if (ihue > 255) {
    ihue = 0;
  }
  for (int idex = 0 ; idex < NUM_LEDS; idex++ ) {
    leds[idex] = CHSV(ihue, thissat, 255);
  }
  FastLED.show();
  delay(Speed3);
}

void animation() {
  if (onoff == false) return;
    switch (Mode) {
    case 1: RBlights();
        break;
    case 2: rainbow_loop();
        break;
    case 3: rainbow_fade();
        break;
    case 4: for (int i = 0; i < NUM_LEDS; i++) leds[i].setRGB(RedColor1, GreenColor1, BlueColor1); FastLED.show();
        break;
    case 5: for (int i = 0; i < NUM_LEDS; i++) leds[i].setRGB(RedColor2, GreenColor2, BlueColor2); FastLED.show();
        break;
    case 6: for (int i = 0; i < NUM_LEDS; i++) leds[i].setRGB(RedColor3, GreenColor3, BlueColor3); FastLED.show();
        break;
    }
}
