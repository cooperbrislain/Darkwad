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
    int val = digitalRead(_pin);
    return (val == HIGH? 1 : 0);
}

void Button::update() {
    return;
}

