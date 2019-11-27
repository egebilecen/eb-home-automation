#include "lcd.h"

//--------------------------------- Definations for SSD1306 128x64 OLED
OLED    OLED_SSD1306_128X64(SDA, SCL);
uint8_t OLED_SSD1306_128X64_STATUS = 0;

void init_oled_ssd1306_128x64()
{
    if(!OLED_SSD1306_128X64.begin(SSD1306_128X64))
    {
        OLED_SSD1306_128X64_STATUS = 0;

        #if DEBUG
            Serial.print("Failed to allocate enough RAM for display buffer. (init_oled_ssd1306_128x64)");
        #endif // DEBUG

        return;
    }

    OLED_SSD1306_128X64.setFont(SmallFont);

    OLED_SSD1306_128X64_STATUS = 1;
}
