<?php
    /*
    RosCMS - ReactOS Content Management System
    Copyright (C) 2005  Klemens Friedl <frik85@reactos.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
    */

	// To prevent hacking activity:
	if ( !defined('ROSCMS_SYSTEM') )
	{
		if ( !defined('ROSCMS_SYSTEM_LOG') ) {
			define ("ROSCMS_SYSTEM_LOG", "Hacking attempt");
		}
		$seclog_section="roscms_interface";
		$seclog_level="70";
		$seclog_reason="Hacking attempt: INCLUDE_login.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		require('securitylog.php'); // open security log
		die("Hacking attempt");
	}

	
	require_once("subsys_login.php");

	$check_ip = true; // check the IP address; avoid this setting if the user is behind a proxy or use more than one pc the same time (a possible security risk, but some persons wanted that behavior ...); Note: this is a per user setting, everyone can change it!
	$do_time_out = true; // session_time enabled
	$session_time = 30; // time-out in minutes
	$luser_tries = 1; // fresh session key will be created after x failed password attempts
	$big_luser = 10; // after x attemps, die(); maybe a brute force script or a script kiddy
	
	
	$is_member = false;
	$random_string = '';
	$address_is_good = false;
	$agent_is_good = false;
	if($check_ip == false) {
		$address_is_good = true;
	}
	$roscms_intern_login_check = "";
	$roscms_intern_login_check_usrgroup = "";
	$roscms_intern_account_group = "";
	$roscms_intern_account_level = "";

	
	// time-out calculation
	$time = '';
	$time_out = false;
	//if($do_time_out == true) {
	
	// 1/100th of a sec
	$real_session_time = $session_time * 6000;
	// timestamp
	$now = explode(' ',microtime());
	$time = $now[1].substr($now[0],2,2);
	settype($time, "double");
	
	
	// check time-out and if no key has been found -> new radom key for the user
	if (isset($_SESSION[$roscms_login_cookie_roscmsusrkey])) {
		echo "<br>ID ".$_SESSION[$roscms_login_cookie_roscmsusrkey];
		$random_string = $_SESSION[$roscms_login_cookie_roscmsusrkey];
		$query_sesid = mysql_query("<br><br>SQL:<br>SELECT usersession_id, usersession_userid, usersession_timestamp_touch, usersession_browseragent, usersession_ipaddress FROM user_sessions WHERE usersession_id = '".$random_string."'"); // or die('DB error (login script)!');
		$result_sesid = mysql_fetch_array($query_sesid); // or die("Your session id has been deleted, oops.<br><br>To solve this problem use the <a href='?page=logout'>logout function</a>!");
		
		if ($result_sesid['usersession_id'] != "") {
			$roscms_currentuser_id=$result_sesid['usersession_userid'];
			echo "<br>ID_FROM DB: ".$roscms_currentuser_id;
			echo "SELECT user_id, user_name, user_rosAcms_password, user_timestamp_touch FROM users WHERE user_id = '".$roscms_currentuser_id."'";
			$query_usr = mysql_query("SELECT user_id, user_name, user_rosAcms_password, user_timestamp_touch FROM users WHERE user_id = '".$roscms_currentuser_id."'"); // or die('DB error (login script)!');
			$result_usr = mysql_fetch_array($query_usr); // or die('DB error (login script)');
			echo "<br>USERNAME: ".$result_usr['user_name']."<br>";
			
			if ($result_usr['user_name'] != "") {
				$roscms_currentuser_latest_touch=$result_usr['user_timestamp_touch'];
				if($roscms_currentuser_latest_touch < ($time - $real_session_time)) {
					$login_usr_key_del_post="DELETE FROM `user_sessions` WHERE `usersession_id` = '$random_string' LIMIT 1 ;";
					$login_usr_key_post_del_list=mysql_query($login_usr_key_del_post);
					
					echo "(DB) session time-out!<br>";
					$time_out = true;
				}
				else { // update the 'timestamp'; login okay :-)
					$login_usr_key_post="UPDATE `users` SET `user_timestamp_touch` = '$time' WHERE `user_id` = '$roscms_currentuser_id' LIMIT 1 ;";
					$login_usr_key_post_list=@mysql_query($login_usr_key_post);
//					echo "Session_ID: ".$random_string."<br>time-out counter: ".($roscms_currentuser_latest_touch-($time - $real_session_time))." in 1/100th of a sec (e.g. 3000 = 30 sec)";
					$is_member = true;
				}
			}
			else { // only hacker/cracker!/script kiddy/brute force script/bot/etc. are able to reach this part
				die("(DB) no valid user session found!<br><br>You are a hacker/cracker!/script kiddy/brute force script/bot/etc.<br><br>RosCMS does not like you, anymore! Your ip address, etc. has been saved and you are on the watch list!");
				$time_out = true;
				// TODO: add it to the watchlist!
			}
		}
	}
	else { // a new visitor
		//die("NOT SESSION_ID FOUND! (login script)<br><br><a href='?page=login'>Login page</a>");
		$is_member = false;
	}
	
	
	
	if ((isset($_POST['logout'])) || ($time_out == true)) {
		//session_unset();
		header("location:?page=logout");
	}
	
	
	// check their IP address..
	if ((isset($_SESSION['remote_addr'])) && ($_SERVER['REMOTE_ADDR'] == $_SESSION['remote_addr'])) {
		$address_is_good = true;
	}
	else {
		$_SESSION['remote_addr'] = $_SERVER['REMOTE_ADDR'];
	}
	
	// check their user agent..
	if ((isset($_SESSION['agent'])) && ($_SERVER['HTTP_USER_AGENT'] == $_SESSION['agent'])) {
		$agent_is_good = true;
	}
	else {
		$_SESSION['agent'] = $_SERVER['HTTP_USER_AGENT'];
	}

?>  