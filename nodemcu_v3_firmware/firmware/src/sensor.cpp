#include "sensor.h"

//--------------------------------- Variable Definations
//---------------- Temperature Sensor
float LAST_TEMPERATURE;

//--------------------------------- Function Definations
//---------------- Temperature Sensor
//-------- LM35
float get_temperature_lm35(bool in_fahrenheit)
{
    float out     = analogRead(LM35::PIN::OUT);
    float temp_c  = out * 3300 / 1024 / 10; //Celcius

    float ret_val = temp_c;

    if(in_fahrenheit)
    {
        float temp_f = (temp_c * 1.8) + 32; //Fahrenheit

        ret_val = temp_f;
    }
    
    #if DEBUG
        Serial.printf("Temperature Sensor Value: %f\n", ret_val);
    #endif // DEBUG

    LAST_TEMPERATURE = ret_val;
    return ret_val;
}

//---------------- LDR Sensor
float get_light_intensity()
{
    float data = analogRead(LDR::PIN::DATA);

    return data;
}

LDR::INTENSITY_LEVEL get_light_intensity_level()
{
    float value = get_light_intensity();

    if(value < 700)
    {
        return LDR::INTENSITY_LEVEL::LOW_LIGHT;
    }
    else
    {
        return LDR::INTENSITY_LEVEL::HIGH_LIGHT;
    }
}

//---------------- PIR Sensor
int unsigned const HC_SR501_DELAY_MS = 3000;

bool detect_movement(long unsigned& last_invoke_ms)
{
    long unsigned current_ms = millis();

    if(current_ms - last_invoke_ms >= HC_SR501_DELAY_MS)
    {
        if(digitalRead(HC_SR501::PIN::OUT) == HIGH)
        {
            last_invoke_ms = millis();
            return true;
        }
    }

    return false;
}
