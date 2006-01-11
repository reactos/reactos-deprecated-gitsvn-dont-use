<?php

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
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - User History</span>
  <ul>
    <li><strong>User History</strong></li>
  </ul>
  <p>Action: <a href="?page=admin&sec=userhistory&sec2=view">reset 
    filters & sort</a></p>
  <?php

	if ($rpm_lang_id == "") {
		$rpm_lang_id="all";
	}
	echo '<p>Language: ';
	if ($rpm_lang_id == "all") {	
		echo '<b>All</b>';
		$ros_cms_intern_users_lang = "AND A1.user_language LIKE '%'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=userhistory&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=all">All</a>';
	}
	// Languages
	$sql_lang="SELECT * 
				FROM languages
				WHERE lang_level != '0'
				ORDER BY 'lang_level' DESC";
	$sql_query_lang=mysql_query($sql_lang);
	while($myrow_lang=mysql_fetch_row($sql_query_lang)) {
		$roscms_sel_lang = $myrow_lang[0];
		echo ' | ';
		if ($rpm_lang_id == $roscms_sel_lang) {	
			echo '<b>'.$myrow_lang[1].'</b>';
			$ros_cms_intern_users_lang = "AND A1.user_language = '".$roscms_sel_lang."'";
		}
		else {
			echo '<a href="?page=admin&amp;sec=userhistory&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$roscms_sel_lang.'">'.$myrow_lang[1].'</a>';
		}
	}


	if ($rpm_filt == "") {
		$rpm_filt="active";
	}
	echo '<p>Filter: ';
	if ($rpm_filt == "active") {	
		echo '<b>active users</b>';
		$ros_cms_intern_users_filt = "AND A1.user_account_enabled = 'yes'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=userhistory&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=active&amp;langid='.$rpm_lang_id.'">active users</a>';
	}
	echo ' | ';
	if ($rpm_filt == "all") {	
		echo '<b>all users</b>';
		$ros_cms_intern_users_filt = "AND A1.user_account_enabled != ''";
	}
	else {
		echo '<a href="?page=admin&amp;sec=userhistory&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=all&amp;langid='.$rpm_lang_id.'">all users</a>';
	}
	echo '</p>';

	if ($rpm_sort == "") {
		$rpm_sort="translator";
	}
	echo '<p>Filter by Group: ';
/*	if ($rpm_sort == "all") {	
		echo '<b>all</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid LIKE '%'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=userhistory&amp;sec2=view&amp;sort=all&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">all</a>';
	}
	echo ' | ';*/
	if ($rpm_sort == "ros_sadmin") {	
		echo '<b>super admin</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid = 'ros_sadmin'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=userhistory&amp;sec2=view&amp;sort=ros_sadmin&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">super admin</a>';
	}
	echo ' | ';
	if ($rpm_sort == "ros_admin") {	
		echo '<b>admin</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid = 'ros_admin'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=userhistory&amp;sec2=view&amp;sort=ros_admin&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">admin</a>';
	}
	echo ' | ';
	if ($rpm_sort == "developer") {	
		echo '<b>developer</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid = 'developer'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=userhistory&amp;sec2=view&amp;sort=developer&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">developer</a>';
	}
	echo ' | ';
	if ($rpm_sort == "moderator") {	
		echo '<b>moderator</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid = 'moderator'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=userhistory&amp;sec2=view&amp;sort=moderator&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">moderator</a>';
	}
	echo ' | ';
	if ($rpm_sort == "translator") {	
		echo '<b>translator</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid = 'translator'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=userhistory&amp;sec2=view&amp;sort=translator&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">translator</a>';
	}
	echo '</p>';

?>
  <?php

// Setting:
$roscms_intern_items_per_page = 50;

$roscms_SET_curpos = "";
$roscms_SET_letter = "";
if (array_key_exists("curpos", $_GET)) $roscms_SET_curpos=htmlspecialchars($_GET["curpos"]);
if (array_key_exists("letter", $_GET)) $roscms_SET_letter=htmlspecialchars($_GET["letter"]);




