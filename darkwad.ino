#include "darkwad.h"

#define LED_FRONT lights[1]
#define LED_REAR lights[4]
#define LED_LEFT lights[2]
#define LED_RIGHT lights[3]

CRGB leds[NUM_LEDS];
Light* lights[NUM_LIGHTS];
Control* controls[NUM_CONTROLS];

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
    StaticJsonDocument<2048> jsonDoc;
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
        int numLights = jsonLights.size();
        for (int i=0; i<numLights; i++) {
            JsonObject jsonLight    = jsonLights[i];
            Light* newLight         = new Light(&leds[0], jsonLight);
            lights[i] = newLight;
            Serial << "Added " << newLight->getName() << '\n';
        }
        config.num_lights = jsonLights.size();
    }

    Serial << config.bikeName << " Lighting Up...\n";
    FastLED.setBrightness(config.brightness);
    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(24)>(leds, NUM_LEDS);
    blink();
    Serial << "It's lit fam!\n";
    delay(100);

    Serial << "Initializing Controls\n";
    if (obj.containsKey("controls")) {
        JsonArray jsonControls = obj["controls"];
        int numControls = jsonControls.size();
        for (int i=0; i<numControls; i++) {
            JsonObject control = jsonControls[i];
            String controlName = control["name"];
            String controlType = control["type"];
            JsonArray jsonPins = control["pins"];
            const int numPins = jsonPins.size();
            int pins[numPins];
            for (int j=0; j<numPins; j++) pins[j] = jsonPins[j];
            Serial << "[" << controlType << "] " << controlName << '\n';
            if (controlType == "Button") {
                Button* newButton = new Button(controlName, pins);
                controls[i] = newButton;
            }
            if (controlType == "DPad") {
                DPad* newDPad = new DPad(controlName, pins);
                controls[i] = newDPad;
            }
        }
        config.num_controls = numControls;
    }

    Serial << config.num_controls << " Controls Initialized...\n";

    blackout();

    Serial << "Starting!\n\n";
    delay(150);
}

// MAIN LOOP

void loop() {

    for (int i=0; i<config.num_controls; i++) {
        if (count % controls[i]->getSampleRate() == 0) {
            Serial << " " << i << " ";
            String controlName = controls[i]->getName();
            int controlState = controls[i]->getState();
            Serial << " | " << controlName << " : " << controlState << " | ";
            controls[i]->update();
        }
    }

    for (int i=0; i<config.num_lights; i++) {
        if (count % lights[i]->getParam(0) == 0) {
            Serial << " " << i << " ";
            lights[i]->update();
        }
    }

    FastLED.show();

    count++;
    delay(1000/speed);
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

void blackout() {
    for (int i=0; i<NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}
