#include "Control.h"

class Button: public Control {
    private:

        int _pin;
        ControlFn _pressFn;
        ControlFn _releaseFn;
        ControlFn _stilldownFn;

    public:

        Button() :
            Control { "Button", CTL_DIGITAL, DEFAULT_CTL_SAMPLE_RATE },
            _pin    { DEFAULT_CTL_PIN }
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
            _pin            { pin }
        { };

        int  getState();
        void update();

        void setPress(ControlFn pressFn);
        void setRelease(ControlFn releaseFn);
        void setStilldown(ControlFn stilldownFn);

};
