<?php

	/* RosCMS v2 to v3 database convert script - part 1 */

	error_reporting(E_ALL);
	ini_set('error_reporting', E_ALL);
	
	if (get_magic_quotes_gpc()) {
		die("ERROR: Disable 'magic quotes' in php.ini (=Off)");
	}
	
	if ( !defined('ROSCMS_SYSTEM') ) {
		define ("ROSCMS_SYSTEM", "Version 0.1"); // to prevent hacking activity
	}


	require("connect.db.php");
	require("inc/data_edit.php");
	

	echo "<h1>RosCMS Database Update - v2 to v3 - Part 1</h1>";

		

	echo "<h3>Pages</h3>";
	echo "<ul>";
	$query_i_page = mysql_query("SELECT * 
									FROM pages
									WHERE page_active = 1
									ORDER BY page_name ASC, page_version DESC, page_active DESC;");
	while ($result_i_page = mysql_fetch_array($query_i_page)) {
		$roscms_i_lang = $result_i_page['page_language'];		
		if ($roscms_i_lang == "all") {
			$roscms_i_lang = "en";
		}
		
		echo "<li>".$result_i_page['page_name']." (".$roscms_i_lang.")</li>";
		
		$query_import_page = mysql_query("SELECT * 
												FROM data_ 
												WHERE data_name = '".mysql_real_escape_string($result_i_page['page_name'])."'
												AND data_type = 'page'
												LIMIT 1;");
		$result_import_page = mysql_fetch_array($query_import_page);
		//echo "<h6>data-ID: ".$result_import_page['data_id']."</h6>";
		
		if ($result_import_page['data_id'] == "") {
			$insert_page = mysql_query("INSERT INTO `data_` ( `data_id` , `data_name` , `data_type` ) 
											VALUES (
												NULL , 
												'".mysql_real_escape_string($result_i_page['page_name'])."', 
												'page'
											);");
		}
			
		$query_import_page2 = mysql_query("SELECT COUNT(*)
												FROM data_revision
												WHERE data_id = '".mysql_real_escape_string($result_import_page['data_id'])."'
												AND rev_language = '".mysql_real_escape_string($roscms_i_lang)."'
												LIMIT 1;");
		$result_import_page2 = mysql_fetch_row($query_import_page2);
		
		if ($result_import_page2[0] <= 0) {
			// revision entry doesn't exist
			$insert_page_rev = mysql_query("INSERT INTO data_revision ( rev_id , data_id , rev_version , rev_language , rev_usrid , rev_datetime , rev_date , rev_time ) 
												VALUES (
													NULL , 
													'".mysql_real_escape_string($result_import_page['data_id'])."', 
													'".mysql_real_escape_string($result_i_page['page_version'])."', 
													'".mysql_real_escape_string($roscms_i_lang)."', 
													'".mysql_real_escape_string($result_i_page['page_usrname_id'])."', 
													'".mysql_real_escape_string($result_i_page['page_date']." ".$result_i_page['page_time'])."',
													'".mysql_real_escape_string($result_i_page['page_date'])."',
													'".mysql_real_escape_string($result_i_page['page_time'])."'
												);");
												
		
			$query_import_page4 = mysql_query("SELECT rev_id
													FROM data_revision
													WHERE data_id = '".mysql_real_escape_string($result_import_page['data_id'])."'
													AND rev_language = '".mysql_real_escape_string($roscms_i_lang)."'
													ORDER BY rev_id DESC
													LIMIT 1;");
			$result_import_page4 = mysql_fetch_array($query_import_page4);
			
			if ($result_import_page4['rev_id'] != "") {
			
				if ($result_i_page['pages_extention'] == "default") {
					$roscms_i_page_extention = "html";
				}
				else {
					$roscms_i_page_extention = $result_i_page['pages_extention'];
				}
				
				/*
					if ($result_i_page['pages_extra'] == "dynamic") {
						$roscms_i_page_extra = ", (
													NULL , 
													'".mysql_real_escape_string($result_import_page4['rev_id'])."', 
													'property', 
													'".mysql_real_escape_string($result_i_page['pages_extra'])."'
												)";
					}
					else {
						$roscms_i_page_extra = "";
					}
					".$roscms_i_page_extra."
					,(
						NULL , 
						'".mysql_real_escape_string($result_import_page4['rev_id'])."', 
						'extention', 
						'".mysql_real_escape_string($roscms_i_page_extention)."'
					)
				*/

				// stext: titel, description, extention, (property)
				$insert_import_page_stext = mysql_query("INSERT INTO data_stext ( stext_id , data_rev_id , stext_name , stext_content ) 
														VALUES 
														(
															NULL , 
															'".mysql_real_escape_string($result_import_page4['rev_id'])."', 
															'description', 
															'".mysql_real_escape_string($result_i_page['page_description'])."'
														),
														(
															NULL , 
															'".mysql_real_escape_string($result_import_page4['rev_id'])."', 
															'title', 
															'".mysql_real_escape_string($result_i_page['page_title'])."'
														)
														;");
				
				// text: content
				$insert_import_page_text = mysql_query("INSERT INTO data_text ( text_id , data_rev_id , text_name , text_content ) 
														VALUES (
															NULL , 
															'".mysql_real_escape_string($result_import_page4['rev_id'])."', 
															'content', 
															'".mysql_real_escape_string($result_i_page['page_text'])."'
														);");
				
				// tag: status = stable
				tag_add($result_import_page['data_id'], $result_import_page4['rev_id'], "status" /* name */, "stable" /* value */, "-1" /* usrid */);

				if ($result_i_page['pages_extra'] == "dynamic") {
					// tag: kind = dynamic
					tag_add($result_import_page['data_id'], $result_import_page4['rev_id'], "kind" /* name */, "dynamic" /* value */, "-1" /* usrid */);
				}
				else {
					// tag: kind = default
					tag_add($result_import_page['data_id'], $result_import_page4['rev_id'], "kind" /* name */, "default" /* value */, "-1" /* usrid */);
				}

				if ($result_i_page['pages_extention'] == "default") {
					// tag: extention = html
					tag_add($result_import_page['data_id'], $result_import_page4['rev_id'], "extention" /* name */, "html" /* value */, "-1" /* usrid */);
				}
				else {
					// tag: extention = ...
					tag_add($result_import_page['data_id'], $result_import_page4['rev_id'], "extention" /* name */, $result_i_page['pages_extention'] /* value */, "-1" /* usrid */);
				}
			}
		}
	}
	echo "</ul>";


	echo "\n<hr />\n";


	echo "<h3>Contents</h3>";
	echo "<ul>";
	$query_i_content = mysql_query("SELECT * 
									FROM content
									WHERE content_active = 1
									AND content_lang != 'xhtml'
									ORDER BY content_lang ASC, content_name ASC, content_version DESC, content_active DESC;");
	while ($result_i_content = mysql_fetch_array($query_i_content)) {
		$roscms_i_lang = $result_i_content['content_lang'];		
		if ($roscms_i_lang == "all") {
			$roscms_i_lang = "en";
		}
		if ($roscms_i_lang == "html") {
			$roscms_i_lang = "en";
		}
		
		echo "<li>".$result_i_content['content_name']." (".$result_i_content['content_lang'].")</li>";
		
		$query_import_content = mysql_query("SELECT * 
												FROM data_ 
												WHERE data_name = '".mysql_real_escape_string($result_i_content['content_name'])."'
												AND data_type = 'content'
												LIMIT 1;");
		$result_import_content = mysql_fetch_array($query_import_content);
		
		
		if ($result_import_content['data_id'] == "") {
			$insert_page = mysql_query("INSERT INTO `data_` ( `data_id` , `data_name` , `data_type` ) 
											VALUES (
												NULL , 
												'".mysql_real_escape_string($result_i_content['content_name'])."', 
												'content'
											);");
		}
		
		$query_import_content3 = mysql_query("SELECT * 
												FROM data_ 
												WHERE data_name = '".mysql_real_escape_string($result_i_content['content_name'])."'
												AND data_type = 'content'
												ORDER BY data_id DESC
												LIMIT 1;");
		$result_import_content3 = mysql_fetch_array($query_import_content3);
		
		$query_import_content2 = mysql_query("SELECT COUNT(*)
												FROM data_revision
												WHERE data_id = '".mysql_real_escape_string($result_import_content3['data_id'])."'
												AND rev_language = '".mysql_real_escape_string($roscms_i_lang)."'
												LIMIT 1;");
		$result_import_content2 = mysql_fetch_row($query_import_content2);
		
		if ($result_import_content2[0] <= 0) {
			echo "<h6>lang: ".$result_i_content['content_lang']."</h6>";
			// revision entry doesn't exist
			$insert_content_rev = mysql_query("INSERT INTO data_revision ( rev_id , data_id , rev_version , rev_language , rev_usrid , rev_datetime , rev_date , rev_time ) 
												VALUES (
													NULL , 
													'".mysql_real_escape_string($result_import_content3['data_id'])."', 
													'".mysql_real_escape_string($result_i_content['content_version'])."', 
													'".mysql_real_escape_string($roscms_i_lang)."', 
													'".mysql_real_escape_string($result_i_content['content_usrname_id'])."', 
													'".mysql_real_escape_string($result_i_content['content_date']." ".$result_i_content['content_time'])."',
													'".mysql_real_escape_string($result_i_content['content_date'])."',
													'".mysql_real_escape_string($result_i_content['content_time'])."'
												);");
												
		
			$query_import_content4 = mysql_query("SELECT rev_id
													FROM data_revision
													WHERE data_id = '".mysql_real_escape_string($result_import_content3['data_id'])."'
													AND rev_language = '".mysql_real_escape_string($roscms_i_lang)."'
													ORDER BY rev_id DESC
													LIMIT 1;");
			$result_import_content4 = mysql_fetch_array($query_import_content4);
			
			if ($result_import_content4['rev_id'] != "") {
			
				// stext: titel, description, extention, (property)
				$insert_import_content_stext = mysql_query("INSERT INTO data_stext ( stext_id , data_rev_id , stext_name , stext_content ) 
														VALUES 
														(
															NULL , 
															'".mysql_real_escape_string($result_import_content4['rev_id'])."', 
															'description', 
															'".mysql_real_escape_string($result_i_content['content_name'])."'
														);");
				
				// text: content
				$insert_import_content_text = mysql_query("INSERT INTO data_text ( text_id , data_rev_id , text_name , text_content ) 
														VALUES (
															NULL , 
															'".mysql_real_escape_string($result_import_content4['rev_id'])."', 
															'content', 
															'".mysql_real_escape_string($result_i_content['content_text'])."'
														);");
				
				// tag: status = stable
				tag_add($result_import_content3['data_id'], $result_import_content4['rev_id'], "status", "stable", "-1");
				
				// tag: visible = all
				tag_add($result_import_content3['data_id'], $result_import_content4['rev_id'], "visible", "all", "-1");

				// tag: kind = default
				tag_add($result_import_content3['data_id'], $result_import_content4['rev_id'], "kind", "default", "-1");
			}
		}
	}
	echo "</ul>";
			
			
	echo "\n<hr />\n";



	echo "<h3>Scripts</h3>";
	echo "<ul>";
	$query_i_script = mysql_query("SELECT * 
									FROM include_text
									WHERE inc_vis  = '1'
									ORDER BY inc_word ASC;");
	while ($result_i_script = mysql_fetch_array($query_i_script)) {
		echo "<li>".$result_i_script['inc_word']."</li>";
		
		$query_import_script = mysql_query("SELECT * 
												FROM data_ 
												WHERE data_name = '".mysql_real_escape_string($result_i_script['inc_word'])."'
												AND data_type = 'script'
												LIMIT 1;");
		$result_import_script = mysql_fetch_array($query_import_script);
		
		
		if ($result_import_script['data_id'] == "") {
			$insert_page = mysql_query("INSERT INTO `data_` ( `data_id` , `data_name` , `data_type` ) 
											VALUES (
												NULL , 
												'".mysql_real_escape_string($result_i_script['inc_word'])."', 
												'script'
											);");
		}
		
		$query_import_script3 = mysql_query("SELECT * 
												FROM data_ 
												WHERE data_name = '".mysql_real_escape_string($result_i_script['inc_word'])."'
												AND data_type = 'script'
												ORDER BY data_id DESC
												LIMIT 1;");
		$result_import_script3 = mysql_fetch_array($query_import_script3);
		
		$query_import_script2 = mysql_query("SELECT COUNT(*)
												FROM data_revision
												WHERE data_id = '".mysql_real_escape_string($result_import_script3['data_id'])."'
												AND rev_language = 'en'
												LIMIT 1;");
		$result_import_script2 = mysql_fetch_row($query_import_script2);
		
		if ($result_import_script2[0] <= 0) {
			// revision entry doesn't exist
			$insert_script_rev = mysql_query("INSERT INTO data_revision ( rev_id , data_id , rev_version , rev_language , rev_usrid , rev_datetime , rev_date , rev_time ) 
												VALUES (
													NULL , 
													'".mysql_real_escape_string($result_import_script3['data_id'])."', 
													'1', 
													'en', 
													'".mysql_real_escape_string($result_i_script['inc_usrname_id'])."', 
													'".mysql_real_escape_string($result_i_script['inc_date']." ".$result_i_script['inc_time'])."',
													'".mysql_real_escape_string($result_i_script['inc_date'])."',
													'".mysql_real_escape_string($result_i_script['inc_time'])."'
												);");
			
												
		
			$query_import_script4 = mysql_query("SELECT rev_id
													FROM data_revision
													WHERE data_id = '".mysql_real_escape_string($result_import_script3['data_id'])."'
													AND rev_language = 'en'
													ORDER BY rev_id DESC
													LIMIT 1;");
			$result_import_script4 = mysql_fetch_array($query_import_script4);
			
			if ($result_import_script4['rev_id'] != "") {
			
				// stext: titel, description, extention, (property)
				$insert_import_script_stext = mysql_query("INSERT INTO data_stext ( stext_id , data_rev_id , stext_name , stext_content ) 
														VALUES 
														(
															NULL , 
															'".mysql_real_escape_string($result_import_script4['rev_id'])."', 
															'description', 
															'".mysql_real_escape_string($result_i_script['inc_word'])."'
														);");
				
				// text: content
				$insert_import_script_text = mysql_query("INSERT INTO data_text ( text_id , data_rev_id , text_name , text_content ) 
														VALUES (
															NULL , 
															'".mysql_real_escape_string($result_import_script4['rev_id'])."', 
															'content', 
															'".mysql_real_escape_string($result_i_script['inc_text'])."'
														);");
				
				// tag: status = stable
				tag_add($result_import_script3['data_id'], $result_import_script4['rev_id'], "status", "stable", "-1");
				
				// tag: visible = admin
				tag_add($result_import_script3['data_id'], $result_import_script4['rev_id'], "visible", "admin", "-1");
				
				if ($result_i_script['inc_extra'] == "template_php") {
					// tag: kind = php
					tag_add($result_import_script3['data_id'], $result_import_script4['rev_id'], "kind", "php", "-1");
				}
				else {
					// tag: kind = var
					tag_add($result_import_script3['data_id'], $result_import_script4['rev_id'], "kind", "var", "-1");
				}
			}
		}
	}
	echo "</ul>";


	echo "\n<hr />\n";




	echo "\n<hr />\n";


?>