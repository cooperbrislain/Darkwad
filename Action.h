#ifndef DARKWAD_ACTION_H
#define DARKWAD_ACTION_H

#include <iostream>
#include <String.h>
#include <ArduinoJson.h>
#include "Light.h"
#include <StreamPrint.h>

class Action {
    private:
        String          _name;
        Light*          _light;
        Light::State*   _state;
    public:
        Action() { };
        Action (String name, Light* light, Light::State* state) :
            _name (name),
            _light (light),
            _state (state)
        { Serial << "Created Action: " << _name << '\n'; }
        void operator () (int x) {
            Serial << _name << '\n';
            _light->setState(_state);
        }
        String getName();
};

#endif //DARKWAD_ACTION_H
