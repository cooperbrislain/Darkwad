#include "Button.h"

void Button::setPress(Action* onPress) { _onPress = onPress; }
void Button::setRelease(Action* onRelease) { _onRelease = onRelease; }
int Button::getState() { return (digitalRead(_pin) == HIGH? 1 : 0); }

void Button::update() {
    int state = this->getState();
    if (_state != state) {
        Serial << this->getName() << ": " << state << "\n";
        if (++_count>=SAMPLES) {
            if (_state == 0 && state == 1) {
                Serial << this->getName() << "->onPress" << "\n";
                (*_onPress)(1);
            }
            if (_state == 1 && state == 0) {
                Serial << this->getName() << "->onRelease" << "\n";
                (*_onRelease)(1);
            }
            _state = state;
            _count = 0;
        }
    } else {
        _count = 0;
        if (_state == 1) {
        }
    }
    return;
}

