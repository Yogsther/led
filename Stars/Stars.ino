#include "FastLED.h"

#define LED_DT 6
#define COLOR_ORDER BRG
#define LED_TYPE WS2812
#define NUM_LEDS 296

uint8_t max_bright = 100;
struct CRGB leds[NUM_LEDS];

int spawnEvery = 10;
int brightnessChange = 4;

int colors[] = {230, 60};

#define DOT_ARR_SIZE 90

class Dot
{
public:
    int brightness, color, pos;
    bool goingUp = true;
    bool alive = false;
    Dot(){};
    Dot(int pos, int color)
    {
        this->pos = pos;
        this->color = colors[random(2)];
        this->brightness = 0;
        this->alive = true;
    };
};

Dot dots[DOT_ARR_SIZE];

void spawnDot(Dot dot)
{
    for (int i = 0; i < DOT_ARR_SIZE; i++)
    {
        if (!dots[i].alive)
        {
            dots[i] = dot;
            return;
        }
    }
}

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

    if (random(spawnEvery) < 1)
    {
        spawnDot(Dot(round(random(NUM_LEDS)), 0));
    }

    for (int i = 0; i < DOT_ARR_SIZE; i++)
    {
        Dot *dot = &dots[i];
        if (dot->alive)
        {

            if (dot->brightness > 255)
                dot->goingUp = false;

            dot->brightness += dot->goingUp ? brightnessChange : -brightnessChange;

            if (dot->brightness < 0)
            {
                dot->alive = false;
            }
            else
            {
                leds[dot->pos] = CHSV(dot->color, 255, dot->brightness);
            }
        }
    }

    FastLED.show();
}
