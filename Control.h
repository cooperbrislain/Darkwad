#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <iostream>
#include <ArduinoJson.h>

#define DEFAULT_PINS 4, 0, 2, 15
#define DEFAULT_SAMPLE_RATE 25

#include "config.h"

#define NULF *[](int arg) { }

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

    Control() :
        _name       { "Control" },
        _type       { CTL_DIGITAL },
        _sampleRate { CTL_SAMPLE_RATE }
    { };
    Control(String name, ControlType type, int sampleRate) :
        _name       { name },
        _type       { type },
        _sampleRate { sampleRate }
    { };
    Control(JsonObject obj) :
        _name       { obj["name"].as<String>() },
        _type       { obj["type"].as<ControlType>() },
        _sampleRate { obj["sampleRate"].as<int>() }
    {
        Serial << obj["name"].as<String>();
    };
    String          getName();
    ControlType     getType();
    virtual int     getState() = 0;
    int             getSampleRate();
    void            setName(String name);
    void            setSampleRate(int sampleRate);
    virtual void    update() = 0;

};

#endif // CONTROL_H