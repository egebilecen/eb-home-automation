#include "functions.h"

//--------------------------------- Function Definations
//---------------- Temperature Sensor
void add_temperature_data(JSONVar* response_out)
{
    float temperature = get_temperature_lm35();
    int   status_code = send_get_request(API_URL, CONN_PORT, 
                                         "/ebha_api.php?add_temperature_data=1&did="+DEVICE_ID+"&val="+String(temperature), 
                                         response_out);
    
    #if DEBUG
        char const *const section_name = "add_temperature_data";

        if(status_code == 1)
        {
            int response_code = (int) (*response_out)["code"];

            if(response_code == 1)
            {
                Serial.printf("New data successfully added to DB. (%s)\n", section_name);      
            }
            else
            {
                Serial.printf("Couldn't add new data to DB. Response code is %d. (%s)\n", response_code, section_name);                    
            }
        }
        else
        {
            Serial.printf("Status code is not 1 for sending temperature data. (%s)\n", section_name);
        }
    #endif // DEBUG
}

//---------------- API
String url_decode(String str)
{
	String encodedString = "";
	char c;
	char code0;
	char code1;
	for (int i = 0; i < str.length(); i++)
	{
		c = str.charAt(i);
		if (c == '+')
		{
			encodedString += ' ';
		}
		else if (c == '%')
		{
			i++;
			code0 = str.charAt(i);
			i++;
			code1 = str.charAt(i);
			c = (_h2int(code0) << 4) | _h2int(code1);
			encodedString += c;
		}
		else
		{
			encodedString += c;
		}

		yield();
	}

	return encodedString;
}

String url_encode(String str)
{
	String encodedString = "";
	char c;
	char code0;
	char code1;
	char code2;
	for (int i = 0; i < str.length(); i++)
	{
		c = str.charAt(i);
		if (c == ' ')
		{
			encodedString += '+';
		}
		else if (isalnum(c))
		{
			encodedString += c;
		}
		else
		{
			code1 = (c & 0xf) + '0';
			if ((c & 0xf) > 9)
			{
				code1 = (c & 0xf) - 10 + 'A';
			}

			c = (c >> 4) &0xf;
			code0 = c + '0';
			if (c > 9)
			{
				code0 = c - 10 + 'A';
			}

			code2 = '\0';
			encodedString += '%';
			encodedString += code0;
			encodedString += code1;
			//encodedString+=code2;
		}

		yield();
	}

	return encodedString;

}

unsigned char _h2int(char c)
{
	if (c >= '0' && c <= '9')
	{
		return ((unsigned char) c - '0');
	}

	if (c >= 'a' && c <= 'f')
	{
		return ((unsigned char) c - 'a' + 10);
	}

	if (c >= 'A' && c <= 'F')
	{
		return ((unsigned char) c - 'A' + 10);
	}

	return (0);
}

void get_command_list(JSONVar* response_out)
{
    int status_code = send_get_request(API_URL, CONN_PORT, 
                                       "/ebha_api.php?command_list=1&did="+DEVICE_ID, 
                                       response_out);

    #if DEBUG
        char const *const section_name = "get_command_list";

        if(status_code == 1)
        {
            int response_code = (int) (*response_out)["code"];

            if(response_code == 1)
            {
                Serial.printf("Successfully fetched command list. (%s)\n", section_name);      
            }
            else
            {
                if(response_code != 0)
                {
                    Serial.printf("An error occured while trying to get command list. Response code is %d. (%s)\n", response_code, section_name);
                }
            }
        }
        else
        {
            Serial.printf("Status code is not 1 for getting command list. (%s, code: %d)\n", section_name, status_code);
        }
    #endif // DEBUG
}

bool delete_command(String const& command)
{
    JSONVar api_response;
    int status_code = send_get_request(API_URL, CONN_PORT, 
                                       "/ebha_api.php?delete_command=1&did="+DEVICE_ID+"&command="+command, 
                                       &api_response);

    #if DEBUG
        char const *const section_name = "delete_command";
    #endif

    if(status_code == 1)
    {
        int response_code = (int) api_response["code"];

        if(response_code == 1)
        {
            #if DEBUG
                Serial.printf("Command successfully deleted from DB. (%s)\n", section_name);   
            #endif

            return true;
        }
        else
        {
            #if DEBUG
                Serial.printf("Couldn't delete command from DB. Response code is %d. (%s)\n", response_code, section_name);   
            #endif
        }
    }
    else
    {
        #if DEBUG
            Serial.printf("Status code is not 1 for deleting command. (%s, code: %d)\n", section_name, status_code);
        #endif
    }

    return false;
}


