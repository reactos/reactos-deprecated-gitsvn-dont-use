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
	echo '<p>Language: ';
	if ($rpm_lang_id == "all") {	
		echo '<b>All</b>';
		$ros_cms_intern_users_lang = "AND A1.user_language LIKE '%'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=all">All</a>';
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
			echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$roscms_sel_lang.'">'.$myrow_lang[1].'</a>';
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
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=active&amp;langid='.$rpm_lang_id.'">active users</a>';
	}
	echo ' | ';
	if ($rpm_filt == "all") {	
		echo '<b>all users</b>';
		$ros_cms_intern_users_filt = "AND A1.user_account_enabled != ''";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=all&amp;langid='.$rpm_lang_id.'">all users</a>';
	}
	echo '</p>';

	if ($rpm_sort == "") {
		$rpm_sort="all";
	}
	echo '<p>Filter by Group: ';
	if ($rpm_sort == "all") {	
		echo '<b>all</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid LIKE '%'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort=all&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">all</a>';
	}
	echo ' | ';
	if ($rpm_sort == "ros_sadmin") {	
		echo '<b>super admin</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid = 'ros_sadmin'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort=ros_sadmin&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">super admin</a>';
	}
	echo ' | ';
	if ($rpm_sort == "ros_admin") {	
		echo '<b>admin</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid = 'ros_admin'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort=ros_admin&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">admin</a>';
	}
	echo ' | ';
	if ($rpm_sort == "developer") {	
		echo '<b>developer</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid = 'developer'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort=developer&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">developer</a>';
	}
	echo ' | ';
	if ($rpm_sort == "moderator") {	
		echo '<b>moderator</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid = 'moderator'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort=moderator&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">moderator</a>';
	}
	echo ' | ';
	if ($rpm_sort == "translator") {	
		echo '<b>translator</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid = 'translator'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort=translator&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">translator</a>';
	}
	echo ' | ';
	if ($rpm_sort == "user") {	
		echo '<b>user</b>';
		$ros_cms_intern_users_sortby=" AND A2.usergroupmember_usergroupid = 'user'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort=user&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">user</a>';
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



	echo '<p align="center">';
	
	if ($roscms_SET_letter == "all" || $roscms_SET_letter == "") {
		echo '  <b>All</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=all" class="letterbarlink">All</a> ';
	}

	if ($roscms_SET_letter == "a") {
		echo '  <b>A</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=a" class="letterbarlink">A</a> ';
	}

	if ($roscms_SET_letter == "b") {
		echo '  <b>B</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=b" class="letterbarlink">B</a> ';
	}

	if ($roscms_SET_letter == "c") {
		echo '  <b>C</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=c" class="letterbarlink">C</a> ';
	}

	if ($roscms_SET_letter == "d") {
		echo '  <b>D</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=d" class="letterbarlink">D</a> ';
	}

	if ($roscms_SET_letter == "e") {
		echo '  <b>E</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=e" class="letterbarlink">E</a> ';
	}

	if ($roscms_SET_letter == "f") {
		echo '  <b>F</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=f" class="letterbarlink">F</a> ';
	}

	if ($roscms_SET_letter == "g") {
		echo '  <b>G</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=g" class="letterbarlink">G</a> ';
	}

	if ($roscms_SET_letter == "h") {
		echo '  <b>H</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=h" class="letterbarlink">H</a> ';
	}

	if ($roscms_SET_letter == "i") {
		echo '  <b>I</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=i" class="letterbarlink">I</a> ';
	}

	if ($roscms_SET_letter == "j") {
		echo '  <b>J</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=j" class="letterbarlink">J</a> ';
	}

	if ($roscms_SET_letter == "k") {
		echo '  <b>K</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=k" class="letterbarlink">K</a> ';
	}

	if ($roscms_SET_letter == "l") {
		echo '  <b>L</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=l" class="letterbarlink">L</a> ';
	}

	if ($roscms_SET_letter == "m") {
		echo '  <b>M</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=m" class="letterbarlink">M</a> ';
	}
	
	if ($roscms_SET_letter == "n") {
		echo '  <b>N</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=n" class="letterbarlink">N</a> ';
	}
	
	if ($roscms_SET_letter == "o") {
		echo '  <b>O</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=o" class="letterbarlink">O</a> ';
	}

	if ($roscms_SET_letter == "p") {
		echo '  <b>P</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=p" class="letterbarlink">P</a> ';
	}

	if ($roscms_SET_letter == "q") {
		echo '  <b>Q</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=q" class="letterbarlink">Q</a> ';
	}

	if ($roscms_SET_letter == "r") {
		echo '  <b>R</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=r" class="letterbarlink">R</a> ';
	}

	if ($roscms_SET_letter == "s") {
		echo '  <b>S</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=s" class="letterbarlink">S</a> ';
	}

	if ($roscms_SET_letter == "t") {
		echo '  <b>T</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=t" class="letterbarlink">T</a> ';
	}

	if ($roscms_SET_letter == "u") {
		echo '  <b>U</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=u" class="letterbarlink">U</a> ';
	}

	if ($roscms_SET_letter == "v") {
		echo '  <b>V</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=v" class="letterbarlink">V</a> ';
	}

	if ($roscms_SET_letter == "w") {
		echo '  <b>W</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=w" class="letterbarlink">W</a> ';
	}

	if ($roscms_SET_letter == "x") {
		echo '  <b>X</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=x" class="letterbarlink">X</a> ';
	}

	if ($roscms_SET_letter == "y") {
		echo '  <b>Y</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=y" class="letterbarlink">Y</a> ';
	}

	if ($roscms_SET_letter == "z") {
		echo '  <b>Z</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=z" class="letterbarlink">Z</a> ';
	}

	echo "</p>";



if (!$roscms_SET_curpos) {
	$roscms_SET_curpos = 0;
}

if ($roscms_SET_letter == "all") {
	$roscms_SET_letter = "%";
}

$query_count_cat=mysql_query("SELECT COUNT('A1.user_id') 
								FROM users A1
								WHERE A1.user_account_enabled = 'yes'
								AND A1.user_account_hidden = 'no'
								AND A1.user_name LIKE  '" . $roscms_SET_letter . "%' 
								$ros_cms_intern_users_filt 
								$ros_cms_intern_users_lang
								ORDER BY A1.user_name ASC ;");	
$result_count_cat = mysql_fetch_row($query_count_cat);

	echo "<p align='center'>";
	$j=0;
	for ($i=0; $i < $result_count_cat[0]; $i += $roscms_intern_items_per_page) {
		$j++;
		if ($roscms_SET_curpos == $i) {
			echo "<b>".$j."</b> ";
		}
		else {
			echo "<a href='?page=admin&amp;sec=usrgrpmbr&amp;sec2=view&amp;sort=".$rpm_sort."&amp;filt=".$rpm_filt."&amp;opt=".$rpm_opt."&amp;langid=".$rpm_lang_id."&amp;letter=".$roscms_SET_letter."&amp;curpos=".$i."'>".$j."</a> ";
		}
	}
	$j=0;
	echo "</p>";

?>
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="#5984C3"> 
      <td width="10%"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Action</strong></font></div></td>
      <td width="10%" bgcolor="#5984C3"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>ID</strong></font></div></td>
      <td width="40%" bgcolor="#5984C3"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Nick</strong></font></div></td>
      <td width="40%"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Usergroup</strong></font></div></td>
    </tr>
    <?php


		$roscms_TEMP_sql = "SELECT A1.user_id, A1.user_name, A2.usergroupmember_userid, A2.usergroupmember_usergroupid 
									FROM users A1, usergroup_members A2
									WHERE A1.user_id = A2.usergroupmember_userid
									AND A1.user_name LIKE  '" . $roscms_SET_letter . "%'
									$ros_cms_intern_users_filt $ros_cms_intern_users_lang $ros_cms_intern_users_sortby
									GROUP BY A1.user_name
									LIMIT ". $roscms_SET_curpos ." , ". $roscms_intern_items_per_page ." 
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
      <td valign="middle" bgcolor="<?php
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
        <div align="center"><a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=edit&amp;sec3=<?php echo $result_page['usergroupmember_usergroupid']; ?>&amp;db_id=<?php echo $result_page['user_id']; ?>"><img src="images/view.gif" alt="Edit Membership" width="19" height="18" border="0"></a>&nbsp;<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=add&amp;sec3=new&amp;db_id=<?php echo $result_page['usergroupmember_userid']; ?>"><img src="images/tool.gif" alt="Add Membership" width="19" height="18" border="0"></a>&nbsp;<a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=delete&amp;sec3=<?php echo $result_page['usergroupmember_usergroupid']; ?>&amp;db_id=<?php echo $result_page['usergroupmember_userid']; ?>"><img src="images/delete.gif" alt="Delete Membership" width="19" height="18" border="0"></a></div></td>
      <td valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"><?php echo "<b>".$result_page['user_id']."</b>"; ?></font></div></td>
      <td valign="middle" bgcolor="<?php echo $farbe; ?>"><font face="Arial, Helvetica, sans-serif"><?php
		$query_usra = mysql_query("SELECT user_id, user_name FROM users WHERE user_id = '".$result_page['user_id']."'") or die('DB error (membership script)!');
		$result_usra = mysql_fetch_array($query_usra); // or die('DB error (show_sessions script)');
	  
	   echo $result_usra['user_name']." <a href='?page=user&amp;sec=profil&amp;sec2=".$result_page['user_id']."'>[Profil]</a>"; ?> 
        </font></td>
      <td valign="middle" bgcolor="<?php echo $farbe; ?>" title="<?php echo $result_page['usergroupmember_usergroupid']; ?>"><div align="left"><font face="Arial, Helvetica, sans-serif"> 
          <?php
		$query_grpb = mysql_query("SELECT usrgroup_name_id, usrgroup_name, usrgroup_description FROM usergroups WHERE usrgroup_name_id = '".$result_page['usergroupmember_usergroupid']."'") or die('DB error (membership script)!');
		$result_grpb = mysql_fetch_array($query_grpb) or die('DB error (membership script)');
	  
		echo $result_grpb['usrgroup_name'];
		 ?>
          </font></div></td>
    </tr>
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
