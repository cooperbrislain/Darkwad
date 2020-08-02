#include darkwad.h

#define DEFAULT_SCAN_LIST_SIZE CONFIG_EXAMPLE_SCAN_LIST_SIZE

#ifdef USE_WIFI
    const char *wifi_ssid = WIFI_SSID;
    const char *wifi_pass = WIFI_PASS;
    byte mac[6] = { };
    byte broadcast[4] = { };
    byte ip[4] = { };
    WiFiClient net_client;
#endif
#ifndef NO_NETWORK
    void mqtt_callback(char* topic, byte* payload, unsigned int length);
    PubSubClient mqtt_client(net_client);
#endif

CRGB leds[NUM_LEDS];
Light lights[NUM_LIGHTS];
TouchControl controls[NUM_CONTROLS];
int speed = GLOBAL_SPEED;
int count = 0;

const char* mqtt_server = MQTT_HOST;
const int mqtt_port = 1883;
const char* mqtt_username = MQTT_USER;
const char* mqtt_key = MQTT_PASS;
const char* mqtt_id = MQTT_CLID;

// SETUP

void setup() {
    Serial.begin(115200);
    Serial.println("Starting up LED Controller");
    delay(10);
    FastLED.setBrightness(BRIGHTNESS_SCALE);
    #ifdef IS_APA102
        FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(24)>(leds, NUM_LEDS);
    #endif
    #ifdef IS_WS2801
        FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    #endif

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
    lights[1] = Light("front", &leds[1], 12, 11);
    lights[2] = Light("left", &leds[1], 0, 10, 1);
    lights[3] = Light("right", &leds[1], 25, 10);
    CRGB* rearLeds[4] = { &leds[11], &leds[12], &leds[24], &leds[25] };
    lights[4] = Light("rear", rearLeds);

    lights[2].set_rgb(CRGB::Orange);
    lights[3].set_rgb(CRGB::Orange);
    lights[4].set_rgb(CRGB::Red);
    lights[1].set_param(0,1);
    lights[2].set_param(0,1);
    lights[3].set_param(0,1);
    lights[4].set_param(0,1);

    for (Light light : lights) {
        light.blink();
    }

    Serial.println("Light Mapping Initialized");

    #ifdef CONTROLS
        controls[0] = TouchControl("left", T1, TOUCH_THRESHOLD,
                [](int val) {
                    lights[2].set_program("chase");
                    lights[2].set_param(0,25);
                    lights[2].set_param(1,50);
                    lights[2].set_on(1); },
                [](int val) { Serial.println("LEFT"); },
                [](int val) { lights[2].set_on(0); }
            );
            controls[1] = TouchControl("right", T0, TOUCH_THRESHOLD,
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
            controls[2] = TouchControl("Green", T2, TOUCH_THRESHOLD,
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

    #ifdef USE_WIFI
        WiFi.begin(wifi_ssid, wifi_pass);
                while (WiFi.status() != WL_CONNECTED) {
                    Serial.println("Connecting to WiFi...");
                    delay(500);
                }
                IPAddress localip = WiFi.localIP();


        mqtt_client.setServer(mqtt_server, mqtt_port);
        mqtt_client.setCallback(mqtt_callback);

        broadcast[0] = ip[0] = localip[0];
        broadcast[1] = ip[1] = localip[1];
        broadcast[2] = ip[2] = localip[2];
        ip[3] = localip[3];
        broadcast[3] = 0xff;
        for (int i=0; i<NUM_LEDS; i++) {
            leds[i] = CRGB::Black;
        }
        FastLED.show();
        delay(100);

        blink_rainbow();
        Serial.println(F("Network configured via DHCP"));
        Serial.print("IP address: ");
        Serial.println(localip);
        delay(100);

        blackout();
        Serial.println("Setup complete, starting...");
    #endif


    delay(150);
}

// MAIN LOOP

void loop() {
#ifndef NO_NETWORK
    if (!mqtt_client.connected()) {
        Serial.println("Reconnecting...");
        reconnect();
    }
    mqtt_client.loop();
#ifdef ARTNET
    artnet.read();
#endif
#endif
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

#ifndef NO_NETWORK

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    DynamicJsonDocument json(1024);
    deserializeJson(json, (char *)payload);
    payload[length] = '\0';
    char* device = strtok(topic,"/");
    char* name = strtok(NULL,"/");
    if (strcmp(device, DEVICE_NAME) == 0
        || strcmp(device, "global") == 0
        || strcmp(device, "all" == 0)) {
        for (int i=0; i<NUM_LIGHTS; i++) {
            if (strcmp(name, lights[i].get_name()) == 0) {
                if (json.containsKey("On")) {
                    bool OnOff = json["On"].as<bool>();
                    if(OnOff) {
                        Serial.println("Turning On");
                        lights[i].turn_on();
                    } else {
                        Serial.println("Turning Off");
                        lights[i].turn_off();
                    }
                }
                if (json.containsKey("Hue")) {
                    int val = json["Hue"].as<int>();
                    int hue = val * 17/24; // 255/360
                    Serial.print("Setting hue to ");
                    Serial.println(hue);
                    lights[i].set_hue(hue);
                }
                if (json.containsKey("Brightness")) {
                    int val = json["Brightness"].as<int>();
                    int bright = val * 51/20; // 255/100
                    Serial.print("Setting brightness to ");
                    Serial.println(bright);
                    lights[i].set_brightness(bright);
                }
                if (json.containsKey("Saturation")) {
                    int val = json["Saturation"].as<int>();
                    int sat = val * 51/20; // 255/100
                    Serial.print("Setting saturation to ");
                    Serial.println(sat);
                    lights[i].set_saturation(sat);
                }
                if (json.containsKey("Program")) {
                    const char* program = json["Program"];
                    lights[i].set_program(program);
                }
                if (json.containsKey("Speed")) { // deprecated, will be removed
                    int val = json["Speed"].as<int>();
                    if (device == "global") {
                        Serial.print("Setting speed to ");
                        Serial.println(val);
                        speed = val;
                    } else {
                        lights[i].set_param(0, val);
                    }
                }
                if (json.containsKey("Params")) {
                    Serial.println("Setting params");
                    JsonArray params = json["Params"].as<JsonArray>();
                    for (int p=0; p<sizeof(params); p++) {
                        int v = params[i].as<int>();
                        Serial.print("Setting param ");
                        Serial.print(p);
                        Serial.print(" to ");
                        Serial.print(v);
                        lights[i].set_param(p, v);
                    }
                }
                if (json.containsKey("Param")) {
                    int p = json["Param"]["p"];
                    int v = json["Param"]["v"];
                    lights[i].set_param(p, v);
                }
            }
        }
    }
}

void reconnect() {
    // Loop until we're reconnected
    while (!mqtt_client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (mqtt_client.connect(mqtt_id,mqtt_username,mqtt_key)) {
            char feed[128];
            sprintf(feed, "/%s/global", DEVICE_NAME);
            mqtt_client.subscribe(feed);
            sprintf(feed, "/%s/all", DEVICE_NAME);
            mqtt_client.subscribe(feed);
            for(int i=0; i<NUM_LIGHTS; i++) {
                lights[i].subscribe(&mqtt_client);
            }
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

#endif

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
