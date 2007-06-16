<?php


//	echo "<p>Sec_Lev: ".$roscms_security_level."</p>";
//	echo "<p>Sec_Memberships: ".$roscms_security_memberships."</p>";
	
	
	$roscms_security_sql_read = roscms_security_sql("read");
	


//	echo "<p>Sec_Read_SQL: ".$roscms_security_sql_read."</p>";



	function roscms_security_sql($kind) {
		global $roscms_branch;
		global $roscms_security_level;
		global $roscms_currentuser_id;
		
		$temp_sql = "";
		$temp_sec_counter = 0;
		$pos = 0;
	
		$query_data_security = mysql_query("SELECT * 
										FROM data_security 
										WHERE sec_branch = '".mysql_real_escape_string($roscms_branch)."';");
		while ($result_data_security = mysql_fetch_array($query_data_security)) {
			if ($result_data_security['sec_lev'.$roscms_security_level.'_'.$kind] == 1) {
				if ($temp_sec_counter > 0) {
					$temp_sql .= " OR";
				}
	
	//			echo "<p>sec_lev".$roscms_security_level."_read = 1 (".$result_data_security['sec_name'].")</p>";
				$temp_sql .= " data_acl = '".mysql_real_escape_string($result_data_security['sec_name'])."' ";
				$temp_sec_counter++;
			}
			
			$query_usergroup_sec = mysql_query("SELECT * 
												FROM usergroup_members 
												WHERE usergroupmember_userid = ".mysql_real_escape_string($roscms_currentuser_id).";");
			while($result_usergroup_sec = mysql_fetch_array($query_usergroup_sec)) {
				$pos = strpos($result_data_security['sec_allow'], "|".$result_usergroup_sec['usergroupmember_usergroupid']."|");
				if ($pos === false) {
					// string not found
				} else {
					if ($temp_sec_counter > 0) {
						$temp_sql .= " OR";
					}
					$temp_sql .= " data_acl = '".mysql_real_escape_string($result_data_security['sec_name'])."' ";
					$temp_sec_counter++;
				}
			}
		}	
		
		if ($temp_sec_counter == 1) {
			$temp_sql = " AND ". $temp_sql ." ";
		}
		else if ($temp_sec_counter > 1) {
			$temp_sql = " AND (". $temp_sql .") ";
		}
		else {
			 $temp_sql = " AND ( 0 = 1 ) "; // FAIL, as the user has no rights to access RosCMS
		}
		
		return $temp_sql;
	}

	function roscms_security_check($d_id) {
		global $roscms_branch;
		global $roscms_security_memberships;
		global $roscms_security_level;
		global $roscms_currentuser_id;


		$query_data_acl = mysql_query("SELECT * 
												FROM data_ 
												WHERE data_id = ".mysql_real_escape_string($d_id)." 
												LIMIT 1;");
		$result_data_acl = mysql_fetch_array($query_data_acl);

		
		$temp_sec_rights = "|";
		
		$query_data_sec_rights = mysql_query("SELECT * 
											FROM data_security 
											WHERE sec_branch = '".mysql_real_escape_string($roscms_branch)."' 
											AND sec_name = '".mysql_real_escape_string($result_data_acl['data_acl'])."' 
											AND sec_lev".mysql_real_escape_string($roscms_security_level)."_read = 1
											LIMIT 1;");
		$result_data_sec_rights = mysql_fetch_array($query_data_sec_rights);
		$temp_sec_rights .= "read|";
		
		if ($result_data_sec_rights['sec_lev'.$roscms_security_level.'_write']) {
			$temp_sec_rights .= "write|";
		}
		if ($result_data_sec_rights['sec_lev'.$roscms_security_level.'_add']) {
			$temp_sec_rights .= "add|";
		}
		if ($result_data_sec_rights['sec_lev'.$roscms_security_level.'_pub']) {
			$temp_sec_rights .= "pub|";
		}
		if ($result_data_sec_rights['sec_lev'.$roscms_security_level.'_trans']) {
			$temp_sec_rights .= "trans|";
		}
		
		return $temp_sec_rights;
	}




	$roscms_intern_usrgrp_policy_view_basic = false;
	$roscms_intern_usrgrp_policy_view_advanced = false;
	$roscms_intern_usrgrp_policy_translate = false;
	$roscms_intern_usrgrp_policy_edit_basic = false;
	$roscms_intern_usrgrp_policy_edit_advanced = false;


	// view_basic
	if ($roscms_intern_usrgrp_trans == true || 
		$roscms_intern_usrgrp_transm == true || 
		$roscms_intern_usrgrp_men == true || 
		$roscms_intern_usrgrp_team == true || 
		$roscms_intern_usrgrp_dev == true || 
		$roscms_intern_usrgrp_admin == true || 
		$roscms_intern_usrgrp_sadmin == true)
	{
		$roscms_intern_usrgrp_policy_view_basic = true;
	}

	// view_advanced
	if ($roscms_intern_usrgrp_team == true || 
		$roscms_intern_usrgrp_dev == true || 
		$roscms_intern_usrgrp_admin == true || 
		$roscms_intern_usrgrp_sadmin == true)
	{
		$roscms_intern_usrgrp_policy_view_advanced = true;
	}

	// translate
	if ($roscms_intern_usrgrp_trans == true || 
		$roscms_intern_usrgrp_transm == true || 
		$roscms_intern_usrgrp_men == true || 
		$roscms_intern_usrgrp_team == true || 
		$roscms_intern_usrgrp_dev == true || 
		$roscms_intern_usrgrp_admin == true || 
		$roscms_intern_usrgrp_sadmin == true)
	{
		$roscms_intern_usrgrp_policy_translate = true;
	}

	// edit_basic
	if ($roscms_intern_usrgrp_men == true || 
		$roscms_intern_usrgrp_team == true || 
		$roscms_intern_usrgrp_dev == true || 
		$roscms_intern_usrgrp_admin == true || 
		$roscms_intern_usrgrp_sadmin == true)
	{
		$roscms_intern_usrgrp_policy_edit_basic = true;
	}

	// edit_advanced
	if ($roscms_intern_usrgrp_admin == true || 
		$roscms_intern_usrgrp_sadmin == true)
	{
		$roscms_intern_usrgrp_policy_edit_advanced = true;
	}

?>