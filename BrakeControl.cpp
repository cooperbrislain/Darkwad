#include "BrakeControl.h"

void BrakeControl::update() {
    int val = this->read();
#ifdef DEBUG
    Serial << _name << '[' << _type << '(' << _pin << ")]:" << val << '[' << _pressed << ']' << '\n';
#endif
    if (val > 0) {
        _pressed++;
        if (_pressed == 10) _pressFn(val);
        if (_pressed > 20) _stilldownFn(val);
    } else {
        if (_pressed > 5) _releaseFn(val);
        _pressed = 0;
    }
}
