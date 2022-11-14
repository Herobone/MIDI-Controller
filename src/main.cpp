#include <Arduino.h>
#include <FastLED.h>

#include "pin_config.h"
#include "config.h"
#include "macros.h"
#include "midi.h"

#define CHANNEL_COUNT 5

CRGBArray<NUM_LEDS> leds;

volatile uint32_t debounceTime = 0;
#define REQUIRED_FOR_DEBOUNCE 5
#define ANALOG_RESOLUTION 12
#define ANALOG_MAX POW(2, ANALOG_RESOLUTION)
#define CONVERSION_FACTOR (uint8_t)(ANALOG_MAX / 128)
#define TO_7BIT(x) x / CONVERSION_FACTOR

Midi midi_control = NULL;

void setup()
{
    Serial.begin(115200);
    FastLED.addLeds<WS2812B, LED_PIN>(leds, NUM_LEDS);
    // Lower Button
    for (uint8_t i = 0; i < 5; i++)
    {
        pinMode(LOWER_BUTTONS[i], INPUT_PULLUP);
        pinMode(UPPER_BUTTONS[i], INPUT_PULLUP);
    }

    // fade everything out
    leds.fadeToBlackBy(255);

    analogReadResolution(ANALOG_RESOLUTION);
    FastLED.setBrightness(100);

    midi_control = Midi(leds);

    Serial.print("Conversion Rate: ");
    Serial.println(CONVERSION_FACTOR);
    Serial.println("Controller started!");
}

signed short lastFaderState[] = {0, 0, 0, 0, 0};
signed short currentFaderState[] = {0, 0, 0, 0, 0};
signed short lastKnobState[] = {0, 0, 0, 0, 0};
signed short currentKnobState[] = {0, 0, 0, 0, 0};

uint8_t lastUpperButtonState[] = {0, 0, 0, 0, 0};
uint8_t lastLowerButtonState[] = {0, 0, 0, 0, 0};
uint8_t currentUpperButtonState[] = {0, 0, 0, 0, 0};
uint8_t currentLowerButtonState[] = {0, 0, 0, 0, 0};

void loop()
{
    midi_control.Recieve();

    for (uint8_t i = 0; i < 5; i++)
    {
        currentFaderState[i] = analogRead(SLIDERS[i]);
        currentLowerButtonState[i] = digitalRead(LOWER_BUTTONS[i]);
        currentUpperButtonState[i] = digitalRead(UPPER_BUTTONS[i]);

        if (currentLowerButtonState[i] != lastLowerButtonState[i])
        {
            if (currentLowerButtonState[i])
                Midi::NoteOff(0, LOWER_BUTTONS[i], 0);
            else
                Midi::NoteOn(0, LOWER_BUTTONS[i], 127);
        }

        if (currentUpperButtonState[i] != lastUpperButtonState[i])
        {
            if (currentUpperButtonState[i])
                Midi::NoteOff(0, UPPER_BUTTONS[i], 0);
            else
                Midi::NoteOn(0, UPPER_BUTTONS[i], 127);
        }

        if (ABS(currentFaderState[i] - lastFaderState[i]) > CONVERSION_FACTOR)
        {
            Midi::ControlChange(0, i + 1, 127 - TO_7BIT(currentFaderState[i]));
            lastFaderState[i] = currentFaderState[i];
        }

        lastLowerButtonState[i] = currentLowerButtonState[i];
        lastUpperButtonState[i] = currentUpperButtonState[i];
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        currentKnobState[i] = analogRead(KNOBS[i]);

        if (ABS(currentKnobState[i] - lastKnobState[i]) > CONVERSION_FACTOR)
        {
            Midi::ControlChange(0, i + 6, TO_7BIT(currentKnobState[i]));
            lastKnobState[i] = currentKnobState[i];
        }
    }
    
}