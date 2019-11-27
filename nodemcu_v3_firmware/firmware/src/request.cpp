#include "request.h"

int send_get_request(String const&       api_url, 
                     int unsigned const& port, 
                     String const        request_url,
                     JSONVar*            data_out)
{
    WiFiClient client;

    // Connect to API.
    #if DEBUG && REQUEST_DEBUG
        Serial.printf("Connecting to API. (%s)\n", api_url.c_str());
    #endif // DEBUG
    
    if(client.connect(api_url, port))
    {
        #if DEBUG && REQUEST_DEBUG
            Serial.printf("Sending get request to %s.\n", request_url.c_str());
        #endif // DEBUG

        // Request data from API.
        client.print(
            String("GET ") + request_url + " HTTP/1.1\r\n"  +
            "Host: " + api_url + "\r\n"                     +
            "Connection: close\r\n"                         +
            "\r\n"
        );

        String response = String("");
        while(client.connected() || client.available())
        {
            if(client.available())
            {
                String res = client.readStringUntil('\n');

                if(res.indexOf("\"code\"") > 0)
                {
                    response += res;
                }
            }
        }

        #if DEBUG && REQUEST_DEBUG
            Serial.print("Response from API:\n");
            Serial.println(response);
        #endif // DEBUG

        if(data_out != nullptr)
        {
            *data_out = JSON.parse(response);

            // Couldn't parse JSON data.
            if(JSON.typeof(*data_out) == "undefined")
            {
                #if DEBUG && REQUEST_DEBUG
                    Serial.print("Couldn't parse JSON string.\n");
                #endif // DEBUG

                return -1;
            }
        }

        client.stop();
        return 1;
    }
    else
    {
        #if DEBUG && REQUEST_DEBUG
            Serial.print("Cannot connect to API.\n");
        #endif // DEBUG

        client.stop();
        return 0;
    }
}
