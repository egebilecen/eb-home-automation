#ifndef lcd_h
#define lcd_h

#include "config.h"
#include <OLED_I2C.h>

//--------------------------------- Declarations for SSD1306 128x64 OLED
// NOT TESTED
uint8_t  extern SmallFont[];
OLED     extern OLED_SSD1306_128X64;
uint8_t  extern OLED_SSD1306_128X64_STATUS;

void init_oled_ssd1306_128x64();

#endif
