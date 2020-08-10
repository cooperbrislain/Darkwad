#ifndef DARKWAD_DPAD_H
#define DARKWAD_DPAD_H

#include "Control.h"

class DPad: public Control {
private:

    enum States { CENTER, UP, DOWN, LEFT, RIGHT };
    int _state;
    int _pins[4];
    int _pressed;
    int _count;

public:

    DPad() :
        Control { "DPad", CTL_DIGITAL, DEFAULT_SAMPLE_RATE },
        _pins   { DEFAULT_PINS }
    { };
    DPad(String name, int* pins) :
        Control { name, CTL_DIGITAL, DEFAULT_SAMPLE_RATE },
        _pins   { pins }
    { };

    int  getState();
    void update();

};

#endif //DARKWAD_DPAD_H