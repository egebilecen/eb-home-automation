<?php
    /**
     * Return Codes:
     * -1 - Database connection error.
     * -2 - Unregistered device id.
     * -3 - An argument is not supplied OR wrong type supplied.
     * -4 - Invalid device id.
     * -5 - Unknown request.
     * 
     *  0 - An error occured during operation (unsuccessful).
     *  1 - Operation successful.
     *  2 - Device's place id is not valid. (place id < 0).
     *  3 - Same data is already registered at DB.
     *  4 - Already a device setting update is pending, wait until it finish 
     *      and try again.
     *  5 - Cannot find data.
     */

    require('ebha_config.php');

    @header('Access-Control-Allow-Origin: *');
    error_reporting(1);

    define("DEVICE_NAME", g("did", null));

    if(DEVICE_NAME == null || !check_device_id(DEVICE_NAME))
    {
        return_json(array(
            "code" => -2
        ));
    }
    else
    {
        $device_data = $db->query("SELECT * FROM devices WHERE device_name=\"".DEVICE_NAME."\"")->fetch(PDO::FETCH_ASSOC);

        define("DEVICE_ID",       $device_data["device_id"]);
        define("DEVICE_PLACE_ID", $device_data["device_place_id"]);
        define("DEVICE_DESC",     $device_data["device_desc"]);
        define("DEVICE_SETTINGS", $device_data["device_settings"]);
    }

    # Login request
    if(g("login"))
    {
        $username = g("username");
        $password = g("password");

        if(!$username || !$password)
        {
            return_json(array("code" => 0));
        }

        $query = $db->query("SELECT user_id FROM users WHERE user_name=\"".$username."\" && user_password=\"".md5($password)."\"");

        if($query->rowCount() > 0)
        {
            return_json(array(
                "code" => 1,
                "user_id" => $query->fetch(PDO::FETCH_ASSOC)["user_id"]
            ));
        }
        else
        {
            return_json(array("code" => 0));
        }
    }
    
    # Temperature sensor data request
    else if(g("temp_sensor_data"))
    {
        $place = g("place");
        $limit = g("limit");

        if(!$place) $place = 0;
        if(!$limit) $limit = 5;
        
        $place_data = $db->query("SELECT * FROM places WHERE place_id=\"".$place."\"")->fetch(PDO::FETCH_ASSOC);

        $data = array_reverse($db->query("SELECT * FROM sensor_temperature_data WHERE sensor_temperature_data_place_id=".$place." ORDER BY sensor_temperature_data_id DESC LIMIT ".$limit)->fetchAll(PDO::FETCH_ASSOC));
        $data_array = array(
            "code" => 1,
            "sensor_data" => array(),
            "place_name"  => $place_data["place_name"]
        );

        for($i=0; $i < count($data); $i++)
        {
            $row = $data[$i];

            $full_hour = explode(" ", $row["sensor_temperature_data_time"])[1];

            $hour   = explode(":", $full_hour)[0];
            $minute = explode(":", $full_hour)[1];

            array_push($data_array["sensor_data"], array(
                "value" => $row["sensor_temperature_data_value"],
                "time"  => $hour.":".$minute
            ));
        }

        return_json($data_array);
    }
    
    # Place list request
    else if(g("place_list"))
    {
        $query = $db->query("SELECT * FROM places")->fetchAll(PDO::FETCH_ASSOC);

        return_json(array(
            "code" => 1,
            "place_list" => $query
        ));
    }

    # Device list request
    else if(g("device_list"))
    {
        $query  = $db->query("SELECT devices.device_id, devices.device_desc, places.place_name FROM devices INNER JOIN places ON devices.device_place_id = places.place_id WHERE device_place_id != 0")->fetchAll(PDO::FETCH_ASSOC);

        return_json(array(
            "code" => 1,
            "device_list" => $query
        ));
    }

    # Add temperature sensor data request
    else if(g("add_temperature_data"))
    {
        if(DEVICE_PLACE_ID < 1)
        {
            return_json(array("code" => 2));
        }

        $sensor_value = g("val");

        $query = $db->query("INSERT INTO sensor_temperature_data SET sensor_temperature_data_id=NULL,
                                                                     sensor_temperature_data_place_id=\"".$device_data["device_place_id"]."\",
                                                                     sensor_temperature_data_value=\"".$sensor_value."\",
                                                                     sensor_temperature_data_time=CURRENT_TIMESTAMP");

        if($db->lastInsertId())
        {
            return_json(array("code" => 1));
        }

        return_json(array("code" => 0));
    }

    # Get command list for given device id
    else if(g("command_list"))
    {
        $query = $db->query("SELECT * FROM commands WHERE command_device_id=\"".DEVICE_ID."\"");

        if($query->rowCount() > 0)
        {
            $db_data = $query->fetchAll(PDO::FETCH_ASSOC);
            $commands = array();
            
            for($i=0; $i < count($db_data); $i++)
            {
                array_push($commands, $db_data[$i]["command_name"]);
            }

            return_json(array(
                "code" => 1,
                "commands" => $commands,
                "command_count" => count($commands)
            ));
        }

        return_json(array("code" => 0));
    }

    # Get ALL commands
    else if(g("all_command_list"))
    {
        $query = $db->query("SELECT commands.command_name, devices.device_id, devices.device_desc, places.place_name FROM commands INNER JOIN devices ON commands.command_device_id = devices.device_id INNER JOIN places ON devices.device_place_id = places.place_id WHERE commands.command_name != \"setting_update\"")->fetchAll(PDO::FETCH_ASSOC);

        return_json(array(
            "code" => 1,
            "command_list" => $query
        ));
    }

    # Add command
    else if(g("add_command"))
    {
        $device_id = g("device_id");
        $command   = g("command");

        if(!$command || !$device_id) return_json(array("code" => -3));

        $_query = $db->query("SELECT * FROM commands WHERE command_name=\"".$command."\" && command_device_id=\"".$device_id."\"");

        if($_query->rowCount() < 1)
        {
            $query = $db->query("INSERT INTO commands SET command_id=NULL, 
            command_name=\"".$command."\", 
            command_device_id=\"".$device_id."\", 
            command_time=CURRENT_TIMESTAMP");

            if($db->lastInsertId())
            {
                return_json(array("code" => 1));
            }

            return_json(array("code" => 0)); 
        }

        return_json(array("code" => 3));
    }

    # Delete command
    else if(g("delete_command"))
    {
        $device_id = g("device_id");
        $command   = g("command");

        if(!$device_id) $device_id = DEVICE_ID;
        if(!$command) return_json(array("code" => -3));

        $query = $db->query("DELETE FROM commands WHERE command_name=\"".$command."\" && command_device_id=\"".$device_id."\"");

        if($query->rowCount() > 0)
        {
            return_json(array("code" => 1));
        }

        return_json(array("code" => 0));
    }

    # Device settings
    else if(g("device_settings"))
    {
        $device_id = g("device_id");

        if(!$device_id)
        {
            return_json(array(
                "code"     => 1,
                "device_settings" => json_decode(DEVICE_SETTINGS)
            ));
        }

        $query = $db->query("SELECT device_settings FROM devices WHERE device_id=\"".$device_id."\"");

        if($query->rowCount() > 0)
        {
            $data = $query->fetch(PDO::FETCH_ASSOC);

            return_json(array(
                "code" => 1,
                "device_settings" => json_decode($data["device_settings"])
            ));
        }

        return_json(array("code" => 0));
    }
    
    # Set device settings
    else if(g("set_device_settings"))
    {

        $device_id = g("device_id");
        $settings  = g("settings");

        if(!$device_id || json_decode($settings) == null) return_json(array("code" => -3));
        
        $_query = $db->query("SELECT * FROM commands WHERE command_device_id=\"".$device_id."\" && command_name=\"setting_update\"");
        
        if($_query->rowCount() > 0)
        {
            return_json(array("code" => 4));
        }

        $query = $db->prepare("UPDATE devices SET device_settings=? WHERE device_id=?");
        $query->execute(array($settings, $device_id));

        if($query->rowCount() > 0)
        {
            $query = $db->query("INSERT INTO commands SET command_id=NULL, 
                                                          command_name=\"setting_update\", 
                                                          command_device_id=\"".$device_id."\", 
                                                          command_time=CURRENT_TIMESTAMP");

            if($db->lastInsertId())
            {
                return_json(array("code" => 1));
            }
        }

        return_json(array("code" => 0));
    }

    # Unreaded notifications
    else if(g("unreaded_notifications"))
    {
        $user_id = g("user_id");

        if(!$user_id) return_json(array("code" => -3));

        $query = $db->query("SELECT notification_id, notification_text, notification_icon, notification_date, notification_background FROM notifications WHERE notification_user_id='".$user_id."' && notification_readed='0' ORDER BY notification_id DESC")->fetchAll(PDO::FETCH_ASSOC);

        return_json(array(
            "code" => 1,
            "unreaded_notifications" => $query
        ));
    }

    # Unreaded notifications
    else if(g("notification_readed"))
    {
        $user_id         = g("user_id");
        $notification_id = g("notification_id");

        if(!$user_id || !$notification_id) return_json(array("code" => -3));

        $query = $db->query("SELECT * FROM notifications WHERE notification_user_id='".$user_id."' && notification_id='".$notification_id."'");

        if($query->rowCount() > 0)
        {
            $query2 = $db->query("UPDATE notifications SET notification_readed='1' WHERE notification_id='".$notification_id."'");

            if($query2->rowCount() > 0)
            {
                return_json(array("code" => 1));
            }

            return_json(array("code" => 0));
        }

        return_json(array("code" => 5));
    }

    # All notifications
    else if(g("all_notifications"))
    {
        $user_id = g("user_id");

        if(!$user_id) return_json(array("code" => -3));
        
        $query = $db->query("SELECT * FROM notifications WHERE notification_user_id='".$user_id."' ORDER BY notification_id DESC");
        
        return_json(array(
            "code" => 1,
            "notification_list" => $query->fetchAll(PDO::FETCH_ASSOC)
        ));
    }

    # Delete notification
    else if(g("delete_notification"))
    {
        $user_id         = g("user_id");
        $notification_id = g("notification_id");

        if(!$user_id || !$notification_id) return_json(array("code" => -3));

        $query = $db->query("DELETE FROM notifications WHERE notification_id='".$notification_id."' && notification_user_id='".$user_id."'");
    
        if($query->rowCount() > 0)
        {
            return_json(array("code" => 1));
        }

        return_json(array("code" => 0));
    }

    # Delete all notifications
    else if(g("delete_all_notifications"))
    {
        $user_id = g("user_id");

        if(!$user_id) return_json(array("code" => -3));

        $query = $db->query("DELETE FROM notifications WHERE notification_user_id='".$user_id."'");
    
        if($query->rowCount() > 0)
        {
            return_json(array("code" => 1));
        }

        return_json(array("code" => 0));
    }

    # Add notification()
    else if(g("add_notification"))
    {
        if(DEVICE_ID < 1) return_json(array("code" => -4));

        $text       = g("text");
        $user_id    = g("user_id");
        $background = g("background");
        $icon       = g("icon");

        if(!$text || !$user_id) return_json(array("code" => -3));

        if(!$background) $background = "DEFAULT"; else $background = "'".$background."'";
        if(!$icon)       $icon       = "DEFAULT"; else $icon       = "'".$icon."'";

        $query = $db->query("INSERT INTO notifications SET notification_id=NULL,
                                                           notification_text='".$text."',
                                                           notification_device_id='".DEVICE_ID."',
                                                           notification_user_id='".$user_id."',
                                                           notification_background=".$background.",
                                                           notification_icon=".$icon.",
                                                           notification_readed='0',
                                                           notification_date=CURRENT_TIMESTAMP");

        if($db->lastInsertId())
        {
            return_json(array("code" => 1)); 
        }

        return_json(array("code" => 0));
    }

    # Unknown request
    else
    {
        return_json(array("code" => -5));
    }
?>
