<?php
/**
* RosCMS auth plug-in for phpBB3
* Written by Colin Finck (mail@colinfinck.de) based on the DB plug-in
*/

/**
* @ignore
*/
if (!defined('IN_PHPBB'))
{
	exit;
}

/**
* Login function
*/
define(ROSCMS_DB_NAME, "roscms");

function login_roscms(&$userid, &$password)
{
	global $db, $config;
	
	// We only check the User ID here.
	if (!$userid)
	{
		return array(
			'status'	=> LOGIN_ERROR_USERNAME,
			'error_msg'	=> 'LOGIN_ERROR_USERNAME',
			'user_row'	=> array('user_id' => ANONYMOUS),
		);
	}

	// Get the phpBB ID of the user belonging to the "roscmsusrkey" cookie
	$sql = "SELECT m.map_subsys_userid " .
	       "FROM " . ROSCMS_DB_NAME . ".user_sessions s, " . ROSCMS_DB_NAME . ".users u, " . ROSCMS_DB_NAME . ".subsys_mappings m " .
	       "WHERE s.usersession_id = '" . $db->sql_escape($_COOKIE["roscmsusrkey"]) . "' AND (s.usersession_expires IS NULL OR NOW() <= s.usersession_expires) " .
	       "AND u.user_id = s.usersession_user_id AND (u.user_setting_ipaddress = 'false' OR s.usersession_ipaddress = '" . $db->sql_escape($_SERVER["REMOTE_ADDR"]) . "') " .
	       "AND (u.user_setting_browseragent = 'false' OR s.usersession_browseragent = '" . $db->sql_escape($_SERVER["HTTP_USER_AGENT"]) . "') " .
	       "AND m.map_roscms_userid = s.usersession_user_id " .
	       "AND m.map_subsys_name = 'phpbb'";
	$result = $db->sql_query($sql);
	$row = $db->sql_fetchrow($result);
	$db->sql_freeresult($result);
	
	// Verify this ID against $userid
	if(!$row || $row["map_subsys_userid"] != $userid)
	{
		return array(
			'status'	=> LOGIN_ERROR_USERNAME,
			'error_msg'	=> 'LOGIN_ERROR_USERNAME',
			'user_row'	=> array('user_id' => ANONYMOUS),
		);
	}
	
	// Now get the user information based on this ID
	$sql = "SELECT user_id, username, user_password, user_passchg, user_pass_convert, user_email, user_type, user_login_attempts " .
	       "FROM " . USERS_TABLE . " " .
	       "WHERE user_id = " . (int)$userid;
	
	$result = $db->sql_query($sql);
	$row = $db->sql_fetchrow($result);
	$db->sql_freeresult($result);

	if (!$row)
	{
		return array(
			'status'	=> LOGIN_ERROR_USERNAME,
			'error_msg'	=> 'LOGIN_ERROR_USERNAME',
			'user_row'	=> array('user_id' => ANONYMOUS),
		);
	}

	// User inactive...
	if ($row['user_type'] == USER_INACTIVE || $row['user_type'] == USER_IGNORE)
	{
		return array(
			'status'		=> LOGIN_ERROR_ACTIVE,
			'error_msg'		=> 'ACTIVE_ERROR',
			'user_row'		=> $row,
		);
	}

	// Successful login
	return array(
		'status'		=> LOGIN_SUCCESS,
		'error_msg'		=> false,
		'user_row'		=> $row,
	);
}

function validate_session_roscms(&$user)
{
	if($user["user_id"] == ANONYMOUS && $_COOKIE["roscmsusrkey"] != "")
	{
		// The user is logged in in RosCMS, but not yet in phpBB. Do that now
		login_box();
	}
	
	return true;
}

?>
