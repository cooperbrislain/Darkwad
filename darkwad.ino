#include "darkwad.h"
#define LED_FRONT lights[1]
#define LED_REAR lights[4]
#define LED_LEFT lights[2]
#define LED_RIGHT lights[3]

CRGB leds[NUM_LEDS];
Light lights[NUM_LIGHTS];
Control* controls[NUM_CONTROLS];

BrakeControl brake1;
Button gripButton, leftSwitch, rightSwitch;

int speed = GLOBAL_SPEED;
int count = 0;

struct Config {
    String bikeName;
    int    speed;
    int    fade;
    int    brightness;
    int    num_leds;
    int    num_lights;
    int    num_params;
    int    bump_led;
    int    num_controls;
};

Config config;

void setup() {
    Serial.begin(115200);

    Serial << "Loading configuration...\n";
    SPIFFS.begin(true);
    StaticJsonDocument<1024> jsonDoc;
    File configFile = SPIFFS.open("/config.json", FILE_READ);
    deserializeJson(jsonDoc, configFile);
    configFile.close();

    JsonObject obj      = jsonDoc.as<JsonObject>();
    config.bikeName     = jsonDoc["bikeName"] | "Darkwad";
    config.speed        = jsonDoc["speed"] | 500;
    config.fade         = jsonDoc["fade"] | 25;
    config.brightness   = jsonDoc["brightness"] | 50;
    config.num_leds     = jsonDoc["num_leds"] | 100;
    config.num_params   = jsonDoc["num_params"] | 3;
    config.bump_led     = jsonDoc["bump_led"] | 0;

    Serial << "Loading Lights\n";

    if (obj.containsKey("lights")) {
        JsonArray jsonLights = obj["lights"];
        for (JsonObject light : jsonLights) {
            String lightName = light["name"];
            Serial << lightName << '\n';
        }
        config.num_lights = jsonLights.size();
    }

    Serial << config.bikeName << " Lighting Up...\n";
    delay(10);

    FastLED.setBrightness(BRIGHTNESS_SCALE);
    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(24)>(leds, NUM_LEDS);

    blink();

    Serial << "LEDs initialized\n";

//    #ifdef BUMP_LED
//        lights[0] = Light("bump", &leds[0], 0, 1);
//    #endif

    Serial << "It's lit fam!\n";

    Serial << "Initializing Controls\n";

    if (obj.containsKey("controls")) {
        JsonArray jsonControls = obj["controls"];
        for (JsonObject control : jsonControls) {
            String controlName = control["name"];
            Serial << controlName << '\n';
        }
        config.num_controls = jsonControls.size();
    }

    Serial << "Controls Initialized...\n";

    blackout();

    Serial << "Starting!\n\n";
    delay(150);
}

// MAIN LOOP

void loop() {

    Serial << "loop: ";

    Serial << "Controls [";
    for (int i=0; i<config.num_controls; i++) {
        if (count % controls[i]->getSampleRate() == 0) {
            Serial << " " << i << " ";
            String controlName = controls[i]->getName();
            int controlState = controls[i]->getState();
//          Serial << " | " << controlName << " : " << controlState << " | ";
            controls[i]->update();
        }
    }

    Serial << "] Lights [";
    for (int i=0; i<config.num_lights; i++) {
        if (count % lights[i].get_param(0) == 0) {
            Serial << " " << i << " ";
            lights[i].update();
        }
    }
    Serial << "]\n";

    FastLED.show();
    count++;
    delay(1000/speed);
    Serial << '\n';
    #ifdef SLOW
        delay(SLOW);
    #endif
}

// LED FUNCTIONS

void blink() {
    for (int i=0; i<NUM_LEDS; i++) {
        leds[i] = CRGB::White;
    }
    FastLED.show();
    delay(25);
    for (int i=0; i<NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}

void blink_rainbow() {
    CHSV color;
    for (int t=0; t<100; t++) {
        color = CHSV((t*5)%255, 255, 100);
        for (int i=0; i<NUM_LEDS; i++) {
            leds[i] = color;
        }
        FastLED.show();
        delay(10);
    }
}

void blackout() {
    for (int i=0; i<NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}
