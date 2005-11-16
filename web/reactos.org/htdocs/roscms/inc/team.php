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
		$seclog_reason="Hacking attempt: team.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}


	if ( !defined('ROSCMS_SYSTEM_Team') ) {
		define ("ROSCMS_SYSTEM_Team", "Team Interface"); // to prevent hacking activity
	}

	if ($roscms_intern_usrgrp_team == true) { // only for developer group member
		create_head($rpm_page_title, $rpm_logo, $roscms_langres);
		create_structure($rpm_page);

		switch ($rpm_sec) {
			case "content":
				if ($rpm_sec2=="view") {
					include("inc/admin_content.php"); 
				}
				else if ($rpm_sec2=="edit") {
					include("inc/admin_content_edit.php"); 
				}
				else if ($rpm_sec2=="delete") {
					include("inc/admin_content.php"); 
				}
				else if ($rpm_sec2=="save") {
					include("inc/admin_content.php"); 
					//include("inc/admin_content_edit.php"); 
				}
				break;
			case "stats":
				include("../roswebstats/rospage_stats/admin/view_stats.php"); 
				break;
			case "help":
				echo "<br><p>under construction ...</p>";
				break;
			case "overview":
			default:
				include("inc/team_overview.php"); 
				break;
		}	
	}
	else { // for all other user groups
		header("location:?page=nopermission");
	}
	
?>
