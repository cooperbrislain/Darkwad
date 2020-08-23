#include "Control.h"

#define SAMPLES 3

class Button: public Control {
    private:

        enum States { UP, DOWN };
        int _state;
        static const int num_pins = 1;
        int _pins[num_pins];
        int _pressed;
        int _count;
        ControlFn _pressFn;
        ControlFn _releaseFn;
        ControlFn _stilldownFn;

    public:

        Button() :
            Control { "Button", CTL_DIGITAL, CTL_SAMPLE_RATE },
            _pressFn        { NULF },
            _stilldownFn    { NULF },
            _releaseFn      { NULF }
        { };
        Button(String name, int* pins, ControlFn pressFn, ControlFn stilldownFn, ControlFn releaseFn) :
            Control         { name, CTL_DIGITAL, CTL_SAMPLE_RATE },
            _pins           { pins[0] },
            _pressFn        { pressFn },
            _stilldownFn    { stilldownFn },
            _releaseFn      { releaseFn }
        { };
        Button(String name, int* pins) :
            Control         { name, CTL_DIGITAL, CTL_SAMPLE_RATE },
            _pins           { pins[0] },
            _pressFn        { NULF },
            _stilldownFn    { NULF },
            _releaseFn      { NULF }
        { };

        int  getState();
        void update();

        void setPress(ControlFn pressFn);
        void setRelease(ControlFn releaseFn);
        void setStilldown(ControlFn stilldownFn);

};
