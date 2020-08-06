#include <Arduino.h>
#include <iostream>
#include "config.h"

#define DEFAULT_CONTROL_PIN T0

typedef void(*ControlFn)(int);

template<class T>
    inline Print &operator <<(Print &stream, T arg)
    { stream.print(arg); return stream; }

class Control {
    public:
        enum ControlType { CTL_DIGITAL, CTL_ANALOG, CTL_TOUCH };
        Control() :
            _pressFn { [](int val){ } },
            _releaseFn { [](int val){ } },
            _stilldownFn { [](int val){ } },
            _pin { DEFAULT_CONTROL_PIN },
            _type { CTL_DIGITAL },
            _name { (String)"button" },
            _threshold { CTL_TOUCH_THRESHOLD },
            _sampleRate { CTL_SAMPLE_RATE }
        { };
        Control(
            String name,
            int pin,
            ControlType type,
            ControlFn pressFn
        ) :
            _pressFn { pressFn },
            _releaseFn { [](int val){ } },
            _stilldownFn { [](int val){ } },
            _pin { pin },
            _name { name },
            _type { type },
            _threshold { CTL_TOUCH_THRESHOLD },
            _sampleRate { CTL_SAMPLE_RATE }
        { };
        Control(
            String name,
            int pin,
            ControlType type,
            ControlFn pressFn,
            ControlFn stilldownFn,
            ControlFn releaseFn
        ) :
            _name { name },
            _pin { pin },
            _type { type },
            _pressFn { pressFn },
            _stilldownFn { stilldownFn },
            _releaseFn { releaseFn },
            _threshold { CTL_TOUCH_THRESHOLD },
            _sampleRate { CTL_SAMPLE_RATE }
        { };
        Control(String name, int pin, ControlType type) :
            _name { name },
            _pin { pin },
            _type { type },
            _pressed { 0 },
            _val { 0 },
            _pressFn { [](int val) { } },
            _releaseFn { [](int val) { } },
            _stilldownFn { [](int val) { } },
            _threshold { CTL_TOUCH_THRESHOLD },
            _sampleRate { CTL_SAMPLE_RATE }
        { };
        int  read();
        int  get_state();
        void set_press(ControlFn pressFn);
        void set_release(ControlFn releaseFn);
        void set_stilldown(ControlFn stilldownFn);
        bool is_pressed();
        void update();
        int _sampleRate;
    private:
        ControlType _type;
        String _name;
        int _pressed = 0;
        int _val;
        int _pin;
        int _threshold;
        ControlFn _pressFn;
        ControlFn _releaseFn;
        ControlFn _stilldownFn;
        friend std::ostream& operator<< (std::ostream& os, ControlType &ctlType);
};