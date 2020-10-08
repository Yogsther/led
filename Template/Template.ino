#include "FastLED.h"

#define LED_DT 6
#define COLOR_ORDER BRG
#define LED_TYPE WS2812
#define NUM_LEDS 296

uint8_t max_bright = 100;
struct CRGB leds[NUM_LEDS];

void setup()
{
    Serial.begin(115200);
    delay(1000);
    LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(max_bright);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
}

void loop()
{
    FastLED.clear();
    /* leds[round(position) % NUM_LEDS] = CHSV(color % 255, 255, 255); */
    FastLED.show();
}
