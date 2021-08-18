#include "BrakeControl.h"

void BrakeControl::update() {
    return;
}

int BrakeControl::getState() {
    int count = 0;
    for (int pin : _pins)
        count += digitalRead(pin) == HIGH? 1 : 0;
    return count;
}
