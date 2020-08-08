#include "Control.h"

class BrakeControl: public Control {
    public:
        BrakeControl(pin1, pin2) :
            pin1 { _pin1 },
            pin2 { _pin2 }
        { };
    private:
        int _pin1;
        int _pin2;
};
