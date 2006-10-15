<?php
    /*
    RosCMS - ReactOS Content Management System
    Copyright (C) 2005  Ge van Geldorp <gvg@reactos.org>

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
	$seclog_reason="Hacking attempt: logout.php";
	define ("ROSCMS_SYSTEM", "Hacking attempt");
	require('securitylog.php'); // open security log
	die("Hacking attempt");
}

require_once("utils.php");

if (isset($_COOKIE['roscmsusrkey'])) 
{
	$del_session_id = $_COOKIE['roscmsusrkey'];
	setcookie("roscmsusrkey", "", time() - 3600, "/", cookie_domain());
	$logout_usr_key_post = "DELETE FROM user_sessions " .
	                       " WHERE usersession_id = '" .
	                               mysql_escape_string($del_session_id) .
	                               "'";
	$logout_usr_key_post_list = mysql_query($logout_usr_key_post)
	                              or die("DB error (logout)!");
}


$target = $_SERVER[ 'PHP_SELF' ];
if ( IsSet( $_SERVER[ 'QUERY_STRING' ] ) ) {
	$target .= '?' . $_SERVER[ 'QUERY_STRING' ];
}
if (isset($_REQUEST['target'])) {
	header("Location: http://" . $_SERVER['HTTP_HOST'] .
                       $_REQUEST['target']);
		exit;
}

header("Location: /?page=index");

exit;

?>
