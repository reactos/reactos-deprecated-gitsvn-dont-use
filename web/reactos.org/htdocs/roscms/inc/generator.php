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
	if ( !defined('ROSCMS_SYSTEM') && $rpm_page != "generate_fast_secret")
	{
		die("die");
	}
	
	if ($rpm_page == "generate_fast_secret") {
		$rpm_sec="generator";
		$rpm_sec2="output";
		$roscms_intern_account_id = "1";
	}



	$RosCMS_global_generator_extention = false; // true/false

	
	$RosCMS_global_current_page_lang = "";
	$RosCMS_global_linkstyle = "";
	$RosCMS_current_page_dynamic_cont_id = "";
	
	include("connect.db.php");
	include("roscms_config.php");





	//echo generator("update", "all");
	
	//echo "<hr />###<hr />";
	
	//echo generator("all", "all");
	
	


/*
 *****************************************************************************************************************************************
 *
 * generator:
 * ==========
 *
 *   $RosCMS_generator_type: 
 *                             "single": single page, e.g. 'about'
 *                             "all": all pages of the given settings
 *                             "update": all altered pages of the given settings; no other settings required
 *
 *   $RosCMS_generator_lang: 
 *                             "all": all languages
 *                             [xy]: choosen language, language code from RosCMS, e.g. "en", "de", etc.
 *
 *   $RosCMS_generator_output: (only for single page)
 *                             "file": output to file
 *                             "client": output to web-client (browser)
 *
 *   $RosCMS_generator_pagename: (only for single page)
 *                             [a-z_0-9]: page name, e.g. 'about'; only in combination with "single" setting, otherwise use: ""
 *
 *   $RosCMS_generator_dynpagecount: (only for single page)
 *                             "0": all dynamic pages of the current page entry
 *                             [1-999]: choosen dynamic page of the current page entry
 *
 *****************************************************************************************************************************************
 */

