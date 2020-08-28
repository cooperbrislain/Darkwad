#include "darkwad.h"

Config          config;
CRGB*           leds;
Light**         lights;
Control**       controls;
std::map<String, Action*> actions;
std::map<String, Light::State*> states;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int count = 0;

void setup() {
    Serial.begin(115200);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial << "SSD1306 allocation failed\n";
        for(;;);
    }

    display.display();
    display.clearDisplay();
    display.print("Test");
    display.display();
    delay(2000);

    Serial << "Loading configuration...\n";
    SPIFFS.begin(true);
    StaticJsonDocument<4096> jsonDoc;
    File configFile = SPIFFS.open("/config.json", FILE_READ);
    deserializeJson(jsonDoc, configFile);
    configFile.close();

    JsonObject obj      = jsonDoc.as<JsonObject>();

    config.name         = jsonDoc["name"] | "Unnamed Project";
    config.speed        = jsonDoc["speed"] | 500;
    config.fade         = jsonDoc["fade"] | 25;
    config.brightness   = jsonDoc["brightness"] | 50;
    config.num_leds     = jsonDoc["num_leds"] | 100;
    config.num_params   = jsonDoc["num_params"] | 3;
    config.bump_led     = jsonDoc["bump_led"] | 0;

    leds = new CRGB[config.num_leds];

    if (obj.containsKey("lights")) {
        JsonArray jsonLights = obj["lights"];
        int numLights = config.num_lights = jsonLights.size();
        lights = new Light*[numLights];
        for (int i=0; i<numLights; i++) {
            lights[i] = new Light(&leds[0], jsonLights[i]);
        }
    }

    if (obj.containsKey("actions")) {
        JsonArray jsonActions = obj["actions"];
        int numActions = jsonActions.size();
        for (int i=0; i<numActions; i++) {
            Action* newAction = actionFromJson(jsonActions[i].as<JsonObject>());
            actions[newAction.getName()] = newAction;
        }
    }

    if (obj.containsKey("states")) {
        JsonArray jsonStates = obj["states"];
        int numStates = config.num_states = jsonStates.size();
        states = new Light::State*[numStates];
        for (int i=0; i<numStates; i++) {
            JsonObject jsonState    = jsonStates[i];
            states[i] = stateFromJson(jsonState);
        }
    }

    if (obj.containsKey("controls")) {
        JsonArray jsonControls = obj["controls"];
        int numControls = config.num_controls = jsonControls.size();
        controls = new Control*[config.num_controls];
        for (int i=0; i<numControls; i++) {
            JsonObject control = jsonControls[i];
            String controlName = control["name"];
            String controlType = control["type"];
            Serial << "[" << controlType << "] " << controlName << '\n';
            if (controlType == "Button") {
                int pin                 = control["pin"];
                Button* newButton       = new Button(controlName, pin);
                JsonObject jsonPress    = control["press"];
                JsonObject jsonRelease  = control["release"];
                if (jsonPress) {
                    Serial << "Binding onPress Action...\n";
                    Light* light;
                    Light::State state;
                    String actionName       = jsonPress["action"];
                    String lightName        = jsonPress["light"];
                    for (int i=0; i<config.num_lights; i++) {
                        if ((String)lights[i]->getName() == lightName) light = lights[i];
                    }
                    JsonObject jsonState    = jsonPress["state"];
                    state.color             = jsonState["color"].as<String>();
                    state.onoff             = jsonState["onoff"].as<int>();
                    state.program           = jsonState["program"].as<String>();
                    if (jsonArray jsonParams = jsonState["params"]) {
                        int numParams =         = jsonState["params"].size();
                        int *params = new int[numParams];
                        for (int j=0; j<numParams; j++) {
                            params[j] = jsonParams[j];
                        }
                        int params
                        state.params = params;
                    }
                    Action* newAction = new Action(actionName, light, state);
                    newButton->setPress(newAction);
                }
                if (jsonRelease) {
                    Serial << "Binding onRelease Action...\n";
                    Light* light;
                    Light::State state;
                    String actionName       = jsonRelease["action"];
                    String lightName        = jsonRelease["light"];
                    for (int i=0; i<config.num_lights; i++) {
                        if ((String)lights[i]->getName() == lightName) light = lights[i];
                    }
                    JsonObject jsonState    = jsonRelease["state"];
                    state.color             = jsonState["color"].as<String>();
                    state.onoff             = jsonState["onoff"].as<int>();
                    JsonArray JsonParams    = jsonState["params"];
                    for (int i=0; i<JsonParams.size(); i++) {
                        state.params[i] = JsonParams[i];
                    }
                    Action* newAction = new Action(actionName, light, state);
                    newButton->setRelease(newAction);
                }
                controls[i] = newButton;
                Serial << "New Button Added.\n";
            }
            if (controlType == "DPad") {
                JsonArray jsonPins = control["pins"];
                const int numPins = jsonPins.size();
                int pins[numPins];
                for (int j=0; j<numPins; j++) pins[j] = jsonPins[j];
                DPad* newDPad = new DPad(controlName, pins);
                controls[i] = newDPad;
            }
        }
    }

    Serial << "Config Loaded: \n";
    Serial << "Lights (" << config.num_lights << ") \n";
    for (int i=0; i<config.num_lights; i++)
        Serial << "   " << lights[i]->getName() << "\n";
    Serial << "Controls (" << config.num_controls << ") \n";
    for (int i=0; i<config.num_controls; i++)
        Serial << "   " << controls[i]->getName() << " (" << controls[i]->getType() << ")\n";
    Serial << "\n";
    delay(100);

    FastLED.setBrightness(config.brightness);
    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(24)>(leds, NUM_LEDS);

    Serial << "It's lit fam!\n";
    delay(150);
}

// MAIN LOOP

void loop() {

    for (int i=0; i<config.num_controls; i++) {
        if (count % controls[i]->getSampleRate() == 0) {
            String controlName = controls[i]->getName();
            int controlState = controls[i]->getState();
            controls[i]->update();
        }
    }

    for (int i=0; i<config.num_lights; i++) {
        if (count % lights[i]->getParam(0) == 0) {
            lights[i]->update();
        }
    }

    FastLED.show();

    count++;
    delay(1000/config.speed);
}

Light::State* stateFromJson(JsonObject jsonState) {
    Light::State* state = new Light::State;
    if (jsonState["color"]) state.color = jsonState["color"].as<String>();
    if (jsonState["onoff"]) state.onoff = jsonState["onoff"].as<int>();
    if (jsonState["program"]) state.program = jsonState["program"].as<String>();
    if (jsonArray jsonParams = jsonState["params"]) {
        int numParams = jsonState["params"].size();
        int *params = new int[numParams];
        for (int j=0; j<numParams; j++) {
            params[j] = jsonParams[j];
        }
        state.params = params;
    }
    return state;
}

Action* actionFromJson(JsonObject jsonAction) {
    String actionName = jsonAction["name"];
    String lightName = jsonAction["light"];
    String stateName = jsonAction["state"];
    Light* light;
    Light::State* state = states[stateName];
    for (int i=0; i<config.num_lights; i++) {
        if ((String)lights[i]->getName() == lightName) light = lights[i];
    }
    return new Action(light, state);
}