#ifndef DARKWAD_H
#define DARKWAD_H

#include <Arduino.h>

#include <string.h>
#include <SPI.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include <iostream>

#include "config.h"

#include "Light.h"
#include "Button.h"
#include "DPad.h"
#include "BrakeControl.h"

#define halt(s) { Serial.println(F( s )); while(1);  }

#endif //DARKWAD_H
