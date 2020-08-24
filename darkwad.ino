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
    delay(2000);
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
            JsonObject light    = jsonLights[i];
            String lightName    = light["name"];
            int led_offset      = light["led_offset"];
            int led_count       = light["led_count"];
            String color        = light["color"];
            JsonArray jsonLeds  = light["leds"];
            Light* newLight;
            if (jsonLeds.size()) {
                CRGB* led_list[jsonLeds.size()];
                led_count = jsonLeds.size();
                Serial << led_count << '\n';
                for (int j=0; j<jsonLeds.size(); j++) {
                    led_list[j] = &leds[jsonLeds[j].as<int>()];
                }
                newLight = new Light(lightName, led_list);
                newLight->setColor()
            } else if (led_count) {
                newLight = new Light(lightName, &leds[0], led_offset, led_count);
            }
            Serial << lightName << '\n';

            lights[i] = newLight;
        }
        config.num_lights = jsonLights.size();
    }

    Serial << config.bikeName << " Lighting Up...\n";
    delay(10);

    FastLED.setBrightness(BRIGHTNESS_SCALE);
    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(24)>(leds, NUM_LEDS);

    blink();

    Serial << "LEDs initialized\n";

    Serial << "It's lit fam!\n";

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

//    Serial << "loop: ";

//    Serial << config.num_controls << " Controls [";
    for (int i=0; i<config.num_controls; i++) {
        if (count % controls[i]->getSampleRate() == 0) {
//            Serial << " " << i << " ";
            String controlName = controls[i]->getName();
            int controlState = controls[i]->getState();
//          Serial << " | " << controlName << " : " << controlState << " | ";
            controls[i]->update();
        }
    }

//    Serial << "] Lights [";
    for (int i=0; i<config.num_lights; i++) {
        if (count % lights[i]->get_param(0) == 0) {
//            Serial << " " << i << " ";
            lights[i]->update();
        }
    }
//    Serial << "]\n";

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
