#ifndef LIGHT_H
#define LIGHT_H

#define NUM_PARAMS 4

#include <Arduino.h>
#include <SPI.h>
#include <FastLED.h>
#include "config.h"
#include <iostream>
#include <ArduinoJson.h>
#include "StreamPrint.h"

class Light {
public:

    typedef int (*progFn)(int);

    struct State {
        String  name;
        int*    params = nullptr;
        String  color = "";
        ProgFn  prog = nullptr;
        int     onoff = -1;
        int     count = -1;
    };

private:

    String name;
    CRGB** leds;
    int num_leds;
    State state;
    int progSolid(int x);
    int progChase(int x);
    int progBlink(int x);
    int progFade(int x);
    int progLfo(int x);
    int progFadeout(int x);
    int progFadein(int x);
    int progLongfade(int x);

public:

    Light() :
        name        { "light" }
        num_leds    { 0 }
    { };
    Light(String _name, CRGB* _leds, int _offset, int _numLeds, int _reverse=0) :
        name    (_name),
        numLeds (_numLeds),
        offset  (_offset)
    {
        this->leds = new CRGB*[numLeds];
        for (int i=0; i<numLeds; i++) {
            this->leds[i] = reverse? &_leds[offset+numLeds-i-1] : &_leds[offset+i];
        }
    };
    Light(String _name, CRGB** _leds) :
        name    (_name),
        numLeds { sizeof(_leds) }
    {
        this->leds = new CRGB*[numLeds];
        for (int i=0; i<sizeof(_leds); i++) {
            this->leds[i] = _leds[i];
        }
    };
    String getName();
    void turnOn();
    void turnOff();
    void turn(int onoff);
    void blink();
    void toggle();
    void setHue(int);
    void setRgb(CRGB);
    void setHsv(CHSV);
    void setHsv(int hue, int sat, int val);
    void setBrightness(int);
    void setSaturation(int);
    void setProgram(String programName);
    void setParam(int p, int v);
    void setParams(int* params);
    void setColor(String colorName);
    void setState(State* state);
    int  getParam(int p);
    CRGB getRgb();
    CHSV getHsv();
    void update();

};

uint8_t nblendU8TowardU8(uint8_t cur, const uint8_t target, uint8_t x);
CRGB fadeTowardColor(CRGB cur, CRGB target, uint8_t x);

#endif // LIGHT_H