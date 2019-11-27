#ifndef request_h
#define request_h

#include "config.h"

int send_get_request(String const&       api_url, 
                     int unsigned const& port, 
                     String const        request_url,
                     JSONVar*            data_out);

#endif
