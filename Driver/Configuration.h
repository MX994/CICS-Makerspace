#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

/*
 * WiFi Configuration
 */

const char *SPMAKER_WIFI_SSID = "Dam";
const char *SPMAKER_WIFI_PASS = "";
/* 
  LEDs
*/
// LED Digital Pins.
#define LED_CICS 2
#define LED_SPMAKER 4
#define LED_STATUS 5

// LED Counts.
#define LED_CICS_CNT 256
#define LED_SPMAKER_CNT 84
#define LED_STATUS_CNT 512

// LED Arrays.
CRGB LEDS_CICS[LED_CICS_CNT];
CRGB LEDS_SPMAKER[LED_SPMAKER_CNT];
CRGB LEDS_STATUS[LED_STATUS_CNT];

// URL to fetch data from.
const char *SPMAKER_DATA_QUERY = "";

// Work Struct
typedef struct {
  // Public members.
  uint32_t CICS_COLOR;
  uint32_t SPMAKER_COLOR;
  uint32_t STATUS_COLOR;
  char *STATUS_TEXT;
  char BRIGHTNESS;  
  // Internal;
  int16_t STATUS_CURR_X;
  int16_t STATUS_CURR_Y;
  uint16_t STATUS_LENGTH;
} SPMAKER_WK_STRUCT;

// Other Private Variables.
FastLED_NeoMatrix *Matrix;
SPMAKER_WK_STRUCT *Wk;
AsyncWebServer Server(80);

#endif
