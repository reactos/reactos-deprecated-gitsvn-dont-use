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

	$g_lang = "";
	$g_cur_page_name = "";
	$g_page_dynid = "";
	
	
	/*function generate_pages($g_language) {
		$query_lang = mysql_query("SELECT * 
									FROM languages 
									ORDER BY lang_level DESC ;");
		while($result_lang = mysql_fetch_array($query_lang)) {
			$g_log .= "\n<hr />\n<h2>".$RosCMS_result_update_per_lang['lang_name']."</h2>\n";
			$g_log .= generate_page("file", "all", $RosCMS_result_update_per_lang['lang_id'], "", "0");
		}
		return $g_log;
	}*/
	
	
	function generate_page($g_page_name, $g_page_lang, $g_page_dynida, $g_output_type) {
		global $roscms_intern_account_id;
		global $roscms_intern_webserver_pages;

		global $g_lang;
		global $g_cur_page_name;
		global $g_page_dynid;
		global $g_linkstyle;

		$g_page_dynid = $g_page_dynida;
		$g_linkstyle = $g_output_type;
		
		$g_log = "";
		
		$g_lang = $g_page_lang;
		
		$gentimeb="";
		$gentimeb = microtime(); 
		$gentimeb = explode(' ',$gentimeb); 
		$gentimeb = $gentimeb[1] + $gentimeb[0]; 
		$pg_startb = $gentimeb; 
		

		$query_g_page = mysql_query("SELECT r.rev_id, r.rev_version, r.rev_usrid, r.rev_datetime, r.rev_date, r.rev_time  
									FROM data_ d, data_revision r 
									WHERE data_name = '".mysql_real_escape_string($g_page_name)."' 
									AND data_type = 'page'
									AND r.data_id = d.data_id
									AND r.rev_version > 0
									AND r.rev_language = '".mysql_real_escape_string($g_lang)."' 
									ORDER BY r.rev_version DESC
									LIMIT 1;");
		$result_g_page = mysql_fetch_array($query_g_page);
		
/*		
		$g_log .= "generate_page(".$g_page_name.", ".$g_lang.", ".$g_page_dynid.")";
		$g_log .= "<p>r.rev_id: ".$result_g_page['rev_id']."</p>";
		$g_log .= "<p>Titel: ".get_stext($result_g_page['rev_id'], "titel")."</p>";
		$g_log .= "<p>Description: ".get_stext($result_g_page['rev_id'], "description")."</p>";
		$g_log .= "<p>Extention: ".get_stext($result_g_page['rev_id'], "extention")."</p>";
		$g_log .= "<p>Content: ".get_text($result_g_page['rev_id'], "content")."</p>";
*/
		
		$g_cur_page_name = $g_page_name;
		
		$g_content = get_text($result_g_page['rev_id'], "content");
		
		// Insert content entries (normal page content and dynamic content like news, newsletter, etc.):
		
		$g_content = preg_replace_callback("(\[#templ_[^][#[:space:]]+\])", "insert_template", $g_content);
		
		//for ($i=0; $i < 3; $i++) { // allow nested contents (3 levels)
			$g_content = preg_replace_callback("(\[#cont_[^][#[:space:]]+\])", "insert_content", $g_content);
		//}
		
		$g_content = preg_replace_callback("(\[#inc_[^][#[:space:]]+\])", "insert_script", $g_content);
	
		// Insert hyperlinks entries:
		$g_content = preg_replace_callback("(\[#link_[^][#[:space:]]+\])", "insert_hyperlink", $g_content);



	
		// RosCMS specific tags:
			// website url:
			$g_content = str_replace("[#roscms_path_homepage]", $roscms_intern_webserver_pages, $g_content);
			if ($g_page_dynid != "" || $g_page_dynid == "0") {
				// current filename:
				$g_content = str_replace("[#roscms_filename]", $g_page_name."_".$g_page_dynid.".html", $g_content);
				// current page name:
				$g_content = str_replace("[#roscms_pagename]", $g_page_name."_".$g_page_dynid, $g_content); 
				// current page title:
				$g_content = str_replace("[#roscms_pagetitle]", ucfirst(get_stext($result_g_page['rev_id'], "title"))." #".$g_page_dynid, $g_content); 
			}
			else {
				// current filename:
				$g_content = str_replace("[#roscms_filename]", $g_page_name.".html", $g_content); 
				// current page name:
				$g_content = str_replace("[#roscms_pagename]", $g_page_name, $g_content); 
				// current page title:
				$g_content = str_replace("[#roscms_pagetitle]", ucfirst(get_stext($result_g_page['rev_id'], "title")), $g_content); 
			}
			// current language:
				$RosCMS_query_current_language = mysql_query("SELECT * 
																FROM languages 
																WHERE lang_id = '".mysql_real_escape_string($g_lang)."'
																LIMIT 1 ;");
				$RosCMS_result_current_language = mysql_fetch_array($RosCMS_query_current_language);
			$g_content = str_replace("[#roscms_language]", $RosCMS_result_current_language['lang_name'], $g_content); 
			// current language:
			$g_content = str_replace("[#roscms_language_short]", $g_lang, $g_content); 
			// current page format (xhtml/html):
			$g_content = str_replace("[#roscms_format]", "html", $g_content); 
			// current date:
			$g_content = str_replace("[#roscms_date]", date("Y-m-d"), $g_content); 
				$zeit = localtime(time() , 1);
			// current time:
			$g_content = str_replace("[#roscms_time]", sprintf("%02d", $zeit['tm_hour']).":".sprintf("%02d",$zeit['tm_min']), $g_content);
				
				$query_usraccount= mysql_query("SELECT user_name 
									FROM users 
									WHERE user_id = '".mysql_real_escape_string($roscms_intern_account_id)."' LIMIT 1 ;");
				$result_usraccount=mysql_fetch_array($query_usraccount);
			// account that generate:
			$g_content = str_replace("[#roscms_user]", $result_usraccount['user_name'], $g_content);
			// account that changed the include text:
			$g_content = str_replace("[#roscms_inc_author]", $result_usraccount['user_name'], $g_content); 
			
			// redirect all bad links to the 404 page:
			$g_content = str_replace("[#link_", $roscms_intern_webserver_pages."?page=404", $g_content);
				
			// current page version:
			$g_content = str_replace("[#roscms_page_version]", $result_g_page['rev_version'], $g_content); 
			


		
//		$g_log .= "<hr />";

		$g_log .= $g_content;
		
		
		$gentimec = microtime(); 
		$gentimec = explode(' ',$gentimec); 
		$gentimec = $gentimec[1] + $gentimec[0]; 
		$pg_endb = $gentimec; 
		$totaltimef = ($pg_endb - $pg_startb); 
		$showtimef = number_format($totaltimef, 4, '.', ''); 
		
		$g_log .= "\n<hr /><p>All pages generated in ".$showtimef." seconds.</p>";

		
		return $g_log;
	}

	function insert_template($matches) {
		global $roscms_intern_account_id;
		global $g_lang;
		global $g_cur_page_name;
				
		// extract the name, e.g. [#cont_about] ... "about"
		$g_content_name = substr($matches[0], 8, (strlen($matches[0])-9)); 
		
		$RosCMS_result_template_temp = insert_match("template", $g_content_name, $g_lang);
		$RosCMS_result_template_temp = str_replace("[#cont_%NAME%]", "[#cont_".$g_cur_page_name."]", $RosCMS_result_template_temp); 

		return $RosCMS_result_template_temp;
	}	
	
	function insert_content($matches) {
		global $roscms_intern_account_id;
		global $g_lang;
				
		// extract the name, e.g. [#cont_about] ... "about"
		$g_content_name = substr($matches[0], 7, (strlen($matches[0])-8)); 
	
		return insert_match("content", $g_content_name, $g_lang);
	}

	function insert_script($matches) {
		global $roscms_intern_account_id;
		global $g_lang;
				
		// extract the name, e.g. [#inc_about] ... "about"
		$g_content_name = substr($matches[0], 6, (strlen($matches[0])-7)); 
		
		return insert_match("script", $g_content_name, $g_lang);
	}

	function insert_match($g_insert_match_type, $g_match_name, $g_match_lang) {
		global $roscms_intern_account_id;
		global $g_page_dynid;
				
		$query_content_temp = "SELECT t.text_content, r.rev_version, r.rev_usrid, r.rev_datetime , r.data_id, r.rev_id
									FROM data_ d, data_revision r, data_text t
									WHERE data_name = '".mysql_real_escape_string($g_match_name)."' 
									AND data_type = '".mysql_real_escape_string($g_insert_match_type)."'
									AND r.data_id = d.data_id
									AND r.rev_version > 0
									AND r.rev_language = '".mysql_real_escape_string($g_match_lang)."'
									AND t.data_rev_id = r.rev_id 
									AND t.text_name = 'content'
									ORDER BY r.rev_version DESC
									LIMIT 1;";

//		echo $query_content_temp;

		$query_content = mysql_query($query_content_temp);
		$result_content = mysql_fetch_array($query_content);
	
		$RosCMS_result_content_temp = $result_content['text_content'];
		
		if ($g_insert_match_type == "script") {
//			echo "<h3>!!".get_tag($result_content['data_id'], $result_content['rev_id'], "kind")."!! &lt;=&gt; get_tag(".$result_content['data_id'].", ".$result_content['rev_id'].", \"kind\")</h3>";
			if (get_tag($result_content['data_id'], $result_content['rev_id'], "kind") == "php") {
				$RosCMS_result_content_temp = eval_template($RosCMS_result_content_temp, $g_page_dynid, $g_match_lang);
			}
		}
//		echo "<p>".$g_match_name." - |".$g_insert_match_type."|</p>";
	
		// latest content changes:
		$query_usraccountc= mysql_query("SELECT user_name, user_fullname  
											FROM users 
											WHERE user_id = '".mysql_real_escape_string($result_content['rev_usrid'])."' LIMIT 1 ;");
		$result_usraccountc=mysql_fetch_array($query_usraccountc);
	
		if ($result_usraccountc['user_fullname']) {
			$RosCMS_result_user_temp = $result_usraccountc['user_fullname']." (".$result_usraccountc['user_name'].")";
		}
		else {
			$RosCMS_result_user_temp = $result_usraccountc['user_name'];
		}
			
		$RosCMS_result_content_temp = str_replace("[#roscms_".$g_insert_match_type."_version]", "<i>Last modified: ".$result_content['rev_datetime'].", rev. ".$result_content['rev_version']." by ".$RosCMS_result_user_temp."</i>", $RosCMS_result_content_temp); 
		
	
		return $RosCMS_result_content_temp;
	}
	
	function insert_hyperlink($matches) {
		global $roscms_intern_account_id;
		global $roscms_intern_webserver_pages;
		global $roscms_intern_webserver_roscms;
		global $g_lang;
		global $g_page_dynid;
		global $g_linkstyle;

		$g_hyperlink_sql1 = "";
		$g_hyperlink_sql2 = "";
		
		$g_link_page_name = substr($matches[0], 7, (strlen($matches[0])-8)); 
		

		$g_link_page_name2 = $g_link_page_name;
		$g_link_page_number = "";
		
		/*$sql_page_link = "SELECT *
										FROM data_revision r, data_ d
										WHERE r.rev_version >= 0 
										AND d.data_name = '".mysql_real_escape_string($g_link_page_name2)."'
										AND d.data_type = 'content'
										AND r.data_id = d.data_id
										ORDER BY r.rev_version DESC
										LIMIT 1;";

		*/
		if ( is_numeric(substr(strrchr($g_link_page_name,"_"), 1, strlen(strrchr($g_link_page_name,"_")-1))) ) { // dynamic
			$g_link_page_name2 = substr($g_link_page_name, 0, strlen($g_link_page_name) - strlen(strrchr($g_link_page_name,"_")));
			$g_link_page_number = substr(strrchr($g_link_page_name,"_"), 1, strlen(strrchr($g_link_page_name,"_")-1));
			/*
			$sql_page_link = "SELECT *
											FROM data_revision r, data_ d, data_tag a, data_tag_name n, data_tag_value v
											WHERE r.rev_version >= 0 
											AND d.data_name = '".mysql_real_escape_string($g_link_page_name2)."'
											AND d.data_type = 'content'
											AND r.data_id = d.data_id
											AND r.data_id = a.data_id 
											AND r.rev_id = a.data_rev_id 
											AND a.tag_usrid = '-1' 
											AND a.tag_name_id = n.tn_id  
											AND a.tag_value_id  = v.tv_id 
											AND (n.tn_name = 'type' AND v.tv_value = 'dynamic')
											AND (n.tn_name = 'number' AND v.tv_value = '".mysql_real_escape_string($g_link_page_number)."')
											ORDER BY r.rev_version DESC
											LIMIT 1;";*/
		}
		/*
		//echo "<hr /><p>".$sql_page_link."</p><hr />";
		
		$query_page_link = mysql_query($sql_page_link);
		$result_page_link = mysql_fetch_array($query_page_link);*/


	
		if ($g_linkstyle == "show") { // Dynamic Preview
			$RosCMS_current_page_link = $roscms_intern_webserver_roscms."?page=data_out&amp;d_f=page&amp;d_u=show&amp;d_fl=g&d_val=".$g_link_page_name2."&amp;d_val2=".$g_lang."&amp;d_val3=".$g_link_page_number;
			
			if ($g_link_page_name == "") {
				$RosCMS_current_page_link = $roscms_intern_webserver_roscms."?page=data_out&amp;d_f=page&amp;d_u=show&amp;d_fl=g&d_val=index&amp;d_val2=".$g_lang."&amp;d_val3=";
			}
		}
		else { // static pages
			$RosCMS_current_page_link_extention = "html";
			/*if ($RosCMS_result_page_link['pages_extention'] != "default") {
				$RosCMS_current_page_link_extention = $RosCMS_result_page_link['pages_extention'];
			}
			*/
			$RosCMS_current_page_link = $roscms_intern_webserver_pages.$g_lang."/".$g_link_page_name.".".$RosCMS_current_page_link_extention;
			
			if ($g_link_page_name == "") {
				$RosCMS_current_page_link = $roscms_intern_webserver_pages.$g_lang."/404.html";
			}
		}
		
		return $RosCMS_current_page_link;
	}


	function eval_template($code, $dyncontid, $dyncontlang) { // function code idea from: http://www.zend.com/zend/art/buffering.php
		ob_start(); 
		
		$roscms_template_var_pageid = "";
		$roscms_template_var_lang = "";
		
		$roscms_template_var_pageid = $dyncontid;
		$roscms_template_var_lang = $dyncontlang;
		
		eval(' ?'.'>'.$code.' <'.'?php ');
		 
		$output = ob_get_contents(); 
		
		ob_end_clean(); 
		return $output; 
	} 
	


?>