#include "Control.h"

#define SAMPLES 3

class Button: public Control {
    private:

        int _state;
        int _pin;
        int _pressed;
        int _count;
        ControlFn _pressFn;
        ControlFn _releaseFn;
        ControlFn _stilldownFn;

    public:

        Button() :
            Control { "Button", CTL_DIGITAL, DEFAULT_CTL_SAMPLE_RATE },
            _pin    { DEFAULT_CTL_PIN },
            _pressFn        { NULF },
            _stilldownFn    { NULF },
            _releaseFn      { NULF }
        { };
        Button(String name, int pin, ControlFn pressFn, ControlFn stilldownFn, ControlFn releaseFn) :
            Control         { name, CTL_DIGITAL, DEFAULT_CTL_SAMPLE_RATE },
            _pin            { pin },
            _pressFn        { pressFn },
            _stilldownFn    { stilldownFn },
            _releaseFn      { releaseFn }
        { };
        Button(String name, int pin) :
            Control         { name, CTL_DIGITAL, DEFAULT_CTL_SAMPLE_RATE },
            _pin            { pin },
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
