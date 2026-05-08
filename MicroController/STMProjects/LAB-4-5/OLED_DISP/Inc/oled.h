#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include "oled_fonts.h"

#define OLED_ADDR   0x3C
#define OLED_WIDTH  128
#define OLED_HEIGHT 64

typedef enum {
    OLED_BLACK = 0,
    OLED_WHITE = 1
} oled_color_t;

int  oled_init(void);
void oled_update(void);
void oled_clear(oled_color_t color);
void oled_pixel(uint16_t x, uint16_t y, oled_color_t color);
void oled_cursor(uint16_t x, uint16_t y);
char oled_char(char c, const oled_font_t *font, oled_color_t color);
char oled_text(const char *text, const oled_font_t *font, oled_color_t color);
void oled_bitmap(uint16_t x, uint16_t y, const unsigned char *bitmap, uint16_t w, uint16_t h, oled_color_t color);

#endif
