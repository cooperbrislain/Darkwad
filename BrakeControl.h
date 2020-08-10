#include "Control.h"


class BrakeControl: public Control {
    private:

        enum States { OFF, SLOW, STOP };
        int* _pins[2];

    public:

        BrakeControl() :
            Control     { "Brake", CTL_DIGITAL, 25 }
        { };
        BrakeControl(String name, int* pins) :
            Control { name, CTL_DIGITAL, 25 },
            _pins   { pins }
        { };
        BrakeControl(String name, int* pins,
            ControlFn pressFn,
            ControlFn stilldownFn,
            ControlFn releaseFn) :
            Control         { name, CTL_DIGITAL, 25},
            _pins           { pins }
        { };

        int  getState();
        void update();

};
