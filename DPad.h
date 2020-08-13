#ifndef DARKWAD_DPAD_H
#define DARKWAD_DPAD_H

#include "Control.h"

class DPad: public Control {
private:

    enum States { CENTER, UP, DOWN, LEFT, RIGHT };
    static const int num_pins = 4;
    int _pins[num_pins];
    int _state;
    int _pressed;
    int _count;

public:

    DPad() :
        Control { "DPad", CTL_DIGITAL, DEFAULT_SAMPLE_RATE }
    { };
    DPad(String name, int* pins) :
        Control { name, CTL_DIGITAL, DEFAULT_SAMPLE_RATE }
    { for (int i=0; i<num_pins; i++) _pins[i] = pins[i]; };

    int  getState();
    void update();

};

#endif //DARKWAD_DPAD_H