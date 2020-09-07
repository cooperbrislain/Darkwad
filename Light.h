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

    struct State {
        String  name;
        int*    params = nullptr;
        String  color = "";
        String  program = "";
        int     onoff = -1;
        int     count = -1;
    };

private:

    CRGB** _leds;
    State _state;
    CRGB _color;
    int _params [NUM_PARAMS];
    int _num_leds;
    int _offset;
    int _last_brightness;
    bool _onoff;
    String _name;
    unsigned int _count;
    unsigned int _index;
    int _prog_solid(int x);
    int _prog_chase(int x);
    int _prog_blink(int x);
    int _prog_fade(int x);
    int _prog_warm(int x);
    int _prog_xmas(int x);
    int _prog_lfo(int x);
    int _prog_fadeout(int x);
    int _prog_fadein(int x);
    int _prog_longfade(int x);
    int _prog_darkwad(int x);
    int (Light::*_prog)(int x);

public:

    Light() :
        _color      { CRGB::White },
        _onoff      { 0 },
        _num_leds   { 0 },
        _name       { "light" },
        _prog       { &Light::_prog_solid },
        _count      { 0 },
        _params     { 1, 0, 0, 0 }
    { };
    Light(String name, CRGB* leds, int offset, int num_leds, int reverse=0) :
        _name       { name },
        _num_leds   { num_leds },
        _color      { CRGB::White },
        _onoff      { 0 },
        _count      { 0 },
        _offset     { offset },
        _prog       { &Light::_prog_solid },
        _params     { 1, 0, 0, 0 }
    {
        _leds = new CRGB*[num_leds];
        for (int i=0; i<num_leds; i++) {
            _leds[i] = reverse? &leds[offset+num_leds-i-1] : &leds[offset+i];
        }
    };
    Light(String name, CRGB** leds) :
        _name       { name },
        _color      { CRGB::White },
        _count      { 0 },
        _offset     { 0 },
        _onoff      { 0 },
        _num_leds   { sizeof(leds) },
        _prog       { &Light::_prog_solid },
        _params     { 1, 0, 0, 0 }
    {
        _leds = new CRGB*[_num_leds];
        for (int i=0; i<sizeof(leds); i++) {
            _leds[i] = leds[i];
        }
    };
    Light(CRGB* leds, JsonObject jsonLight) :
        _name       { jsonLight["name"].as<String>() },
        _onoff      { 0 },
        _offset     { 0 },
        _count      { 0 },
        _color      { CRGB::White },
        _prog       { &Light::_prog_solid },
        _params     { 1, 0, 0, 0 }
    {
        if (jsonLight["leds"]) {
            JsonArray jsonLeds = jsonLight["leds"];
            int led_count = jsonLeds.size();
            _leds = new CRGB*[led_count];
            for (int i=0; i<led_count; i++) {
                _leds[i] = &leds[jsonLeds[i].as<int>()];
            }
            _num_leds = led_count;

        } else if (jsonLight["led_count"]) {
            int led_count   = jsonLight["led_count"];
            int led_offset  = jsonLight["led_offset"];
            int reverse     = jsonLight["reverse"];
            _leds = new CRGB*[led_count];
            for (int i=0; i<led_count; i++) {
                _leds[i] = reverse? &leds[led_offset+led_count-i-1] : &leds[led_offset+i];
            }
            _num_leds = led_count;
        } else {
            // bad
        }
        if (jsonLight["program"])   setProgram(jsonLight["program"]);
        if (jsonLight["color"])     setColor(jsonLight["color"]);
        if (jsonLight["params"]) {
            JsonArray jsonParams = jsonLight["params"];
            int numParams = jsonParams.size();
            for (int i=0; i<numParams; i++) {
                _params[i] = jsonParams[i].as<int>();
            }
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