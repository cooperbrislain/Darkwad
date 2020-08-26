#ifndef DARKWAD_ACTION_H
#define DARKWAD_ACTION_H

#include <String.h>
#include <ArduinoJson.h>
#include "Light.h"

class Action {
    private:
        String          _actionName;
        Light*          _light;
        Light::State    _state;
    public:
        Action() { };
        Action (String actionName, Light* light, Light::State state) :
            _actionName (actionName),
            _light (light),
            _state (state)
        { }
        void operator () (int x) { _light->setState(_state); }
};

#endif //DARKWAD_ACTION_H
