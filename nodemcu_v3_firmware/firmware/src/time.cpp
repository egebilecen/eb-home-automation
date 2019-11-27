#include "time.h"

bool is_time_passed(int unsigned   time_ms, 
                    long unsigned& prev_ms, 
                    long unsigned& current_ms,
                    bool           set_prev_ms)
{
    if(current_ms - prev_ms >= time_ms)
    {
        if(set_prev_ms) prev_ms = current_ms;

        return true;
    }

    return false;
}

int unsigned sec_to_ms(float sec)
{
    if(sec <= 0) return 0;

    return sec * 1000;
}

int unsigned min_to_ms(float min)
{
    if(min <= 0) return 0;

    return min * 60000;
}
