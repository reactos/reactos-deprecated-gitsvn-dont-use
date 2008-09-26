<?php
    /*
    ReactOS DynamicFrontend (RDF)
    Copyright (C) 2007  Klemens Friedl <frik85@reactos.org>

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
	
	if ( !defined('ROSCMS_LOGIN') )
	{
		die("Hacking attempt");
	}
		
	require_once("logon/subsys_login.php");
	
	$rdf_user_id = roscms_subsys_login('', ROSCMS_LOGIN_OPTIONAL, "");
	
	if ($rdf_user_id == 0) {
		$rdf_user_id = 0;
		$rdf_user_name = "Anonymous Guest";
		$rdf_user_level = 0; // 0 = guest, 1 = registered user, 2 = membership, 3 = moderator, 4 = author, 5 = admin
		$rdf_user_timezone = 0;
		$rdf_user_timezone_name = "UTC";
	}
	else {
		$sql_user = "SELECT u.user_id, u.user_name, u.user_level, u.user_timezone, z.tz_code, z.tz_value  
						FROM users u, user_timezone z
						WHERE u.user_id = ".mysql_real_escape_string($rdf_user_id)."
						AND u.user_account_enabled LIKE 'yes'
						AND u.user_timezone = z.tz_code 
						LIMIT 1;";
		$query_user = mysql_query($sql_user);
		$result_user = mysql_fetch_array($query_user);
		
		if ($result_user['user_id'] != "" && $result_user['user_id'] > 1 && $result_user['user_level'] >= 1 && $result_user['user_level'] <= 5) {
			$rdf_user_id = $result_user['user_id'];
			$rdf_user_name = $result_user['user_name'];
			$rdf_user_level = $result_user['user_level'];
			$rdf_user_timezone = $result_user['tz_value'];
			$rdf_user_timezone_name = $result_user['tz_code'];
		}
		else {
			$rdf_user_id = 0;
			$rdf_user_name = "Anonymous Guest";
			$rdf_user_level = 0;
			$rdf_user_timezone = 0;
			$rdf_user_timezone_name = "UTC";
		}
	}
	
	


?>