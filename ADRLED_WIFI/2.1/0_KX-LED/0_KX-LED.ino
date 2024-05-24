#define ERASE_EEPROM false
#define EEPROM_SIZE 512

#define LED_PIN 22
#define NUM_LEDS 1111
#define BUZZER_PIN 2

#define otaName "KXLED-ESP32"
#define ssidAP "ESP32-Configure"
#define passwordAP ""
#define udpPort 5394
#define udpSendAddress "192.168.1.255"
#define MAX_UDP_PACKET_SIZE 255
#define NETWORK_KEY_SIZE 6
const char NETWORK_KEY[] = "KXLED:";

String wifiSsid = "";
String wifiPassword = "";

bool OnOff = true;
bool TempOn = false;
bool TempOff = false;
int MAX_CURRENT = 10000;
uint8_t Brightness = 255;
uint8_t Mode = 1;

float RainbowLoopStep = 0.5;
int RainbowLoopSpeed = 5;
int RainbowFadeSpeed = 100;
int StrobeRandomColorsSpeed = 100;
int emsLightsSpeed = 10;
int rusLightsSpeed = 10;
uint8_t StaticHueAll = 255;
uint8_t StaticSatAll = 255;

uint8_t FireHueGap = 50;             // заброс по hue
uint8_t FireStep = 15;               // шаг огня
uint8_t FireHueStart = 0;            // начальный цвет огня (0 красный, 80 зелёный, 140 молния, 190 розовый)
uint8_t FireMinBrightness = 200;     // мин. яркость огня
uint8_t FireMinSaturation = 150;     // мин. насыщенность
uint8_t FireMaxSaturation = 255;     // макс. насыщенность

#include "FastLED.h"
#include "EEPROM.h"
#include "WiFi.h"
#include "WiFiUdp.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "DNSServer.h"
#include "ESPmDNS.h"
#include "ArduinoOTA.h"

CRGB leds[NUM_LEDS];
AsyncWebServer serverAP(80);
DNSServer dnsServer;
WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  if (ERASE_EEPROM || EEPROM.read(0) != 123) updEEPROM();
  else getEEPROM();

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_CURRENT);
  FastLED.setBrightness(Brightness);

  if (!WiFiConnectSTA()) WiFiConnectAP();

  fillAll(CRGB::Black);
}

void loop() {
  if (WiFi.getMode() == WIFI_AP) networkAPLoop();
  else networkSTALoop();
  animation();
}