#include "FastLED.h"

#define LED_DT 6
#define COLOR_ORDER BRG
#define LED_TYPE WS2812
#define NUM_LEDS 296

#define MAX_EXPLOSIONS 2

uint8_t max_bright = 100;
struct CRGB leds[NUM_LEDS];

float resistance = .01f;
float brightnessDecrease = 1.2;
float explosionSize = .15f;
float chanceOfSpawn = .1; // % each tick (.01 > 100%)

#define NUM_COLORS 2
int colors[] = {230, 60};

#define PARTICLE_SIZE 12

class Dot
{
public:
    int color;
    float speed, position, brightness;
    int direction; // -1 Left, 1 Right
    bool alive;
    Dot()
    {
        this->alive = false;
    };
    Dot(int position, int direction, float speed, int color = 0)
    {
        this->alive = true;
        this->position = position;
        this->direction = direction;
        this->speed = speed + (random(-3, 3));
        this->color = color + random(-2, 2);
        this->brightness = 255 - random(20);

        if (this->speed <= 0)
            this->speed = .5;
    };

    void Run()
    {
        if (this->speed > 0)
            this->position += this->speed * explosionSize * this->direction;

        this->speed -= resistance * explosionSize;
        this->brightness -= brightnessDecrease;

        if (this->brightness > 0)
        {
            if (this->position > 0 && this->position < NUM_LEDS)
                leds[this->GetPosition()] = CHSV(this->GetColor(), 255, this->brightness);
        }
        else
        {
            this->alive = false;
        }
    };

    int GetPosition()
    {
        return round(this->position);
    };

    int GetBrightness()
    {
        return this->brightness >= 0 ? round(this->brightness) : 0;
    };

    int GetColor()
    {
        return this->color;
    };
};

class Explosion
{
public:
    Dot dots[PARTICLE_SIZE];
    bool alive;
    bool epxloded;
    float speed, flicker;

    float position, origin;
    int destination, color;

    Explosion()
    {
        this->alive = false;
    };
    Explosion(int position, int color)
    {
        this->position = position > NUM_LEDS / 2 ? NUM_LEDS - 1 : 0;
        this->destination = position;
        this->alive = true;
        this->epxloded = false;
        this->color = color;
        this->speed = 1;
        this->origin = this->position;
        this->flicker = 1;
    };

    void Explode()
    {
        this->epxloded = true;
        for (int i = 0; i < PARTICLE_SIZE; i++)
        {
            if (random(5) > 1)
                dots[i] = Dot(this->position, i < PARTICLE_SIZE / 2 ? 1 : -1, i < PARTICLE_SIZE / 2 ? (PARTICLE_SIZE / 2) - i : PARTICLE_SIZE - i, this->color);
        }
    };

    void Run()
    {
        if (!this->alive)
            return;

        if (!this->epxloded)
        {
            if (this->speed <= .08)
            {
                this->Explode();
            }
            else
            {
                /* this->flicker -= .01;
                if (this->flicker < 0)
                    this->flicker = 1; */

                float distance = abs(this->position - this->destination);
                float originalDistance = abs(this->origin - this->destination);
                float progress = distance / originalDistance;

                speed = sin((progress * PI) / 2);

                this->position += this->speed * (this->position - this->destination > 0 ? -1 : 1);
                if (this->position > 0 && this->position < NUM_LEDS)
                    leds[round(this->position)] = CHSV(this->color, 255, 255 * flicker);
            }
        }
        else
        {
            bool alive = false;
            for (int i = 0; i < PARTICLE_SIZE; i++)
            {
                if (dots[i].alive)
                {
                    dots[i].Run();
                    alive = true;
                }
            }
            if (!alive)
                this->alive = false;
        }
    };
};

Explosion explosions[MAX_EXPLOSIONS];

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

    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (!explosions[i].alive && random(10000) / 100 < chanceOfSpawn)
        {

            int color = colors[random(NUM_COLORS)];
            explosions[i] = Explosion(round(random(NUM_LEDS)), color);
        }
    }

    FastLED.clear();

    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        explosions[i].Run();
    }

    FastLED.show();
}
