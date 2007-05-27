<?php
    /*
    RosCMS - ReactOS Content Management System
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

	// To prevent hacking activity:
	if ( !defined('ROSCMS_SYSTEM') )
	{
		die("Hacking attempt");
	}


	/*if ( !defined('ROSCMS_SYSTEM_ADMIN') ) {
		define ("ROSCMS_SYSTEM_ADMIN", "Admin Interface"); // to prevent hacking activity
	}*/

	if ($roscms_intern_usrgrp_policy_view_basic == true) { // view_basic
		if ($rpm_site == "") {
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			//create_structure($rpm_page);
		}
	

		include("inc/data_list.php");

	}
	else { // for all other user groups
		header("location:?page=nopermission");
	}
?>
