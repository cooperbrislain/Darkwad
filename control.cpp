#include "Arduino.h"
#include "control.h"
#include "config.h"

#define DEBUG

template<class T>
    inline Print &operator <<(Print &stream, T arg)
    { stream.print(arg); return stream; }

void Control::update() {
    int val = this->read();
    #ifdef DEBUG
        Serial.println();
    #endif
    if (val >= _threshold) {
        _pressed++;
        if (_pressed == 10) _pressFn(val);
        if (_pressed > 20) _stilldownFn(val);
    } else {
        if (_pressed > 5) _releaseFn(val);
        _pressed = 0;
    }
}

int Control::read() {
    switch (_type) {
        case CTL_TOUCH:
            return touchRead(_pin);
        default:
        case CTL_DIGITAL:
            return digitalRead(_pin);
        case CTL_ANALOG:
            return analogRead(_pin);
    }
}

bool Control::is_pressed() {
    return _pressed!=0;
}

void Control::set_press(ControlFn pressFn) {
    _pressFn = pressFn;
}

void Control::set_release(ControlFn releaseFn) {
    _releaseFn = releaseFn;
}

void Control::set_stilldown(ControlFn stilldownFn) {
    _stilldownFn = stilldownFn;
}

std::ostream& operator<< (std::ostream& os, const Control &ctl)
{
    switch(ctl._type)
    {
        case Control::CTL_ANALOG:
            os << 'A' << ctl._pin << ": " << ctl._val;
            break;
        default:
        case Control::CTL_DIGITAL:
            os << 'D' << ctl._pin << ": " << ctl._val;
            break;
        case Control::CTL_TOUCH:
            os << 'T' << ctl._pin << ": " << ctl._val;
            break;
    }
    return os;
}
