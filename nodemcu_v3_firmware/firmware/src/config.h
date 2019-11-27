#ifndef config_h
#define config_h

//--------------------------------- Libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>

//--------------------------------- Defines
//---------------- Debug
#define DEBUG         (0)
#define REQUEST_DEBUG (0)

//---------------- LCD
#define OLED_SCREEN (0)

//---------------- Device
// #define SDA D1
// #define SCL D2

//--------------------------------- Global Variable Declarations
//---------------- WiFi Declerations
String const extern SSID;
String const extern SSID_PASS;

//---------------- API Declerations
String   const extern API_URL;
uint32_t const extern CONN_PORT;

//---------------- Device Declerations
String const extern DEVICE_ID;
int    const extern DEVICE_USER_ID;

//-------- Action Declarations
uint8_t const extern NO_ACTION;
uint8_t const extern ACTION_TEMPERATURE_ADD;
uint8_t const extern ACTION_SETTING_UPDATE;
uint8_t const extern ACTION_LCD_UPDATE;
uint8_t       extern LAST_ACTION;

//-------- Command Declerations
String   const extern CMD_UPDATE_TEMPERATURE;
String   const extern CMD_SETTING_UPDATE;

//-------- Default Device Setting Declerations
int     unsigned extern TEMP_SENSOR_WORK_INTERVAL;
int     unsigned extern CMD_EXEC_INTERVAL;
uint8_t unsigned extern IS_MOVEMENT_DETECTION_ACTIVE;
int     unsigned extern MOVEMENT_DETECTION_INTERVAL;

#endif
