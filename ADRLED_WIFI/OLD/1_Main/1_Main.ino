//#define ERASE_DATA
#define DEBUG
//#define IRCHECK

#include <FastLED.h> // подключаем библиотеку фастлед
#include <GyverTimer.h>
#include <EEPROM.h>
#include <NecDecoder.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer serverA(80);
WiFiServer server(80);

const char* ssid = "Homenet";
const char* password = "savran51155115";

byte baza = 0;     // изменение оттенка LED
int Mode = 1;
int BRIGHTNESS = 100;
int OLD_BRIGHTNESS=0;
int Speed1 = 5;
int Speed2 = 5;
int Speed3 = 5;
byte HUE1 = 255;
byte HUE2 = 255;
byte HUE3 = 255;
byte HUE4 = 255;
byte HUE5 = 255;
byte HUE6 = 255;
byte SAT1 = 255;
byte SAT2 = 255;
byte SAT3 = 255;
byte SAT4 = 255;
byte SAT5 = 255;
byte SAT6 = 255;
bool onoff = true;
bool mode1pick;

#define LED_INFO 13
#define NUM_LEDS 1111
#define BUZZER_PIN D1
#define PIN D4
#define IR_PIN D2
#define STATE_PIN D7
#define MAX_AMPS 39

#define IR_BTN_1      0xA2 //1
#define IR_BTN_2      0x62 //2
#define IR_BTN_3      0xE2 //3
#define IR_BTN_C1     0x22 //4
#define IR_BTN_C2     0x2  //5
#define IR_BTN_C3     0xC2 //6
#define IR_BTN_C4     0xE0 //7
#define IR_BTN_C5     0xA8 //8
#define IR_BTN_C6     0x90 //9
#define IR_BTN_STAR   0x68 //*
#define IR_BTN_ZERO   0x98 //0
#define IR_BTN_HASH   0xB0 //#
#define IR_BTN_ONOFF  0x38 //OK
#define IR_BTN_UP     0x18 //UP
#define IR_BTN_DOWN   0x4A //DOWN
#define IR_BTN_RIGHT  0x5A //RIGHT
#define IR_BTN_LEFT   0x10 //LEFT

CRGB leds[NUM_LEDS];
GTimer SleepTimer(MS);
GTimer LedOffTimer(MS);
GTimer LedTickOff(MS);
GTimer LedTickOn(MS);
NecDecoder ir;

void IRAM_ATTR irIsr() {
  ir.tick();
}

void setup() {
  #ifdef ERASE_DATA
    EEPROMSAVE();
  #endif
  Serial.begin(115200);
  //if (EEPROM.read(1) != 1) EEPROMSAVE();
  pinMode(LED_INFO, OUTPUT);
  FastLED.addLeds <WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  if (MAX_AMPS > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_AMPS * 1000);
  FastLED.setBrightness(BRIGHTNESS);
  attachInterrupt(IR_PIN, irIsr, FALLING);
  EEPROMGET();
  SerialDebug();
  wificonnect();
}

void loop() {
  #ifdef IRCHECK
    irread();
    ledticks();
    return;
  #endif 
  WiFiRead();
  ircheck();
  ledticks();
  sleepT();
  animation();
}

void SerialDebug() {
  Serial.println("-------------------------------------------------");
  Serial.print("Если не 1, то первый запуск: ");
  Serial.println(EEPROM.read(1));
  if (onoff == true) Serial.println("ON");
  else Serial.println("OFF");
  Serial.print("Режим: ");
  Serial.println(Mode);
  Serial.print("Режим1: ");
  Serial.println(mode1pick);
  Serial.print("Яркость: ");
  Serial.println(BRIGHTNESS);
  Serial.print("SP1: ");
  Serial.println(Speed1);
  Serial.print("SP2: ");
  Serial.println(Speed2);
  Serial.print("SP3: ");
  Serial.println(Speed3);
}

void ledticks() {
  if (LedTickOff.isReady()) {
    digitalWrite(LED_INFO, LOW);
    LedTickOn.setTimeout(5000);
    LedTickOff.stop(); 
  }
  if (LedTickOn.isReady()) {
    digitalWrite(LED_INFO, HIGH);
    LedTickOff.setTimeout(2500);
    LedTickOn.stop();
  }
  if (LedOffTimer.isReady()) {
    digitalWrite(LED_INFO, LOW);
  }
}

void sleepT() {
  if (SleepTimer.isReady()) {
    FastLED.clear();
    FastLED.show();
    onoff = false;
    EEPROMSAVE();
  }
}

int smincr(int value, int incr_step, int mininmum, int maximum) {
  int val_buf = value + incr_step;
  val_buf = constrain(val_buf, mininmum, maximum);
  return val_buf;
}

void EEPROMSAVE() {
  EEPROM.put(1, 1);
  EEPROM.put(4, onoff);
  EEPROM.put(8, BRIGHTNESS);
  EEPROM.put(11, Mode);
  EEPROM.put(14, HUE1);
  EEPROM.put(17, HUE2);
  EEPROM.put(20, HUE3);
  EEPROM.put(23, HUE4);
  EEPROM.put(26, HUE5);
  EEPROM.put(29, HUE6);
  EEPROM.put(32, SAT1);
  EEPROM.put(35, SAT2);
  EEPROM.put(38, SAT3);
  EEPROM.put(41, Speed1);
  EEPROM.put(44, Speed2);
  EEPROM.put(47, Speed3);
  EEPROM.put(50, SAT4);
  EEPROM.put(53, SAT5);
  EEPROM.put(56, SAT6);
}

void EEPROMGET() {
  EEPROM.get(4, onoff);
  EEPROM.get(8, BRIGHTNESS);
  EEPROM.get(11, Mode);
  EEPROM.get(14, HUE1);
  EEPROM.get(17, HUE2);
  EEPROM.get(20, HUE3);
  EEPROM.get(23, HUE4);
  EEPROM.get(26, HUE5);
  EEPROM.get(29, HUE6);
  EEPROM.get(32, SAT1);
  EEPROM.get(35, SAT2);
  EEPROM.get(38, SAT3);
  EEPROM.get(41, Speed1);
  EEPROM.get(44, Speed2);
  EEPROM.get(47, Speed3);
  EEPROM.get(50, SAT4);
  EEPROM.get(53, SAT5);
  EEPROM.get(56, SAT6);
}