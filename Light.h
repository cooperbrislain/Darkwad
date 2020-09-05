#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>
#include <SPI.h>
#include <FastLED.h>
#include <string>
#include <map>
#include "config.h"
#include <iostream>
#include <ArduinoJson.h>
#include "StreamPrint.h"

typedef void (*ProgFn)();

std::map<String, CRGB> colorMap;
std::map<String, ProgFn> progMap;

class Light {

public:

    struct State {
        String  name;
        int*    params = nullptr;
        CRGB    color;
        ProgFn  prog  = nullptr;
        int     onoff = -1;
        int     count = -1;
    };

private:

    String  name;
    CRGB**  leds;
    int     numLeds;
    State   state;

    void progSolid();
    void progChase();
    void progBlink();
    void progFade();
    void progLfo();
    void progFadeout();
    void progFadein();
    void progLongfade();

public:

    Light() :
        name        { "light" },
        numLeds     { 0 }
    { };
    Light(String _name, CRGB* _leds, int _offset, int _numLeds, int _reverse=0) :
        name    (_name),
        numLeds (_numLeds)
    {
        int offset = _offset;
        this->leds = new CRGB*[numLeds];
        for (int i=0; i<numLeds; i++) {
            this->leds[i] = _reverse? &_leds[offset+numLeds-i-1] : &_leds[offset+i];
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

    void    setState(State* _state);

    State   getState();

    void turnOn();
    void turnOff();
    void toggle();
    void turn(int _onoff);
    void blink();

    void update();

    CHSV getHsv();
    CRGB getRgb();
    int getParam(int);
    void setLeds(CRGB** _leds, int _numLeds);
    void setHue(int);
    void setRgb(CRGB);
    void setHsv(CHSV);
    void setHsv(int _h, int _s, int _v);
    void setBrightness(int);
    void setSaturation(int);
    void setParam(int _p, int _v);
    void setParams(int*);
    void setProgram(String);
    void setProgram(ProgFn);
    void setColor(String);

};

uint8_t nblendU8TowardU8(uint8_t cur, const uint8_t target, uint8_t x);
CRGB fadeTowardColor(CRGB cur, CRGB target, uint8_t x);

#endif // LIGHT_H