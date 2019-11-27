#ifndef sensor_h
#define sensor_h

#include "config.h"

//--------------------------------- Variable Declarations
//---------------- Temperature Sensor
float extern LAST_TEMPERATURE;

//--------------------------------- PIN Declarations
//---------------- Temperature Sensor
//-------- LM35
struct LM35
{
    enum PIN : uint8_t
    {
        OUT = A0
    };
};

//---------------- LDR Sensor
struct LDR
{
    enum PIN : uint8_t
    {
        DATA = A0
    };

    enum INTENSITY_LEVEL : uint8_t
    {
        LOW_LIGHT  = 1,
        HIGH_LIGHT
    };
};

//---------------- PIR Sensor
struct HC_SR501
{
    enum PIN : uint8_t
    {
        OUT = D0
    };
};

int unsigned const extern HC_SR501_DELAY_MS;

//--------------------------------- Function Declarations
//---------------- Temperature Sensor
//-------- LM35
float get_temperature_lm35(bool in_fahrenheit = false);

//---------------- LDR Sensor
float get_light_intensity();
LDR::INTENSITY_LEVEL get_light_intensity_level();

//---------------- PIR Sensor
bool detect_movement(long unsigned& last_invoke_ms);

#endif
