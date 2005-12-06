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
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Group Members</span> 
  <ul>
    <li><strong>Group Members</strong></li>
  </ul>
  <p>Action: <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=add&amp;sec3=new&amp;db_id=new">New Membership</a> | <a href="?page=admin&sec=usrgrpmbr&sec2=view">reset 
    filters & sort</a></p>
  <?php


	if ($rpm_lang_id == "") {
		$rpm_lang_id="all";
	}
	//echo '<p>Language: ';
	//echo '<b>All</b>';
	$ros_cms_intern_users_lang = " ";
	//echo '</p>';

	if ($rpm_filt == "") {
		$rpm_filt="active";
	}
	//echo '<p>Filter: ';
	//echo '<b>active</b>';
	$ros_cms_intern_users_filt = " ";
	//echo '</p>';

	if ($rpm_filt == "") {
		$rpm_filt="active";
	}
	echo '<p>Filter: ';
	if ($rpm_filt == "active") {	
		echo '<b>all</b>';
		$ros_cms_intern_users_filt = " ";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=active&amp;langid='.$rpm_lang_id.'">all</a>';
	}
	echo ' | ';
	if ($rpm_filt == "sadmin") {	
		echo '<b>super admin</b>';
		$ros_cms_intern_users_filt = " WHERE `usergroupmember_usergroupid` = 'ros_sadmin' ";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=sadmin&amp;langid='.$rpm_lang_id.'">super admin</a>';
	}
	echo ' | ';
	if ($rpm_filt == "admin") {	
		echo '<b>admin</b>';
		$ros_cms_intern_users_filt = " WHERE `usergroupmember_usergroupid` = 'ros_admin' ";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=admin&amp;langid='.$rpm_lang_id.'">admin</a>';
	}
	echo ' | ';
	if ($rpm_filt == "dev") {	
		echo '<b>developer</b>';
		$ros_cms_intern_users_filt = " WHERE `usergroupmember_usergroupid` = 'developer' ";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=dev&amp;langid='.$rpm_lang_id.'">developer</a>';
	}
	echo ' | ';
	if ($rpm_filt == "mod") {	
		echo '<b>moderator</b>';
		$ros_cms_intern_users_filt = " WHERE `usergroupmember_usergroupid` = 'moderator' ";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=mod&amp;langid='.$rpm_lang_id.'">moderator</a>';
	}
	echo ' | ';
	if ($rpm_filt == "trans") {	
		echo '<b>translator</b>';
		$ros_cms_intern_users_filt = " WHERE `usergroupmember_usergroupid` = 'translator' ";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=trans&amp;langid='.$rpm_lang_id.'">translator</a>';
	}
	echo ' | ';
	if ($rpm_filt == "user") {	
		echo '<b>normal user</b>';
		$ros_cms_intern_users_filt = " WHERE `usergroupmember_usergroupid` = 'user' ";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=user&amp;langid='.$rpm_lang_id.'">normal user</a>';
	}

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
<?php

// Setting:
$roscms_intern_items_per_page = 50;


$roscms_SET_curpos = "";
if (array_key_exists("curpos", $_GET)) $roscms_SET_curpos=htmlspecialchars($_GET["curpos"]);
if (!$roscms_SET_curpos) {
	$roscms_SET_curpos = 0;
}

$query_count_cat=mysql_query("SELECT COUNT('usergroupmember_userid') 
								FROM `usergroup_members` 
								" . $ros_cms_intern_users_filt . "
								ORDER BY `usergroupmember_userid` ASC ;");	
$result_count_cat = mysql_fetch_row($query_count_cat);

	echo "<p align='center'>";
	$j=0;
	for ($i=0; $i < $result_count_cat[0]; $i += $roscms_intern_items_per_page) {
		$j++;
		if ($roscms_SET_curpos == $i) {
			echo "<b>".$j."</b> ";
		}
		else {
			echo "<a href='?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort=".$rpm_sort."&amp;filt=".$rpm_filt."&amp;opt=".$rpm_opt."&amp;langid=".$rpm_lang_id."&amp;curpos=".$i."'>".$j."</a> ";
		}
	}
	$j=0;
	echo "</p>";

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
				ORDER BY '$ros_cms_intern_users_sortby' $ros_cms_intern_users_sort  LIMIT ". $roscms_SET_curpos ." , ". $roscms_intern_items_per_page ." ;") ;

	}
	else {
	//AND user_account_hidden != 0
		$query_page = mysql_query("SELECT * 
				FROM usergroup_members
				$ros_cms_intern_users_filt  $ros_cms_intern_users_lang
				ORDER BY '$ros_cms_intern_users_sortby' $ros_cms_intern_users_sort  LIMIT ". $roscms_SET_curpos ." , ". $roscms_intern_items_per_page ." ;") ;
	}

	$farbe1="#E2E2E2";
	$farbe2="#EEEEEE";
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
							 ?>"> 
        <div align="center"><a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=edit&amp;sec3=<?php echo $result_page['usergroupmember_usergroupid']; ?>&amp;db_id=<?php echo $result_page['usergroupmember_userid']; ?>"><img src="images/view.gif" alt="Edit Membership" width="19" height="18" border="0"></a>&nbsp;<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=add&amp;sec3=new&amp;db_id=<?php echo $result_page['usergroupmember_userid']; ?>"><img src="images/tool.gif" alt="Add Membership" width="19" height="18" border="0"></a>&nbsp;<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=delete&amp;sec3=<?php echo $result_page['usergroupmember_usergroupid']; ?>&amp;db_id=<?php echo $result_page['usergroupmember_userid']; ?>"><img src="images/delete.gif" alt="Delete Membership" width="19" height="18" border="0"></a></div></td>
      <td width="7%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"><?php echo "<b>".$result_page['usergroupmember_userid']."</b>"; ?></font></div></td>
      <td width="10%" valign="middle" bgcolor="<?php echo $farbe; ?>"><font face="Arial, Helvetica, sans-serif"><?php
		$query_usra = mysql_query("SELECT user_id, user_name FROM users WHERE user_id = '".$result_page['usergroupmember_userid']."'") or die('DB error (membership script)!');
		$result_usra = mysql_fetch_array($query_usra); // or die('DB error (show_sessions script)');
	  
	   echo $result_usra['user_name']." <a href='?page=user&amp;sec=profil&amp;sec2=".$result_page['usergroupmember_userid']."'>[Profil]</a>"; ?> 
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
<p align="center"><b><?php

	echo ($roscms_SET_curpos+1)." to ";

	if (($roscms_SET_curpos + $roscms_intern_items_per_page) > $result_count_cat[0]) {
		echo $result_count_cat[0];
	}
	else {
		echo ($roscms_SET_curpos + $roscms_intern_items_per_page);
	}
		
	echo " of ".$result_count_cat[0]; 
	
?></b></p>
  <?php
  	$roscms_infotable = "user";
	include("inc/inc_description_table.php");
?>
</div>
