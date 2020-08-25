#include "Control.h"

#define SAMPLES 3

class Button: public Control {
    private:

        enum States { UP, DOWN };

        int                 _state;
        uint8_t             _pin;
        int                 _pressed;
        int                 _count;
        ControlFn           _pressFn;
        ControlFn           _releaseFn;
        ControlFn           _stilldownFn;

    public:

        Button() :
            Control { "Button", CTL_DIGITAL, CTL_SAMPLE_RATE },
            _pressFn        { NULF },
            _stilldownFn    { NULF },
            _releaseFn      { NULF }
        { };
        Button(String name, int pin, ControlFn pressFn, ControlFn stilldownFn, ControlFn releaseFn) :
            Control         { name, CTL_DIGITAL, CTL_SAMPLE_RATE },
            _pin            { pin },
            _pressFn        { pressFn },
            _stilldownFn    { stilldownFn },
            _releaseFn      { releaseFn }
        {
            pinMode(_pin, INPUT);
        };
        Button(String name, int pin) :
            Control         { name, CTL_DIGITAL, CTL_SAMPLE_RATE },
            _pin            { pin },
            _pressFn        { NULF },
            _stilldownFn    { NULF },
            _releaseFn      { NULF }
        {
            pinMode(_pin, INPUT);
        };
        int  getState();
        void update();

        void setPress(ControlFn pressFn);
        void setRelease(ControlFn releaseFn);
        void setStilldown(ControlFn stilldownFn);

};
