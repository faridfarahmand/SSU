#include "app.h"
#include "oled.h"
#include "oled_horse.h"

static void draw_bar(uint16_t adc_value)
{
    uint32_t bar_width;
    uint16_t x;
    uint16_t y;

    bar_width = ((uint32_t)adc_value * OLED_WIDTH) / 4095UL;

    for (x = 0U; x < OLED_WIDTH; x++) {
        for (y = 54U; y < 63U; y++) {
            if (x < bar_width) {
                oled_pixel(x, y, OLED_WHITE);
            } else {
                oled_pixel(x, y, OLED_BLACK);
            }
        }
    }
}

void delay(){
	for (volatile int i = 0; i < 10; i++);
}
int main(void)
{
    uint16_t adc_value;
    uint32_t millivolts;
    char text[24];


    oled_init();
    adc1_pa0_init();
/*
    while (1)
    {
        adc_value = adc1_read();
        millivolts = adc_to_millivolts(adc_value);

        oled_clear(OLED_BLACK);

        oled_cursor(0, 0);
        oled_text("ADC PA0", &font_11x18, OLED_WHITE);

        oled_cursor(0, 22);
        snprintf(text, sizeof(text), "Raw: %4u", adc_value);
        oled_text(text, &font_7x10, OLED_WHITE);

        oled_cursor(0, 36);
        snprintf(text, sizeof(text), "Volt:%lu.%03lu V",
                 millivolts / 1000UL,
                 millivolts % 1000UL);
        oled_text(text, &font_7x10, OLED_WHITE);

        draw_bar(adc_value);

        oled_update();

        delay();
    }

    //Uncomment this section if you want to Print Hello
    /*
    oled_clear(OLED_BLACK);
    oled_text("Hello", &font_16x26, OLED_WHITE);
    oled_update();
    while (1);
    */

	//SSU Logo
    while (1){
        oled_clear(OLED_WHITE);

    	oled_bitmap(0, 0, logo, 128, 64, OLED_BLACK);
    	oled_update();
    	for (int i=0;i<200;i++){
    		delay();
    	}
    	oled_clear(OLED_BLACK);

    	oled_bitmap(0, 0, logo, 128, 64, OLED_WHITE);
    	oled_update();
    }



	//This Part is for displaying a Horse running
	/*

    while (1)
    {
        oled_clear(OLED_BLACK);
        oled_bitmap(0, 0, horse2, 128, 64, OLED_WHITE);
        oled_update();

        delay();

        oled_clear(OLED_BLACK);
        oled_bitmap(0, 0, horse3, 128, 64, OLED_WHITE);
        oled_update();

        delay();
        oled_clear(OLED_BLACK);
        oled_bitmap(0, 0, horse4, 128, 64, OLED_WHITE);
        oled_update();

        delay();

        oled_clear(OLED_BLACK);
        oled_bitmap(0, 0, horse5, 128, 64, OLED_WHITE);
        oled_update();

        delay();

        oled_clear(OLED_BLACK);
        oled_bitmap(0, 0, horse6, 128, 64, OLED_WHITE);
        oled_update();

        delay();

        oled_clear(OLED_BLACK);
        oled_bitmap(0, 0, horse7, 128, 64, OLED_WHITE);
        oled_update();

        delay();
        oled_clear(OLED_BLACK);
        oled_bitmap(0, 0, horse8, 128, 64, OLED_WHITE);
        oled_update();

        delay();
        oled_clear(OLED_BLACK);
        oled_bitmap(0, 0, horse9, 128, 64, OLED_WHITE);
        oled_update();

        delay();
        oled_clear(OLED_BLACK);
        oled_bitmap(0, 0, horse10, 128, 64, OLED_WHITE);
        oled_update();

        delay();
    }*/
}
