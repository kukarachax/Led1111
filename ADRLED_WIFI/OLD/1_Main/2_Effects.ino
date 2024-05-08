void animation() {
  if (onoff == false)
  {
    FastLED.clear();
    FastLED.show();
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
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE1,SAT1,255);
      FastLED.show();
      break;
    case 5:
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE2,SAT2,255);
      FastLED.show();
      break;
    case 6:
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE3,SAT3,255);
      FastLED.show();
      break;
    case 7:
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE4,SAT4,255);
      FastLED.show();
      break;
    case 8:
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE5,SAT5,255);
      FastLED.show();
      break;
    case 9:
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(HUE6,SAT6,255);
      FastLED.show();
      break;
    }
  }
}


void GreenIndicate() {
  for (int a=0;a<4;a++) {
    for (int i=0;i<NUM_LEDS;i++) {
      leds[i] = CRGB::Green;
    }
    FastLED.show();
    delay(200);
    FastLED.clear();
    FastLED.show();
    delay(200);
  }
}

void RedIndicate() {
  for (int a=0;a<4;a++) {
    for (int i=0;i<NUM_LEDS;i++) {
      leds[i] = CRGB::Red;
    }
    FastLED.show();
    delay(200);
    FastLED.clear();
    FastLED.show();
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
  
  FastLED.show();
  delay(Speed1);
}

void rainbow_loop()
{ //-m3-LOOP HSV RAINBOW
  idex++;
  ihue = ihue + thisstep;
  if (idex >= NUM_LEDS)
  {
    idex = 0;
  }
  if (ihue > 255)
  {
    ihue = 0;
  }
  leds[idex] = CHSV(ihue, thissat, 255);
  FastLED.show();
  delay(Speed2);
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
  FastLED.show();
  delay(Speed3);
}


//---------------UTILITY-----------------

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
