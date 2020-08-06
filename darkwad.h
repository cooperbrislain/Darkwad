#ifndef DARKWAD_H
#define DARKWAD_H

#include <Arduino.h>

#include <string.h>
#include <SPI.h>
#include <FastLED.h>
#include <iostream>

#include "config.h"

#include "light.h"
#include "control.h"

#define halt(s) { Serial.println(F( s )); while(1);  }

void reconnect();
void blink();
void blink_rainbow();
void blackout();

void mqtt_callback(char* topic, byte* payload, unsigned int length);

#endif //DARKWAD_H
