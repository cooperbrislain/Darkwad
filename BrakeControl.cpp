#include "BrakeControl.h"

void BrakeControl::update() {
    return;
}

int BrakeControl::getState() {
    return (digitalRead(_pin1) == HIGH? 1:0) + (digitalRead(_pin2) == HIGH? 1:0);
}
