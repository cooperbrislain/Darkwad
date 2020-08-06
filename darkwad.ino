#include "darkwad.h"

CRGB leds[NUM_LEDS];
Light lights[NUM_LIGHTS];
Control controls[NUM_CONTROLS];

int speed = GLOBAL_SPEED;
int count = 0;

void setup() {
    Serial.begin(115200);

    Serial << "Darkwad Lighting Up";

    delay(10);

    FastLED.setBrightness(BRIGHTNESS_SCALE);
    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(24)>(leds, NUM_LEDS);

    blink();

    Serial << "LEDs initialized";

    #ifdef BUMP_LED
        lights[0] = Light("bump", &leds[0], 0, 1);
    #endif

//  Bike Mapping
//        lights[1] = Light("front", &leds[1], 0, 25); // FRONT
//        lights[2] = Light("left", &leds[1], 25, 25, 1); // LEFT
//        lights[3] = Light("right", &leds[1], 50, 25); // RIGHT
//        lights[4] = Light("rear", &leds[1], 75, 25); // REAR

//  Test Bar Mapping
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

    Serial << "It's lit fam!";

    pinMode(T8, INPUT);
    pinMode(T9, INPUT);
    pinMode(14, INPUT);

    #ifdef CONTROLS
        controls[0] = Control("left", T8, Control::CTL_DIGITAL,
                [](int val) {
                    lights[1].set_program("chase");
                    lights[1].set_param(0,25);
                    lights[1].set_param(1,50);
                    lights[1].set_on(1); },
                [](int val) { Serial << "LEFT" << '\n'; },
                [](int val) { lights[1].set_on(0); }
            );
            controls[1] = Control("right", T9, Control::CTL_DIGITAL,
                [](int val) {
                    lights[3].set_program("chase");
                    lights[3].set_param(0,25);
                    lights[3].set_param(1,50);
                    lights[3].set_on(1);
                    },
                [](int val) { Serial << "RIGHT" << '\n'; },
                [](int val) {
                    lights[3].set_on(0);
                }
            );
            controls[2] = Control("button", 12, Control::CTL_TOUCH,
                [](int val) { lights[2].turn_on(); },
                [](int val) { Serial << "BUTTON" << '\n'; },
                [](int val) { lights[2].set_on(0); }
            );
            controls[3] = Control("brake", 14, Control::CTL_DIGITAL,
                [](int val) { lights[4].turn_on(); },
                [](int val) { Serial << "BRAKE" << '\n'; },
                [](int val) { lights[4].set_on(0); }
            );
//            controls[4] = AnalogControl("X", T0);
//            controls[5] = AnalogControl("Y", T1);
//            controls[6] = ButtonControl("Z", T2);
    #endif // CONTROLS

    blackout();

    delay(150);
}

// MAIN LOOP

void loop() {
    for (int i=0; i<NUM_CONTROLS; i++)
        if (count%controls[i]._sampleRate == 0)
            controls[i].update();
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
