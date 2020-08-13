#include "DPad.h"

int DPad::getState() {
    for (int i=0; i<4; i++) {
        int pin = _pins[i];
        int val = digitalRead(pin);
        if (val == HIGH) {
            return pin+1;
        }
    }
    return 0;
}

void DPad::update() {
    int state = this->getState();
    switch (state) {
        case UP:
            Serial << "D-UP";
            break;
        case DOWN:
            Serial << "D-DOWN";
            break;
        case LEFT:
            Serial << "D-LEFT";
            break;
        case RIGHT:
            Serial << "D-RIGHT";
            break;
        case CENTER:
        default:
            break;
    }
    _state = state;
}

