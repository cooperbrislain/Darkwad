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

typedef int (*ProgFn)(CRGB**, int*, int);

class Light {

public:

    class Prog {

    private:

        String          name;
        CRGB**          leds;
        ProgFn          prog;
        Light*          light;

    public:

        Prog (String _name) :
            name    (_name)
        { };
        Prog(String _name, CRGB** _leds, Light* _light) :
            name    (_name),
            leds    (_leds),
            light   (_light)
        {
            Serial << "Created Program: " << name << '\n';
        }
        void operator () (int x) {

        }
        String getName();
    };

    struct State {
        String      name;
        int*        params = nullptr;
        CRGB        color;
        Prog*       prog  = nullptr;
        int         onoff = -1;
        int         count = -1;
    };

private:

    String name;
    CRGB** leds;
    int numLeds;
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

    void    setState(State* state);

    State   getState();

    void turnOn();
    void turnOff();
    void toggle();
    void turn(int onoff);
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
    void setParams(int* _params);
    void setProgram(String _progName);
    void setColor(String colorName);

};

uint8_t nblendU8TowardU8(uint8_t cur, const uint8_t target, uint8_t x);
CRGB fadeTowardColor(CRGB cur, CRGB target, uint8_t x);

#endif // LIGHT_H