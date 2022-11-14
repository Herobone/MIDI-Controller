#ifndef PIN_CONFIG_H_
#define PIN_CONFIG_H_

#include <stdint.h>

#include <avr/pgmspace.h>
#include <variant.h>

#define TFT_CS 10        // Hallowing display control pins: chip select
#define TFT_RST 8        // Display reset
#define TFT_DC 9         // Display data/command select
#define TFT_BACKLIGHT 11 // Display backlight pin
#define LED_PIN 12

const PROGMEM uint8_t LOWER_BUTTONS[] = {49, 50, 51, 52, 53};

const PROGMEM uint8_t UPPER_BUTTONS[] = {44, 45, 46, 47, 48};

const PROGMEM uint8_t SLIDERS[] = {A0, A1, A2, A3, A4};

const PROGMEM uint8_t KNOBS[] = {A7, A8, A5, A6};

#endif // PIN_CONFIG_H_