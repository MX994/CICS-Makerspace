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

#endif
