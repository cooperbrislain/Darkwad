#ifndef DARKWAD_H
#define DARKWAD_H

#include <Arduino.h>

#include <string.h>
#include <SPI.h>
#include <FastLED.h>
#include <ArduinoJson.h>

#include "config.h"

#include "light.h"
#include "touchcontrol.h"

//#include "esp_wifi.h"
//#include "esp_log.h"
//#include "esp_event.h"
//#include "nvs_flash.h"

#ifdef TOUCH
    #define TOUCH_PIN T0
    #ifndef TOUCH_THRESHOLD
        #define TOUCH_THRESHOLD 50
    #endif
#endif

#ifndef BRIGHTNESS_SCALE
    #define BRIGHTNESS_SCALE 50
#endif

#define halt(s) { Serial.println(F( s )); while(1);  }

void reconnect();
void blink();
void blink_rainbow();
void blackout();

void mqtt_callback(char* topic, byte* payload, unsigned int length);

#endif //DARKWAD_H