?>
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="#5984C3"> 
      <td width="10%"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Action</strong></font></div></td>
      <td width="20%" bgcolor="#5984C3"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Name</strong></font></div></td>
      <td width="70%" bgcolor="#5984C3"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>History</strong></font></div>        </td>
    </tr>
    <?php


		$roscms_TEMP_sql = "SELECT A1.user_id, A1.user_name, A2.usergroupmember_userid, A2.usergroupmember_usergroupid 
									FROM users A1, usergroup_members A2
									WHERE A1.user_id = A2.usergroupmember_userid
									AND A1.user_name LIKE  '%' 
									$ros_cms_intern_users_filt $ros_cms_intern_users_lang $ros_cms_intern_users_sortby
									GROUP BY A1.user_name 
									 ;";
//		echo $roscms_TEMP_sql;
		$query_page = mysql_query($roscms_TEMP_sql) ;

	$color1=$roscms_intern_color1;
	$color2=$roscms_intern_color2;
	$colorcounter="0";
	//$farbe="#CCCCC";
	
	while($result_page = mysql_fetch_array($query_page)) { // users
?>
    <tr>
      <td valign="top" bgcolor="<?php
								$colorcounter++;
								if ($colorcounter == "1") {
									echo $color1;
									$farbe = $color1;
								}
								elseif ($colorcounter == "2") {
									$colorcounter="0";
									echo $color2;
									$farbe = $color2;
								}
							 ?>"> 
        <div align="center"><a href="?page=admin&amp;sec=userhistory&amp;sec2=edit&amp;sec3=<?php echo $result_page['usergroupmember_usergroupid']; ?>&amp;db_id=<?php echo $result_page['user_id']; ?>"><img src="images/view.gif" alt="Edit Membership" width="19" height="18" border="0"></a>&nbsp;<a href="?page=admin&amp;sec=userhistory&amp;sec2=add&amp;sec3=new&amp;db_id=<?php echo $result_page['usergroupmember_userid']; ?>"><img src="images/tool.gif" alt="Add Membership" width="19" height="18" border="0"></a>&nbsp;<a href="?page=admin&amp;sec=userhistory&amp;sec2=delete&amp;sec3=<?php echo $result_page['usergroupmember_usergroupid']; ?>&amp;db_id=<?php echo $result_page['usergroupmember_userid']; ?>"><img src="images/delete.gif" alt="Delete Membership" width="19" height="18" border="0"></a></div></td>
      <td valign="top" bgcolor="<?php echo $farbe; ?>"> <div align="left">
        <p align="center"><font face="Arial, Helvetica, sans-serif"><?php echo $result_page['user_id']. " - "; 
		$query_usra = mysql_query("SELECT user_id, user_name FROM users WHERE user_id = '".$result_page['user_id']."'") or die('DB error (membership script)!');
		$result_usra = mysql_fetch_array($query_usra); // or die('DB error (show_sessions script)');
	  
	   echo "<b>".$result_usra['user_name']."</b><br /><a href='?page=user&amp;sec=profil&amp;sec2=".$result_page['user_id']."'>[Profil]</a>"; ?>
            </font></p>
        <p><font face="Arial, Helvetica, sans-serif">            <?php
			echo "<ul>";
			$query_grpmbr = mysql_query("SELECT * 
										FROM `usergroup_members` 
										WHERE `usergroupmember_userid` = ".$result_usra['user_id']." 
										ORDER BY `usergroupmember_usergroupid` ASC ;") ;
			while($result_grpmbr = mysql_fetch_array($query_grpmbr)) {
				echo "<li>".$result_grpmbr['usergroupmember_usergroupid']."</li>";
			}
			echo "</ul>";
		 ?>
            </font></p>
      </div></td>
      <td valign="top" bgcolor="<?php echo $farbe; ?>"><?php 

	// Contents	
		echo "<b><u><i>Contents:</i></u></b> ";
		$query_count_contents=mysql_query("SELECT COUNT('content_id') 
											FROM `content` 
											WHERE `content_usrname_id` = ".$result_page['user_id']."
											ORDER BY `content_id` DESC;");	
		$result_count_contents = mysql_fetch_row($query_count_contents);
		echo $result_count_contents[0]."<br /><br />";
		if ($result_count_contents[0] != 0) {
			$query_entries_contents = mysql_query("SELECT * 
													FROM `content` 
													WHERE `content_usrname_id` = ".$result_page['user_id']."
													ORDER BY `content_id` DESC 
													LIMIT 0 , 25 ;") ;
			while($result_entries_contents = mysql_fetch_array($query_entries_contents)) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$result_entries_contents['content_id']." - ".$result_entries_contents['content_name']." <font size='1'>- ".$result_entries_contents['content_lang']." / ".$result_entries_contents['content_type']." (".$result_entries_contents['content_date']." ".$result_entries_contents['content_time'].")</font><br />";
			}
			if ($result_count_contents[0] > 25) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;...<br />";
			}
			echo "<br />";
		}


	// Pages	
		echo "<b><u><i>Pages:</i></u></b> ";
		$query_count_contents=mysql_query("SELECT COUNT('page_id') 
											FROM `pages` 
											WHERE `page_usrname_id` = ".$result_page['user_id']."
											ORDER BY `page_id` DESC;");	
		$result_count_contents = mysql_fetch_row($query_count_contents);
		echo $result_count_contents[0]."<br /><br />";
		if ($result_count_contents[0] != 0) {
			$query_entries_contents = mysql_query("SELECT * 
													FROM `pages` 
													WHERE `page_usrname_id` = ".$result_page['user_id']."
													ORDER BY `page_id` DESC 
													LIMIT 0 , 25 ;") ;
			while($result_entries_contents = mysql_fetch_array($query_entries_contents)) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$result_entries_contents['page_id']." - ".$result_entries_contents['page_name']." <font size='1'>- ".$result_entries_contents['page_language']." / ".$result_entries_contents['pages_extention']." (".$result_entries_contents['page_date']." ".$result_entries_contents['page_time'].")</font><br />";
			}
			if ($result_count_contents[0] > 25) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;...<br />";
			}
			echo "<br />";
		}


	// Pages	
		echo "<b><u><i>DynContent:</i></u></b> ";
		$query_count_contents=mysql_query("SELECT COUNT('dyn_id') 
											FROM `dyn_content` 
											WHERE `dyn_content_usrname_id` = ".$result_page['user_id']."
											ORDER BY `dyn_id` DESC;");	
		$result_count_contents = mysql_fetch_row($query_count_contents);
		echo $result_count_contents[0]."<br /><br />";
		if ($result_count_contents[0] != 0) {
			$query_entries_contents = mysql_query("SELECT * 
													FROM `dyn_content` 
													WHERE `dyn_content_usrname_id` = ".$result_page['user_id']."
													ORDER BY `dyn_id` DESC 
													LIMIT 0 , 25 ;") ;
			while($result_entries_contents = mysql_fetch_array($query_entries_contents)) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$result_entries_contents['dyn_id']." - ".$result_entries_contents['dyn_content_name']."_".$result_entries_contents['dyn_content_id'].".".$result_entries_contents['dyn_content_nr']." <font size='1'>- ".$result_entries_contents['dyn_content_lang']." (".$result_entries_contents['dyn_content_date']." ".$result_entries_contents['dyn_content_time'].")</font><br />";
			}
			if ($result_count_contents[0] > 25) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;...<br />";
			}
			echo "<br />";
		}

	  ?></td>
    </tr>
    <?php	
	}	// end while
?>
  </table>
  <?php
  	$roscms_infotable = "user";
	include("inc/inc_description_table.php");
?>
</div>
