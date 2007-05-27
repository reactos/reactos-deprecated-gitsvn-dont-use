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
	if ( !defined('ROSCMS_SYSTEM') OR !defined('ROSCMS_SYSTEM_ADMIN') )
	{
		if ( !defined('ROSCMS_SYSTEM_LOG') ) {
			define ("ROSCMS_SYSTEM_LOG", "Hacking attempt");
		}
		$seclog_section="roscms_admin_interface";
		$seclog_level="50";
		$seclog_reason="Hacking attempt: admin_users.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}

?> 
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Group Members</span> 
  <ul>
    <li><strong>Group Members</strong></li>
  </ul>


  <p>Action: <a href="#">New Membership</a> | <a href="?page=admin&sec=usrgrpmbr&sec2=view">reset 
    filters & sort</a></p>
<?php


	if ($rpm_lang_id == "") {
		$rpm_lang_id="all";
	}
	echo '<p>Language: ';
	echo '<b>All</b>';
	$ros_cms_intern_users_lang = " ";
	echo '</p>';

	if ($rpm_filt == "") {
		$rpm_filt="active";
	}
	echo '<p>Filter: ';
	echo '<b>active</b>';
	$ros_cms_intern_users_filt = " ";
	echo '</p>';

	if ($rpm_sort == "") {
		$rpm_sort="id";
	}
	echo '<p>Sorted by: ';
	if ($rpm_sort == "id") {	
		echo '<b>user ID</b>';
		$ros_cms_intern_users_sortby="usergroupmember_userid";
		$ros_cms_intern_users_sort="ASC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort=id&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">user ID</a>';
	}
	echo ' | ';
	if ($rpm_sort == "group") {	
		echo '<b>group ID</b>';
		$ros_cms_intern_users_sortby="usergroupmember_usergroupid";
		$ros_cms_intern_users_sort="ASC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort=group&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">group ID</a>';
	}
	echo '</p>';

?>
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="#5984C3"> 
      <td width="6%"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Action</strong></font></div></td>
      <td width="4%" bgcolor="#5984C3"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>ID</strong></font></div></td>
      <td width="20%" bgcolor="#5984C3"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Nick</strong></font></div></td>
      <td width="20%"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Usergroup</strong></font></div></td>
      <td width="50%" bgcolor="#5984C3"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Usergroup 
          description</strong></font></div></td>
    </tr>
    <?php

	if($roscms_intern_account_level==100) {
		$query_page = mysql_query("SELECT * 
				FROM usergroup_members
				$ros_cms_intern_users_filt $ros_cms_intern_users_lang
				ORDER BY '$ros_cms_intern_users_sortby' $ros_cms_intern_users_sort") ;
	}
	else {
		$query_page = mysql_query("SELECT * 
				FROM usergroup_members
				$ros_cms_intern_users_filt AND user_account_hidden != 0 $ros_cms_intern_users_lang
				ORDER BY '$ros_cms_intern_users_sortby' $ros_cms_intern_users_sort") ;
	}

	$farbe1=$roscms_intern_color1;
	$farbe2=$roscms_intern_color2;
	$zaehler="0";
	//$farbe="#CCCCC";
	
	while($result_page = mysql_fetch_array($query_page)) { // users
?>
    <tr> 
      <td width="9%" valign="middle" bgcolor="<?php
								$zaehler++;
								if ($zaehler == "1") {
									echo $farbe1;
									$farbe = $farbe1;
								}
								elseif ($zaehler == "2") {
									$zaehler="0";
									echo $farbe2;
									$farbe = $farbe2;
								}
							 ?>" title="RosCMS action buttons:&#10;&#10;* View account&#10;* Delete account&#10;* Email&#10;* Website"> 
        <div align="center"><img src="images/view.gif" alt="View" width="19" height="18" border="0"></div></td>
      <td width="7%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"><?php echo "<b>".$result_page['usergroupmember_userid']."</b>"; ?></font></div></td>
      <td width="10%" valign="middle" bgcolor="<?php echo $farbe; ?>"><font face="Arial, Helvetica, sans-serif"><?php
		$query_usra = mysql_query("SELECT user_id, user_name FROM users WHERE user_id = '".$result_page['usergroupmember_userid']."'") or die('DB error (membership script)!');
		$result_usra = mysql_fetch_array($query_usra) or die('DB error (show_sessions script)');
	  
	   echo $result_usra['user_name']; ?> 
        </font></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>" title="<?php echo $result_page['usergroupmember_usergroupid']; ?>"><div align="left"><font face="Arial, Helvetica, sans-serif"> 
          <?php
		//echo "SELECT usrgroup_name_id, usrgroup_name FROM usergroups WHERE usrgroup_name_id = '".$result_page['usergroupmember_usergroupid']."'<br>";
		$query_grpb = mysql_query("SELECT usrgroup_name_id, usrgroup_name, usrgroup_description FROM usergroups WHERE usrgroup_name_id = '".$result_page['usergroupmember_usergroupid']."'") or die('DB error (membership script)!');
		$result_grpb = mysql_fetch_array($query_grpb) or die('DB error (membership script)');
	  
		echo $result_grpb['usrgroup_name']; 
		 ?>
          </font></div></td>
      <td width="15%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <font face="Arial, Helvetica, sans-serif"><?php
		echo substr(htmlentities($result_grpb['usrgroup_description'], ENT_QUOTES), 0, 50)."...";
		?></font></tr>
    <?php	
	}	// end while
?>
  </table>
  <?php
  	$roscms_infotable = "user";
	include("inc/inc_description_table.php");
?>
</div>
