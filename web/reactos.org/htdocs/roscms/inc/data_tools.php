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

	function get_stext($r_revid, $r_entr_name) {
		$query_stext = mysql_query("SELECT stext_content  
									FROM data_stext
									WHERE data_rev_id = '".mysql_real_escape_string($r_revid)."' 
									AND stext_name = '".mysql_real_escape_string($r_entr_name)."'
									ORDER BY stext_id DESC
									LIMIT 1;");
		$result_stext = mysql_fetch_array($query_stext);
		
		return $result_stext['stext_content'];
	}
	
	function get_text($r_revid, $r_entr_name) {
		$query_text = mysql_query("SELECT text_content  
									FROM data_text
									WHERE data_rev_id = '".mysql_real_escape_string($r_revid)."' 
									AND text_name = '".mysql_real_escape_string($r_entr_name)."'
									ORDER BY text_id DESC
									LIMIT 1;");
		$result_text = mysql_fetch_array($query_text);
		
		return $result_text['text_content'];
	}
	

	function get_tag($r_dataid, $r_revid, $r_entr_name) {
	
		$query_edit_mef_tag_get_id = mysql_query("SELECT tn_id, tn_name 
													FROM data_tag_name
													WHERE tn_name = '".mysql_real_escape_string($r_entr_name)."'
													LIMIT 1;");
		$result_edit_mef_tag_get_id = mysql_fetch_array($query_edit_mef_tag_get_id);
				
		// tag
		$query_edit_mef_tag_get_id_val = mysql_query("SELECT tag_value_id 
												FROM data_tag 
												WHERE data_id = '".mysql_real_escape_string($r_dataid)."'
												AND data_rev_id = '".mysql_real_escape_string($r_revid)."'
												AND tag_name_id = '".mysql_real_escape_string($result_edit_mef_tag_get_id['tn_id'])."'
												LIMIT 1;");
		$result_edit_mef_tag_get_id_val = mysql_fetch_array($query_edit_mef_tag_get_id_val);

		//echo "<p>tagvalue-ID: ".$result_edit_mef_tag_get_id_val['tag_value_id']."</p>";
		
		// tag value
		$query_edit_mef_tag_get_value = mysql_query("SELECT tv_value 
													FROM data_tag_value 
													WHERE tv_id = '".mysql_real_escape_string($result_edit_mef_tag_get_id_val['tag_value_id'])."'
													LIMIT 1;");
		$result_edit_mef_tag_get_value = mysql_fetch_array($query_edit_mef_tag_get_value);
		
		return $result_edit_mef_tag_get_value['tv_value'];
	}




?>