<?php
$vt_kadi  = '***';
$vt_sifre = '***';
$vt_host  = 'localhost';
$vt_name  = 'eb_home_automation';

try
{
    $db = new PDO('mysql:host='.$vt_host.';dbname='.$vt_name.';',$vt_kadi,$vt_sifre);
    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	
    ## CHARSET ##
	$db->query("SET NAMES utf8");
	$db->query("SET CHARACTER SET utf8");
}
catch( PDOException $e )
{
    return_json(array(
        'code' => -1
	));
}

//FUNCTIONS
function return_json($data)
{
	echo json_encode($data);
	die();
}

function g($param_name, $default_val="")
{
	$ret = trim($_GET[$param_name]);

	if(!$ret && $default_val != "")
	{
		return $default_val;
	}

	return $ret;
}

function check_device_id($did)
{
	global $db;

	$query = $db->prepare("SELECT device_name FROM devices WHERE device_name=?");
	$query->execute(array($did));

	if($query->rowCount() > 0)
	{
		return true;
	}

	return false;
}
