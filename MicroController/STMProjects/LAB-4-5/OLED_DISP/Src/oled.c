#include <string.h>
#include "i2c.h"
#include "oled.h"

#define OLED_CMD  0x00
#define OLED_DATA 0x40
#define OLED_PAGES (OLED_HEIGHT / 8)

static uint8_t oled_buffer[OLED_WIDTH * OLED_PAGES];
static uint16_t cursor_x;
static uint16_t cursor_y;

static int oled_command(uint8_t value)
{
    return i2c1_write_byte(OLED_ADDR, OLED_CMD, value);
}

static int oled_data_block(const uint8_t *data, uint16_t len)
{
    uint8_t packet[1 + OLED_WIDTH];
    if (len > OLED_WIDTH) {
        return -1;
    }

    packet[0] = OLED_DATA;
    memcpy(&packet[1], data, len);
    return i2c1_write(OLED_ADDR, packet, (uint16_t)(len + 1U));
}

int oled_init(void)
{
    i2c1_init();

    if (oled_command(0xAE) < 0) return -1;
    if (oled_command(0x20) < 0) return -1;
    if (oled_command(0x10) < 0) return -1;
    if (oled_command(0xB0) < 0) return -1;
    if (oled_command(0xC8) < 0) return -1;
    if (oled_command(0x00) < 0) return -1;
    if (oled_command(0x10) < 0) return -1;
    if (oled_command(0x40) < 0) return -1;
    if (oled_command(0x81) < 0) return -1;
    if (oled_command(0xFF) < 0) return -1;
    if (oled_command(0xA1) < 0) return -1;
    if (oled_command(0xA6) < 0) return -1;
    if (oled_command(0xA8) < 0) return -1;
    if (oled_command(0x3F) < 0) return -1;
    if (oled_command(0xA4) < 0) return -1;
    if (oled_command(0xD3) < 0) return -1;
    if (oled_command(0x00) < 0) return -1;
    if (oled_command(0xD5) < 0) return -1;
    if (oled_command(0xF0) < 0) return -1;
    if (oled_command(0xD9) < 0) return -1;
    if (oled_command(0x22) < 0) return -1;
    if (oled_command(0xDA) < 0) return -1;
    if (oled_command(0x12) < 0) return -1;
    if (oled_command(0xDB) < 0) return -1;
    if (oled_command(0x20) < 0) return -1;
    if (oled_command(0x8D) < 0) return -1;
    if (oled_command(0x14) < 0) return -1;
    if (oled_command(0xAF) < 0) return -1;

    oled_clear(OLED_BLACK);
    oled_update();
    cursor_x = 0U;
    cursor_y = 0U;
    return 0;
}

void oled_update(void)
{
    uint8_t page;

    for (page = 0U; page < OLED_PAGES; page++) {
        oled_command((uint8_t)(0xB0U + page));
        oled_command(0x00);
        oled_command(0x10);
        oled_data_block(&oled_buffer[OLED_WIDTH * page], OLED_WIDTH);
    }
}

void oled_clear(oled_color_t color)
{
    memset(oled_buffer, (color == OLED_WHITE) ? 0xFF : 0x00, sizeof(oled_buffer));
}

void oled_pixel(uint16_t x, uint16_t y, oled_color_t color)
{
    if ((x >= OLED_WIDTH) || (y >= OLED_HEIGHT)) {
        return;
    }

    if (color == OLED_WHITE) {
        oled_buffer[x + (y / 8U) * OLED_WIDTH] |=  (1U << (y % 8U));
    } else {
        oled_buffer[x + (y / 8U) * OLED_WIDTH] &= ~(1U << (y % 8U));
    }
}

void oled_cursor(uint16_t x, uint16_t y)
{
    cursor_x = x;
    cursor_y = y;
}

char oled_char(char c, const oled_font_t *font, oled_color_t color)
{
    uint32_t row;
    uint32_t col;
    uint32_t bits;

    if ((font == 0) || (c < 32) || (c > 126)) {
        return 0;
    }

    if ((cursor_x + font->width > OLED_WIDTH) || (cursor_y + font->height > OLED_HEIGHT)) {
        return 0;
    }

    for (row = 0U; row < font->height; row++) {
        bits = font->data[(uint32_t)(c - 32) * font->height + row];
        for (col = 0U; col < font->width; col++) {
            if ((bits << col) & 0x8000U) {
                oled_pixel((uint16_t)(cursor_x + col), (uint16_t)(cursor_y + row), color);
            } else {
                oled_pixel((uint16_t)(cursor_x + col), (uint16_t)(cursor_y + row), (color == OLED_WHITE) ? OLED_BLACK : OLED_WHITE);
            }
        }
    }

    cursor_x += font->width;
    return c;
}

char oled_text(const char *text, const oled_font_t *font, oled_color_t color)
{
    while ((text != 0) && (*text != '\0')) {
        if (oled_char(*text, font, color) != *text) {
            return *text;
        }
        text++;
    }
    return 0;
}

void oled_bitmap(uint16_t x, uint16_t y, const unsigned char *bitmap, uint16_t w, uint16_t h, oled_color_t color)
{
    uint16_t i;
    uint16_t j;
    uint16_t byte_width;

    if (bitmap == 0) {
        return;
    }

    byte_width = (uint16_t)((w + 7U) / 8U);

    for (j = 0U; j < h; j++) {
        for (i = 0U; i < w; i++) {
            uint8_t b = bitmap[j * byte_width + (i / 8U)];
            if ((b & (0x80U >> (i % 8U))) != 0U) {
                oled_pixel((uint16_t)(x + i), (uint16_t)(y + j), color);
            }
        }
    }
}
