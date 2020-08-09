#include "BrakeControl.h"

void BrakeControl::update() {
    int val = this->getState();
//    Serial << val << '\n';
}

int BrakeControl::getState() {
    int val = (digitalRead(_pin1) == HIGH? 1:0) +  (digitalRead(_pin2) == HIGH? 1:0);
    return val;
}
