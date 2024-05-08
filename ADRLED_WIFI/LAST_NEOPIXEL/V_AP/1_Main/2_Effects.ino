void animation() {
  if (onoff == false)
  {
    strip.clear();
    strip.show();
  }
  else {
    switch (Mode)
    {
    case 1: 
      paintWBR();
      break;
    case 2:
      rainbow_loop();
      break;
    case 3:
      rainbow_fade();
      break;
    case 4:
      Fire(55, 120);
      break;
    case 5:
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE1,SAT1,255);
      strip.show();
      break;
    case 6:
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE2,SAT2,255);
      strip.show();
      break;
    case 7:
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE3,SAT3,255);
      strip.show();
      break;
    case 8:
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE4,SAT4,255);
      strip.show();
      break;
    case 9:
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE5,SAT5,255);
      strip.show();
      break;
    case 10:
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE6,SAT6,255);
      strip.show();
      break;
    }
  }
}


void GreenIndicate() {
  for (int a=0;a<4;a++) {
    for (int i=0;i<NUM_LEDS;i++) {
      leds[i] = CRGB::Green;
    }
    strip.show();
    delay(200);
    strip.clear();
    strip.show();
    delay(200);
  }
}

void RedIndicate() {
  for (int a=0;a<4;a++) {
  tone(BUZZER_PIN, 500, 500);
    for (int i=0;i<NUM_LEDS;i++) {
      leds[i] = CRGB::Red;
    }
    strip.show();
    delay(200);
    strip.clear();
    strip.show();
    delay(200);
  }
}



//--------------------EFFECTS------------------------
int EVENODD = NUM_LEDS % 2;
int BOTTOM_INDEX = 0;
int TOP_INDEX = int(NUM_LEDS / 2);
int idex = 0;
int thishue = 0;
int thissat = 255;
int thisstep = 10;
int ihue = 0;
unsigned long rainbow_timer;
float RAINBOW_STEP_2 = 0.5;
float rainbow_steps;
int this_color;

// White-Blue-Red
void paintWBR()
{
  // Russian flag contains 3 colors
  #define COLOR_COUNT 3
  increment_index();

  int delta = get_delta(NUM_LEDS, COLOR_COUNT);
  int whiteIndex = idex;
  int blueIndex = shift_index(whiteIndex, delta, NUM_LEDS);
  int redIndex = shift_index(blueIndex, delta, NUM_LEDS);
  #undef COLOR_COUNT

  leds[whiteIndex].setRGB(255, 255, 255);
  leds[blueIndex].setRGB(0, 0, 255);
  leds[redIndex].setRGB(255, 0, 0);
  
  strip.show();
  delay(Speed1);
}

void Fire(int Cooling, int Sparking) {
  static byte heat[NUM_LEDS];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  strip.show();
  delay(Speed4);
}


void rainbow_loop()
{
  delay(0);
  if (millis() - rainbow_timer > 30) {
    rainbow_timer = millis();
    this_color += Speed2;
    if (this_color > 255) this_color = 0;
    if (this_color < 0) this_color = 255;
  }
  rainbow_steps = this_color;
  delay(0);
  for (int i = 0; i < NUM_LEDS; i++) {
    delay(0);
    leds[i] = CHSV((int)floor(rainbow_steps), 255, 255);
    rainbow_steps += RAINBOW_STEP_2;
    if (rainbow_steps > 255) rainbow_steps = 0;
    if (rainbow_steps < 0) rainbow_steps = 255;
  }
  strip.show();
  delay(0);
}


void rainbow_fade()
{ //-m2-FADE ALL LEDS THROUGH HSV RAINBOW
  ihue++;
  if (ihue > 255)
  {
    ihue = 0;
  }
  for (int idex = 0; idex < NUM_LEDS; idex++)
  {
    leds[idex] = CHSV(ihue, thissat, 255);
  }
  strip.show();
  delay(Speed3);
}


//---------------UTILITY-----------------

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                    // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if ( t192 > 0x40 ) {            // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
}

int antipodal_index(int i)
{
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX)
  {
    iN = (i + TOP_INDEX) % NUM_LEDS;
  }
  return iN;
}

// Delta - offset between colors
int get_delta(int leds_count, int colors_count)
{
  return int(leds_count / colors_count);
}

int shift_index(int current_index, int delta, int leds_count)
{
  return (current_index + delta) % leds_count;
}

void increment_index()
{
  if (idex++ >= NUM_LEDS)
    idex = 0;
}
