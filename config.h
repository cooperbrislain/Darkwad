#ifndef DARKWAD_CONFIG_H
#define DARKWAD_CONFIG_H

#define SSD1306_128_32

//#define DEBUG

#define IS_ESP32
#define IS_APA102

#define DATA_PIN            19
#define CLOCK_PIN           18

#define NUM_LIGHTS          5
#define NUM_PARAMS          4

#define CONTROLS
#define NUM_CONTROLS        4
#define CTL_TOUCH_THRESHOLD 20
#define CTL_SAMPLE_RATE     5

#define OLED_DATA_PIN       36
#define OLED_CLOCK_PIN      39
#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define OLED_RESET          -1

#endif // DARKWAD_CONFIG_H