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

	if ( !defined('ROSCMS_SYSTEM_LOG') ) {
		die("Hacking attempt");
	}

	@include("db/connect_db.inc.php"); // database connection script

	if(session_is_registered('varusr') || $_SESSION['varusr'] != "") {
		$roscms_intern_login_check_username=$_SESSION['varusr'];
		$roscms_intern_login_check_password=$_SESSION['varpass'];
	}
	else {
		$roscms_intern_login_check_username = $HTTP_COOKIE_VARS["rosname"];
		$roscms_intern_login_check_password = $HTTP_COOKIE_VARS["rospass"];
	}
	
	@$accountlogin_query = @mysql_query("SELECT usr_name, usr_roscms_passwd, usr_usergroup, usr_name_id FROM user WHERE usr_name = '".$roscms_intern_login_check_username."'") or die('DB error (login)!');
	@$accountlogin_result = @mysql_fetch_array($accountlogin_query);


	$usrgrp=$accountlogin_result['usr_usergroup'];
	
	// Usergroup:
	@$accountloginlevel_query = @mysql_query("SELECT * 
										FROM `usergroups` 
										WHERE 1 AND `usrgroup_name_id` = '$usrgrp'") or die('DB error (login)!');
	@$accountloginlevel_result = @mysql_fetch_array($accountloginlevel_query);
	
	
	// Vars for other php scripts: (only use this vars for login/account check!)
	
	// Usergroup:
	$roscms_intern_account_level=$accountloginlevel_result['usrgroup_securitylevel'];
	$roscms_intern_account_group=$accountloginlevel_result['usrgroup_name'];
	
	// User:
	$roscms_intern_account_id=$accountlogin_result['usr_name_id'];


	$roscms_intern_log_text = "<b>Collected Information:</b><br>\n  GATEWAY_INTERFACE: $GATEWAY_INTERFACE <br>\n  SERVER_NAME: $SERVER_NAME <br>\n  SERVER_SOFTWARE: $SERVER_SOFTWARE <br>\n  SERVER_PROTOCOL: $SERVER_PROTOCOL <br>\n  REQUEST_METHOD: $REQUEST_METHOD <br>\n  QUERY_STRING: $QUERY_STRING <br>\n  DOCUMENT_ROOT: $DOCUMENT_ROOT <br>\n  HTTP_ACCEPT: $HTTP_ACCEPT <br>\n  HTTP_ACCEPT_CHARSET: $HTTP_ACCEPT_CHARSET <br>\n  HTTP_ENCODING: $HTTP_ENCODING <br>\n  HTTP_ACCEPT_LANGUAGE: $HTTP_ACCEPT_LANGUAGE<br>\n  HTTP_CONNECTION: $HTTP_CONNECTION <br>\n  HTTP_HOST: $HTTP_HOST <br>\n  HTTP_REFERER: $HTTP_REFERER <br>\n  HTTP_USER_AGENT: $HTTP_USER_AGENT <br>\n	  REMOTE_ADDR: $REMOTE_ADDR <br>\n  REMOTE_PORT: $REMOTE_PORT <br>\n  SCRIPT_FILENAME: $SCRIPT_FILENAME <br>\n  SERVER_ADMIN: $SERVER_ADMIN <br>\n  SERVER_PORT: $SERVER_PORT <br>\n  SERVER_SIGNATURE: $SERVER_SIGNATURE <br>\n  PATH_TRANSLATED: $PATH_TRANSLATED <br>\n  SCRIPT_NAME: $SCRIPT_NAME <br>\n  REQUEST_URI: $REQUEST_URI <br>\n";

	@$accountloginlevel_query = @mysql_query("INSERT INTO `roscms_security_log` ( `roscms_sec_log_id` , `roscms_sec_log_section` , `roscms_sec_log_priority` , `roscms_sec_log_reason` , `roscms_sec_log_user` , `roscms_sec_log_useraccount` , `roscms_sec_log_userip` , `roscms_sec_log_referrer` , `roscms_sec_log_text` , `roscms_sec_log_date` , `roscms_sec_log_time` , `roscms_sec_log_visible` ) 
		VALUES ('', '$seclog_section', '$seclog_level', '$seclog_reason', 'roscms_system', '$roscms_intern_account_id', '$REMOTE_ADDR', '$HTTP_REFERER', '$roscms_intern_log_text', CURDATE( ) , CURTIME( ) , '1');") or die('DB error (login)!');
	@$accountloginlevel_result = @mysql_fetch_array($accountloginlevel_query);



?>