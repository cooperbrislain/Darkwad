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

void Light::setRgb(CRGB _color) { state.color = _color; }
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

void Light::setSaturation(int _s) {
    CHSV hsvColor = getHsv();
    hsvColor.s = min(_s, 100);
    setHsv(hsvColor);
}

void Light::setBrightness(int _v) {
    CHSV hsvColor = getHsv();
    hsvColor.v = min(_v, 100);
    setHsv(hsvColor);
}

void Light::setProgram(String _progName) {
    this->state.prog = (progMap[_progName]);
}

void Light::setProgram(ProgFn _prog) {
    Serial << this->getName() << " set program to custom... ";
    this->state.prog = _prog;
}

void Light::setParams(int* _params) {
    int numParams = sizeof(_params);
    for (int i=0; i<numParams; i++) {
        state.params[i] = _params[i];
    }
}

void Light::setParam(int _p, int _v) {
    state.params[_p] = _v;
}


void Light::setState(State* _state) {
    try {
        Serial << "Setting light state: " << "[" << _state->name << ": ";
        if (_state->onoff != -1) {
            Serial << (_state->onoff==1? "On" : "Off") << "|";
            turn(_state->onoff);
        }
        if (_state->prog) {
            Serial << "Prog:" << _state->prog << "|";
            setProgram(_state->prog);
        }
        if (_state->params != nullptr) {
            Serial << "Params...|";
            setParams(_state->params);
        }
        if (_state->color) {
            Serial << "Color:" << _state->color;
            setRgb(_state->color);
        }
        if (_state->count != -1) {
            state.count = _state->count;
        }
        Serial << "]\n";
    } catch (int e) {
        Serial << "An Exception occurred: " << e << "\n";
    }
}

void Light::blink() {
    for (int i=0; i<numLeds; i++) {
        *(CRGB*)leds[i] = CRGB::White;
    }
    update();
    delay(25);
    for (int i=0; i<numLeds; i++) {
        *(CRGB*)leds[i] = CRGB::Black;
    }
    update();
}

// programs

void Light::progSolid() {
    for (int i=0; i<numLeds; i++) {
        *leds[i] = state.color;
    }
}

void Light::progFade() {
    for(int i=0; i<numLeds; i++) {
        leds[i]->fadeToBlackBy(state.params[1]);
    }
}

void Light::progFadein() {
    bool stillFading = false;
    for(int i=0; i<numLeds; i++) {
        *leds[i] = fadeTowardColor(*leds[i], state.color, state.params[1]);
        if (*leds[i] != state.color) stillFading = true;
    }
    if (!stillFading) state.prog = &Light::progSolid;
}

void Light::progFadeout() {
    bool stillFading = false;
    for(int i=0; i<numLeds; i++) {
        leds[i]->fadeToBlackBy(state.params[1]);
        if (*leds[i]) stillFading = true;
    }
    if (!stillFading) state.onoff = false;
}

void Light::progChase() {
    progFade(state.params[1]);
    *leds[state.count%numLeds] = state.color;
}

void Light::progLongfade() {
    bool stillFading = false;
    if(state.count%10 == 0) {
        for(int i=0; i<numLeds; i++) {
            leds[i]->fadeToBlackBy(1);
            if (*leds[i]) stillFading = true;
        }
        if (!stillFading) state.onoff = false;
    }
}

void Light::progBlink() {
    progFade();
    int x = state.params[1];
    if (state.count%x == 0) {
        for(int i=0; i<numLeds; i++) {
            *leds[i] = state.color;
        }
    }
}

void Light::progTwinkle() {
    progFade();
    int x = state.params[1];
    int index;
    if (rand()%100 > state.params[1]) {
        index = rand()%numLeds;
        state.params[1] = index;
    } else {
        index = state.params[1];
    }
    leds[i] += 64;
}

// Helper Functions

CRGB fadeTowardColor(CRGB _c, CRGB _t, uint8_t _x) {
    return CRGB(
        nblendU8TowardU8(_c.r, _t.r, _x),
        nblendU8TowardU8(_c.g, _t.g, _x),
        nblendU8TowardU8(_c.b, _t.b, _x)
    );
}

uint8_t nblendU8TowardU8(uint8_t cur, const uint8_t target, uint8_t x) {
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

progMap["solid"]    = &Light::progSolid;