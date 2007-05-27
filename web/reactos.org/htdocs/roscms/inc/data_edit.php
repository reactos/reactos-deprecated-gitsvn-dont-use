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

	global $roscms_intern_webserver_roscms;
	global $roscms_intern_script_name;
	
	global $roscms_intern_account_id;
	global $roscms_intern_login_check_username;
	global $roscms_standard_language;
	
	global $RosCMS_GET_d_use;
	global $RosCMS_GET_d_flag;
	global $RosCMS_GET_d_id;
	global $RosCMS_GET_d_r_id;
	
	global $RosCMS_GET_d_name;
	global $RosCMS_GET_d_type;
	global $RosCMS_GET_d_r_lang;
	global $RosCMS_GET_d_template;
	
	global $RosCMS_GET_d_value;
	global $RosCMS_GET_d_value2;
	global $RosCMS_GET_d_value3;
	global $RosCMS_GET_d_value4;
	global $RosCMS_GET_d_value5;
	
	
	if ($RosCMS_GET_d_use == "mef") {
		include("inc/data_edit_tag.php");
		// Prevent caching:
		header("Content-type: text/html");
		header("Expires: Sun, 28 Jul 1996 05:00:00 GMT");    // Date in the past
		header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT"); 
															 // always modified
		header("Cache-Control: no-store, no-cache, must-revalidate");  // HTTP/1.1
		header("Cache-Control: post-check=0, pre-check=0", false);
		header("Pragma: no-cache");                          // HTTP/1.0

		//echo "<p>".$RosCMS_GET_d_flag.", ".$RosCMS_GET_d_id.", ".$RosCMS_GET_d_r_id."</p>";
	
	
		switch ($RosCMS_GET_d_flag) {
			default:
				show_edit_data();
				show_edit_data_tag();
				show_edit_data2();
				show_edit_data_form(true, true);
				break;
			case "newentry":  // create entry - interface
				newentryselect();
				break;
			case "newentry2": // advanced interface - submit
				echo newentryadd2(true);
				break;
			case "newentry3": // normal static interface - submit
				$temp_d_type = $RosCMS_GET_d_type;
				$temp_d_r_lang = $RosCMS_GET_d_r_lang;
				
				$RosCMS_GET_d_type = "page";
				$RosCMS_GET_d_r_lang = $roscms_standard_language;
				echo newentryadd2(false, "stable", $RosCMS_GET_d_template);
				
				echo "<hr />dazwischen: ".$temp_d_type."###".$temp_d_r_lang."#";
				
				$RosCMS_GET_d_type = $temp_d_type;
				$RosCMS_GET_d_r_lang = $temp_d_r_lang;	
				echo newentryadd2(true);
				break;
			case "addtag":
				tag_add($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $RosCMS_GET_d_value, $RosCMS_GET_d_value2, $RosCMS_GET_d_value3);
				show_edit_data_tag();
				break;
			case "deltag":
				tag_delete($RosCMS_GET_d_value, $RosCMS_GET_d_value2);
				show_edit_data_tag();
				break;
			case "changetag":
				tag_delete($RosCMS_GET_d_value4, $RosCMS_GET_d_value3);
				tag_add($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $RosCMS_GET_d_value /* name */, $RosCMS_GET_d_value2 /* value */, $RosCMS_GET_d_value3 /* usrid */);
				echo getTagId($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $roscms_intern_account_id /* usrid */, $RosCMS_GET_d_value /* name */);
				break;
			case "changetag2":
				//echo "<p>getTagId:".getTagId($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $RosCMS_GET_d_value3, $RosCMS_GET_d_value)."</p>";
				tag_delete(getTagId($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $RosCMS_GET_d_value3 /* usrid */, $RosCMS_GET_d_value /* name */), $RosCMS_GET_d_value3 /* usrid */);
				tag_add($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $RosCMS_GET_d_value /* name */, $RosCMS_GET_d_value2 /* value */, $RosCMS_GET_d_value3 /* usrid */);
				echo getTagId($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $RosCMS_GET_d_value3 /* usrid */, $RosCMS_GET_d_value /* name */);
				break;
			case "changetag3":
				$query_edit_mef_tag_max_tn = mysql_query("SELECT tn_id 
															FROM data_tag_name 
															WHERE tn_name = '".mysql_real_escape_string($RosCMS_GET_d_value)."'
															LIMIT 1;");
				$result_edit_mef_tag_max_tn = mysql_fetch_array($query_edit_mef_tag_max_tn);
				
				$query_edit_mef_tag_max = mysql_query("SELECT tag_id  
														FROM data_tag 
														WHERE data_id = '".mysql_real_escape_string($RosCMS_GET_d_id)."'
														AND data_rev_id = '".mysql_real_escape_string($RosCMS_GET_d_r_id)."'
														AND tag_name_id = '".mysql_real_escape_string($result_edit_mef_tag_max_tn['tn_id'])."' ;");
				while($result_edit_mef_tag_max = mysql_fetch_array($query_edit_mef_tag_max)) {
					tag_delete(getTagId($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $RosCMS_GET_d_value3 /* usrid */, $RosCMS_GET_d_value /* name */), $RosCMS_GET_d_value3 /* usrid */);
				}
				tag_add($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $RosCMS_GET_d_value /* name */, $RosCMS_GET_d_value2 /* value */, $RosCMS_GET_d_value3 /* usrid */);
				echo getTagId($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $RosCMS_GET_d_value3 /* usrid */, $RosCMS_GET_d_value /* name */);
				break;
			case "diff": // compare two entries
				echo diff_entries($RosCMS_GET_d_value /* rev_id entry 1 */, $RosCMS_GET_d_value2 /* rev_id entry 2 */);
				break;
			case "changetags":
				changetags($RosCMS_GET_d_value /* entry counter */, $RosCMS_GET_d_value2 /* entry rev_id's */, $RosCMS_GET_d_value3 /* entry flag */);
				break;
		}
	}
	
	
	function newentryselect() {
		echo "<div id=\"frmadd\" style=\"border-bottom: 1px solid #bbb; border-bottom-width: 1px; border-right: 1px solid #bbb; border-right-width: 1px; background: #FFFFFF none repeat scroll 0%;\">";
		echo "<div style=\"margin:10px;\">";

		echo '<br /><label for="txtaddinterface">Interface:</label><br />';
		echo '<select id="txtaddinterface" onchange="changecreateinterface()">';
		echo '	<option value="wizard">Page &amp; Content</option>';
		echo '	<option value="advanced">Single Entry</option>';
		echo '</select><br /><br />';

		echo '<div id="addwizard" style="display: block;">';
		
		echo '<br /><label for="txtaddkind">Kind:</label><br />';
		echo '<select id="txtaddkind" onchange="changecreatekind()">';
		echo '	<option value="static">Default</option>';
		echo '	<option value="dynamic">Dynamic Entry (news, newsletter, etc.)</option>';
		echo '</select><br />';

		echo '<div id="addwstatic" style="display: block;">';
		echo '<br /><label for="txtaddentryname2">Name:</label><br />';
		echo '<input type="text" id="txtaddentryname2" /><br /><br />';
		
		echo '<label for="txtaddtemplate">Template:</label><br />';
		echo '<select id="txtaddtemplate">';
		$query_templates_stable = mysql_query("SELECT d.data_name, r.rev_id
								FROM data_revision r, data_ d 
								WHERE r.rev_version > 0 
								AND r.data_id = d.data_id 
								AND d.data_type = 'template' 
								ORDER BY d.data_name ASC;");
		while ($result_templates_stable = mysql_fetch_array($query_templates_stable)) {
			echo '	<option value="'. $result_templates_stable['rev_id'] .'">'. $result_templates_stable['data_name'] .'</option>';
		}
		echo '</select>';
		echo '</div>'; /* end of wizard static */
		
		echo '<div id="addwdynamic" style="display: none;">';
		echo '<br /><label for="txtadddynsource">Source:</label><br />';
		echo '<select id="txtadddynsource">';
		echo '	<option value="news_page">News</option>';
		echo '	<option value="newsletter">Newsletter</option>';
		echo '	<option value="interview">Interview</option>';
		echo '</select>';
		echo '</div>'; /* end of wizard dynamic */
		
		echo '</div>'; /* end of wizard mode */

		
		echo '<div id="addadvanced" style="display: none;">';
		
		echo '<br /><label for="txtaddentryname">Name:</label><br />';
		echo '<input type="text" id="txtaddentryname" /><br /><br />';
		
		echo '<label for="txtaddentrytype">Type:</label><br />';
		echo '<select id="txtaddentrytype">';
		echo '	<option value="page">Page</option>';
		echo '	<option value="content">Content</option>';
		echo '	<option value="dyncontent">Dynamic Content</option>';
		echo '	<option value="template">Template</option>';
		echo '	<option value="script">Script</option>';
		echo '</select><br /><br />';

		
        echo '<label for="txtaddentrylang">Language</label><br />';
        echo '<select id="txtaddentrylang">';
		
			$query_language = mysql_query("SELECT * 
											FROM languages
											WHERE lang_level > '0'
											ORDER BY lang_name ASC ;");
			while($result_language=mysql_fetch_array($query_language)) {
				echo '<option value="';
				/*if ($result_language['lang_level'] == '10') {
					echo "all";
				}
				else {*/
					echo $result_language['lang_id'];
				//}
				echo '">'.$result_language['lang_name'].'</option>';
				
			}
			
		echo '</select>';
		echo "</div>"; /* end of advanced mode */

		echo "<br /><br /><button type=\"button\" onclick=\"createentry()\">Create</button> ";

		echo "</div>";
		echo "</div>";
	}
		
	function newentryadd2($tmp_show_output = false, $tmp_entry_status = "draft", $tmp_layout_template = "") {
		global $RosCMS_GET_d_name;
		global $RosCMS_GET_d_type;
		global $RosCMS_GET_d_r_lang;

		global $RosCMS_GET_d_id;
		global $RosCMS_GET_d_r_id;
		global $roscms_intern_account_id;
		
		
		$query_mef_addnew_data = mysql_query("SELECT * 
												FROM data_ 
												WHERE data_name = '".mysql_real_escape_string($RosCMS_GET_d_name)."'
												AND data_type = '".mysql_real_escape_string($RosCMS_GET_d_type)."'
												LIMIT 1;");
		$result_mef_addnew_data = mysql_fetch_array($query_mef_addnew_data);
		
		if ($result_mef_addnew_data['data_id'] == "") {
			$insert_data_new = mysql_query("INSERT INTO data_ ( data_id , data_name , data_type ) 
											VALUES (
												NULL , 
												'".mysql_real_escape_string($RosCMS_GET_d_name)."', 
												'".mysql_real_escape_string($RosCMS_GET_d_type)."'
											);");
											
			$query_mef_addnew_data = "";
			$result_mef_addnew_data = "";
			$query_mef_addnew_data = mysql_query("SELECT * 
													FROM data_ 
													WHERE data_name = '".mysql_real_escape_string($RosCMS_GET_d_name)."'
													AND data_type = '".mysql_real_escape_string($RosCMS_GET_d_type)."'
													LIMIT 1;");
			$result_mef_addnew_data = mysql_fetch_array($query_mef_addnew_data);
		}
		
		$query_mef_addnew_data_rev = mysql_query("SELECT COUNT(*)
												FROM data_revision
												WHERE data_id = '".mysql_real_escape_string($result_mef_addnew_data['data_id'])."'
												AND rev_language = '".mysql_real_escape_string($RosCMS_GET_d_r_lang)."'
												LIMIT 1;");
		$result_mef_addnew_data_rev = mysql_fetch_row($query_mef_addnew_data_rev);
		
		if ($result_mef_addnew_data_rev[0] <= 0) {
			// revision entry doesn't exist
			$insert_data_save = mysql_query("INSERT INTO data_revision ( rev_id , data_id , rev_version , rev_language , rev_usrid , rev_datetime , rev_date , rev_time ) 
												VALUES (
													NULL , 
													'".mysql_real_escape_string($result_mef_addnew_data['data_id'])."', 
													'0', 
													'".mysql_real_escape_string($RosCMS_GET_d_r_lang)."', 
													'".mysql_real_escape_string($roscms_intern_account_id)."', 
													NOW( ),
													CURDATE( ),
													CURTIME( )
												);");
			$query_data_save2 = mysql_query("SELECT rev_id
														FROM data_revision
														WHERE data_id = '".mysql_real_escape_string($result_mef_addnew_data['data_id'])."'
														AND rev_version = '0'
														AND rev_language = '".mysql_real_escape_string($RosCMS_GET_d_r_lang)."'
														AND rev_usrid = '".mysql_real_escape_string($roscms_intern_account_id)."'
														ORDER BY rev_datetime DESC;");
			$result_data_save2 = mysql_fetch_array($query_data_save2);
			
			
			if ($RosCMS_GET_d_type == "page") {
				echo "<h3>PAAAAGGGGEEE!!!</h3>";
				$temp_sql_type = ", (
														NULL , 
														'".mysql_real_escape_string($result_data_save2['rev_id'])."', 
														'".mysql_real_escape_string('extention')."', 
														'".mysql_real_escape_string('html')."'
													),
													(
														NULL , 
														'".mysql_real_escape_string($result_data_save2['rev_id'])."', 
														'".mysql_real_escape_string('titel')."', 
														'".mysql_real_escape_string(ucfirst($RosCMS_GET_d_name))."'
													)";
			}
			else {
				$temp_sql_type = "";
			}
			
			$insert_data_save_stext = mysql_query("INSERT INTO data_stext ( stext_id , data_rev_id , stext_name , stext_content ) 
													VALUES 
													(
														NULL , 
														'".mysql_real_escape_string($result_data_save2['rev_id'])."', 
														'".mysql_real_escape_string('description')."', 
														'".mysql_real_escape_string(' ')."'
													)
													". $temp_sql_type ."
													;");
			
			if ($tmp_layout_template != "") {
				$temp_sql_content = " '[#templ_".mysql_real_escape_string($tmp_layout_template)."]' ";
			}
			else {
				$temp_sql_content = " ' ' ";
			}

													
			$insert_data_save_text = mysql_query("INSERT INTO data_text ( text_id , data_rev_id , text_name , text_content ) 
													VALUES (
														NULL , 
														'".mysql_real_escape_string($result_data_save2['rev_id'])."', 
														'".mysql_real_escape_string('content')."', 
													". $temp_sql_content ."
													);");
			//echo $result_data_save2['rev_id'];
			
			tag_add($result_mef_addnew_data['data_id'], $result_data_save2['rev_id'], "status", $tmp_entry_status, "-1");
			
			$RosCMS_GET_d_id = $result_mef_addnew_data['data_id'];
			$RosCMS_GET_d_r_id = $result_data_save2['rev_id'];
			
			if ($tmp_show_output == true) {
				show_edit_data();
				show_edit_data_tag();
				show_edit_data2();
				show_edit_data_form(true, true);
			}
			
			echo "<p>RosCMS_GET_d_template: ".$tmp_layout_template." ; show_output: ".$tmp_show_output."</p>";

			
		}
		else {
			if ($tmp_show_output == true) {
				// nothing todo, even revision entry is already there
				echo "<p>Entry already exist.</p>";
				newentryselect();
			}
		}
		
		//echo "<p>".$RosCMS_GET_d_name.", ".$RosCMS_GET_d_type.", ".$RosCMS_GET_d_r_lang."</p>";
		
		return "";
	}

	function diff_entries($diff1, $diff2) {
		// @TODO: add short text and optional long text additional entries
		
		$query_diff1_data = mysql_query("SELECT d.data_id, d.data_name, d.data_type, r.rev_id, r.rev_version, r.rev_language, r.rev_datetime, u.user_name 
											FROM data_ d, data_revision r, users u
											WHERE r.data_id = d.data_id 
											AND r.rev_id = '".mysql_real_escape_string($diff1)."'
											AND r.rev_usrid = u.user_id
											LIMIT 1;");
		$result_diff1_data = mysql_fetch_array($query_diff1_data);
		$result_diff1_data_text = mysql_query("SELECT t.text_name, t.text_content 
													FROM data_revision r, data_text t
													WHERE r.rev_id = t.data_rev_id
													AND r.rev_id = '".mysql_real_escape_string($diff1)."'
													AND t.text_name = 'content' 
													ORDER BY text_name ASC;");
		$result_diff1_data_text = mysql_fetch_array($result_diff1_data_text);
		
	
		$query_diff2_data = mysql_query("SELECT d.data_id, d.data_name, d.data_type, r.rev_id, r.rev_version, r.rev_language, r.rev_datetime, u.user_name 
											FROM data_ d, data_revision r, users u
											WHERE r.data_id = d.data_id 
											AND r.rev_id = '".mysql_real_escape_string($diff2)."'
											AND r.rev_usrid = u.user_id
											LIMIT 1;");
		$result_diff2_data = mysql_fetch_array($query_diff2_data);
		$result_diff2_data_text = mysql_query("SELECT t.text_name, t.text_content 
													FROM data_revision r, data_text t
													WHERE r.rev_id = t.data_rev_id
													AND r.rev_id = '".mysql_real_escape_string($diff2)."'
													AND t.text_name = 'content' 
													ORDER BY text_name ASC;");
		$result_diff2_data_text = mysql_fetch_array($result_diff2_data_text);
		
		echo "<div style=\"display: block; border-bottom: 1px solid #bbb; border-bottom-width: 1px; border-right: 1px solid #bbb; border-right-width: 1px; background: #FFFFFF none repeat scroll 0%;\"><div style=\"margin:10px;\">";
		echo "<table width=\"100%\" border=\"0\"><tr><td>";
		echo "<b>".$result_diff1_data['data_name']."</b><ul><li>ID: ".$result_diff1_data['rev_id']."</li><li>Type: ".$result_diff1_data['data_type']."</li><li>Version: ".$result_diff1_data['rev_version']."</li><li>Language: ".$result_diff1_data['rev_language']."</li><li>User: ".$result_diff1_data['user_name']."</li></ul>";
		echo "</td><td>";
		echo "<b>".$result_diff2_data['data_name']."</b><ul><li>ID: ".$result_diff2_data['rev_id']."</li><li>Type: ".$result_diff2_data['data_type']."</li><li>Version: ".$result_diff2_data['rev_version']."</li><li>Language: ".$result_diff2_data['rev_language']."</li><li>User: ".$result_diff2_data['user_name']."</li></ul>";
		echo "</td></tr></table>";
		echo "</div></div>";
		echo "<div id=\"frmeditdiff1\" style=\"display: none;\">".$result_diff1_data_text['text_content']."</div>";
		echo "<div id=\"frmeditdiff2\" style=\"display: none; \">".$result_diff2_data_text['text_content']."</div>";
		echo "<br />";
		echo "<div id=\"frmeditdiff\" style=\"display: block; border-bottom: 1px solid #bbb; border-bottom-width: 1px; border-right: 1px solid #bbb; border-right-width: 1px; background: #F2F2F2 none repeat scroll 0%;\"><div style=\"margin:10px;\">&nbsp;</div>";	
	}
	
	function changetags($entr_count, $entr_revid, $entr_flag) {
		global $roscms_intern_account_id;
		
		echo "<p>COUNT: ".$entr_count."; IDs: ".$entr_revid."</p>";
		
		if ($entr_count > 0) {
			$entry_ids = split("-", $entr_revid);
			
			
			if ($entr_flag == "mp") {
				$entry_ids2 = split("_", $entry_ids[0]);
				echo "===>".$entry_ids2[1];
			
				include("inc/data_export_page.php");
				$query_preview_page = mysql_query("SELECT d.data_name, d.data_id, r.rev_id, r.rev_language 
													FROM data_revision r, data_ d
													WHERE r.rev_id = '".mysql_real_escape_string($entry_ids2[1])."'
													AND r.data_id = d.data_id 
													LIMIT 1;");
				$result_preview_page = mysql_fetch_array($query_preview_page);
				
				$temp_dynamic = getTagValue($result_preview_page['data_id'], $result_preview_page['rev_id'],  '-1', 'number');
				echo "<h1>!! preview !!</h1>\n";
				echo generate_page($result_preview_page['data_name'], $result_preview_page['rev_language'], $temp_dynamic, "show");
			
			}
			else {
				echo "<ul>";
				for ($i=0; $i < count($entry_ids); $i++) {
				
					$entry_ids2 = split("_", $entry_ids[$i]);
					echo "<li>".$entry_ids2[0]." == ".$entry_ids2[1]."</li>";
					
					$query_rev_data = mysql_query("SELECT * 
														FROM data_revision 
														WHERE rev_id = '".mysql_real_escape_string($entry_ids2[1])."' 
														LIMIT 1;");
					$result_rev_data = mysql_fetch_array($query_rev_data);
	
	
					$t_tagid = "";
					
					// 'as': add star
					// 'xs': remove star
					if ($entr_flag == "as" || $entr_flag == "xs") {
						$t_tagid = getTagId($result_rev_data['data_id'], $result_rev_data['rev_id'], $roscms_intern_account_id, "star");
					}
					// 'ms': mark as stable
					// 'mn': mark as new
					else if ($entr_flag == "ms" || $entr_flag == "mn") {
						$t_tagid = getTagId($result_rev_data['data_id'], $result_rev_data['rev_id'], "-1", "status");
					}
					
					if ($t_tagid != "") {
						tag_delete($t_tagid, $roscms_intern_account_id);
					}
					
					if ($entr_flag == "as") {
						tag_add($result_rev_data['data_id'], $result_rev_data['rev_id'], "star" /* name */, "on" /* value */, $roscms_intern_account_id /* usrid */);
					}
					// else (remove star) do nothing ;)
					
					if ($entr_flag == "ms") {
						// @todo: check that only one entry per name is stable, promt a decicion-box, if an entry should be moved to archive
						tag_add($result_rev_data['data_id'], $result_rev_data['rev_id'], "status" /* name */, "stable" /* value */, "-1" /* usrid */);
					}
					else if ($entr_flag == "mn") {
						tag_add($result_rev_data['data_id'], $result_rev_data['rev_id'], "status" /* name */, "new" /* value */, "-1" /* usrid */);
					}
					
					
					// 'ms': mark as stable
					if ($entr_flag == "ms") {
						$update_rev_ver = mysql_query("UPDATE data_revision SET rev_version = '10' WHERE rev_id = '".mysql_real_escape_string($result_rev_data['rev_id'])."' LIMIT 1;");
					}
					
					// 'mn': mark as new
					if ($entr_flag == "mn") {
						$update_rev_ver = mysql_query("UPDATE data_revision SET rev_version = '0' WHERE rev_id = '".mysql_real_escape_string($result_rev_data['rev_id'])."' LIMIT 1;");
					}
	
					// 'va': move to archive
					if ($entr_flag == "va") {
						//
					}
					
					// 'xe': delete entries
					if ($entr_flag == "xe") {
						deleteRevision($result_rev_data['rev_id']);
					}
				}
				echo "</ul>";
			}
		}
	}
	
	function show_edit_data() {
		global $RosCMS_GET_d_id;
		global $RosCMS_GET_d_r_id;
		global $roscms_intern_account_id;
		
		// Database Entry
		$query_edit_mef_data = mysql_query("SELECT d.data_id, d.data_name, d.data_type, r.rev_id, r.rev_version, r.rev_language, r.rev_datetime, u.user_name 
											FROM data_ d, data_revision r, users u
											WHERE r.data_id = d.data_id 
											AND r.rev_id = '".mysql_real_escape_string($RosCMS_GET_d_r_id)."'
											AND r.data_id = '".mysql_real_escape_string($RosCMS_GET_d_id)."'
											AND r.rev_usrid = u.user_id
											LIMIT 1;");
		$result_edit_mef_data = mysql_fetch_array($query_edit_mef_data);
		echo "<div style=\"padding-bottom: 3px;\"><span class=\"frmeditheader\">";
		echo "<span onclick=\"bchangestar(".$RosCMS_GET_d_id.",".$RosCMS_GET_d_r_id.",'star','addtagn', '".$roscms_intern_account_id."', 'editstar')\" style=\"cursor: pointer;\">";
		echo "<img id=\"editstar\" class=\"".getTagId($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $roscms_intern_account_id, 'star')."\" src=\"images/star_".getTagValue($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $roscms_intern_account_id, 'star')."_small.gif\" alt=\"\" style=\"width:13px; height:13px; border:0px;\" /></span>";
		echo "&nbsp;".$result_edit_mef_data['data_name'];
			$temp_dynamic = getTagValue($RosCMS_GET_d_id, $RosCMS_GET_d_r_id,  '-1', 'number');
			if ($result_edit_mef_data['data_type'] == "content" && $temp_dynamic != "") {
				echo "_".$temp_dynamic;
			}
		echo "</span> &nbsp; type: <span class=\"frmeditheader\">".$result_edit_mef_data['data_type']."</span> &nbsp; ";
		echo "version: <span  id=\"mefrverid\" class=\"frmeditheader\">".$result_edit_mef_data['rev_version']."</span> &nbsp; ";
		echo "language: <span id=\"mefrlang\" class=\"frmeditheader\">".$result_edit_mef_data['rev_language']."</span> &nbsp; ";
		echo "user: <span id=\"mefrusrid\" class=\"frmeditheader\">".$result_edit_mef_data['user_name']."</span> &nbsp; ";
		echo "<span id=\"frmedittags\" class=\"filterbutton\" onclick=\"TabOpenClose(this.id)\"><img id=\"frmedittagsi\" src=\"images/tab_closed.gif\" alt=\"\" style=\"width:11px; height:11px; border:0px;\" />&nbsp;Details</span></div>";
		
		
		echo "<div id=\"frmedittagsc\" style=\"display: none; border-bottom: 1px solid #bbb; border-bottom-width: 1px; border-right: 1px solid #bbb; border-right-width: 1px; background: #F2F2F2 none repeat scroll 0%;\"><div style=\"margin:10px;\" id=\"frmedittagsc2\">";
	}
	
	function show_edit_data_tag() {
		global $RosCMS_GET_d_id;
		global $RosCMS_GET_d_r_id;
		global $roscms_intern_account_id;
		global $RosCMS_GET_d_flag;
	
			// Tags
			$t_counter_etagusr = "";
			$query_edit_mef_data_tag = mysql_query("SELECT a.tag_id, a.tag_usrid, n.tn_name, v.tv_value 
													FROM data_ d, data_revision r, data_tag a, data_tag_name n, data_tag_value v
													WHERE (a.data_id = '0' OR (
																a.data_id = '".mysql_real_escape_string($RosCMS_GET_d_id)."'
																AND a.data_id = d.data_id
															)
														)
													AND (a.data_rev_id = '0' OR (
																a.data_rev_id = '".mysql_real_escape_string($RosCMS_GET_d_r_id)."'
																AND a.data_rev_id = r.rev_id
															)
														)
													AND (a.tag_usrid = '-1' OR a.tag_usrid = '0' OR a.tag_usrid = '".mysql_real_escape_string($roscms_intern_account_id)."')
													AND a.tag_name_id = n.tn_id 
													AND a.tag_value_id  = v.tv_id 
													ORDER BY tag_usrid ASC, tn_name ASC;");
			while($result_edit_mef_data_tag = mysql_fetch_array($query_edit_mef_data_tag)) {
				if ($result_edit_mef_data_tag['tag_usrid'] != $t_counter_etagusr) {
					if ($result_edit_mef_data_tag['tag_usrid'] == "-1") {
						echo "<br /><div class=\"frmeditheadline\">System Tags</div>";
					}
					else if ($result_edit_mef_data_tag['tag_usrid'] == "0") {
						echo "<br /><div class=\"frmeditheadline\">Tags</div>";
					}
					else if ($result_edit_mef_data_tag['tag_usrid'] == $roscms_intern_account_id) {
						echo "<br /><div class=\"frmeditheadline\">myTags</div>";
					}
				}
				$t_counter_etagusr = $result_edit_mef_data_tag['tag_usrid'];
				echo "<b>".ucfirst($result_edit_mef_data_tag['tn_name']).":</b>&nbsp;".$result_edit_mef_data_tag['tv_value'];
				echo "&nbsp;&nbsp;<span class=\"filterbutton\" onclick=\"bdeltag(".$RosCMS_GET_d_id.",".$RosCMS_GET_d_r_id.",'".$result_edit_mef_data_tag['tag_id']."', '".$roscms_intern_account_id."')\"><img src=\"images/remove.gif\" alt=\"\" style=\"width:11px; height:11px; border:0px;\" />&nbsp;Delete</span><br />";
			}
			echo "<br /><label for=\"addtagn\"><b>Tag:</b></label>&nbsp;";
			echo "<input type=\"text\" id=\"addtagn\" size=\"20\" maxlength=\"250\" value=\"\" />&nbsp;<button type=\"button\" onclick=\"";
			echo "baddtag(".$RosCMS_GET_d_id.",".$RosCMS_GET_d_r_id.",'tag','addtagn', '".$roscms_intern_account_id."')";
			echo "\">Add</button><br /><br />";
			
			
			echo "<br /><label for=\"addtags1\"><b>SysTag:</b></label>&nbsp;";
			echo "<input type=\"text\" id=\"addtags1\" size=\"20\" maxlength=\"250\" value=\"\" />&nbsp;";
			echo "<input type=\"text\" id=\"addtags2\" size=\"20\" maxlength=\"250\" value=\"\" /> &nbsp;";
			
			echo "<button type=\"button\" onclick=\"";
			echo "baddtag(".$RosCMS_GET_d_id.",".$RosCMS_GET_d_r_id.",'addtags1','addtags2','0')";
			echo "\">Add ALL-USER</button> ";
			
			echo "<button type=\"button\" onclick=\"";
			echo "baddtag(".$RosCMS_GET_d_id.",".$RosCMS_GET_d_r_id.",'addtags1','addtags2','-1')";
			echo "\">Add SYSTEM</button><br />";
			
			echo "<br />";
			
	}
	
	function show_edit_data2() {
		echo "</div></div>";
	}
	
	function show_edit_data_form($RosCMS_intern_data_edit_stext, $RosCMS_intern_data_edit_text) {
		global $RosCMS_GET_d_id;
		global $RosCMS_GET_d_r_id;
		
		echo '<div style="background:#FFFFFF; border-bottom: 1px solid #bbb; border-bottom-width: 1px; border-right: 1px solid #bbb; border-right-width: 1px;"><div style="margin:10px;"><div style="width:95%;">';
		echo '<form method="post" action="http://tinymce.moxiecode.com/dump.php?example=true"><br />';
	
	
		if ($RosCMS_intern_data_edit_stext == true) {
			// Short Text
			$t_counter_stext = 0;
			$query_edit_mef_data_stext = mysql_query("SELECT s.stext_name, s.stext_content 
														FROM data_revision r, data_stext s
														WHERE r.rev_id = s.data_rev_id
														AND r.rev_id = '".mysql_real_escape_string($RosCMS_GET_d_r_id)."'
														ORDER BY stext_name ASC;");
			while($result_edit_mef_data_stext = mysql_fetch_array($query_edit_mef_data_stext)) {
				$t_counter_stext++;
				echo "<label for=\"estext".$t_counter_stext."\" class=\"frmeditheadline\"><b>".ucfirst($result_edit_mef_data_stext['stext_name']).":</b></label><span id=\"edstext".$t_counter_stext."\" style=\"display:none;\">".$result_edit_mef_data_stext['stext_name']."</span><br />";
				echo "<input name=\"estext".$t_counter_stext."\" type=\"text\" id=\"estext".$t_counter_stext."\" size=\"50\" maxlength=\"250\" value=\"".$result_edit_mef_data_stext['stext_content']."\" /><br /><br />";
			}
			echo '<span id="entrydataid" class="'.$RosCMS_GET_d_id.'">&nbsp;</span><span id="entrydatarevid" class="'.$RosCMS_GET_d_r_id.'">&nbsp;</span><div id="estextcount" class="'.$t_counter_stext.'">&nbsp;</div>';
		
		}
		
		if ($RosCMS_intern_data_edit_text == true) {
			// Text
			$t_counter_text = 0;
			$query_edit_mef_data_text = mysql_query("SELECT t.text_name, t.text_content 
														FROM data_revision r, data_text t
														WHERE r.rev_id = t.data_rev_id
														AND r.rev_id = '".mysql_real_escape_string($RosCMS_GET_d_r_id)."'
														ORDER BY text_name ASC;");
			while($result_edit_mef_data_text = mysql_fetch_array($query_edit_mef_data_text)) {
				$t_counter_text++;
				echo '<label class="frmeditheadline" for="elm'.$t_counter_text.'">'.ucfirst($result_edit_mef_data_text['text_name']).'</label>';
				echo "<button type=\"button\" id=\"butRTE".$t_counter_text."\" onclick=\"toggleEditor('elm".$t_counter_text."', this.id)\">Rich Text</button> <span id=\"swraped".$t_counter_text."\"><input id=\"wraped".$t_counter_text."\" type=\"checkbox\" onclick=\"toggleWordWrap(this.id, 'elm".$t_counter_text."');\" checked=\"checked\" /><label for=\"wraped".$t_counter_text."\">Word wrap</label></span><span id=\"edtext".$t_counter_text."\" style=\"display:none;\">".$result_edit_mef_data_text['text_name']."</span><br />";
				echo "<textarea name=\"elm".$t_counter_text."\" cols=\"80\" rows=\"15\" class=\"mceEditor\" id=\"elm".$t_counter_text."\" style=\"width: 100%; background-color:#FFFFFF;\" >".$result_edit_mef_data_text['text_content']."</textarea><br /><br />";
			}
			echo '<div id="elmcount" class="'.$t_counter_text.'">&nbsp;</div>';
		}
	
		echo '</form>';
		echo '<button type=\"button\" onclick="edit_form_submit('.$RosCMS_GET_d_id.','.$RosCMS_GET_d_r_id.')">Submit</button> &nbsp; ';
		echo '<button type=\"button\" onclick="edit_form_submit_draft('.$RosCMS_GET_d_id.','.$RosCMS_GET_d_r_id.')">Save Draft</button> &nbsp; ';
		echo '<button type=\"button\" disabled="disabled">Discard</button> &nbsp; '; /* @unimplemented: discard function */
		echo '&nbsp;&nbsp;<span id="mefasi"></div></div></div>';
	}
	
	/*
		<p>&nbsp;</p>
		<input type="button" value="Load" onClick="ajaxLoad();" />
		<input type="button" value="Save" onClick="ajaxSave();" />
		<input type="submit" name="save" value="Submit" />
		<input type="reset" name="reset" value="Reset" />
	*/
	
	
	function tag_add($RosCMS_GET_d_id, $RosCMS_GET_d_r_id, $RosCMS_GET_d_value, $RosCMS_GET_d_value2, $RosCMS_GET_d_value3) {
		global $roscms_intern_account_id;
		
//		echo "<h4>TAG_ADD: ".$RosCMS_GET_d_id.", ".$RosCMS_GET_d_r_id.", ".$RosCMS_GET_d_value.", ".$RosCMS_GET_d_value2.", ".$RosCMS_GET_d_value3." == ".$roscms_intern_account_id."</h4>";

		$RosCMS_intern_current_tag_name_id = "";
		$RosCMS_intern_current_tag_value_id = "";

		if ($RosCMS_GET_d_value3 == $roscms_intern_account_id || $RosCMS_GET_d_value3 == 0|| $RosCMS_GET_d_value3 == -1) { /* @unimplemented: account group membership check */

			// tag name
			$query_edit_mef_tag_add_tn = mysql_query("SELECT tn_id, tn_name 
														FROM data_tag_name
														WHERE tn_name = '".mysql_real_escape_string($RosCMS_GET_d_value)."'
														LIMIT 1;");
			$result_edit_mef_tag_add_tn = mysql_fetch_array($query_edit_mef_tag_add_tn);
			if ($result_edit_mef_tag_add_tn['tn_name'] == $RosCMS_GET_d_value) {
				$RosCMS_intern_current_tag_name_id = $result_edit_mef_tag_add_tn['tn_id'];
			}
			else {
				// add tag name:
				$insert_edit_mef_tag_add_tn = mysql_query("INSERT INTO data_tag_name ( tn_id , tn_name ) 
															VALUES ( NULL , '".mysql_real_escape_string($RosCMS_GET_d_value)."' );");
				
				$query_edit_mef_tag_add_tn2 = mysql_query("SELECT tn_id, tn_name 
															FROM data_tag_name 
															WHERE tn_name = '".mysql_real_escape_string($RosCMS_GET_d_value)."'
															LIMIT 1;");
				$result_edit_mef_tag_add_tn2 = mysql_fetch_array($query_edit_mef_tag_add_tn2);
				if ($result_edit_mef_tag_add_tn2['tn_name'] == $RosCMS_GET_d_value) {
					$RosCMS_intern_current_tag_name_id = $result_edit_mef_tag_add_tn2['tn_id'];
				}
				else {
					die("ERROR: tag_add_name was not successful");
				}			
			}
	
//			echo "<p>TAG_NAME_ID: ".$RosCMS_intern_current_tag_name_id."</p>";
	
			// tag value
			$query_edit_mef_tag_add_tv = mysql_query("SELECT tv_id, tv_value 
														FROM data_tag_value 
														WHERE tv_value = '".mysql_real_escape_string($RosCMS_GET_d_value2)."'
														LIMIT 1;");
			$result_edit_mef_tag_add_tv = mysql_fetch_array($query_edit_mef_tag_add_tv);
			if ($result_edit_mef_tag_add_tv['tv_value'] == $RosCMS_GET_d_value2) {
				$RosCMS_intern_current_tag_value_id = $result_edit_mef_tag_add_tv['tv_id'];
			}
			else {
				// add tag value:
				$insert_edit_mef_tag_add_tv = mysql_query("INSERT INTO data_tag_value ( tv_id , tv_value ) 
															VALUES ( NULL , '".mysql_real_escape_string($RosCMS_GET_d_value2)."' );");
				
				$query_edit_mef_tag_add_tv2 = mysql_query("SELECT tv_id, tv_value 
															FROM data_tag_value
															WHERE tv_value = '".mysql_real_escape_string($RosCMS_GET_d_value2)."'
															LIMIT 1;");
				$result_edit_mef_tag_add_tv2 = mysql_fetch_array($query_edit_mef_tag_add_tv2);
				if ($result_edit_mef_tag_add_tv2['tv_value'] == $RosCMS_GET_d_value2) {
					$RosCMS_intern_current_tag_value_id = $result_edit_mef_tag_add_tv2['tv_id'];
				}
				else {
					die("ERROR: tag_add_value was not successful");
				}			
			}
//			echo "<p>TAG_VALUE_ID: ".$RosCMS_intern_current_tag_value_id."</p>";

			// tag
			$query_edit_mef_tag_add = mysql_query("SELECT COUNT('tag_id') 
													FROM data_tag 
													WHERE data_id = '".mysql_real_escape_string($RosCMS_GET_d_id)."'
													AND data_rev_id = '".mysql_real_escape_string($RosCMS_GET_d_r_id)."'
													AND tag_name_id = '".mysql_real_escape_string($RosCMS_intern_current_tag_name_id)."'
													AND tag_value_id = '".mysql_real_escape_string($RosCMS_intern_current_tag_value_id)."'
													AND tag_usrid = '".mysql_real_escape_string($RosCMS_GET_d_value3)."' ;");
			$result_edit_mef_tag_add = mysql_fetch_row($query_edit_mef_tag_add);
			
//			echo "<p>ANZAHL_TAGS: ".$result_edit_mef_tag_add[0]."</p>";
			
			if ($result_edit_mef_tag_add[0] <= 0) {
				$insert_edit_mef_tag_add = mysql_query("INSERT INTO data_tag ( tag_id , data_id , data_rev_id , tag_name_id , tag_value_id , tag_usrid ) 
														VALUES (NULL , 
														'".mysql_real_escape_string($RosCMS_GET_d_id)."',
														'".mysql_real_escape_string($RosCMS_GET_d_r_id)."',
														'".mysql_real_escape_string($RosCMS_intern_current_tag_name_id)."',
														'".mysql_real_escape_string($RosCMS_intern_current_tag_value_id)."',
														'".mysql_real_escape_string($RosCMS_GET_d_value3)."');");
/*				echo "INSERT INTO data_tag ( tag_id , data_id , data_rev_id , tag_name_id , tag_value_id , tag_usrid ) 
														VALUES (NULL , 
														'".mysql_real_escape_string($RosCMS_GET_d_id)."',
														'".mysql_real_escape_string($RosCMS_GET_d_r_id)."',
														'".mysql_real_escape_string($RosCMS_intern_current_tag_name_id)."',
														'".mysql_real_escape_string($RosCMS_intern_current_tag_value_id)."',
														'".mysql_real_escape_string($RosCMS_GET_d_value3)."');";
*/
			}
		}
	}
	
	function tag_delete($RosCMS_GET_d_value, $RosCMS_GET_d_value2) {
		global $roscms_intern_account_id;

		//echo "<h4>".$RosCMS_GET_d_value.", ".$RosCMS_GET_d_value2." # ".$roscms_intern_account_id."</h4>";

		$RosCMS_intern_current_tag_name_id = "";
		$RosCMS_intern_current_tag_value_id = "";
		
		if ($RosCMS_GET_d_value2 == $roscms_intern_account_id || $RosCMS_GET_d_value2 == 0|| $RosCMS_GET_d_value2 == -1) { /* @unimplemented: account group membership check */

			// tag
			$query_edit_mef_tag_del = mysql_query("SELECT tag_name_id, tag_value_id
													FROM data_tag 
													WHERE tag_id = '".mysql_real_escape_string($RosCMS_GET_d_value)."'
													LIMIT 1;");
			$result_edit_mef_tag_del = mysql_fetch_array($query_edit_mef_tag_del);
			
			// tag name
			$query_edit_mef_tag_del_tn = mysql_query("SELECT COUNT('tag_name_id')
														FROM data_tag 
														WHERE tag_name_id = '".mysql_real_escape_string($result_edit_mef_tag_del['tag_name_id'])."';");
			$result_edit_mef_tag_del_tn = mysql_fetch_row($query_edit_mef_tag_del_tn);
			if ($result_edit_mef_tag_del_tn[0] <= 1) {
				$delete_edit_mef_tag_del_tn = mysql_query("DELETE FROM data_tag_name
															WHERE tn_id = '".mysql_real_escape_string($result_edit_mef_tag_del['tag_name_id'])."'
															LIMIT 1;");
			}
	
			// tag value
			$query_edit_mef_tag_del_tv = mysql_query("SELECT COUNT('tag_value_id')
														FROM data_tag 
														WHERE tag_value_id = '".mysql_real_escape_string($result_edit_mef_tag_del['tag_value_id'])."';");
			$result_edit_mef_tag_del_tv = mysql_fetch_row($query_edit_mef_tag_del_tv);
			if ($result_edit_mef_tag_del_tv[0] <= 1) {
				$delete_edit_mef_tag_del_tv = mysql_query("DELETE FROM data_tag_value
															WHERE tv_id = '".mysql_real_escape_string($result_edit_mef_tag_del['tag_value_id'])."'
															LIMIT 1;");
			}
			
			// tag
			$delete_edit_mef_tag_del_tag = mysql_query("DELETE FROM data_tag
														WHERE tag_id = '".mysql_real_escape_string($RosCMS_GET_d_value)."'
														LIMIT 1;");
		}
	}	
		
		
	function deleteRevision($temprevid) {
		$delete_data_save_rev = mysql_query("DELETE FROM data_revision WHERE rev_id = '".mysql_real_escape_string($temprevid)."' LIMIT 1;");
		$delete_data_save_stext = mysql_query("DELETE FROM data_stext WHERE data_rev_id = '".mysql_real_escape_string($temprevid)."';");
		$delete_data_save_text = mysql_query("DELETE FROM data_text WHERE data_rev_id = '".mysql_real_escape_string($temprevid)."';");
		
		$query_delete_tags = mysql_query("SELECT * 
											FROM data_tag 
											WHERE data_rev_id = '".mysql_real_escape_string($temprevid)."' ;");
		while ($result_delete_tags = mysql_fetch_array($query_delete_tags)) {
			$delete_data_save_stext = mysql_query("DELETE FROM data_tag WHERE data_rev_id = '".mysql_real_escape_string($temprevid)."' LIMIT 1;");

			// delete tag name
			$query_delete_tag_name = mysql_query("SELECT * 
												FROM data_tag 
												WHERE tag_name_id = '".mysql_real_escape_string($result_delete_tags['tag_name_id'])."' ;");
			$result_delete_tag_name = mysql_fetch_row($query_delete_tag_name);
			if ($result_delete_tag_name[0] <= 0) {
				$delete_data_save_tn = mysql_query("DELETE FROM data_tag_name WHERE tn_id = '".mysql_real_escape_string($result_delete_tags['tag_name_id'])."' LIMIT 1;");
			}
			
			// delete tag value
			$query_delete_tag_value = mysql_query("SELECT * 
												FROM data_tag 
												WHERE tag_name_id = '".mysql_real_escape_string($result_delete_tags['tag_value_id'])."' ;");
			$result_delete_tag_value = mysql_fetch_row($query_delete_tag_value);
			if ($result_delete_tag_value[0] <= 0) {
				$delete_data_save_tv = mysql_query("DELETE FROM data_tag_value WHERE tn_id = '".mysql_real_escape_string($result_delete_tags['tag_value_id'])."' LIMIT 1;");
			}
		}		
	}
?>