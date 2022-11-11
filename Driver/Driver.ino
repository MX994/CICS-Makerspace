#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "Configuration.h"

void setup() {
  Serial.begin(9600);

  // WiFi initialization.
  Serial.println("Connecting to WiFi...");
  WiFi.begin(SPMAKER_WIFI_SSID, SPMAKER_WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    // Without a delay, status will be called many times, 
    // which causes the ESP32 to crash.
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("Connected. Current IP Address is ");
  Serial.println(WiFi.localIP());

  // LED initialization.
  Serial.println("Setting up LEDs...");
  ConfigureLEDs();
  Serial.println("Setting up backend work...");
  SetupWkStruct();
  Serial.println("Done!");
}

void ConfigureLEDs() {
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000); // 5V, 3A.
  FastLED.setBrightness(128);
  FastLED.addLeds<WS2812, LED_CICS, GRB>(LEDS_CICS, LED_CICS_CNT);
  FastLED.addLeds<WS2812, LED_SPMAKER, GRB>(LEDS_SPMAKER, LED_SPMAKER_CNT);
  FastLED.addLeds<WS2812, LED_STATUS, GRB>(LEDS_STATUS, LED_STATUS_CNT);
  
  Matrix = new FastLED_NeoMatrix(LEDS_STATUS, 32, 16, 1, 1, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
  Matrix->setTextWrap(false);
  Matrix->begin();
}

void SetupWkStruct() {
  Wk = (SPMAKER_WK_STRUCT *)malloc(sizeof(SPMAKER_WK_STRUCT));
  Server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    // Home stuff.
    if (request->hasParam("CICS_COLOR")) {
      Wk->CICS_COLOR = strtoul(request->getParam("CICS_COLOR")->value().substring(2).c_str(), NULL, 16);
    }
    if (request->hasParam("SPMAKER_COLOR")) {
      Wk->SPMAKER_COLOR = strtoul(request->getParam("SPMAKER_COLOR")->value().substring(2).c_str(), NULL, 16);
    }
    if (request->hasParam("STATUS_COLOR")) {
      Wk->STATUS_COLOR = strtoul(request->getParam("STATUS_COLOR")->value().substring(2).c_str(), NULL, 16);
    }
    if (request->hasParam("BRIGHTNESS")) {
      Wk->BRIGHTNESS = request->getParam("STATUS_COLOR")->value();
      FastLED.setBrightness(Wk->BRIGHTNESS);
    }
    if (request->hasParam("STATUS_TEXT")) {
      Wk->STATUS_TEXT = request->getParam("STATUS_TEXT");
      Wk->STATUS_LENGTH = strlen(Wk->STATUS_TEXT);
      Wk->STATUS_X = Wk->STATUS_LENGTH * 6;
    }
  });
}

void loop() {
  // CICS Logo.
  fill_solid(LEDS_CICS, 64, Wk->CICS_COLOR);
  fill_solid(LEDS_CICS + 160, 96, Wk->CICS_COLOR);

  // Makerspace Logo.
  fill_solid(LEDS_SPMAKER, LED_SPMAKER_CNT, Wk->SPMAKER_COLOR);
  
  // Status scrolling.
  Matrix->fillScreen(0);
  Matrix->setCursor(Wk->STATUS_X, Wk->STATUS_Y);
  Matrix->setTextColor(Wk->STATUS_COLOR);
  Matrix->print(Wk->STATUS_TEXT);
  if (--Wk->STATUS_X < Wk->STATUS_LENGTH * -6) {
    Wk->STATUS_X = Wk->STATUS_LENGTH * 6;
  }
  
  FastLED.show();
}
