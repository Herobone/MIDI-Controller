#ifndef MIDI_H_
#define MIDI_H_

#include <stdint.h>

#include <pixelset.h>

#include "config.h"

class Midi
{
    private:
    CRGB *leds_;
public:
    static void NoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity);
    static void NoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity);
    static void ControlChange(uint8_t channel, uint8_t control, uint8_t value);
    void Recieve();

    Midi(CRGB *leds): leds_(leds) {}
};

#endif // MIDI_H_