#include "config.h"
#include "time.h"

//--------------------------------- Global Variable Definations
//---------------- WiFi Definations
String   const SSID      = "xxx";
String   const SSID_PASS = "xxx";

//---------------- API Definations
String   const API_URL   = "xxx.tk";
uint32_t const CONN_PORT = 80;

//---------------- Device Definations
String const DEVICE_ID      = "xxx";
int    const DEVICE_USER_ID = 1;

//-------- Action Definations
uint8_t const NO_ACTION              = 0;
uint8_t const ACTION_TEMPERATURE_ADD = 1;
uint8_t const ACTION_SETTING_UPDATE  = 2;
uint8_t const ACTION_LCD_UPDATE      = 3;
uint8_t       LAST_ACTION            = NO_ACTION;

//-------- Command Definations
String   const CMD_UPDATE_TEMPERATURE = "update_temperature";
String   const CMD_SETTING_UPDATE     = "setting_update";

//-------- Default Device Setting Definations
int     unsigned TEMP_SENSOR_WORK_INTERVAL    = min_to_ms(10.f); //10 min
int     unsigned CMD_EXEC_INTERVAL            = sec_to_ms(10.f); //10 sec
uint8_t unsigned IS_MOVEMENT_DETECTION_ACTIVE = 0;
int     unsigned MOVEMENT_DETECTION_INTERVAL  = min_to_ms(5.f);  //5  min
