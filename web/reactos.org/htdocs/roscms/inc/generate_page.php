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

/*

	ReactOS CMS - Page Generator
	
	(c) by Klemens Friedl <frik85>
	
*/


	// To prevent hacking activity:
	if ( !defined('ROSCMS_SYSTEM') && $rpm_page != "generate_fast_secret")
	{
		die("");
	}
	
	if ($rpm_page == "generate_fast_secret") {
		$rpm_sec="generator";
		$rpm_sec2="output";
		$roscms_intern_account_id = "1";
	}
	

	if ($roscms_intern_usrgrp_admin == true || $roscms_intern_usrgrp_dev == true) { // only for (super) admin and dev group members



		set_time_limit(0);
	
		
		$rpm_gen_lang="";
		$rpm_gen_dynid="";
		if (array_key_exists("gen_lang", $_GET)) $rpm_gen_lang=htmlspecialchars($_GET["gen_lang"]);
		if (array_key_exists("gen_dynid", $_GET)) $rpm_gen_dynid=htmlspecialchars($_GET["gen_dynid"]);
	
		
		include("inc/generator.php");
	
	
		// Type
		if ($rpm_site != "") {
			$RosCMS_temp_type = "single";
		}
		else {
			if ($rpm_newcontent == "true") {
				$RosCMS_temp_type = "update";
			}
			else {
				$RosCMS_temp_type = "all";
			}
		}
	
		// Lang
		if ($rpm_gen_lang == "") {
			$RosCMS_temp_lang = "all";
		}
		else {
			$RosCMS_temp_lang = $rpm_gen_lang;
		}
	
		// Output
		if ($rpm_sec2 == "output" || $rpm_sec2 == "genpage") {
			$RosCMS_temp_output = "file";
		}
		elseif ($rpm_sec2 == "view") {
			$RosCMS_temp_output = "client";
		}
		else {
			die("output?");
		}
		
		if ($rpm_site == "") {
			$rpm_site = "index";
		}
		
		if ($rpm_gen_dynid == "") {
			$rpm_gen_dynid = 0;
		}
		

		if ($RosCMS_temp_lang == "allpages") {
			$RosCMS_temp_generator_log = "";
			$RosCMS_query_update_per_lang = mysql_query("SELECT * 
															FROM `languages` 
															ORDER BY `lang_level` DESC ;");
			while($RosCMS_result_update_per_lang = mysql_fetch_array($RosCMS_query_update_per_lang)) {
		 		$RosCMS_temp_generator_log .= "\n<hr />\n<h2>".$RosCMS_result_update_per_lang['lang_name']."</h2>\n";
				$RosCMS_temp_generator_log .= generator($RosCMS_temp_type, $RosCMS_result_update_per_lang['lang_id'], $RosCMS_temp_output, $rpm_site, $rpm_gen_dynid);
			}
			echo $RosCMS_temp_generator_log;
		}
		else {
//			echo "<hr /><p>generator(".$RosCMS_temp_type.", ".$RosCMS_temp_lang.", ".$RosCMS_temp_output.", ".$rpm_site.", ".$rpm_gen_dynid.");</p><hr />";
			echo generator($RosCMS_temp_type, $RosCMS_temp_lang, $RosCMS_temp_output, $rpm_site, $rpm_gen_dynid);
		}
		
		set_time_limit(30);
		
	}
	else {
		die("");
	}

?>
