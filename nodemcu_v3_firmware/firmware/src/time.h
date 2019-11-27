#ifndef time_h
#define time_h 

bool is_time_passed(int unsigned   time_ms, 
                    long unsigned& prev_ms, 
                    long unsigned& current_ms,
                    bool           set_prev_ms = false);

int unsigned sec_to_ms(float sec);

int unsigned min_to_ms(float min);

#endif
