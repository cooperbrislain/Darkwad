#include "Control.h"

// getters

String Control::getName() { return _name; }
ControlType Control::getType() { return _type; }
int Control::getSampleRate() { return _sampleRate; }

// setters

void Control::setName(String name) { _name = name; }
void Control::setSampleRate(int sampleRate) { _sampleRate = sampleRate; }

// functions

std::ostream& operator<< (std::ostream& os, const ControlType ctlType) {
    switch(ctlType) {
        case CTL_ANALOG:
            return os << 'A';
        default:
        case CTL_DIGITAL:
            return os << 'D';
        case CTL_TOUCH:
            return os << 'T';
    }
}