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
	
	global $roscms_intern_account_id;
	
	$RosCMS_GET_d_format = ""; // data export format (xml, text)
	$RosCMS_GET_d_use = ""; // data usage (where the data will be used)
	$RosCMS_GET_d_filter = ""; // data filters string
	$RosCMS_GET_d_filter2 = ""; // data filters string
	$RosCMS_GET_d_curpos = ""; // data current pointer position (for table: "page 2 of 55")
	$RosCMS_GET_d_flag = ""; // data flag
	$RosCMS_GET_d_arch = ""; // data archive flag, means that the data is stored in the archive tables
	$RosCMS_GET_d_value = ""; // data transport value
	$RosCMS_GET_d_value2 = ""; // data transport value
	$RosCMS_GET_d_value3 = ""; // data transport value
	$RosCMS_GET_d_value4 = ""; // data transport value
	$RosCMS_GET_d_value5 = ""; // data transport value
	
	$RosCMS_GET_d_id = ""; // data_id
	$RosCMS_GET_d_name = ""; // data_name (e.g. "about")
	$RosCMS_GET_d_type = ""; // data_type (e.g. "page", "content", etc.)
	$RosCMS_GET_d_r_id = ""; // data rev id
	$RosCMS_GET_d_r_ver = ""; // data rev version
	$RosCMS_GET_d_r_lang = ""; // data rev language (e.g. "en", "de", etc.)
	$RosCMS_GET_d_r_usr = ""; // data rev user-id
	$RosCMS_GET_d_template = ""; // data template


	
	
	if (array_key_exists("d_f", $_GET)) $RosCMS_GET_d_format=htmlspecialchars($_GET["d_f"]);
	if (array_key_exists("d_u", $_GET)) $RosCMS_GET_d_use=htmlspecialchars($_GET["d_u"]);
	if (array_key_exists("d_filter", $_GET)) $RosCMS_GET_d_filter=htmlspecialchars($_GET["d_filter"]);
	if (array_key_exists("d_filter2", $_GET)) $RosCMS_GET_d_filter2=htmlspecialchars($_GET["d_filter2"]);
	if (array_key_exists("d_cp", $_GET)) $RosCMS_GET_d_curpos=htmlspecialchars($_GET["d_cp"]);
	if (array_key_exists("d_fl", $_GET)) $RosCMS_GET_d_flag=htmlspecialchars($_GET["d_fl"]);
	if (array_key_exists("d_arch", $_GET)) $RosCMS_GET_d_arch=htmlspecialchars($_GET["d_arch"]);
	if (array_key_exists("d_val", $_GET)) $RosCMS_GET_d_value=htmlspecialchars($_GET["d_val"]);
	if (array_key_exists("d_val2", $_GET)) $RosCMS_GET_d_value2=htmlspecialchars($_GET["d_val2"]);
	if (array_key_exists("d_val3", $_GET)) $RosCMS_GET_d_value3=htmlspecialchars($_GET["d_val3"]);
	if (array_key_exists("d_val4", $_GET)) $RosCMS_GET_d_value4=htmlspecialchars($_GET["d_val4"]);
	if (array_key_exists("d_val5", $_GET)) $RosCMS_GET_d_value5=htmlspecialchars($_GET["d_val5"]);

	if (array_key_exists("d_id", $_GET)) $RosCMS_GET_d_id=htmlspecialchars($_GET["d_id"]);
	if (array_key_exists("d_name", $_GET)) $RosCMS_GET_d_name=htmlspecialchars($_GET["d_name"]);
	if (array_key_exists("d_type", $_GET)) $RosCMS_GET_d_type=htmlspecialchars($_GET["d_type"]);
	if (array_key_exists("d_r_id", $_GET)) $RosCMS_GET_d_r_id=htmlspecialchars($_GET["d_r_id"]);
	if (array_key_exists("d_r_ver", $_GET)) $RosCMS_GET_d_r_ver=htmlspecialchars($_GET["d_r_ver"]);
	if (array_key_exists("d_r_lang", $_GET)) $RosCMS_GET_d_r_lang=htmlspecialchars($_GET["d_r_lang"]);
	if (array_key_exists("d_r_usr", $_GET)) $RosCMS_GET_d_r_usr=htmlspecialchars($_GET["d_r_usr"]);
	if (array_key_exists("d_template", $_GET)) $RosCMS_GET_d_template=htmlspecialchars($_GET["d_template"]);
	

	
	switch ($RosCMS_GET_d_format) {
		default:
			die("");
			break;
		case "xml":
			include("inc/data_export_xml.php");
			break;
		case "text":
			include("inc/data_export_text.php");
			break;
		case "page":
			include("inc/data_export_page.php");
			switch ($RosCMS_GET_d_use) {
				default:
				case "show":
					if ( is_numeric($RosCMS_GET_d_value) ) {
						$query_show_revision = mysql_query("SELECT d.data_name, r.rev_id, d.data_id, r.rev_language 
													FROM data_ d, data_revision r 
													WHERE r.rev_id = '".mysql_real_escape_string($RosCMS_GET_d_value)."' 
													AND r.data_id = d.data_id
													ORDER BY r.rev_version DESC
													LIMIT 1;");
						$result_show_revision = mysql_fetch_array($query_show_revision);
						$RosCMS_GET_d_value = $result_show_revision['data_name'];
						$RosCMS_GET_d_value2 = $result_show_revision['rev_language'];
						$RosCMS_GET_d_value3 = get_tag($result_show_revision['data_id'], $result_show_revision['rev_id'], "number");
					}
					
					echo "<h1>preview</h1>\n";
					echo "<p>generate_page(".$RosCMS_GET_d_value.", ".$RosCMS_GET_d_value2.", ".$RosCMS_GET_d_value3.", ".$RosCMS_GET_d_use.")</p>";
					echo generate_page($RosCMS_GET_d_value, $RosCMS_GET_d_value2, $RosCMS_GET_d_value3, $RosCMS_GET_d_use);
					break;
				case "output":
					break;
			}
			break;

	}




?>