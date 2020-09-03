#include "Light.h"

void Light::update() {
    Serial << state.name << "\n";
//    (state.*prog)(this->state.params[1]);
    state.count++;
}

String Light::getName() { return name; }

CHSV Light::getHsv() { return rgb2hsv_approximate(state.color); }
CRGB Light::getRgb() { return state.color; }
int  Light::getParam(int _p) { return state.params[_p]; }

void Light::turnOn() { state.onoff = 1; }
void Light::turnOff() { state.onoff = 0; }
void Light::turn(int _onoff) { state.onoff = _onoff; }
void Light::toggle() { state.onoff = (state.onoff==1? 0 : 1); }

void Light::setRgb(CRGB _color) { state.color = _color;}
void Light::setHsv(int _h, int _s, int _v) { state.color = CHSV(_h,_s,_v); }
void Light::setHsv(CHSV _color) { state.color = _color; }
void Light::setColor(String _color) { state.color = colorMap[_color]; }

void Light::setLeds(CRGB** _leds, int _numLeds) {
    leds = _leds;
    numLeds = _numLeds;
}

void Light::setHue(int val) {
    CHSV hsv_color = getHsv();
    hsv_color.h = val;
    setHsv(hsv_color);
}

void Light::setSaturation(int val) {
    CHSV hsv_color = getHsv();
    hsv_color.s = min(val, 100);
    setHsv(hsv_color);
}

void Light::setBrightness(int val) {
    CHSV hsv_color = getHsv();
    hsv_color.v = min(val, 100);
    setHsv(hsv_color);
}

void Light::setProgram 
//void Light::setProgram(String progName) {
//    if (progName == "solid") _prog = &Light::_prog_solid;
//    if (progName == "chase") {
//        _prog = &Light::_prog_chase;
//        _params[1] = _params[1]? _params[1] : 35;
//    }
//    if (progName == "fade")  _prog = &Light::_prog_fade;
//    if (progName == "blink") _prog = &Light::_prog_blink;
//    if (progName == "warm") {
//        _prog = &Light::_prog_warm;
//        _params[0] = 50;
//    }
//    if (progName == "lfo") _prog = &Light::_prog_lfo;
//}

void Light::setParams(int* params) {
    for (int i=0; i<sizeof(params); i++) {
        _params[i] = params[i];
    }
}

void Light::setParam(int p, int v) {
    _params[p] = v;
}

void Light::setState(State* state) {
    try {
        Serial << "Setting light state: " << "[" << state->name << ": ";
        if (state->onoff != -1) {
            Serial << (state->onoff==1? "On" : "Off") << "|";
            turn(state->onoff);
        }
        if (state->program != "") {
            Serial << "Prog:" << state->program << "|";
            setProgram(state->program);
        }
        if (state->params != nullptr) {
            Serial << "Params...|";
            setParams(state->params);
        }
        if (state->color != "") {
            Serial << "Color:" << state->color;
            setColor(state->color);
        }
        if (state->count != -1) {
            _count = state->count;
        }
        Serial << "]\n";
    } catch (int e) {
        Serial << "An Exception occurred: " << e << "\n";
    }
}

void Light::blink() {
    for (int i=0; i<_num_leds; i++) {
        *(CRGB*)_leds[i] = CRGB::White;
    }
    update();
    delay(25);
    for (int i=0; i<_num_leds; i++) {
        *(CRGB*)_leds[i] = CRGB::Black;
    }
    update();
}

// programs

int Light::_prog_solid(int x) {
    for (int i=0; i<_num_leds; i++) {
        *_leds[i] = _color;
    }
    return 0;
}

int Light::_prog_fade(int x) {
    for(int i=0; i<_num_leds; i++) {
        _leds[i]->fadeToBlackBy(x);
    }
    return 0;
}

int Light::_prog_fadein(int x) {
    bool still_fading = false;
    for(int i=0; i<_num_leds; i++) {
        *_leds[i] = fadeTowardColor(*_leds[i], _color, x);
        if (*_leds[i] != _color) still_fading = true;
    }
    if (!still_fading) _prog = &Light::_prog_solid;
    return 0;
}

int Light::_prog_fadeout(int x) {
    bool still_fading = false;
    for(int i=0; i<_num_leds; i++) {
        _leds[i]->fadeToBlackBy(x);
        if (*_leds[i]) still_fading = true;
    }
    if (!still_fading) _onoff = false;
    return 0;
}

int Light::_prog_chase(int x) {
    // params: 0: Chase Speed
    //         1: Fade Speed
    _prog_fade(_params[1]);
    *_leds[_count%_num_leds] = _color;
    return 0;
}

auto progChase = [ _this, _leds] (_t) -> int {
    *_leds[_t%_this.numLeds] = _this.state.color;
    return ++t;
}

Light::Prog* chase = new Light::Prog(light, )
[ _light, _leds] () -> int { body }

int Light::_prog_warm(int x) {
    if (_count%7 == 0) _prog_fade(10);

    if (_count%11 == 0) {
        _index = random(_num_leds);
        CHSV wc = rgb2hsv_approximate(_color);
        wc.h = wc.h + random(11)-5;
        wc.s = random(128)+128;
        wc.v &=x;
        _color = wc;
    }
    *_leds[_index] += _color;
    return 0;
}

int Light::_prog_xmas(int x) {
    if (_count%7 == 0) _prog_fade(1);

    if (_count%11 == 0) {
        _index = random(_num_leds);
        CHSV wc = rgb2hsv_approximate(_color);
        wc.h = wc.h + random(11)-5;
        wc.s = random(128)+128;
        wc.v &=x;
        _color = wc;
    }
    *_leds[_index] += _color;
    return 0;
}

int Light::_prog_lfo(int x) {
    int wc = _color;
    wc%=(int)round((sin(_count*3.14/180)+0.5)*255);
    for(int i=0; i<_num_leds; i++) {
        *_leds[i] = wc;
    }
    return 0;
}

int Light::_prog_longfade(int x) {
    bool still_fading = false;
    if(_count%10 == 0) {
        for(int i=0; i<_num_leds; i++) {
            _leds[i]->fadeToBlackBy(1);
            if (*_leds[i]) still_fading = true;
        }
        if (!still_fading) _onoff = false;
    }
    return 0;
}

int Light::_prog_blink(int x) {
    _prog_fade(25);
    if (!x) x = 25;
    if (_count%x == 0) {
        for(int i=0; i<_num_leds; i++) {
            *_leds[i] = _color;
        }
    }
    return 0;
}

// Helper Functions

CRGB fadeTowardColor(CRGB cur, CRGB target, uint8_t x) {
    CRGB newc;
    newc.red = nblendU8TowardU8(cur.red, target.red, x);
    newc.green = nblendU8TowardU8(cur.green, target.green, x);
    newc.blue = nblendU8TowardU8(cur.blue, target.blue, x);
    return newc;
}

uint8_t nblendU8TowardU8(uint8_t cur, const uint8_t target, uint8_t x) {
    uint8_t newc;
    if (cur == target) return cur;
    if (cur < target) {
        uint8_t delta = target - cur;
        delta = scale8_video(delta, x);
        return cur + delta;
    } else {
        uint8_t delta = cur - target;
        delta = scale8_video(delta, x);
        return cur - delta;
    }
}

colorMap["red"]     = CRGB::Red;
colorMap["orange"]  = CRGB::Orange;
colorMap["blue"]    = CRGB::Blue;
colorMap["green"]   = CRGB::Green;
colorMap["black"]   = CRGB::Black;
colorMap["white"]   = CRGB::White;