bool send_notification(String const& text,
                       int    const  user_id,
                       String const& background_color,
                       String const& icon,
                       JSONVar*      response_out)
{
    int status_code = send_get_request(API_URL, CONN_PORT, 
                                       "/ebha_api.php?add_notification=1&did="+DEVICE_ID+"&user_id="+String(user_id)+"&text="+url_encode(text)+"&background="+url_encode(background_color)+"&icon="+url_encode(icon), 
                                       response_out);

    #if DEBUG
        char const *const section_name = "send_notification";
    #endif

    if(status_code == 1)
    {
        int response_code = (int) (*response_out)["code"];

        if(response_code == 1)
        {
            #if DEBUG
                Serial.printf("Notification successfully added to DB. (%s)\n", section_name);   
            #endif

            return true;
        }
        else
        {
            #if DEBUG
                Serial.printf("Couldn't add new notification to DB. Response code is %d. (%s)\n", response_code, section_name);   
            #endif
        }
    }
    else
    {
        #if DEBUG
            Serial.printf("Status code is not 1 for adding notification. (%s, code: %d)\n", section_name, status_code);
        #endif
    }

    return false;
}

//---------------- Device
bool update_settings(JSONVar* response_out)
{
    #if DEBUG
        char const *const section_name = "update_settings";
        Serial.print("Updating device settings...\n");
    #endif // DEBUG

    int status_code = send_get_request(API_URL, CONN_PORT, 
                                       "/ebha_api.php?device_settings=1&did="+DEVICE_ID, 
                                       response_out);

    if(status_code == 1)
    {
        int response_code = (int) (*response_out)["code"];

        if(response_code == 1)
        {
            int temp_sensor_work_interval = (int) (*response_out)["device_settings"]["temp_sensor_work_interval"];
            int cmd_exec_interval         = (int) (*response_out)["device_settings"]["cmd_exec_interval"];
            int movement_detection        = (int) (*response_out)["device_settings"]["movement_detection"];
            int mov_detect_interval       = (int) (*response_out)["device_settings"]["mov_detect_interval"];
        
            TEMP_SENSOR_WORK_INTERVAL    = temp_sensor_work_interval;
            CMD_EXEC_INTERVAL            = cmd_exec_interval;
            IS_MOVEMENT_DETECTION_ACTIVE = movement_detection;
            MOVEMENT_DETECTION_INTERVAL  = mov_detect_interval;

            #if DEBUG
                Serial.print("Following settings has been successfully set.\n");
                Serial.printf("Temperature Sensor Work Interval: %dms\n", temp_sensor_work_interval);
                Serial.printf("Command Execute Interval        : %dms\n", cmd_exec_interval);
                Serial.printf("Movement Detection              : %d\n",   movement_detection);
                Serial.printf("Movement Detection Interval     : %dms\n", mov_detect_interval);
            #endif

            return true;
        }
        else
        {
            #if DEBUG
                Serial.printf("An error occured while trying to get device settings. Response code is %d. (%s)\n", response_code, section_name);                    
            #endif
        }
    }
    else
    {
        #if DEBUG
            Serial.printf("Status code is not 1 for getting device settings. (%s, code: %d)\n", section_name, status_code);
        #endif
    }

    return false;
}

void command_handler(String const& command,
                     JSONVar*      response_out)
{
    if(command == CMD_UPDATE_TEMPERATURE)
    {
        add_temperature_data(response_out);
        set_last_action(ACTION_TEMPERATURE_ADD);
        send_notification(String("Anlık sıcaklık eklendi."), DEVICE_USER_ID, "", "question", response_out);
    }
    else if(command == CMD_SETTING_UPDATE)
    {
        update_settings(response_out);
        set_last_action(ACTION_SETTING_UPDATE);
        send_notification(String("Aygıt ayarları güncellendi."), DEVICE_USER_ID, "#6495ed", "cogs", response_out);
    }
    
    delete_command(command);

    #if DEBUG
        Serial.printf("Command handler for %s has ended.\n", command.c_str());
    #endif
}

void set_last_action(uint8_t action)
{
    LAST_ACTION = action;
}
