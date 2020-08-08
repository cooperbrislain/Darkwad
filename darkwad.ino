#include "darkwad.h"
#define LED_FRONT lights[1]
#define LED_REAR lights[4]
#define LED_LEFT lights[2]
#define LED_RIGHT lights[3]

CRGB leds[NUM_LEDS];
Light lights[NUM_LIGHTS];
Control controls[NUM_CONTROLS];

int speed = GLOBAL_SPEED;
int count = 0;

void setup() {
    Serial.begin(115200);

    Serial << "Darkwad Lighting Up";

    SPIFFS.begin(true);
    File file = SPIFFS.open("/test.txt", FILE_WRITE);
    file << "Test";
    file.close();

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

    lights[1] = Light("front", &leds[1], 7, 11);
    lights[2] = Light("left", &leds[1], 0, 5, 1);
    lights[3] = Light("right", &leds[1], 20, 5);
    CRGB* rearLeds[4] = { &leds[6], &leds[7], &leds[19], &leds[20] };
    lights[4] = Light("rear", rearLeds);

//  Test Bar Mapping

    LED_LEFT.set_rgb(CRGB::Orange);
    LED_RIGHT.set_rgb(CRGB::Orange);
    LED_REAR.set_rgb(CRGB::Red);

    for (int i=0; i<NUM_LIGHTS; i++) {
        lights[i].turn_off();
        delay(100);
    }

    Serial << "It's lit fam!";

    pinMode(T8, INPUT);
    pinMode(T9, INPUT);
    pinMode(14, INPUT);

    controls[0] = Control("left", T8, Control::CTL_DIGITAL,
        [](int val) {
            LED_LEFT.set_program("chase");
            LED_LEFT.set_param(0,25);
            LED_LEFT.set_param(1,50);
            LED_LEFT.turn_on();
        },
        [](int val) { Serial << "LEFT" << '\n'; },
        [](int val) { LED_LEFT.turn_off(); }
    );
    controls[1] = Control("right", T9, Control::CTL_DIGITAL,
        [](int val) {
            LED_RIGHT.set_program("chase");
            LED_RIGHT.set_param(0,25);
            LED_RIGHT.set_param(1,50);
            LED_RIGHT.turn_on();
        },
        [](int val) { Serial << "RIGHT" << '\n'; },
        [](int val) { LED_RIGHT.turn_off(); }
    );
    controls[2] = Control("button", 12, Control::CTL_TOUCH,
        [](int val) {  },
        [](int val) {
            Serial << "BUTTON" << '\n';
            LED_FRONT.turn_on();
        },
        [](int val) {
            LED_FRONT.turn_off();
            LED_LEFT.turn_off();
            LED_RIGHT.turn_off();
        }
    );
    controls[3] = Control("brake1", 14, Control::CTL_DIGITAL,
        [](int val) { LED_REAR.turn_on(); },
        [](int val) { Serial << "BRAKE 1" << '\n'; },
        [](int val) { LED_REAR.turn_off(); }
    );
//            controls[4] = Control("brake2", 27, Control::CTL_DIGITAL,
//                [](int val) { },
//                [](int val) { Serial << "BRAKE 2" << '\n'; },
//                [](int val) { }
//            );
//            controls[4] = AnalogControl("X", T0);
//            controls[5] = AnalogControl("Y", T1);
//            controls[6] = ButtonControl("Z", T2);

    blackout();
    delay(150);
}

// MAIN LOOP

void loop() {
    File file = SPIFFS.open("/test.txt");
    while(file.available()) {
        Serial << file.read();
    }
    file.close();

    for (int i=0; i<NUM_CONTROLS; i++)
        if (count%controls[i]._sampleRate == 0)
            controls[i].update();
    for (int i=0; i<NUM_LIGHTS; i++)
        if (count%lights[i].get_param(0) == 0)
            lights[i].update();

    FastLED.show();
    count++;
    delay(1000/speed);
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
