#ifndef OLED_FONTS_H
#define OLED_FONTS_H

#include <stdint.h>

typedef struct {
    uint8_t width;
    uint8_t height;
    const uint16_t *data;
} oled_font_t;

extern const oled_font_t font_7x10;
extern const oled_font_t font_11x18;
extern const oled_font_t font_16x26;

#endif
