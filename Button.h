#include "Control.h"

#define SAMPLES 3

class Button: public Control {
    private:

        enum States { UP, DOWN };

        int                 _state;
        uint8_t             _pin;
        int                 _pressed;
        int                 _count;
        Action              _onPress;
        Action              _onRelease;

    public:

        Button() :
            Control { "Button", CTL_DIGITAL, DEFAULT_SAMPLE_RATE }
        { };
        Button(String name, int pin, Action onPress, Action onRelease) :
            Control         { name, CTL_DIGITAL, DEFAULT_SAMPLE_RATE },
            _pin            ( pin ),
            _onPress        ( onPress ),
            _onRelease      ( onRelease )
        {
            pinMode(_pin, INPUT);
        };
        Button(String name, int pin) :
            Control         { name, CTL_DIGITAL, DEFAULT_SAMPLE_RATE },
            _pin            ( pin )
        {
            pinMode(_pin, INPUT);
        };
        int  getState();
        void update();

        void setPress(Action onPress);
        void setRelease(Action onRelease);

};
