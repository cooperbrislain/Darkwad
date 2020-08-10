#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <iostream>
#include "config.h"

typedef void(*ControlFn)(int);
enum ControlType { CTL_DIGITAL, CTL_ANALOG, CTL_TOUCH };

#define NULF [](int arg) { }

template<class T> inline Print &operator <<(Print &stream, T arg) { stream.print(arg); return stream; }

class Control {
protected:

    String      _name;
    ControlType _type;
    int         _sampleRate;
    friend std::ostream& operator<< (std::ostream& os, ControlType &ctlType);

public:

    Control() :
        _name { "Control" },
        _type { CTL_DIGITAL },
        _sampleRate { CTL_SAMPLE_RATE }
    { };
    Control(String name, ControlType type, int sampleRate) :
        _name { name },
        _type { type },
        _sampleRate { sampleRate }
    { Serial << "New Control Created\n"; };
    String          getName();
    ControlType     getType();
    virtual int     getState() = 0;
    int             getSampleRate();
    void            setName(String name);
    void            setSampleRate(int sampleRate);
    virtual void    update() = 0;

};

#endif // CONTROL_H