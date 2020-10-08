#include "FastLED.h"

int color = 0;
bool rainbow = false;
bool changeColorOnNew = true;
bool coolPattern = false;
bool halloween = true;

#define LED_DT 6
#define COLOR_ORDER BRG
#define LED_TYPE WS2812
#define NUM_LEDS 296

uint8_t max_bright = 100;
struct CRGB leds[NUM_LEDS];

bool on = false;

#define TRAIL_SIZE 100
double speed = 1;
bool going_left = false;
/* CRGB color = CRGB(255, 0, 0); */

class Trail
{
public:
    int pos, hue;
    double life;
    Trail()
    {
        this->life = -1;
        this->pos = 0;
        this->hue = 0;
    }

    void setPos(int pos, int hue)
    {
        this->life = random(100, 255);
        this->pos = pos;
        this->hue = hue;
    }
    void run()
    {
        this->life -= 2;
    }
};

double position = 0;
Trail trail[TRAIL_SIZE];

void setup()
{
    Serial.begin(115200);
    delay(1000);

    LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);

    FastLED.setBrightness(max_bright);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
}

void addTrail(int pos)
{
    for (int i = 0; i < TRAIL_SIZE; i++)
    {
        if (trail[i].life < 0)
        {
            trail[i].setPos(pos % NUM_LEDS, color);
            break;
        }
    }
}

int getMax(int val1, int val2)
{
    if (val1 > val2)
        return val1;
    else
        return val2;
}

int getMin(int val1, int val2)
{
    if (val1 < val2)
        return val1;
    else
        return val2;
}

void loop()
{

   
    if (coolPattern)
    {
        on = !on;
        color = on ? 0 : 167;
    }

    if (rainbow)
    {
        color++;
        color %= 255;
    }
    
    FastLED.clear();
    /* double calculatedSpeed = (abs(sin((position / (NUM_LEDS)) - (NUM_LEDS / 2)))) * 3 + .2;
    if (calculatedSpeed > 1)
        calculatedSpeed = 1; */
    double calculatedSpeed = 1;

    int prevPos = round(position);

    /* if (position > NUM_LEDS - 1)
        going_left = false;
    else if (position < 0)
        going_left = true;

    position += going_left ? calculatedSpeed : -calculatedSpeed; */
    position += calculatedSpeed;

    position = fmod(position, NUM_LEDS);
    if (changeColorOnNew && position == 0)
    {
        color = random(255);
    }

    /* int from = getMin(prevPos, round(position));
    int to = getMax(prevPos, round(position));

    for (int i = from - 1; i < to; i++)
    {
        addTrail(i);
    } */

    addTrail(round(position));

    for (int i = 0; i < TRAIL_SIZE; i++)
    {
        if (trail[i].life > 0) leds[trail[i].pos] = CHSV(trail[i].hue % 255, 255, round(trail[i].life));
        trail[i].run();
    }

    leds[round(position) % NUM_LEDS] = CHSV(color % 255, 255, 255);

    FastLED.show();
}