function generator ( $RosCMS_generator_type = "client", $RosCMS_generator_lang = "en", $RosCMS_generator_output = "single", $RosCMS_generator_pagename = "index", $RosCMS_generator_dynpagecount = "0") {
	
	if ($RosCMS_generator_type == "single") { // single page
//		echo '<p>generator_page('.$RosCMS_generator_output.', "single", '.$RosCMS_generator_lang.', '.$RosCMS_generator_pagename.', '.$RosCMS_generator_dynpagecount.');</p>';
		return generator_page($RosCMS_generator_output, "single", $RosCMS_generator_lang, $RosCMS_generator_pagename, $RosCMS_generator_dynpagecount);
	}
	elseif ($RosCMS_generator_type == "all") { // all pages
		if ($RosCMS_generator_lang == "all") {
			$RosCMS_generator_log = "";

			$gentimeb="";
			$gentimeb = microtime(); 
			$gentimeb = explode(' ',$gentimeb); 
			$gentimeb = $gentimeb[1] + $gentimeb[0]; 
			$pg_startb = $gentimeb; 

			$RosCMS_query_update_per_lang = mysql_query("SELECT * 
															FROM `languages` 
															ORDER BY `lang_level` DESC ;");
			while($RosCMS_result_update_per_lang = mysql_fetch_array($RosCMS_query_update_per_lang)) {
		 		$RosCMS_generator_log .= "\n<hr />\n<h2>".$RosCMS_result_update_per_lang['lang_name']."</h2>\n";
				$RosCMS_generator_log .= generator_page("file", "all", $RosCMS_result_update_per_lang['lang_id'], "", "0");
			}

			$gentimec = microtime(); 
			$gentimec = explode(' ',$gentimec); 
			$gentimec = $gentimec[1] + $gentimec[0]; 
			$pg_endb = $gentimec; 
			$totaltimef = ($pg_endb - $pg_startb); 
			$showtimef = number_format($totaltimef, 4, '.', ''); 

			$RosCMS_generator_log .= "\n<hr /><p>All pages generated in ".$showtimef." seconds.</p>";

			// Set all "altered" flags for all entries to '0':
			generator_page_update_finalize();
			
			return $RosCMS_generator_log;
		}
		else {
			$RosCMS_generator_log = "";

			$gentimeb="";
			$gentimeb = microtime(); 
			$gentimeb = explode(' ',$gentimeb); 
			$gentimeb = $gentimeb[1] + $gentimeb[0]; 
			$pg_startb = $gentimeb; 

			$RosCMS_generator_log .= generator_page("file", "all", $RosCMS_generator_lang, "", "0");

			$gentimec = microtime(); 
			$gentimec = explode(' ',$gentimec); 
			$gentimec = $gentimec[1] + $gentimec[0]; 
			$pg_endb = $gentimec; 
			$totaltimef = ($pg_endb - $pg_startb); 
			$showtimef = number_format($totaltimef, 4, '.', ''); 
			
			$RosCMS_generator_log .= "\n<p>All '".$RosCMS_generator_lang."' pages generated in ".$showtimef." seconds.</p>";
			return $RosCMS_generator_log;
		}
	}
	else { // altered pages
		if ($RosCMS_generator_lang == "all") {
			$RosCMS_generator_log = "";

			$gentimeb="";
			$gentimeb = microtime(); 
			$gentimeb = explode(' ',$gentimeb); 
			$gentimeb = $gentimeb[1] + $gentimeb[0]; 
			$pg_startb = $gentimeb; 
				
			$RosCMS_query_update_per_lang = mysql_query("SELECT * 
															FROM `languages` 
															ORDER BY `lang_level` DESC ;");
			while($RosCMS_result_update_per_lang = mysql_fetch_array($RosCMS_query_update_per_lang)) {	
		 		$RosCMS_generator_log .= "\n<hr />\n<h2>".$RosCMS_result_update_per_lang['lang_name']."</h2>\n";
				$RosCMS_generator_log .= generator_page_prepare_update($RosCMS_result_update_per_lang['lang_id']);
			}
			
			$gentimec = microtime(); 
			$gentimec = explode(' ',$gentimec); 
			$gentimec = $gentimec[1] + $gentimec[0]; 
			$pg_endb = $gentimec; 
			$totaltimef = ($pg_endb - $pg_startb); 
			$showtimef = number_format($totaltimef, 4, '.', ''); 
			
			$RosCMS_generator_log .= "<p>All pages updated in ".$showtimef." seconds.</p>";
			
			// Set all "altered" flags for all entries to '0':
			generator_page_update_finalize();

			return $RosCMS_generator_log;
		}
		else {
			$RosCMS_generator_log = "";
			
			$gentimeb="";
			$gentimeb = microtime(); 
			$gentimeb = explode(' ',$gentimeb); 
			$gentimeb = $gentimeb[1] + $gentimeb[0]; 
			$pg_startb = $gentimeb; 
			
			$RosCMS_generator_log .= generator_page_prepare_update($RosCMS_generator_lang);
			
			$gentimec = microtime(); 
			$gentimec = explode(' ',$gentimec); 
			$gentimec = $gentimec[1] + $gentimec[0]; 
			$pg_endb = $gentimec; 
			$totaltimef = ($pg_endb - $pg_startb); 
			$showtimef = number_format($totaltimef, 4, '.', ''); 
			
			$RosCMS_generator_log .= "<p>All '".$RosCMS_generator_lang."' pages updated in ".$showtimef." seconds.</p>";
			
			return $RosCMS_generator_log;
		}
	}

}


/*
 *****************************************************************************************************************************************
 *
 * generator_page: (intern function, use "generator()" from outside, see above)
 * ===============
 *
 *   $RosCMS_generator_output: 
 *                             "file": output to file
 *                             "client": output to web-client (browser)
 *
 *   $RosCMS_generator_type: 
 *                             "single": single page, e.g. 'about'
 *                             "all": all pages of the given settings
 *
 *   $RosCMS_generator_lang: 
 *                             "all": all languages
 *                             [xy]: choosen language, language code from RosCMS, e.g. "en", "de", etc.
 *
 *   $RosCMS_generator_pagename: 
 *                             [a-z_0-9]: page name, e.g. 'about'; only in combination with "single" setting, otherwise use: ""
 *
 *   $RosCMS_generator_dynpagecount: 
 *                             "0": all dynamic pages of the current page entry
 *                             [1-999]: choosen dynamic page of the current page entry
 *
 *****************************************************************************************************************************************
 */

function generator_page ($RosCMS_generator_output = "client", $RosCMS_generator_type = "single", $RosCMS_generator_lang = "en", $RosCMS_generator_pagename = "index", $RosCMS_generator_dynpagecount = "0") {
	global $RosCMS_current_page_dynamic_cont_id;
	global $RosCMS_global_linkstyle;
	
	$RosCMS_current_page_dynamic_flag="";

	$RosCMS_generator_log = "";

	
	if ($RosCMS_generator_output == "client") {
		$RosCMS_global_linkstyle = "dynamic";
	}
	else {
		$RosCMS_global_linkstyle = "";
	}


//	$RosCMS_generator_log .= "<hr />config: ".$RosCMS_generator_type."|".$RosCMS_generator_lang."|".$RosCMS_generator_pagename."|".$RosCMS_generator_dynpagecount;

	set_time_limit(0);

	if ($RosCMS_generator_type == "single" && $RosCMS_generator_type == "all") {	
		// This page was generated in ... - part 1
		$gentime = microtime(); 
		$gentime = explode(' ',$gentime); 
		$gentime = $gentime[1] + $gentime[0]; 
		$pg_start = $gentime; 
	}
	
	// Which Page(s):
	if ($RosCMS_generator_type == "single") {	
		$RosCMS_generator_pagename_sql = "page_name = '".mysql_real_escape_string($RosCMS_generator_pagename)."' AND ";
	}
	elseif ($RosCMS_generator_type == "all") {
		$RosCMS_generator_pagename_sql = "";
	}
	else {
		die ("<p>RosCMS Generator: which page(s) ?</p>");
	}
	
	// Which Language(s):
	if ($RosCMS_generator_lang == "all") {	
		$RosCMS_generator_lang_sql = "";
	}
	else {
		$RosCMS_generator_lang_sql = "page_language = '".mysql_real_escape_string($RosCMS_generator_lang)."' OR ";
	}
	
	// Dynamic Content setting (0=all):
	if ($RosCMS_generator_dynpagecount == "0") {
		$RosCMS_generator_current_sql_dynpagecount = "";
	}
	else {
		$RosCMS_generator_current_sql_dynpagecount = "AND dyn_content_id ='".mysql_real_escape_string($RosCMS_generator_dynpagecount)."'";
	}

	if ($RosCMS_generator_output == "file") {
		$RosCMS_generator_log .= "\n<ul>\n";
	}

	$RosCMS_generator_log_entrycounter = 0;

	$RosCMS_query_page = mysql_query("SELECT pages_extra, page_name, page_title, page_text, page_version, page_usrname_id 
			FROM pages
			WHERE ".$RosCMS_generator_pagename_sql." page_visible = '1' 
			AND page_active = '1' 
			AND (".$RosCMS_generator_lang_sql." page_language = 'all') 
			ORDER BY 'page_name' ASC ;");
			
	while($RosCMS_result_page = mysql_fetch_array($RosCMS_query_page)) {
		// This page was generated in (for every page) ...
		if ($RosCMS_generator_output == "file") {
			$gentimeb="";
			$gentimeb = microtime(); 
			$gentimeb = explode(' ',$gentimeb); 
			$gentimeb = $gentimeb[1] + $gentimeb[0]; 
			$pg_startb = $gentimeb; 
		}
	
		if ($RosCMS_result_page['pages_extra'] == "dynamic") {
			$RosCMS_current_page_dynamic_flag = true;
			$RosCMS_query_page_dynamic = mysql_query("SELECT dyn_content_id, dyn_content_name 
												FROM `dyn_content` 
												WHERE `dyn_content_nr` = '1' 
												AND `dyn_content_name` = '".mysql_real_escape_string($RosCMS_result_page['page_name'])."' 
												AND dyn_content_active = '1' 
												AND dyn_content_visible = '1' 
												AND (dyn_content_lang = '".mysql_real_escape_string($RosCMS_generator_lang)."' OR dyn_content_lang = 'all') 
												".$RosCMS_generator_current_sql_dynpagecount." 
												ORDER BY dyn_content_id ASC, dyn_content_lang DESC ;");
		}
		else {
			$RosCMS_current_page_dynamic_flag = false;
			//echo "<br><br>--NOTDynamic--";
			$RosCMS_query_page_dynamic = mysql_query("SELECT * 
												FROM `pages` 
												WHERE 1=1 LIMIT 1 "); // temp
		}
		while($RosCMS_result_page_dynamic = mysql_fetch_array($RosCMS_query_page_dynamic)) {
			$RosCMS_generator_log_entrycounter++;
			
			if (array_key_exists("dyn_content_id", $RosCMS_result_page_dynamic)) $RosCMS_current_page_dynamic_cont_id=$RosCMS_result_page_dynamic['dyn_content_id'];

			$RosCMS_generator_page_content = $RosCMS_result_page['page_text'];
			
			if ($RosCMS_generator_output == "client") {
				echo generator_page_data($RosCMS_result_page['page_name'],
											 $RosCMS_result_page['page_title'], 
											 $RosCMS_generator_page_content,
											 $RosCMS_generator_lang,
											 $RosCMS_result_page['page_usrname_id'], 
											 $RosCMS_result_page['page_version'], 
											 $RosCMS_current_page_dynamic_flag
										 );
			}
			elseif ($RosCMS_generator_output == "file") {
				$RosCMS_generator_log .= generator_page_output_file( $RosCMS_result_page['page_name'], 
																			generator_page_data($RosCMS_generator_pagename, 
																				$RosCMS_result_page['page_title'], 
																				$RosCMS_generator_page_content, 
																				$RosCMS_generator_lang, 
																				$RosCMS_result_page['page_usrname_id'], 
																				$RosCMS_result_page['page_version'],
																				$RosCMS_current_page_dynamic_flag
																			),
																			$RosCMS_current_page_dynamic_cont_id
																		);
																		/*
																		$RosCMS_current_page, 
																		$RosCMS_current_page_title, 
																		$RosCMS_current_page_content, 
																		$RosCMS_generator_page_lang, 
																		$RosCMS_generator_page_usrid, 
																		$RosCMS_generator_page_version, 
																		$RosCMS_current_page_dynamic_flag
																		
																		$RosCMS_current_page_name, 
																		$RosCMS_current_page_content, 
																		$RosCMS_current_page_dynamic_cont_id*/
				$showtimef="";
			}

		}
		
		
		// This page was generated in (for every page) ...
		if ($RosCMS_generator_output == "file") {
			$gentimec = microtime(); 
			$gentimec = explode(' ',$gentimec); 
			$gentimec = $gentimec[1] + $gentimec[0]; 
			$pg_endb = $gentimec; 
			$totaltimef = ($pg_endb - $pg_startb); 
			$showtimef = number_format($totaltimef, 4, '.', ''); 
			$RosCMS_generator_log .= " &nbsp;&nbsp;&nbsp; (".$showtimef." sec; #".$RosCMS_generator_log_entrycounter.")</li>\n";
		}
	}
	
	if ($RosCMS_generator_output == "file") {
		$RosCMS_generator_log .= "</ul>\n";
	}

	if ($RosCMS_generator_type == "single" && $RosCMS_generator_type == "all") {	
		// This page was generated in ... - part 2
		$gentime = microtime(); 
		$gentime = explode(' ',$gentime); 
		$gentime = $gentime[1] + $gentime[0]; 
		$pg_end = $gentime; 
		$totaltime = ($pg_end - $pg_start); 
		$showtime = number_format($totaltime, 4, '.', ''); 
		$RosCMS_generator_log .= "<p><center><i>The static pages have been generated in " . $showtime . " seconds.</i></center></p>";
	}	
	
	return $RosCMS_generator_log;
}


function generator_page_data ($RosCMS_current_page, $RosCMS_current_page_title, $RosCMS_current_page_content, $RosCMS_generator_page_lang, $RosCMS_generator_page_usrid = "0", $RosCMS_generator_page_version = "1", $RosCMS_current_page_dynamic_flag) {
	global $RosCMS_global_current_page_lang;
	global $roscms_intern_path_server;
	global $RosCMS_current_page_dynamic_cont_id;
	global $roscms_intern_account_id;
	global $roscms_intern_usrnameid;
	global $RosCMS_global_generator_extention;


	//echo "<hr />".$RosCMS_current_page_content."|".$RosCMS_generator_page_lang."<hr />";
			
	// Set current language as global for this 'round':
	$RosCMS_global_current_page_lang = $RosCMS_generator_page_lang;

	// Insert content entries (normal page content and dynamic content like news, newsletter, etc.):
	$RosCMS_current_page_content = preg_replace_callback("(\[#cont_[^][#[:space:]]+\])", "insert_content", $RosCMS_current_page_content);

	// Insert 'include text' entries (variables, small text pieces and php code for dynamic contents):
	$RosCMS_current_page_content = preg_replace_callback("(\[#inc_[^][#[:space:]]+\])", "insert_inctext", $RosCMS_current_page_content);

	// Insert hyperlinks entries:
	$RosCMS_current_page_content = preg_replace_callback("(\[#link_[^][#[:space:]]+\])", "insert_hyperlink", $RosCMS_current_page_content);

	// RosCMS specific tags:
		// website url:
		$RosCMS_current_page_content = str_replace("[#roscms_path_homepage]", $roscms_intern_path_server, $RosCMS_current_page_content);
		if ($RosCMS_current_page_dynamic_flag == true) {
			// current filename:
			$RosCMS_current_page_content = str_replace("[#roscms_filename]", $RosCMS_current_page."_".$RosCMS_current_page_dynamic_cont_id.".html", $RosCMS_current_page_content);
			// current page name:
			$RosCMS_current_page_content = str_replace("[#roscms_pagename]", $RosCMS_current_page."_".$RosCMS_current_page_dynamic_cont_id, $RosCMS_current_page_content); 
			// current page title:
			$RosCMS_current_page_content = str_replace("[#roscms_pagetitle]", ucfirst($RosCMS_current_page_title)." #".$RosCMS_current_page_dynamic_cont_id, $RosCMS_current_page_content); 
		}
		else {
			// current filename:
			$RosCMS_current_page_content = str_replace("[#roscms_filename]", $RosCMS_current_page.".html", $RosCMS_current_page_content); 
			// current page name:
			$RosCMS_current_page_content = str_replace("[#roscms_pagename]", $RosCMS_current_page, $RosCMS_current_page_content); 
			// current page title:
			$RosCMS_current_page_content = str_replace("[#roscms_pagetitle]", ucfirst($RosCMS_current_page_title), $RosCMS_current_page_content); 
		}
		// current language:
			$RosCMS_query_current_language = mysql_query("SELECT * 
															FROM `languages` 
															WHERE `lang_id` = '".mysql_real_escape_string($RosCMS_global_current_page_lang)."'
															LIMIT 1 ;");
			$RosCMS_result_current_language = mysql_fetch_array($RosCMS_query_current_language);
		$RosCMS_current_page_content = str_replace("[#roscms_language]", $RosCMS_result_current_language['lang_name'], $RosCMS_current_page_content); 
		// current language:
		$RosCMS_current_page_content = str_replace("[#roscms_language_short]", $RosCMS_global_current_page_lang, $RosCMS_current_page_content); 
		// current page format (xhtml/html):
		$RosCMS_current_page_content = str_replace("[#roscms_format]", "html", $RosCMS_current_page_content); 
		// current date:
		$RosCMS_current_page_content = str_replace("[#roscms_date]", date("Y-m-d"), $RosCMS_current_page_content); 
			$zeit = localtime(time() , 1);
		// current time:
		$RosCMS_current_page_content = str_replace("[#roscms_time]", sprintf("%02d", $zeit['tm_hour']).":".sprintf("%02d",$zeit['tm_min']), $RosCMS_current_page_content);
			$query_usraccountb= mysql_query("SELECT user_name 
								FROM `users` 
								WHERE `user_id` = '".mysql_real_escape_string($roscms_intern_account_id)."' LIMIT 1 ;");
			$result_usraccountb=mysql_fetch_array($query_usraccountb);
			
			$roscms_intern_usrnameid = $RosCMS_generator_page_usrid;
			$query_usraccount= mysql_query("SELECT user_name 
								FROM `users` 
								WHERE `user_id` = '".mysql_real_escape_string($roscms_intern_usrnameid)."' LIMIT 1 ;");
			$result_usraccount=mysql_fetch_array($query_usraccount);
		// account that generate:
		$RosCMS_current_page_content = str_replace("[#roscms_user]", $result_usraccountb['user_name'], $RosCMS_current_page_content);
		// account that changed the include text:
		$RosCMS_current_page_content = str_replace("[#roscms_inc_author]", $result_usraccount['user_name'], $RosCMS_current_page_content); 
		
		// redirect all bad links to the 404 page:
		$RosCMS_current_page_content = str_replace("[#link_", $roscms_intern_path_server."?page=404", $RosCMS_current_page_content);
			
		// current page version:
		$RosCMS_current_page_content = str_replace("[#roscms_page_version]", $RosCMS_generator_page_version, $RosCMS_current_page_content); 
		
		
		// Replace high chars by their html-escaped version:
		if ($RosCMS_global_generator_extention == true) {
			$RosCMS_current_page_content = roscms_unicode_escape($RosCMS_current_page_content);
		}
		

	return $RosCMS_current_page_content;
}


function eval_template($code, $dyncontid) // function code idea from: http://www.zend.com/zend/art/buffering.php
{ 
	ob_start(); 
	$roscms_template_var_pageid="";
	$roscms_template_var_pageid=$dyncontid;
	eval(' ?'.'>'.$code.' <'.'?php '); 
	$output = ob_get_contents(); 
	ob_end_clean(); 
	return $output; 
} 


function insert_content($matches) {
	global $RosCMS_global_current_page_lang;
	global $roscms_intern_account_id;
	
	// extract the name, e.g. [#cont_about] ... "about"
	$RosCMS_current_content_name = substr($matches[0], 7, (strlen($matches[0])-8)); 
	
	$RosCMS_query_content = mysql_query("SELECT content_text, content_date, content_time, content_lang, content_version 
											FROM content
											WHERE content_name = '".mysql_real_escape_string($RosCMS_current_content_name)."'
											AND content_visible = '1'
											AND content_active = '1'
											AND (content_lang = '".mysql_real_escape_string($RosCMS_global_current_page_lang)."' OR content_lang = 'all' OR content_lang = 'html') 
											ORDER BY content_lang DESC
											LIMIT 1 ;");
	$RosCMS_result_content = mysql_fetch_array($RosCMS_query_content);

	$RosCMS_result_content_temp = $RosCMS_result_content['content_text'];


	// latest content changes:
	$query_usraccountc= mysql_query("SELECT user_name, user_fullname  
										FROM `users` 
										WHERE `user_id` = '".mysql_real_escape_string($roscms_intern_account_id)."' LIMIT 1 ;");
	$result_usraccountc=mysql_fetch_array($query_usraccountc);

	if ($result_usraccountc['user_fullname']) {
		$RosCMS_result_user_temp = $result_usraccountc['user_fullname']." (".$result_usraccountc['user_name'].")";
	}
	else {
		$RosCMS_result_user_temp = $result_usraccountc['user_name'];
	}
		
	$RosCMS_result_content_temp = str_replace("[#roscms_content_version]", "<i>Last modified: ".$RosCMS_result_content['content_date']." ".$RosCMS_result_content['content_time'].", rev. ".$RosCMS_result_content['content_version']." by ".$RosCMS_result_user_temp."</i>", $RosCMS_result_content_temp); 


	return $RosCMS_result_content_temp;
}

function insert_inctext($matches) {
	global $RosCMS_global_current_page_lang;
	global $RosCMS_current_page_dynamic_cont_id;

	// extract the name, e.g. [#inc_about] ... "about"
	$RosCMS_current_intext_word = substr($matches[0], 6, (strlen($matches[0])-7)); 
	
	$RosCMS_query_intext = mysql_query("SELECT inc_word, inc_text, inc_lang, inc_vis, inc_extra
											FROM include_text
											WHERE inc_word = '".mysql_real_escape_string($RosCMS_current_intext_word)."'
											AND (inc_lang = '".mysql_real_escape_string($RosCMS_global_current_page_lang)."' OR inc_lang = 'html' OR inc_lang = 'all')
											AND inc_vis= '1' 
											ORDER BY inc_lang DESC
											LIMIT 1 ;");
	$RosCMS_result_intext = mysql_fetch_array($RosCMS_query_intext);

		
	if ($RosCMS_result_intext['inc_extra'] == "template_php") {
		//echo "<hr />".$RosCMS_current_page_content." -=- ".$matches[0]." - ".$RosCMS_result_intext['inc_extra']." - <b/>".$RosCMS_result_intext["inc_word"]."</b>";
		return eval_template($RosCMS_result_intext["inc_text"], $RosCMS_current_page_dynamic_cont_id);
	}
	else {
		return $RosCMS_result_intext['inc_text'];
	}
}

function insert_hyperlink($matches) {
	global $RosCMS_global_current_page_lang;
	global $roscms_intern_path_server;
	global $RosCMS_global_linkstyle;
	global $roscms_intern_path_gererator;
	
	// extract the name, e.g. [#link_about] ... "about"
	$RosCMS_current_page_name = substr($matches[0], 7, (strlen($matches[0])-8)); 
	//echo "<hr />???????".strrchr($RosCMS_current_page_name,"_");
	
	if ( is_numeric(substr(strrchr($RosCMS_current_page_name,"_"), 1, strlen(strrchr($RosCMS_current_page_name,"_")-1))) ) {
		//echo "<hr />ZAHL!!!!!".substr(strrchr($RosCMS_current_page_name,"_"), 1, strlen(strrchr($RosCMS_current_page_name,"_")-1));
		$RosCMS_current_page_name_save = substr($RosCMS_current_page_name, 0, strlen($RosCMS_current_page_name) - strlen(strrchr($RosCMS_current_page_name,"_")));
	}
	else {
		$RosCMS_current_page_name_save = $RosCMS_current_page_name;
	}
	
	$RosCMS_query_page_link = mysql_query("SELECT pages_extra, page_name, pages_extention
											FROM pages
											WHERE page_name = '".mysql_real_escape_string($RosCMS_current_page_name_save)."' 
											AND page_visible = '1' 
											AND page_active = '1' 
											AND (page_language = '".mysql_real_escape_string($RosCMS_global_current_page_lang)."' OR page_language = 'all') 
											ORDER BY page_language DESC
											LIMIT 1 ;");
	$RosCMS_result_page_link = mysql_fetch_array($RosCMS_query_page_link);


	if ($RosCMS_global_linkstyle == "dynamic") { // Dynamic Preview
		if ($RosCMS_result_page_link['pages_extra'] == "dynamic") {
			$RosCMS_current_page_link = $roscms_intern_path_server.$roscms_intern_path_gererator."?page=admin&amp;sec=generator&amp;sec2=view&amp;site=".$RosCMS_result_page_link['page_name']."&amp;gen_lang=".$RosCMS_global_current_page_lang."&amp;gen_dynid=".substr(strrchr($RosCMS_current_page_name,"_"), 1, strlen(strrchr($RosCMS_current_page_name,"_")-1));
		}
		else {
			$RosCMS_current_page_link = $roscms_intern_path_server.$roscms_intern_path_gererator."?page=admin&amp;sec=generator&amp;sec2=view&amp;site=".$RosCMS_result_page_link['page_name']."&amp;gen_lang=".$RosCMS_global_current_page_lang;
		}
		if ($RosCMS_result_page_link['page_name'] == "") {
			$RosCMS_current_page_link = $roscms_intern_path_server.$roscms_intern_path_gererator."?page=admin&amp;sec=generator&amp;sec2=view&amp;site=index&amp;lang=".$RosCMS_global_current_page_lang;
		}
	}
	else { // static pages
		$RosCMS_current_page_link_extention = "html";
		if ($RosCMS_result_page_link['pages_extention'] != "default") {
			$RosCMS_current_page_link_extention = $RosCMS_result_page_link['pages_extention'];
		}
		
		if ($RosCMS_result_page_link['pages_extra'] == "dynamic") {
			$RosCMS_current_page_link = $roscms_intern_path_server.$RosCMS_global_current_page_lang."/".$RosCMS_result_page_link['page_name'].strrchr($RosCMS_current_page_name,"_").".".$RosCMS_current_page_link_extention;
		}
		else {
			$RosCMS_current_page_link = $roscms_intern_path_server.$RosCMS_global_current_page_lang."/".$RosCMS_result_page_link['page_name'].".".$RosCMS_current_page_link_extention;
		}
		if ($RosCMS_result_page_link['page_name'] == "") {
			$RosCMS_current_page_link = $roscms_intern_path_server.$RosCMS_global_current_page_lang."/404.html";
		}
	}
	
	return $RosCMS_current_page_link;
}


function generator_page_output_file ( $RosCMS_current_page_name, $RosCMS_current_page_content, $RosCMS_current_page_dynamic_cont_id ) {
	global $RosCMS_global_current_page_lang;
	global $roscms_intern_version;
	global $roscms_intern_account_id;

	// Extract the dynamic page id:
	if ( is_numeric(substr(strrchr($RosCMS_current_page_name,"_"), 1, strlen(strrchr($RosCMS_current_page_name,"_")-1))) ) {
		$RosCMS_current_page_name_save = substr($RosCMS_current_page_name, 0, strlen($RosCMS_current_page_name) - strlen(strrchr($RosCMS_current_page_name,"_")));
	}
	else {
		$RosCMS_current_page_name_save = $RosCMS_current_page_name;
	}
	
	$RosCMS_query_page_out_file = mysql_query("SELECT pages_extention, pages_extra, page_name
											FROM pages
											WHERE page_name = '".mysql_real_escape_string($RosCMS_current_page_name_save)."' 
											AND page_visible = '1' 
											AND page_active = '1' 
											AND (page_language = '".mysql_real_escape_string($RosCMS_global_current_page_lang)."' OR page_language = 'all') 
											ORDER BY page_language DESC
											LIMIT 1 ;");
	$RosCMS_result_page_out_file = mysql_fetch_array($RosCMS_query_page_out_file);


	// file extention: 
	$output_fileformat = "";
	if ($RosCMS_result_page_out_file['pages_extention'] == "default") {
		$output_fileformat = "html";
	}
	else {
		$output_fileformat = $RosCMS_result_page_out_file['pages_extention'];
	}

	// file name:
	if ($RosCMS_result_page_out_file['pages_extra'] == "dynamic") {
		$RosCMS_current_page_out_file = "../".$RosCMS_global_current_page_lang."/".$RosCMS_result_page_out_file['page_name']."_".$RosCMS_current_page_dynamic_cont_id.".".$output_fileformat;
	}
	else {
		$RosCMS_current_page_out_file = "../".$RosCMS_global_current_page_lang."/".$RosCMS_result_page_out_file['page_name'].".".$output_fileformat;
	}
	
	// write file:
	$fp = fopen($RosCMS_current_page_out_file,"w");
	flock($fp,2);
	fputs($fp,$RosCMS_current_page_content); // write content
	fputs($fp,"\n\n<!-- Generated with ".$roscms_intern_version." (new) -->");
	flock($fp,3);
	fclose($fp);
	

	// Update generation time & user information:
	$page_gen_timestamp_query="UPDATE `pages` 
								SET `page_generate_usrid` = '".mysql_real_escape_string($roscms_intern_account_id)."', 
								`page_generate_timestamp` = '".mysql_real_escape_string(time())."' 
								WHERE page_name = '".mysql_real_escape_string($RosCMS_current_page_name_save)."' 
								AND page_visible = '1' 
								AND page_active = '1' 
								AND (page_language = '".mysql_real_escape_string($RosCMS_global_current_page_lang)."' OR page_language = 'all') 
								ORDER BY page_language DESC
								LIMIT 1 ;";
	$page_gen_timestamp_list=mysql_query($page_gen_timestamp_query);
	
	
	return "  <li>".$RosCMS_current_page_out_file;
}

function generator_page_prepare_update ( $RosCMS_generator_update_language ) {

	$array_pages_dyn = array();
	$array_content = array();
	$array_pages = array();
	
	$RosCMS_generator_update_log = "";
	$RosCMS_generator_update_dynpageid = "";
	
	/*
	 * Update Plans (per language):
	 *
	 *   Dynamic Content Pages:
	 *
	 *  	dyn_content => inc_text => pages
	 *
	 *
	 *   Normal Pages:
	 *
	 *  	inc_text => content => pages
	 *  	content => pages
	 *  	pages
	 *
	 */
	
	$RosCMS_generator_update_log .= "<h3>page update process - <u>".$RosCMS_generator_update_language."</u></h3>\n";
	


	$RosCMS_generator_update_log .= "<p>Update <b>dynamic content</b> pages:</p>\n";
	
	$RosCMS_query_page_dynamic = mysql_query("SELECT dyn_content_id, dyn_content_name
							FROM dyn_content
							WHERE dyn_content_nr = '1' 
							AND dyn_content_active = '1' 
							AND dyn_content_visible = '1' 
							AND dyn_altered = '1'  ;");
	while($RosCMS_result_page_dynamic = mysql_fetch_array($RosCMS_query_page_dynamic)) {
		$RosCMS_generator_update_dynpageid = $RosCMS_result_page_dynamic['dyn_content_id'];
//		echo "<br /><br /><br />+ ".$RosCMS_result_page_dynamic['dyn_content_name']."_".$RosCMS_result_page_dynamic['dyn_content_id'];
		$RosCMS_query_intext = mysql_query("SELECT inc_word
								FROM include_text
								WHERE inc_vis = '1' 
								AND inc_text LIKE '%".mysql_real_escape_string($RosCMS_result_page_dynamic['dyn_content_name'])."%' ;");
		while($RosCMS_result_intext = mysql_fetch_array($RosCMS_query_intext)) {
//			echo "<br /> &nbsp; |-".$RosCMS_result_intext['inc_word'];

			$RosCMS_query_pages3 = mysql_query("SELECT pages_extra, page_name
									FROM pages
									WHERE page_visible = '1' 
									AND page_active = '1' 
									AND page_text LIKE '%".mysql_real_escape_string($RosCMS_result_intext['inc_word'])."%'
									AND (page_language = '".mysql_real_escape_string($RosCMS_generator_update_language)."' OR page_language = 'all')
									ORDER BY page_language DESC ;");
			while($RosCMS_result_pages3 = mysql_fetch_array($RosCMS_query_pages3)) {		
				if ($RosCMS_result_pages3['pages_extra'] == "dynamic") {
					if ($RosCMS_result_pages3['page_name'] != "" && tool_array_val_exists($array_pages_dyn, $RosCMS_result_pages3['page_name']."_".$RosCMS_generator_update_dynpageid)) {
//						echo "<br /> &nbsp;&nbsp;&nbsp;&nbsp; |-".$RosCMS_result_pages3['page_name']."_".$RosCMS_generator_update_dynpageid;
						$array_pages_dyn[count($array_pages_dyn)] = $RosCMS_result_pages3['page_name']."_".$RosCMS_generator_update_dynpageid;
					}
				}
				else {
					if ($RosCMS_result_pages3['page_name'] != "" && tool_array_val_exists($array_pages, $RosCMS_result_pages3['page_name'])) {
//						echo "<br /> &nbsp;&nbsp;&nbsp;&nbsp; |-".$RosCMS_result_pages3['page_name'];
						$array_pages[count($array_pages)] = $RosCMS_result_pages3['page_name'];
					}
				}

			}
		}
	}
//	echo "<hr />";
	$RosCMS_generator_update_log .= "<ul>";
	foreach($array_pages_dyn as $key=>$val) {
		$RosCMS_current_page_name = $val;

		if ( is_numeric(substr(strrchr($RosCMS_current_page_name,"_"), 1, strlen(strrchr($RosCMS_current_page_name,"_")-1))) ) {

			$gentimeb="";
			$gentimeb = microtime(); 
			$gentimeb = explode(' ',$gentimeb); 
			$gentimeb = $gentimeb[1] + $gentimeb[0]; 
			$pg_startb = $gentimeb; 	
		
			$RosCMS_current_page_name_save = substr($RosCMS_current_page_name, 0, strlen($RosCMS_current_page_name) - strlen(strrchr($RosCMS_current_page_name,"_")));
			$RosCMS_current_page_name_save2 = substr(strrchr($RosCMS_current_page_name,"_"), 1, strlen(strrchr($RosCMS_current_page_name,"_")-1));
			
			generator_page("file", "single", $RosCMS_generator_update_language, $RosCMS_current_page_name_save, $RosCMS_current_page_name_save2);

			$gentimec = microtime(); 
			$gentimec = explode(' ',$gentimec); 
			$gentimec = $gentimec[1] + $gentimec[0]; 
			$pg_endb = $gentimec; 
			$totaltimef = ($pg_endb - $pg_startb); 
			$showtimef = number_format($totaltimef, 4, '.', ''); 
	
			$RosCMS_generator_update_log .= "<li>".$val.".html &nbsp;&nbsp;&nbsp; (".$showtimef." sec; #".($key+1).")</li>\n";
		}
		else {
			$RosCMS_generator_update_log .= "<li>".$key." => ".$val." is NOT a dynamic content page!</li>\n";
		}
	}
	$RosCMS_generator_update_log .= "</ul>";




	$RosCMS_generator_update_log .= "<p>Check <b>content</b> and <b>include_text</b> for changes ...</p>\n";
	
	$RosCMS_query_intext2 = mysql_query("SELECT inc_word
							FROM include_text
							WHERE inc_vis = '1'
							AND inc_altered = '1'
							AND (inc_lang = '".mysql_real_escape_string($RosCMS_generator_update_language)."' OR inc_lang = 'html' OR inc_lang  = 'all') 
							ORDER BY inc_lang DESC ;");
	while($RosCMS_result_intext2 = mysql_fetch_array($RosCMS_query_intext2)) {
		$RosCMS_query_content = mysql_query("SELECT content_name
								FROM content
								WHERE content_active = '1' 
								AND content_visible = '1' 
								AND content_text LIKE '%[#inc_".mysql_real_escape_string($RosCMS_result_intext2['inc_word'])."]%'
								AND (content_lang  = '".mysql_real_escape_string($RosCMS_generator_update_language)."' OR content_lang  = 'all') 
								ORDER BY content_lang DESC ;");
		while ($RosCMS_result_content = mysql_fetch_array($RosCMS_query_content)) {
			if ($RosCMS_result_content['content_name'] != "" && tool_array_val_exists($array_content, $RosCMS_result_content['content_name'])) {
				$array_content[count($array_content)] = $RosCMS_result_content['content_name'];
			} 
		}
	}
	
	foreach($array_content as $key=>$val) {

		$RosCMS_query_pages = mysql_query("SELECT page_name
								FROM pages
								WHERE page_visible = '1' 
								AND page_active = '1' 
								AND page_text LIKE '%[#cont_".mysql_real_escape_string($val)."]%'
								AND (page_language = '".mysql_real_escape_string($RosCMS_generator_update_language)."' OR page_language = 'all') 
								ORDER BY page_language DESC ;");
		while($RosCMS_result_pages = mysql_fetch_array($RosCMS_query_pages)) {
			if ($RosCMS_result_pages['page_name'] != "" && tool_array_val_exists($array_pages, $RosCMS_result_pages['page_name'])) {
				$array_pages[count($array_pages)] = $RosCMS_result_pages['page_name'];
			} 
		} 
	}


	
	$RosCMS_generator_update_log .= "<p>Update <b>pages</b>:</p>\n";
	
	$RosCMS_query_content2 = mysql_query("SELECT content_name
							FROM content
							WHERE content_visible = '1'
							AND content_active = '1' 
							AND content_altered = '1'
							AND (content_lang  = '".mysql_real_escape_string($RosCMS_generator_update_language)."' OR content_lang  = 'all') 
							ORDER BY content_lang DESC ;");
	while($RosCMS_result_content2 = mysql_fetch_array($RosCMS_query_content2)) {

//		echo "<br />+ ".$RosCMS_result_content2['content_name'];
		$RosCMS_query_pages2 = mysql_query("SELECT page_name
								FROM pages
								WHERE page_visible = '1' 
								AND page_active = '1'
								AND pages_extra = '' 
								AND page_text LIKE '%[#cont_".mysql_real_escape_string($RosCMS_result_content2['content_name'])."]%'
								AND (page_language = '".mysql_real_escape_string($RosCMS_generator_update_language)."' OR page_language = 'all') 
								ORDER BY page_language DESC ;");
		while($RosCMS_result_pages2 = mysql_fetch_array($RosCMS_query_pages2)) {
			if ($RosCMS_result_pages2['page_name'] != "" && tool_array_val_exists($array_pages, $RosCMS_result_pages2['page_name'])) {
//					echo "<br>&nbsp;&nbsp; |- ".$RosCMS_result_pages2['page_name'];
					$array_pages[count($array_pages)] = $RosCMS_result_pages2['page_name'];
			} 
		} 
	}

//	echo "<br />+ ".$RosCMS_result_content2['content_name'];
	$RosCMS_query_pages4 = mysql_query("SELECT page_name
							FROM pages
							WHERE page_visible = '1' 
							AND page_active = '1' 
							AND page_generate_force = '1'
							AND (page_language = '".mysql_real_escape_string($RosCMS_generator_update_language)."' OR page_language = 'all') 
							ORDER BY page_language DESC ;");
	while($RosCMS_result_pages4 = mysql_fetch_array($RosCMS_query_pages4)) {
		if ($RosCMS_result_pages4['page_name'] != "" && tool_array_val_exists($array_pages, $RosCMS_result_pages4['page_name'])) {
//				echo "<br>&nbsp;&nbsp; |- ".$RosCMS_result_pages4['page_name'];
				$array_pages[count($array_pages)] = $RosCMS_result_pages4['page_name'];
		} 
	} 



	$RosCMS_generator_update_log .= "<ul>";

	foreach($array_pages as $key=>$val) {
		$gentimeb="";
		$gentimeb = microtime(); 
		$gentimeb = explode(' ',$gentimeb); 
		$gentimeb = $gentimeb[1] + $gentimeb[0]; 
		$pg_startb = $gentimeb; 	
	
		generator_page("file", "single", $RosCMS_generator_update_language, $val, "0");
		
		$gentimec = microtime(); 
		$gentimec = explode(' ',$gentimec); 
		$gentimec = $gentimec[1] + $gentimec[0]; 
		$pg_endb = $gentimec; 
		$totaltimef = ($pg_endb - $pg_startb); 
		$showtimef = number_format($totaltimef, 4, '.', ''); 

		$RosCMS_generator_update_log .= "<li>".$val.".html &nbsp;&nbsp;&nbsp; (".$showtimef." sec; #".($key+1).")</li>\n";
	}
	$RosCMS_generator_update_log .= "</ul>";
	

//	echo "<hr />";


	return $RosCMS_generator_update_log;

}

function generator_page_update_finalize () {

	$RosCMS_temp_update = mysql_query("UPDATE `content` SET `content_altered` = '0' WHERE `content_altered` = '1' ;");
	$RosCMS_temp_update = mysql_query("UPDATE `dyn_content` SET `dyn_altered` = '0' WHERE `dyn_altered` = '1' ;");
	$RosCMS_temp_update = mysql_query("UPDATE `include_text` SET `inc_altered` = '0' WHERE `inc_altered` = '1' ;");
	$RosCMS_temp_update = mysql_query("UPDATE `pages` SET `page_generate_force` = '0' WHERE `page_generate_force` = '1' ;");

}

function tool_array_val_exists ( $RosCMS_temp_arr, $RosCMS_temp_val ) {
	$RosCMS_temp_exist = true;
//	echo "<h1>TOOL:</h1>";
	foreach($RosCMS_temp_arr as $key=>$val) {
//		echo "<br />????????? ".$val." <=> ".$RosCMS_temp_val;
		if ($RosCMS_temp_exist == false) {
//			echo ":S<hr />";
			break;
		}
		if ($RosCMS_temp_val == $val) {
			$RosCMS_temp_exist = false;
//			echo "<hr />!!! ".$key." => ".$val." !!!<hr />\n";
			break;
		}
	}
	return $RosCMS_temp_exist;
}

?>
