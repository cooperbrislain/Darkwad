#include "darkwad.h"

CRGB leds[NUM_LEDS];
Light lights[NUM_LIGHTS];
TouchControl controls[NUM_CONTROLS];
int speed = GLOBAL_SPEED;
int count = 0;

// SETUP

void setup() {
    Serial.begin(115200);
    Serial.println("Starting up LED Controller");
    delay(10);
    FastLED.setBrightness(BRIGHTNESS_SCALE);
    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(24)>(leds, NUM_LEDS);

    blink();

    Serial.println("LEDs initialized");

    #ifdef BUMP_LED
        lights[0] = Light("bump", &leds[0], 0, 1);
    #endif
//        lights[1] = Light("front", &leds[1], 0, 25); // FRONT
//        lights[2] = Light("left", &leds[1], 25, 25, 1); // LEFT
//        lights[3] = Light("right", &leds[1], 50, 25); // RIGHT
//        lights[4] = Light("rear", &leds[1], 75, 25); // REAR
    /* Test setup */
    lights[2] = Light("left", &leds[1], 0, 5, 1);
    lights[1] = Light("front", &leds[1], 7, 11);
    lights[3] = Light("right", &leds[1], 20, 5);
    CRGB* rearLeds[4] = { &leds[6], &leds[7], &leds[19], &leds[20] };
    lights[4] = Light("rear", rearLeds);

    lights[2].set_rgb(CRGB::Orange);
    lights[3].set_rgb(CRGB::Orange);
    lights[4].set_rgb(CRGB::Red);
    lights[0].set_param(0,1);
    lights[1].set_param(0,1);
    lights[2].set_param(0,1);
    lights[3].set_param(0,1);
    lights[4].set_param(0,1);

    for (Light light : lights) {
        light.blink();
    }

    Serial.println("Light Mapping Initialized");

    #ifdef CONTROLS
        controls[0] = TouchControl("left", T8, 20,
                [](int val) {
                    lights[2].set_program("chase");
                    lights[2].set_param(0,25);
                    lights[2].set_param(1,50);
                    lights[2].set_on(1); },
                [](int val) { Serial.println("LEFT"); },
                [](int val) { lights[2].set_on(0); }
            );
            controls[1] = TouchControl("right", T9, 20,
                [](int val) {
                    lights[3].set_program("chase");
                    lights[3].set_param(0,25);
                    lights[3].set_param(1,50);
                    lights[3].set_on(1);
                    },
                [](int val) { Serial.println("RIGHT"); },
                [](int val) {
                    lights[3].set_on(0);
                }
            );
            controls[2] = TouchControl("Green", T5, 25,
                [](int val) { lights[1].turn_on(); },
                [](int val) { Serial.println("GREEN"); },
                [](int val) { lights[1].set_on(0); }
            );
            controls[3] = TouchControl("Red", T3, TOUCH_THRESHOLD,
                [](int val) { lights[4].turn_on(); },
                [](int val) { Serial.println("RED"); },
                [](int val) { lights[4].set_on(0); }
            );
    #endif // CONTROLS

    delay(150);
}

// MAIN LOOP

void loop() {
    #ifdef TOUCH
        for (int i=0; i<NUM_CONTROLS; i++) controls[i].update();
    #endif
    for (int i=0; i<NUM_LIGHTS; i++)
        if (count%lights[i].get_param(0) == 0)
            lights[i].update();
    FastLED.show();
    count++;
    #ifdef SLOW
        delay(SLOW);
    #endif
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
