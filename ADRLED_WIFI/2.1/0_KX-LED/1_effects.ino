uint32_t tmrPaintWBR;
int idexWBR;

int get_delta(int leds_count, int colors_count) {
  return int(leds_count / colors_count);
}

int shift_index(int current_index, int delta, int leds_count) {
  return (current_index + delta) % leds_count;
}

void increment_index() {
  if (idexWBR++ >= NUM_LEDS) idexWBR = 0;
}

void paintWBR() {
  if (millis() - tmrPaintWBR > rusLightsSpeed) {
    tmrPaintWBR = millis();
    increment_index();

    int delta = get_delta(NUM_LEDS, 3);
    int whiteIndex = idexWBR;
    int blueIndex = shift_index(whiteIndex, delta, NUM_LEDS);
    int redIndex = shift_index(blueIndex, delta, NUM_LEDS);

    leds[whiteIndex].setRGB(255, 255, 255);
    leds[blueIndex].setRGB(0, 0, 255);
    leds[redIndex].setRGB(255, 0, 0);
  }
}

void fillAll(CRGB rgbHue) {
  fill_solid(leds, NUM_LEDS, rgbHue);
  FastLED.show();
}

void StrobeAll(CRGB strobeHue1, CRGB strobeHue2, int delayStr, uint8_t amount) {
  bool stateStr = false;

  for (uint8_t i = 0; i < amount; i++) {
    if (stateStr) fill_solid(leds, NUM_LEDS, strobeHue1);
    else fill_solid(leds, NUM_LEDS, strobeHue2);

    stateStr = !stateStr;

    FastLED.show();
    
    delay(delayStr);
  }
}


// ONOFF FADE EFFECT ------------------
void effectFadeOn() {
  if (!OnOff && !TempOff) {
    TempOff = true;
    for (uint8_t i = 0; i < Brightness; i+=5) {
      animation();
      FastLED.setBrightness(i);
      FastLED.show();
    }
  }

  FastLED.setBrightness(Brightness);
  FastLED.show();
  TempOff = false;
  OnOff = true;
}

void effectFadeOff() {
  if (OnOff && !TempOn) {
    TempOn = true;
    for (int i = Brightness; i > 5; i-=5) {
      animation();
      FastLED.setBrightness(i);
      FastLED.show();
    }
  }
  
  FastLED.setBrightness(0);
  FastLED.show();
  TempOn = false;
  OnOff = false;
}
// ONOFF FADE EFFECT ------------------


// RAINBOW FADE EFFECT ----------------
uint32_t tmrRainbowFade;
int ihue, rainbowFadeIdex;

void rainbowFade() {
  if (millis() - tmrRainbowFade > RainbowFadeSpeed) {
    tmrRainbowFade = millis();
    ihue++;
    if (ihue > 255) ihue = 0;
    for (int rainbowFadeIdex = 0; rainbowFadeIdex < NUM_LEDS; rainbowFadeIdex++) {
      leds[rainbowFadeIdex] = CHSV(ihue, 255, 255);
    }
  }
}
// RAINBOW FADE EFFECT ----------------


// RAINBOW LOOP EFFECT ----------------
uint32_t rainbow_timer;
float rainbow_steps;
int this_color;

void rainbowLoop() {
  if (millis() - rainbow_timer > 30) {
    rainbow_timer = millis();
    this_color += RainbowLoopSpeed;
    if (this_color > 255) this_color = 0;
    if (this_color < 0) this_color = 255;
  }
  rainbow_steps = this_color;
  for (int i = 0; i < NUM_LEDS; i++) {
    delay(0);
    leds[i] = CHSV((int)floor(rainbow_steps), 255, 255);
    rainbow_steps += RainbowLoopStep;
    if (rainbow_steps > 255) rainbow_steps = 0;
    if (rainbow_steps < 0) rainbow_steps = 255;
  }
}
// RAINBOW LOOP EFFECT ----------------


// FIRE EFFECT ------------------------
int FireCounter = 0;
#define FOR_i(from, to) for(int i = (from); i < (to); i++)
#define FOR_j(from, to) for(int j = (from); j < (to); j++)

void firePerlin(bool all255 = false) {
  static uint32_t prevTime;

  if (millis() - prevTime > 20) {
    prevTime = millis();
    int thisPos = 0, lastPos = 0;
    FOR_i(0, NUM_LEDS) {
      leds[i] = getFireColor((inoise8(i * FireStep, FireCounter)), all255);
    }
    FireCounter += 20;
  }
}

CHSV getFireColor(int val, bool all255) {
  if (all255) 
    return CHSV(FireHueStart + map(val, 0, 255, 0, 255), constrain(map(val, 0, 255, 255, 255), 0, 255), constrain(map(val, 0, 255, 255, 255), 0, 255));
  else 
    return CHSV(FireHueStart + map(val, 0, 255, 0, FireHueGap), constrain(map(val, 0, 255, FireMaxSaturation, FireMinSaturation), 0, 255), constrain(map(val, 0, 255, FireMinBrightness, 255), 0, 255)); 
}
// FIRE EFFECT ------------------------

// EMLIGHTS EFFECT --------------------
uint32_t tmrEmsLights;
int thishue = 0;
int emsLightsIdex;
int TOP_INDEX = int(NUM_LEDS / 2);

void emsLights() {
  if (millis() - tmrEmsLights > emsLightsSpeed) {
    tmrEmsLights = millis();
    emsLightsIdex++;

    if (emsLightsIdex >= NUM_LEDS) 
      emsLightsIdex = 0;

    int thathue = (thishue + 160) % 255;
    leds[emsLightsIdex] = CHSV(thishue, 255, 255);
    leds[antipodal_index(emsLightsIdex)] = CHSV(thathue, 255, 255);
  }
}

int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) iN = (i + TOP_INDEX) % NUM_LEDS;
  return iN;
}
// EMLIGHTS EFFECT --------------------

uint32_t tmrStrobeRandomColors;

void StrobeRandomColors() {
  if (millis() - tmrStrobeRandomColors > StrobeRandomColorsSpeed) {
    tmrStrobeRandomColors = millis();
    fill_solid(leds, NUM_LEDS, CHSV(random(0, 255), 255, 255));
  }
}


void StaticColor() {
  fill_solid(leds, NUM_LEDS, CHSV(StaticHueAll, StaticSatAll, 255));
}


void animation() {
  switch (Mode) {
  case 0: 
    FastLED.setBrightness(0);
    break;
  case 1:
    rainbowLoop();
    break;
  case 2:
    rainbowFade();
    break;
  case 3:
    firePerlin(false);
    break;
  case 4:
    firePerlin(true);
    break;
  case 5:
    emsLights();
    break;
  case 6:
    paintWBR();
    break;
  case 7:
    StrobeRandomColors();
    break;
  case 8:
    StaticColor();
    break;
  default:
    break;
  }

  if (OnOff) FastLED.show();
}