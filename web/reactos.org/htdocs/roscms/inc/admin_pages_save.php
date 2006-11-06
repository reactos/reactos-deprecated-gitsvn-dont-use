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
		$seclog_section="roscms_interface";
		$seclog_level="50";
		$seclog_reason="Hacking attempt: admin_pages_save.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}

?>
 <div class="contentSmall"><span class="contentSmallTitle">Admin Interface - Pages - Save</span> 
<ul>
<li><strong><a href="?page=<?php echo $rpm_page; ?>&sec=pages&sec2=view">Pages</a></strong> 
  <ul>
	<li>Save page</li>
  </ul>
</li>
</ul>
<?php 
	if ($roscms_intern_account_level>20) {
		$page_pageid=$_POST['txt_pageid'];
		$page_langa=$_POST['txt_langa'];
		$page_vis=$_POST['page_visible'];
		$page_extra=$_POST['txt_extra'];
		$page_savemode=$_POST['page_rad_opt'];
		$page_pagetitle=$_POST['txt_pagetitle'];
		$page_pagedesc=$_POST['txt_pagedesc'];
		$page_pageextention=$_POST['txt_pageextent'];
		$page_pageactive=$_POST['page_active'];
		
		
		if ($page_langa == "") {
			$page_langa = "all";
		}
		
		if ($page_vis == "yes") {
			$page_vis="1";
		}
		else {
			$page_vis="0";
		}

		if ($page_pageactive == "yes") {
			$page_pageactive="1";
		}
		else {
			$page_pageactive="0";
		}
		
		$page_data=$_POST['textarea_pages'];

		if ($page_savemode == "update") {
			$page_storeinfo="update";
			$page_post="UPDATE `pages` SET `page_name` = '". mysql_real_escape_string($page_pageid) ."',
				`page_language` = '". mysql_real_escape_string($page_langa) ."',
				`pages_extra` = '". mysql_real_escape_string($page_extra) ."',
				`page_title` = '". mysql_real_escape_string($page_pagetitle) ."',
				`page_description` = '". mysql_real_escape_string($page_pagedesc) ."',
				`page_text` = '". mysql_real_escape_string($page_data) ."',
				`page_visible` = '". mysql_real_escape_string($page_vis) ."',
				`pages_extention` = '". mysql_real_escape_string($page_pageextention) ."',
				`page_active` = '". mysql_real_escape_string($page_pageactive) ."',
				`page_generate_force` = '1'
				`page_date` = CURDATE( ) ,
				`page_time` = CURTIME( ) WHERE `page_id` = '$rpm_db_id' LIMIT 1 ;";
				//`page_active` = '1',
				// `page_usrname_id` = '$roscms_intern_account_id',
		}
		else {
			// Page Version:
			$page_storeinfo="save";
			$query_page_save_version = mysql_query("SELECT * 
										FROM `pages` 
										WHERE 1 AND `page_name` 
										LIKE '$page_pageid'
										ORDER BY `page_version` DESC LIMIT 1;");
			$result_page_save_version = mysql_fetch_array($query_page_save_version);
			$page_version=$result_page_save_version['page_version'];
			$page_version++;
			//echo "PAGE_VER:".$page_version;	
			
			$query_page_save_equal = mysql_query("SELECT * 
													FROM `pages` 
													WHERE `page_name` = '$page_pageid' AND `page_language` = '$page_langa';") ;
			while($result_page_savea = mysql_fetch_array($query_page_save_equal)) {
				// Set all versions not active:
				$page_na=$result_page_savea['page_id'];
				//echo "<br>".$page_na."||";
				//$page_na="9";
				$page_postc="UPDATE `pages` SET `page_active` = '0' WHERE `page_id` = '$page_na' LIMIT 1 ;";
				$page_post_listc=mysql_query($page_postc);
				//echo "§!";
			}
		
			$page_post="INSERT INTO `pages` ( `page_id` , `page_name` , `page_language` , `pages_extra` , `page_text` , `page_version` , `page_active` , `page_visible` , `page_date` , `page_time` , `page_usrname_id` , `page_title` , `page_description` )
				VALUES ('', '". mysql_real_escape_string($page_pageid) ."', '". mysql_real_escape_string($page_langa) ."', '". mysql_real_escape_string($page_extra) ."', '". mysql_real_escape_string($page_data) ."', '". mysql_real_escape_string($page_version) ."', '1', '1', CURDATE( ), CURTIME( ), '". mysql_real_escape_string($roscms_intern_account_id) ."', '". mysql_real_escape_string($page_pagetitle) ."', '". mysql_real_escape_string($page_pagedesc) ."');";
		}	
		
		$page_post_list=mysql_query($page_post);
	
	
		$query_page = mysql_query("SELECT * 
				FROM pages
				WHERE page_id = '$rpm_db_id'") ;
		$result_page = mysql_fetch_array($query_page);
	
		echo "<p>".$roscms_langres['ContTrans_Save5']." '".$result_page['page_name']."' (id='".$rpm_db_id."') ".$roscms_langres['ContTrans_Save2']." (methode: ".$page_storeinfo.")</p>";
	}
	else {
		echo "<p>No access!</p>"; ?>
		<script language="JavaScript">
			<!--
				window.setTimeout('window.location.href="?page=noaccess"','1')
			-->
		</script>
<?php
	}
?>

<p><a href="?page=<?php echo $rpm_page; ?>&amp;sec=generator&amp;sec2=view&amp;site=<?php echo $result_page['page_name']; ?>&amp;lang=en&amp;forma=html&amp;skin=default" target="_blank"><?php echo $roscms_langres['ContTrans_Save6']; ?></a></p>
  <?php echo "<p><a href=".$_SERVER['HTTP_REFERER'].">".$roscms_langres['ContTrans_Save7']."</a></p>"; ?>
</div>
