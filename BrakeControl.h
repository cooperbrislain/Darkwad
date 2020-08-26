#include "Control.h"

class BrakeControl: public Control {
    private:

        enum States { OFF, SLOW, STOP };
        static const int num_pins = 2;
        int _pins[num_pins];

    public:

        BrakeControl() :
            Control { "Brake", CTL_DIGITAL, 25 }
        { };
        BrakeControl(String name, int* pins) :
            Control { name, CTL_DIGITAL, 25 }
        { for (int i=0; i<num_pins; i++) _pins[i] = pins[i]; };
        BrakeControl(String name, int* pins, Action onPress, Action onRelease) :
            Control { name, CTL_DIGITAL, 25 }
        { for (int i=0; i<num_pins; i++) _pins[i] = pins[i]; };

        int  getState();
        void update();

};
