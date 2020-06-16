var API_URL   = "http://twitchchatbot.tk/ebha_api.php";
var DEVICE_ID = "d2c437e597ddbb6c56e391465b3c51c2";
var USER_ID   = null;

var DEVICE_SETTINGS_STR = '{"temp_sensor_work_interval":$temp_sensor_work_interval,"cmd_exec_interval":$cmd_exec_interval,"movement_detection":$movement_detection,"mov_detect_interval":$mov_detect_interval}';

var COMMAND_LIST = [
    {
        name : "update_temperature",
        text : "Sıcaklık verisini güncelle"
    }
];
