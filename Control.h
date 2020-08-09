#include <Arduino.h>
#include <iostream>
#include "config.h"

#define DEFAULT_CTL_PIN T0
#define DEFAULT_CTL_SAMPLE_RATE 10

typedef void(*ControlFn)(int);
enum ControlType { CTL_DIGITAL, CTL_ANALOG, CTL_TOUCH };

template<class T> inline Print &operator <<(Print &stream, T arg) { stream.print(arg); return stream; }

class Control {
    protected:
        String      _name;
        ControlType _type;
        int         _sampleRate;
        friend std::ostream& operator<< (std::ostream& os, ControlType &ctlType);
    public:
        String          getName();
        ControlType     getType();
        virtual int     getState() = 0;
        int             getSampleRate();
        void            setName(String name);
        void            setSampleRate(int sampleRate);
        virtual void    update() = 0;
};