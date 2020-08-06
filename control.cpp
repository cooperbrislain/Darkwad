#include "control.h"

void Control::update() {
    int val = this->read();
    #ifdef DEBUG
        Serial << _name << '[' << _type << '(' << _pin << ")]:" << val << '[' << _pressed << ']' << '\n';
    #endif
    if ((_type == CTL_TOUCH && val <= _threshold) || (_type == CTL_DIGITAL && val == 1)) {
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
            return touchRead(_pin) <= _threshold? 1 : 0;
        default:
        case CTL_DIGITAL:
            return digitalRead(_pin) == HIGH? 1 : 0;
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

std::ostream& operator<< (std::ostream& os, const Control::ControlType ctlType)
{
    switch(ctlType)
    {
        case Control::CTL_ANALOG:
            return os << 'A';
        default:
        case Control::CTL_DIGITAL:
            return os << 'D';
        case Control::CTL_TOUCH:
            return os << 'T';
    }
}
