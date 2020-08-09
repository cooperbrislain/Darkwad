#include "Control.h"

#define DEFAULT_CTL_PIN1 12
#define DEFAULT_CTL_PIN2 14

class BrakeControl: public Control {
    private:

        int _pin1;
        int _pin2;
        ControlFn _pressFn;
        ControlFn _stilldownFn;
        ControlFn _releaseFn;

    public:

        BrakeControl() :
            Control     { "Brake", CTL_DIGITAL, DEFAULT_CTL_SAMPLE_RATE },
            _pin1       { DEFAULT_CTL_PIN1 },
            _pin2       { DEFAULT_CTL_PIN2 }
        { };
        BrakeControl(String name, int pin1, int pin2) :
            Control     { name, CTL_DIGITAL, DEFAULT_CTL_SAMPLE_RATE },
            _pin1       { pin1 },
            _pin2       { pin2 }
        { };
        BrakeControl(String name, int pin1, int pin2,
            ControlFn pressFn,
            ControlFn stilldownFn,
            ControlFn releaseFn) :
            Control         { name, CTL_DIGITAL, DEFAULT_CTL_SAMPLE_RATE },
            _pin1           { pin1 },
            _pin2           { pin2 },
            _pressFn        { pressFn },
            _stilldownFn    { stilldownFn },
            _releaseFn      { releaseFn }
        { };

        int  getState();
        void update();

};
