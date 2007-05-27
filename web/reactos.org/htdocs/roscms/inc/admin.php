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
		$seclog_section="roscms_admin_interface";
		$seclog_level="50";
		$seclog_reason="Hacking attempt: admin.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}


	if ( !defined('ROSCMS_SYSTEM_ADMIN') ) {
		define ("ROSCMS_SYSTEM_ADMIN", "Admin Interface"); // to prevent hacking activity
	}

	if ($roscms_intern_usrgrp_admin == true) { // only for (super) admin group member
		if ($rpm_site == "") {
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			create_structure($rpm_page);
		}
	
		switch ($rpm_sec) {
			case "generator": // static page generator (output/view)
				if ($rpm_site == "") {
					echo '<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Generator</span>';
					include("inc/generate_page.php"); 
					echo "</div>";
				}
				else {
					if ($rpm_sec3 == "menu") {
						include("admin_generate_menu.php");
					}
					if ($rpm_sec3 == "menutop") {
						include("admin_generate_menu_topframe.php");
					}
					else {
						include("inc/generate_page.php"); 
					}
				}
				break;
			default:
			case "generate": // generator menu page
				include("inc/admin_generator.php"); 
				break;
			case "pages":
				if ($rpm_sec2=="view") {
					include("inc/admin_pages.php"); 
				}
				else if ($rpm_sec2=="edit") {
					include("inc/admin_pages_edit.php"); 
				}
				else if ($rpm_sec2=="delete") {
					include("inc/admin_pages_delete.php"); 
				}
				else if ($rpm_sec2=="save") {
					include("inc/admin_pages_save.php"); 
				}
				break;
			case "content":
				if ($rpm_sec2=="view") {
					include("inc/admin_content.php"); 
				}
				else if ($rpm_sec2=="edit") {
					include("inc/admin_content_edit.php"); 
				}
				else if ($rpm_sec2=="delete") {
					include("inc/admin_content.php"); 
					//include("inc/admin_content_delete.php"); 
				}
				else if ($rpm_sec2=="save") {
					include("inc/admin_content_edit.php"); 
					//include("inc/admin_content_save.php"); 
				}
				break;
			case "dyncontent":
				if ($rpm_sec2=="view") {
					include("inc/admin_dyncontent.php"); 
				}
				else if ($rpm_sec2=="edit") {
					include("inc/admin_dyncontent_edit.php"); 
				}
				/*else if ($rpm_sec2=="delete") {
					include("inc/admin_content.php"); 
					//include("inc/admin_content_delete.php"); 
				}*/
				else if ($rpm_sec2=="save") {
					include("inc/admin_dyncontent_edit.php"); 
				}
				break;
			case "inctext":
				if ($rpm_sec2=="view" || $rpm_sec2=="") {
					include("inc/admin_inctext.php"); 
				}
				else if ($rpm_sec2=="edit") {
					include("inc/admin_inctext_edit.php"); 
				}
				else if ($rpm_sec2=="delete") {
					include("inc/nopermission.php"); 
				}
				else if ($rpm_sec2=="save") {
					include("inc/admin_inctext_edit.php"); 
				}
				break;
			case "languages":
				if ($rpm_sec2=="view") {
					include("inc/admin_languages.php"); 
				}
				else if ($rpm_sec2=="edit") {
					include("inc/admin_languages_edit.php"); 
				}
				else if ($rpm_sec2=="delete") {
					include("inc/admin_languages.php"); 
					//include("inc/admin_languages_delete.php"); 
				}
				else if ($rpm_sec2=="save") {
					include("inc/admin_languages.php"); 
					//include("inc/admin_languages_save.php"); 
				}
				break;
			case "users":
				include("inc/admin_user_accounts.php"); 
				break;
			case "usergroups":
				include("inc/admin_user_groups.php"); 
				break;
			case "userhistory":
				include("inc/admin_user_history.php"); 
				break;
			case "usrgrpmbr":
				if ($rpm_sec2=="view" || $rpm_sec2=="") {
					include("inc/admin_user_group_members.php"); 
				}
				else if ($rpm_sec2=="edit") {
					include("inc/admin_user_group_members_edit.php"); 
				}
				else if ($rpm_sec2=="add") {
					include("inc/admin_user_group_members_edit.php"); 
				}
				else if ($rpm_sec2=="save") {
					include("inc/admin_user_group_members_edit.php"); 
				}
				else if ($rpm_sec2=="insert") {
					include("inc/admin_user_group_members_edit.php"); 
				}
				else if ($rpm_sec2=="delete") {
					include("inc/admin_user_group_members_edit.php"); 
				}
				break;
			case "sessions":
				include("inc/admin_user_sessions.php"); 
				break;
			case "seclog":
				if ($rpm_sec2=="view") {
					include("inc/admin_seclog.php"); 
				}
				else if ($rpm_sec2=="edit") {
					include("inc/admin_seclog_edit.php"); 
				}
				break;
			case "stats":
				include("../roswebstats/rospage_stats/admin/view_stats.php"); 
				break;
			case "help":
				include("inc/admin_help.php"); 
				break;
			case "subsys":
				include("inc/admin_subsys.php");
				break;
			case "overview":
				include("inc/admin_overview.php"); 
				break;
		}	
	}
	else { // for all other user groups
		header("location:?page=nopermission");
	}
?>
