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
		$seclog_section="roscms_user_interface";
		$seclog_level="50";
		$seclog_reason="Hacking attempt: user.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}


	if ( !defined('ROSCMS_SYSTEM_USER') ) {
		define ("ROSCMS_SYSTEM_USER", "User Interface"); // to prevent hacking activity
	}

	if ($roscms_intern_usrgrp_user == true) { // only for 'user' group member
		create_head($rpm_page_title, $rpm_logo, $roscms_langres);
		create_structure($rpm_page);

		switch ($rpm_sec) {
			case "account":
				if ($rpm_sec2=="edit") {
					include("inc/user_account_edit.php"); 
				}
				elseif ($rpm_sec2=="save") {
					include("inc/user_account_edit.php"); 
				}
				else {
					include("inc/user_account.php"); 
				}
				break;
			case "profil":
				include("inc/user_profil.php"); 
				break;
			case "help":
				include("inc/user_help.php"); 
				break;
			case "accountlist": // Account List
				include("inc/user_accounts.php"); 
				break;
			case "gls": // Global Login System
				include("inc/user_gls.php"); 
				break;
			case "msg":
				include("inc/user_overview.php"); 
				//include("inc/user_msg.php"); 
				break;
			case "stats":
				include("../roswebstats/rospage_stats/admin/view_stats.php"); 
				break;
			case "overview":
			default:
				include("inc/user_overview.php"); 
				break;
		}	
	}
	else { // for all other user groups
		header("location:?page=nopermission");
	}

?>
