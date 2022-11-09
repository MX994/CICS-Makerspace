#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Configuration.h"

FastLED_NeoMatrix *matrix;

int x;
int pass = 0;
char currString[] = "WE ARE OPEN";
String host_url = "https://localhost:8080";

void bottom_matrix_loop() {
  Serial.println(x);
  matrix->fillScreen(0);
  matrix->setCursor(x, 4);
  matrix->setTextColor(0xFF);
  matrix->print(currString);
  if(--x < strlen(currString) * -6) {
    x = matrix->width();
  }
  delay(100);
}

String getQuote(String input_quote){
    HTTPClient http;
    String quote;
    http.begin(host_url + "/quote/" + input_quote);
    int httpCode = http.GET();
    if (httpCode > 0) {
        String payload = http.getString();
        int start = payload.indexOf("quote");
        int end = payload.length() - 2;
        quote = payload.substring(start+8, end-3);
    }
    http.end();
    return quote;
}

int * getCICSMatrix(String brightnessId, String colorId, String brightness){
    int ans[] = {0, 0, 0, 0};
    HTTPClient http;
    HTTPClient http2;
    http.begin(host_url + "/cics/brightness/" + brightnessId);
    http2.begin(host_url + "/cics/color/" + colorId);
    int httpCode = http.GET();
    int i = 0;
    if (httpCode > 0) {
        String payload = http.getString();
        int start = payload.indexOf("color") + 7;
        int end = payload.length() - 1;
        String color = payload.substring(start, end);
        int index = color.indexOf(",");
        while (index == -1){
            ans[i] = color.substring(start, index).toInt();
            i+=1;
            index = color.indexOf(",", index+1);
        }
    }
    http.end();
    int httpCode2 = http2.GET();
    if (httpCode2 > 0) {
        String payload2 = http2.getString();
        int start2 = payload2.indexOf("brightness") + 12;
        int end2 = payload2.length() - 2;
        ans[i] = payload2.substring(start2, end2).toInt();
    }
    http2.end();
    return ans;
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
