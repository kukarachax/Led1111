//#define ERASE_DATA
#define DEBUG
//#define IRCHECK
//#define APFORCE

#include <FastLED.h>
#include <GyverTimer.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer serverAP(80);
WiFiServer server(80);
/*
char ssid[] = "Homenet";
char password[] = "savran51155115";
*/

char* ssid = "";
char* password = "";
const char* ssidAP = "ESP32-Configure";
const char* passwordAP = "12345678";

byte baza = 0;     // изменение оттенка LED
int Mode = 1;
int BRIGHTNESS = 100;
int OLD_BRIGHTNESS=0;
int Speed1 = 5;
int Speed2 = 5;
int Speed3 = 5;
int Speed4 = 5;
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
#define BUZZER_PIN 2
#define IR_PIN 12
#define PIN 22
#define STATE_PIN 20
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

void setup() {
  EEPROM.begin(200);
  #ifdef ERASE_DATA
    EEPROMSAVE();
  #endif
  if (EEPROM.read(0) != 1) EEPROMSAVE();
  EEPROMGET();

  Serial.begin(115200);
  
  pinMode(LED_INFO, OUTPUT);

  FastLED.addLeds <WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  if (MAX_AMPS > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_AMPS * 1000);
  FastLED.setBrightness(BRIGHTNESS);

  SerialDebug();

  #ifdef APFORCE
    APactivate();
  #else
    wificonnect();
  #endif
}

void loop() {
  #ifdef IRCHECK
    //irread();
    ledticks();
    return;
  #endif 
  WiFiRead();
  //ircheck();
  ledticks();
  sleepT();
  animation();
}

void SerialDebug() {
  Serial.println("-------------------------------------------------");
  Serial.print("Если не 1, то первый запуск: ");
  Serial.println(EEPROM.read(0));
  if (onoff == true) Serial.println("ON");
  else Serial.println("OFF");
  Serial.print("Режим: ");
  Serial.println(Mode);
  Serial.print("Яркость: ");
  Serial.println(BRIGHTNESS);
  Serial.print("SP1: ");
  Serial.println(Speed1);
  Serial.print("SP2: ");
  Serial.println(Speed2);
  Serial.print("SP3: ");
  Serial.println(Speed3);
  Serial.print("SP4: ");
  Serial.println(Speed4);
  Serial.print("HUE1: ");
  Serial.println(HUE1);
  Serial.print("HUE2: ");
  Serial.println(HUE2);
  Serial.print("HUE3: ");
  Serial.println(HUE3);
  Serial.print("HUE4: ");
  Serial.println(HUE4);
  Serial.print("HUE5: ");
  Serial.println(HUE5);
  Serial.print("HUE6: ");
  Serial.println(HUE6);
  Serial.print("SAT1: ");
  Serial.println(SAT1);
  Serial.print("SAT2: ");
  Serial.println(SAT2);
  Serial.print("SAT3: ");
  Serial.println(SAT3);
  Serial.print("SAT4: ");
  Serial.println(SAT4);
  Serial.print("SAT5: ");
  Serial.println(SAT5);
  Serial.print("SAT6: ");
  Serial.println(SAT6);
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
  EEPROM.put(0, 1);
  EEPROM.put(4, onoff);
  EEPROM.put(8, BRIGHTNESS);
  EEPROM.put(12, Mode);
  EEPROM.put(16, HUE1);
  EEPROM.put(20, HUE2);
  EEPROM.put(24, HUE3);
  EEPROM.put(28, HUE4);
  EEPROM.put(32, HUE5);
  EEPROM.put(36, HUE6);
  EEPROM.put(40, SAT1);
  EEPROM.put(44, SAT2);
  EEPROM.put(48, SAT3);
  EEPROM.put(52, Speed1);
  EEPROM.put(56, Speed2);
  EEPROM.put(60, Speed3);
  EEPROM.put(64, Speed4);
  EEPROM.put(68, SAT4);
  EEPROM.put(72, SAT5);
  EEPROM.put(76, SAT6);
  EEPROM.commit();
}

void EEPROMGET() {
  EEPROM.get(4, onoff);
  EEPROM.get(8, BRIGHTNESS);
  EEPROM.get(12, Mode);
  EEPROM.get(16, HUE1);
  EEPROM.get(20, HUE2);
  EEPROM.get(24, HUE3);
  EEPROM.get(28, HUE4);
  EEPROM.get(32, HUE5);
  EEPROM.get(36, HUE6);
  EEPROM.get(40, SAT1);
  EEPROM.get(44, SAT2);
  EEPROM.get(48, SAT3);
  EEPROM.get(52, Speed1);
  EEPROM.get(56, Speed2);
  EEPROM.get(60, Speed3);
  EEPROM.get(64, Speed4);
  EEPROM.get(68, SAT4);
  EEPROM.get(72, SAT5);
  EEPROM.get(76, SAT6);
}

String EepromGetWiFiSettings(bool wit) {
    uint8_t SSIDlength; 
    uint8_t PASSlength;

    String TEMPSSID = "";
    String TEMPPASS = "";

    EEPROM.get(100, SSIDlength);
    EEPROM.get(104, PASSlength);

    for (int i=0;i<SSIDlength;i++) {
      TEMPSSID += char(EEPROM.read(110+i));
    }
    Serial.println();
    for (int i=0;i<PASSlength;i++) {
      TEMPPASS += char(EEPROM.read(150+i));
    }
    Serial.println();
    if (wit == true) return TEMPSSID;
    else return TEMPPASS;
}
