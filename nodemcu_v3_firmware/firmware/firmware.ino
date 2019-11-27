/*
    EB Home Automation - NodeMCU (v3) Firmware
    Author : Ege Bilecen
    Date   : 28.11.2019 - 00.09
    Version: Work in progress
*/

//--------------------------------- Includes
#include "src/config.h"
#include "src/time.h"
#include "src/functions.h"

//--------------------------------- Variables
//---------------- API
JSONVar api_response;

//---------------- Millis
long unsigned temp_sensor_prev_ms = 0;
long unsigned command_prev_ms     = 0;
long unsigned hc_sr501_prev_ms    = 0;
long unsigned mov_detect_prev_ms  = 0;

void setup()
{
    // Configure pins.
    pinMode(LM35::PIN::OUT,     INPUT);
    pinMode(HC_SR501::PIN::OUT, INPUT);

    // Open serial port.
    #if DEBUG
        Serial.begin(115200);
    #endif // DEBUG

    // Connect to wifi.
    #if DEBUG
        Serial.printf("\nConnecting to %s.", SSID.c_str());
    #endif // DEBUG
    WiFi.begin(SSID, SSID_PASS);

    while(WiFi.status() != WL_CONNECTED)
    {
        #if DEBUG
            Serial.print(".");
        #endif // DEBUG
        delay(500);
    }

    #if DEBUG
        Serial.printf("\nSuccessfully connected to %s.\n\n", SSID.c_str());
    #endif // DEBUG

    update_settings(&api_response);

    #if OLED_SCREEN
        #if DEBUG
            Serial.print("Initializing OLED...\n");
        #endif // DEBUG

        init_oled_ssd1306_128x64();
    #endif // OLED_SCREEN

    #if DEBUG
        Serial.print("\n");
    #endif // DEBUG

    send_notification(String("Aygıt başlatıldı."), DEVICE_USER_ID, "#ff8300", "microchip", &api_response);
}

void loop()
{
    //--------------------------------- Variabls
    //---------------- Timing Variables
    long unsigned current_ms = millis();

    //--------------------------------- Main
    //---------------- Timed Controls
    // Send temperature sensor data
    if(is_time_passed(TEMP_SENSOR_WORK_INTERVAL, temp_sensor_prev_ms, current_ms))
    {
        add_temperature_data(&api_response);

        set_last_action(ACTION_TEMPERATURE_ADD);
        temp_sensor_prev_ms = millis();
    }

    // Check and do commands
    else if(is_time_passed(CMD_EXEC_INTERVAL, command_prev_ms, current_ms))
    {
        #if DEBUG
            Serial.print("Getting new command(s).\n");
        #endif // DEBUG
        get_command_list(&api_response);

        int command_count = (int) api_response["command_count"];

        if(command_count > 0)
        {
            #if DEBUG
                Serial.print("New command(s) found. Processing...\n");
            #endif // DEBUG

            for(int i=0; i < command_count; i++)
            {
                String const command = String((char const*) api_response["commands"][i]);

                #if DEBUG
                    Serial.printf("Running command handler for %s.\n", command.c_str());
                #endif // DEBUG

                command_handler(command, &api_response);
            }
        }
        else
        {
            #if DEBUG
                Serial.print("No new command detected. Skipping...\n");
            #endif // DEBUG
        }

        //set last invoke time
        command_prev_ms = millis();
    }

    else
    {
        set_last_action(NO_ACTION);
    }
    //---------------- End of Timed Controls
    //---------------- Continous Controls
    if(IS_MOVEMENT_DETECTION_ACTIVE)
    {
        if(is_time_passed(MOVEMENT_DETECTION_INTERVAL, mov_detect_prev_ms, current_ms) || mov_detect_prev_ms == 0)
        {
            if(detect_movement(hc_sr501_prev_ms))
            {
                #if DEBUG
                    Serial.print("Movement is detected at place. Sending notification to user.\n");
                #endif // DEBUG

                send_notification(String("Hareket tespit edildi!"), DEVICE_USER_ID, "#fa4251", "", &api_response);
                
                //set last invoke time
                mov_detect_prev_ms = millis();
            }
        }
    }
    //---------------- End of Continous Controls
    //--------------------------------- End of Main

    if(LAST_ACTION != NO_ACTION)
    {
        #if DEBUG
            Serial.print("\n");
        #endif // DEBUG

        #if OLED_SCREEN
            if(LAST_ACTION == ACTION_TEMPERATURE_ADD)
            {
                if(OLED_SSD1306_128X64_STATUS)
                {
                    #if DEBUG
                        Serial.print("Temperature data has changed. Updating OLED...\n");
                    #endif // DEBUG

                    OLED_SSD1306_128X64.clrScr();
                    OLED_SSD1306_128X64.print("Sıcaklık: "+String(LAST_TEMPERATURE)+"C", LEFT, 0);
                    OLED_SSD1306_128X64.update();

                    set_last_action(ACTION_LCD_UPDATE);
                }
            }
        #endif // OLED_SCREEN
    }
}
