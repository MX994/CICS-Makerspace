#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <WiFi.h>
#include "Configuration.h"

FastLED_NeoMatrix *matrix;

int x;
int pass = 0;
char currString[] = "WE ARE OPEN";

void bottom_matrix_loop() {
  matrix->fillScreen(0);
  matrix->setCursor(x, 4);
  matrix->setTextColor(0xFF);
  matrix->print(currString);
  if(--x < -strlen(currString) * 6) {
    x = matrix->width();
  }
  delay(100);
}

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
}

void ConfigureLEDs() {
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000); // 5V, 3A.
  FastLED.setBrightness(128);
  FastLED.addLeds<WS2812, LED_CICS, GRB>(LEDS_CICS, LED_CICS_CNT);
  FastLED.addLeds<WS2812, LED_SPMAKER, GRB>(LEDS_SPMAKER, LED_SPMAKER_CNT);
  FastLED.addLeds<WS2812, LED_STATUS, GRB>(LEDS_STATUS, LED_STATUS_CNT);
  
  matrix = new FastLED_NeoMatrix(LEDS_STATUS, 32, 16, 1, 1, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
  matrix->setTextWrap(false);
  matrix->begin();
}

void rainbow_wave(uint8_t thisSpeed, uint8_t deltaHue) {     // The fill_rainbow call doesn't support brightness levels.
 uint8_t thisHue = beatsin8(thisSpeed,0,255);                // A simple rainbow wave.
 fill_rainbow(LEDS_SPMAKER, LED_SPMAKER_CNT, thisHue, deltaHue);            // Use FastLED's fill_rainbow routine.
} // rainbow_wave()

void loop() {
  fill_solid(LEDS_CICS, 64, 0x800000);
  fill_solid(LEDS_CICS + 192, 64, 0x800000);
  rainbow_wave(10, 10); 
  bottom_matrix_loop();
  FastLED.show();
}
