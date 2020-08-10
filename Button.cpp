#include "Button.h"

void Button::setPress(ControlFn pressFn) {
    _pressFn = pressFn;
}

void Button::setStilldown(ControlFn stilldownFn) {
    _stilldownFn = stilldownFn;
}

void Button::setRelease(ControlFn releaseFn) {
    _releaseFn = releaseFn;
}

int Button::getState() {
    int val = digitalRead(_pins[0]);
    return (val == HIGH? 1 : 0);
}

void Button::update() {
    int state = this->getState();
    if (_state != state) {
        if (++_count>=SAMPLES) {
            if (_state == 0 && state == 1) {
                this->_pressFn(1);
            }
            if (_state == 1 && state == 0) {
                this->_releaseFn(1);
            }
            _state = state;
            _count = 0;
        }
    } else {
        _count = 0;
        if (_state == 1) {
            this->_stilldownFn(1);
        }
    }
    return;
}

