#include "FastLED.h"
#include <GyverTimer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>

WiFiServer server(80);

char* ssid = "Homenet";
char* password = "savran51155115";

byte baza = 0;     // изменение оттенка LED
int Mode = 1;
int BRIGHTNESS = 100;
int Speed1 = 50;
int Speed2 = 50;
int Speed3 = 50;
byte RedColor1 = 255;
byte RedColor2 = 255;
byte RedColor3 = 255;
byte GreenColor1 = 255;
byte GreenColor2 = 255;
byte GreenColor3 = 255;
byte BlueColor1 = 255;
byte BlueColor2 = 255;
byte BlueColor3 = 255;
bool onoff = true;

#define LED_INFO 5
#define NUM_LEDS 1200
#define PIN D4
#define STATE_PIN 7
#define MAX_AMPS 1

CRGB leds[NUM_LEDS];
GTimer SleepTimer(MS);
GTimer LedOffTimer(MS);
GTimer LedTickOff(MS);
GTimer LedTickOn(MS);


void setup() {
  Serial.begin(115200);
  pinMode(LED_INFO, OUTPUT);
  FastLED.addLeds <WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_AMPS * 1000);
  wificonnect();
  EEPROMGET();
  FastLED.setBrightness(BRIGHTNESS);
}

void wificonnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    for (int i=0;i<NUM_LEDS;i++) {
      leds[i] = CRGB::Yellow;
    }
    FastLED.show();
  }
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

  server.begin();
  Serial.println(WiFi.localIP());
}


void serprint() {
  Serial.println(onoff);
  Serial.println(BRIGHTNESS);
  Serial.println(Mode);
  Serial.println(RedColor1);
  Serial.println(RedColor2);
  Serial.println(RedColor3);
  Serial.println(GreenColor1);
  Serial.println(GreenColor2);
  Serial.println(GreenColor3);
  Serial.println(BlueColor1);
  Serial.println(BlueColor2);
  Serial.println(BlueColor3);
  Serial.println(Speed1);
  Serial.println(Speed2);
  Serial.println(Speed3);
}

void loop() {
  BT_Send_Read();
  ledticks();
  sleepT();
  animation();
}

void ledticks() {
  if (LedTickOff.isReady()) {
    LedTickOn.setTimeout(5000);
    LedTickOff.stop(); 
  }
  if (LedTickOn.isReady()) {
    LedTickOff.setTimeout(5000);
    LedTickOn.stop();
  }
  if (LedOffTimer.isReady()) {
    digitalWrite(LED_INFO, LOW);
  }
}

void sleepT() {
  if (SleepTimer.isReady()) {
    onoff = false;
    EEPROMSAVE();
  }
}

int smincr(int value, int step, int minc, int maxc, bool action) {
  if (value == minc || value == maxc) return value;
  if (action == true) { 
    if (value != maxc) {
      value = value+step;
    } 
  } 
  else if (action == false) {
    if (value != minc) {
      value = value-step;
    }
  }
  
  return value;
}

void colorPicker(bool act, int color) {
  if (onoff == false) return;
  switch (Mode) {
  case 4: 
    switch (color) {
      case 1: 
        if (act == true) RedColor1 = smincr(RedColor1, 5, 0, 255, true);
        else RedColor1 = smincr(RedColor1, 5, 0, 255, false);
        break;
      case 2: 
        if (act == true) GreenColor1 = smincr(GreenColor1, 5, 0, 255, true);
        else GreenColor1 = smincr(GreenColor1, 5, 0, 255, false);
        break;
      case 3: 
        if (act == true) BlueColor1 = smincr(BlueColor1, 5, 0, 255, true);
        else BlueColor1 = smincr(BlueColor1, 5, 0, 255, false);
        break;
      } 
    break;
  case 5:
    switch (color) {
      case 1: 
        if (act == true) RedColor2 = smincr(RedColor2, 5, 0, 255, true);
        else RedColor2 = smincr(RedColor2, 5, 0, 255, false);
        break;
      case 2: 
        if (act == true) GreenColor2 = smincr(GreenColor2, 5, 0, 255, true);
        else GreenColor2 = smincr(GreenColor2, 5, 0, 255, false);
        break;
      case 3: 
        if (act == true) BlueColor2 = smincr(BlueColor2, 5, 0, 255, true);
        else BlueColor2 = smincr(BlueColor2, 5, 0, 255, false);
        break;
      }
    break;
  case 6:
    switch (color) {
      case 1: 
        if (act == true) RedColor3 = smincr(RedColor3, 5, 0, 255, true);
        else RedColor3 = smincr(RedColor3, 5, 0, 255, false);
        break;
      case 2: 
        if (act == true) GreenColor3 = smincr(GreenColor3, 5, 0, 255, true);
        else GreenColor3 = smincr(GreenColor3, 5, 0, 255, false);
        break;
      case 3: 
        if (act == true) BlueColor3 = smincr(BlueColor3, 5, 0, 255, true);
        else BlueColor3 = smincr(BlueColor3, 5, 0, 255, false);
        break;
      }
    break;
    EEPROMSAVE();
  }
}

void EEPROMSAVE() {
  EEPROM.put(1, 1);
  EEPROM.put(4, onoff);
  EEPROM.put(8, BRIGHTNESS);
  EEPROM.put(11, Mode);
  EEPROM.put(14, RedColor1);
  EEPROM.put(17, RedColor2);
  EEPROM.put(20, RedColor3);
  EEPROM.put(23, GreenColor1);
  EEPROM.put(26, GreenColor2);
  EEPROM.put(29, GreenColor3);
  EEPROM.put(22, BlueColor1);
  EEPROM.put(25, BlueColor2);
  EEPROM.put(28, BlueColor3);
  EEPROM.put(31, Speed1);
  EEPROM.put(34, Speed2);
  EEPROM.put(37, Speed3);
}

void EEPROMGET() {
  EEPROM.get(4, onoff);
  EEPROM.get(8, BRIGHTNESS);
  EEPROM.get(11, Mode);
  EEPROM.get(14, RedColor1);
  EEPROM.get(17, RedColor2);
  EEPROM.get(20, RedColor3);
  EEPROM.get(23, GreenColor1);
  EEPROM.get(26, GreenColor2);
  EEPROM.get(29, GreenColor3);
  EEPROM.get(22, BlueColor1);
  EEPROM.get(25, BlueColor2);
  EEPROM.get(28, BlueColor3);
  EEPROM.get(31, Speed1);
  EEPROM.get(34, Speed2);
  EEPROM.get(37, Speed3);
}
