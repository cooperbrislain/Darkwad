#ifndef DARKWAD_CONFIG_H
#define DARKWAD_CONFIG_H

//#define DEBUG

#define DEVICE_NAME "darkwad"
#define MQTT_CLID "darkwad"

#define IS_ESP32

#define IS_APA102
#define DATA_PIN 21
#define CLOCK_PIN 22

#define BRIGHTNESS_SCALE 100
#define GLOBAL_SPEED 500
#define FADE 50

#define NUM_LEDS 101
#define NUM_LIGHTS 5
#define NUM_PARAMS 4
#define BUMP_LED

#define CONTROLS
#define NUM_CONTROLS 4
#define TOUCH_PIN T0
#define ANALOG_PIN 4
#define DIGITAL_PIN 12
#define CTL_TOUCH_THRESHOLD 20
#define CTL_SAMPLE_RATE 5

#endif // DARKWAD_CONFIG_H