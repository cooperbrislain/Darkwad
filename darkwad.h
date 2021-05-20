#ifndef DARKWAD_H
#define DARKWAD_H

#include <Arduino.h>

#include <string>
#include <map>
#include <iostream>

#include <SPIFlash.h>
#include <SPI.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"

#include "Light.h"
#include "Button.h"
#include "DPad.h"
#include "BrakeControl.h"

#define LED_FRONT lights[1]
#define LED_REAR lights[4]
#define LED_LEFT lights[2]
#define LED_RIGHT lights[3]

struct Config {
    String name;
    int    speed;
    int    fade;
    int    brightness;
    int    num_leds;
    int    num_lights;
    int    num_controls;
    int    num_params;
    int    bump_led;
};

#define halt(s) { Serial.println(F( s )); while(1);  }

Light::State* stateFromJson(JsonObject jsonState);
Action* actionFromJson(JsonObject jsonAction);

#endif //DARKWAD_H
