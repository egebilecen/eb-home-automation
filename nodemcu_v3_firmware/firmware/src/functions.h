#ifndef functions_h
#define functions_h

#include "config.h"
#include "request.h"
#include "sensor.h"

//--------------------------------- Function Declerations
//---------------- Temperature Sensor
void add_temperature_data(JSONVar* response_out);

//---------------- API
String url_decode(String str);
String url_encode(String str);
char unsigned _h2int(char c);

void get_command_list(JSONVar* response_out);
bool delete_command(String const& command);
bool send_notification(String const& text,
                       int    const  user_id,
                       String const& background_color,
                       String const& icon,
                       JSONVar*      response_out);

//---------------- Device
bool update_settings(JSONVar* response_out);
void command_handler(String const& command,
                     JSONVar*      response_out);
void set_last_action(uint8_t action);

#endif
