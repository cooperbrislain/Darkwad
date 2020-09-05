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

std::map<String, CRGB> colorMap;

class Light {

public:

    typedef int (Light::*ProgFn)(int*, int*, int**);

    struct State {
        String  name;
        int*    params = nullptr;
        int*    progVars = nullptr;
        CRGB    color;
        ProgFn  prog  = nullptr;
        int     onoff = -1;
        int     count = -1;
    };

    class Prog {
    private:

        Light*      light;
        int*        params; // set externally at invocation
        int*        vars;   // operated on internally on each loop
        int**       refs;   // refs to external variables
        ProgFn      prog;

    public:
        Prog (Light* _light, ProgFn _prog, int* _params, int** refs) :
                light   (_light),
                prog    {_prog},
                params  {_params},
                refs    { _refs? _refs : [] }
                vars    { 0,0,0,0 };
        {
        };
        void operator() () {
            progFn(params, vars, refs);
        }
    };

private:

    String  name;
    CRGB**  leds;
    int     numLeds;
    State   state;

    int progSolid(int*, int*, int**);
    int progChase(int*, int*, int**);
    int progBlink(int*, int*, int**);
    int progFade(int*, int*, int**);
    int progLfo(int*, int*, int**);
    int progFadeout(int*, int*, int**);
    int progFadein(int*, int*, int**);
    int progLongfade(int*, int*, int**);
    int progTwinkle(int* int* int**);

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
    void setProgram(ProgFn);
    void setColor(String);

};

std::map<String, Light::ProgFn*> progMap;

uint8_t nblendU8TowardU8(uint8_t cur, const uint8_t target, uint8_t x);
CRGB fadeTowardColor(CRGB cur, CRGB target, uint8_t x);

#endif // LIGHT_H