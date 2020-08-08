#include <Arduino.h>
#include <iostream>
#include "config.h"

#define DEFAULT_CTL_PIN T0

typedef void(*ControlFn)(int);

template<class T>
    inline Print &operator <<(Print &stream, T arg)
    { stream.print(arg); return stream; }

class Control {
    public:
        enum ControlType { CTL_DIGITAL, CTL_ANALOG, CTL_TOUCH };
//        Control() :
//            _pressFn { [](int val){ } },
//            _releaseFn { [](int val){ } },
//            _stilldownFn { [](int val){ } },
//            _pin { DEFAULT_CONTROL_PIN },
//            _type { CTL_DIGITAL },
//            _name { (String)"button" },
//            _threshold { CTL_TOUCH_THRESHOLD },
//            _sampleRate { CTL_SAMPLE_RATE }
//        { };
//        Control(
//            String name,
//            int pin,
//            ControlType type,
//            ControlFn pressFn
//        ) :
//            _pressFn { pressFn },
//            _releaseFn { [](int val){ } },
//            _stilldownFn { [](int val){ } },
//            _pin { pin },
//            _name { name },
//            _type { type },
//            _threshold { CTL_TOUCH_THRESHOLD },
//            _sampleRate { CTL_SAMPLE_RATE }
//        { };
//        Control(
//            String name,
//            int pin,
//            ControlType type,
//            ControlFn pressFn,
//            ControlFn stilldownFn,
//            ControlFn releaseFn
//        ) :
//            _name { name },
//            _pin { pin },
//            _type { type },
//            _pressFn { pressFn },
//            _stilldownFn { stilldownFn },
//            _releaseFn { releaseFn },
//            _threshold { CTL_TOUCH_THRESHOLD },
//            _sampleRate { CTL_SAMPLE_RATE }
//        { };
        int  read();
        virtual Control getState();
        virtual void update();
        bool isPressed();
        int _sampleRate;
    private:
        String _name;
        ControlType type;
        int _state;
        friend std::ostream& operator<< (std::ostream& os, ControlType &ctlType);
};