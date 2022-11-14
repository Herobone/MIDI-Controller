#include "midi.h"

#include <MIDIUSB.h>

#include "pin_config.h"

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void Midi::NoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    midiEventPacket_t noteOn = {0x09, (uint8_t)(0x90 | channel), pitch, velocity};
    MidiUSB.sendMIDI(noteOn);
}
void Midi::NoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    midiEventPacket_t noteOff = {0x08, (uint8_t)(0x80 | channel), pitch, velocity};
    MidiUSB.sendMIDI(noteOff);
}
void Midi::ControlChange(uint8_t channel, uint8_t control, uint8_t value)
{
    midiEventPacket_t event = {0x0B, (uint8_t)(0xB0 | channel), control, value};
    MidiUSB.sendMIDI(event);
}

void Midi::Recieve() {
        midiEventPacket_t rx = MidiUSB.read();
    if (rx.header != 0)
    {
        switch (rx.header & 0x0F)
        {
        case 0x00: // Misc. Reserved for future extensions.
            break;
        case 0x01: // Cable events. Reserved for future expansion.
            break;
        case 0x02: // Two-byte System Common messages
        case 0x0C: // Program Change
        case 0x0D: // Channel Pressure
            // do something with 2 byte messages
            break;
        case 0x03: // Three-byte System Common messages
        case 0x08: // Note-off
        case 0x09: // Note-on
        case 0x0A: // Poly-KeyPress
        case 0x0B: // Control Change
        case 0x0E: // PitchBend Change
            // do something with 3 byte messages
            for (uint8_t i = 0; i < 5; i++)
            {
                if (UPPER_BUTTONS[i] == rx.byte2)
                {
                    leds_[i] = CRGB(0, rx.byte3 * 2, 0);
                }
            }
            Serial.println("Recieved note");

            FastLED.show();
            break;
        case 0x04: // SysEx starts or continues
            // append sysex buffer with 3 bytes
            break;
        case 0x05: // Single-byte System Common Message or SysEx ends with the following single byte
            // append sysex buffer with 1 byte
            // process completed sysex buffer
            // init sysex buffer empty
            break;
        case 0x06: // SysEx ends with the following two bytes
            // append sysex buffer with 2 bytes
            // process completed sysex buffer
            // init sysex buffer empty
            break;
        case 0x07: // SysEx ends with the following three bytes
            // append sysex buffer with 3 bytes
            // process completed sysex buffer
            // init sysex buffer empty
            break;
        case 0x0F: // Single Byte, TuneRequest, Clock, Start, Continue, Stop, etc.
            // process since byte messages
            break;
        }
    }
}