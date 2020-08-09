#include "Control.h"

class Button: public Control {
    private:

        int _pin;
        ControlFn _pressFn;
        ControlFn _releaseFn;
        ControlFn _stilldownFn;

    public:

        Button() :
            _name           { "Button" },
            _type           { CTL_DIGITAL },
            _pin            { DEFAULT_CTL_PIN },
            _sampleRate     { DEFAULT_CTL_SAMPLE_RATE }
            _pressFn        { [](int val){ } },
            _stilldownFn    { [](int val){ } },
            _releaseFn      { [](int val){ } }
        { };
        Button(String name, int pin, ControlFn pressFn, ControlFn stilldownFn, ControlFn releaseFn) :
            _name           { name },
            _type           { CTL_DIGITAL },
            _pin            { pin },
            _pressFn        { pressFn },
            _stilldownFn    { stilldownFn },
            _releaseFn      { releaseFn },
            _sampleRate     { CTL_SAMPLE_RATE }
        { };

        int  getState();
        void update();

        void setPress(ControlFn pressFn) : _pressFn { pressFn };
        void setRelease(ControlFn releaseFn) : _releaseFn { releaseFn };
        void setStilldown(ControlFn stilldownFn) : _stilldownFn { stilldownFn };
};
