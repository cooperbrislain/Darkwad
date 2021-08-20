#include "Light.h"

void Light::update() {
    (this->*_prog)(_state.params[1]);
    _state.count++;
}

CHSV Light::getHsv() { return rgb2hsv_approximate(_state.color); }
CRGB Light::getRgb() { return _state.color; }
int  Light::getParam(int p) { return _state.params[p]; }
String Light::getName() { return _name; }

void Light::turnOn() { _state.on = 1; }
void Light::turnOff() { _state.on = 0; }
void Light::turn(int onoff) { _state.on = onoff; }
void Light::toggle() { _state.on = (_state.on==1? 0 : 1); }

void Light::setRgb(CRGB color) { _state.color = color; }
void Light::setHsv(int hue, int sat, int val) { _state.color = CHSV(hue, sat, val); }
void Light::setHsv(CHSV color) { _state.color = color; }

void Light::setColor(String color) {
    char hexCode[6];
    color.toCharArray(hexCode, sizeof(hexCode));
    int newColor = strtol(hexCode, 0, 16);
    _state.color = newColor;
    return;
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

void Light::setProgram(String progName) {
    if (progName == "solid") _prog = &Light::_prog_solid;
    if (progName == "chase") {
        _prog = &Light::_prog_chase;
        _state.params[1] = _state.params[1]? _state.params[1] : 35;
    }
    if (progName == "fade")  _prog = &Light::_prog_fade;
    if (progName == "blink") _prog = &Light::_prog_blink;
    if (progName == "warm") {
        _prog = &Light::_prog_warm;
        _state.params[0] = 50;
    }
    if (progName == "lfo") _prog = &Light::_prog_lfo;
}

void Light::setParams(int* params) {
    for (int i=0; i<sizeof(params); i++) {
        _state.params[i] = params[i];
    }
}

void Light::setParam(int p, int v) {
    _state.params[p] = v;
}

void Light::setState(State* state) {
    try {
        Serial << "Setting light state: " << "[" << state->name << ": ";
        if (state->onoff != -1) {
            Serial << (state->on ? "On" : "Off") << "|";
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
        *_leds[i] = _state.color;
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
        *_leds[i] = fadeTowardColor(*_leds[i], _state.color, x);
        if (*_leds[i] != _state.color) still_fading = true;
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
    _prog_fade(_state.params[1]);
    *_leds[_state.count%_num_leds] = _state.color;
    return 0;
}

int Light::_prog_warm(int x) {
    if (_state.count%7 == 0) _prog_fade(10);

    if (_state.count%11 == 0) {
        _index = random(_num_leds);
        CHSV wc = rgb2hsv_approximate(_state.color);
        wc.h = wc.h + random(11)-5;
        wc.s = random(128)+128;
        wc.v &=x;
        _state.color = wc;
    }
    *_leds[_index] += _state.color;
    return 0;
}

int Light::_prog_xmas(int x) {
    if (_state.count%7 == 0) _prog_fade(1);

    if (_state.count%11 == 0) {
        _index = random(_num_leds);
        CHSV wc = rgb2hsv_approximate(_state.color);
        wc.h = wc.h + random(11)-5;
        wc.s = random(128)+128;
        wc.v &=x;
        _state.color = wc;
    }
    *_leds[_index] += _state.color;
    return 0;
}

int Light::_prog_lfo(int x) {
    int wc = _state.color;
    wc%=(int)round((sin(_state.count*3.14/180)+0.5)*255);
    for(int i=0; i<_num_leds; i++) {
        *_leds[i] = wc;
    }
    return 0;
}

int Light::_prog_longfade(int x) {
    bool still_fading = false;
    if(_state.count%10 == 0) {
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
    if (_state.count%x == 0) {
        for(int i=0; i<_num_leds; i++) {
            *_leds[i] = _state.color;
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