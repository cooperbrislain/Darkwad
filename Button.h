#include <Control.h>

class Button: public Control {
    public:
        Button() :
            _name { 'Button' },
            _type { CTL_DIGITAL },
            _pin { DEFAULT_CTL_PIN },
            _pressFn { [](int val){ } },
            _stilldownFn { [](int val){ } },
            _releaseFn { [](int val){ } },
            _sampleRate { CTL_SAMPLE_RATE }
        { };
        Button(String name, int pin, ControlFn pressFn, ControlFn releaseFn, ControlFn stilldownFn) :
            _name { name },
            _type { CTL_DIGITAL },
            _pin { pin },
            _pressFn { pressFn },
            _releaseFn { releaseFn },
            _stilldownFn { stilldownFn }
            _sampleRate { CTL_SAMPLE_RATE }
        { };
        void setPress(ControlFn pressFn);
        void setRelease(ControlFn releaseFn);
        void setStilldown(ControlFn stilldownFn);
    private:
        ControlFn _pressFn;
        ControlFn _releaseFn;
        ControlFn _stilldownFn;
};
