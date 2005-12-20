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
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - User Accounts </span> 
  <ul>
    <li><strong>User Accounts</strong></li>
  </ul>


  <p>Action: <a href="#">New User Account</a> | <a href="?page=admin&sec=users&sec2=view">reset 
    filters & sort</a></p>
<?php
	if($roscms_intern_account_level>50) {
    /*            $rpm_user_active = "";
                $rpm_user_active_set="";
		if(array_key_exists("user_active", $_GET)) $rpm_user_active=$_GET["user_active"];
		if(array_key_exists("user_avtive_set", $_GET)) $rpm_user_active_set=$_GET["user_active_set"];
	
		if ($rpm_user_active != "" AND $rpm_user_active_set != "") {
			$user_postc="UPDATE `users` SET `user_active` = '$rpm_user_active' WHERE `user_id` = '$rpm_user_active_set' LIMIT 1 ;";
			$user_post_listc=mysql_query($user_postc);
		}*/
	}


	if ($rpm_lang_id == "") {
		$rpm_lang_id="all";
	}
	echo '<p>Language: ';
	if ($rpm_lang_id == "all") {	
		echo '<b>All</b>';
		$ros_cms_intern_users_lang = "AND user_language LIKE '%'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=all">All</a>';
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
			$ros_cms_intern_users_lang = "AND user_language = '".$roscms_sel_lang."'";
		}
		else {
			echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$roscms_sel_lang.'">'.$myrow_lang[1].'</a>';
		}
	}


	if ($rpm_filt == "") {
		$rpm_filt="active";
	}
	echo '<p>Filter: ';
	if ($rpm_filt == "active") {	
		echo '<b>active users</b>';
		$ros_cms_intern_users_filt = "WHERE `user_account_enabled` = 'yes'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=active&amp;langid='.$rpm_lang_id.'">active users</a>';
	}
	echo ' | ';
	if ($rpm_filt == "all") {	
		echo '<b>all users</b>';
		$ros_cms_intern_users_filt = "WHERE `user_account_enabled` != ''";
	}
	else {
		echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=all&amp;langid='.$rpm_lang_id.'">all users</a>';
	}
	echo '</p>';

	if ($rpm_sort == "") {
		$rpm_sort="id";
	}
	echo '<p>Sorted by: ';
	if ($rpm_sort == "id") {	
		echo '<b>user ID</b>';
		$ros_cms_intern_users_sortby="user_id";
		$ros_cms_intern_users_sort="ASC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort=id&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">user ID</a>';
	}
	echo ' | ';
	if ($rpm_sort == "nick") {	
		echo '<b>nick</b>';
		$ros_cms_intern_users_sortby="user_name";
		$ros_cms_intern_users_sort="ASC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort=nick&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">nick</a>';
	}
	echo ' | ';
	if ($rpm_sort == "active") {	
		echo '<b>active</b>';
		$ros_cms_intern_users_sortby="user_account_enabled";
		$ros_cms_intern_users_sort="DESC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort=active&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">active</a>';
	}
	echo ' | ';
	if ($rpm_sort == "visible") {	
		echo '<b>visible</b>';
		$ros_cms_intern_users_sortby="user_account_hidden";
		$ros_cms_intern_users_sort="ASC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort=visible&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">visible</a>';
	}
	echo ' | ';
	if ($rpm_sort == "country") {	
		echo '<b>country</b>';
		$ros_cms_intern_users_sortby="user_country";
		$ros_cms_intern_users_sort="ASC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort=country&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">country</a>';
	}
	echo ' | ';
	if ($rpm_sort == "language") {	
		echo '<b>language</b>';
		$ros_cms_intern_users_sortby="user_language";
		$ros_cms_intern_users_sort="ASC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort=language&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">language</a>';
	}
	echo ' | ';
	if ($rpm_sort == "occupation") {	
		echo '<b>occupation</b>';
		$ros_cms_intern_users_sortby="user_occupation";
		$ros_cms_intern_users_sort="ASC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort=occupation&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">occupation</a>';
	}
	echo ' | ';
	if ($rpm_sort == "counter") {	
		echo '<b>counter</b>';
		$ros_cms_intern_users_sortby="user_login_counter";
		$ros_cms_intern_users_sort="DESC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort=counter&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">counter</a>';
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
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=all" class="letterbarlink">All</a> ';
	}

	if ($roscms_SET_letter == "a") {
		echo '  <b>A</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=a" class="letterbarlink">A</a> ';
	}

	if ($roscms_SET_letter == "b") {
		echo '  <b>B</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=b" class="letterbarlink">B</a> ';
	}

	if ($roscms_SET_letter == "c") {
		echo '  <b>C</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=c" class="letterbarlink">C</a> ';
	}

	if ($roscms_SET_letter == "d") {
		echo '  <b>D</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=d" class="letterbarlink">D</a> ';
	}

	if ($roscms_SET_letter == "e") {
		echo '  <b>E</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=e" class="letterbarlink">E</a> ';
	}

	if ($roscms_SET_letter == "f") {
		echo '  <b>F</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=f" class="letterbarlink">F</a> ';
	}

	if ($roscms_SET_letter == "g") {
		echo '  <b>G</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=g" class="letterbarlink">G</a> ';
	}

	if ($roscms_SET_letter == "h") {
		echo '  <b>H</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=h" class="letterbarlink">H</a> ';
	}

	if ($roscms_SET_letter == "i") {
		echo '  <b>I</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=i" class="letterbarlink">I</a> ';
	}

	if ($roscms_SET_letter == "j") {
		echo '  <b>J</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=j" class="letterbarlink">J</a> ';
	}

	if ($roscms_SET_letter == "k") {
		echo '  <b>K</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=k" class="letterbarlink">K</a> ';
	}

	if ($roscms_SET_letter == "l") {
		echo '  <b>L</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=l" class="letterbarlink">L</a> ';
	}

	if ($roscms_SET_letter == "m") {
		echo '  <b>M</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=m" class="letterbarlink">M</a> ';
	}
	
	if ($roscms_SET_letter == "n") {
		echo '  <b>N</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=n" class="letterbarlink">N</a> ';
	}
	
	if ($roscms_SET_letter == "o") {
		echo '  <b>O</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=o" class="letterbarlink">O</a> ';
	}

	if ($roscms_SET_letter == "p") {
		echo '  <b>P</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=p" class="letterbarlink">P</a> ';
	}

	if ($roscms_SET_letter == "q") {
		echo '  <b>Q</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=q" class="letterbarlink">Q</a> ';
	}

	if ($roscms_SET_letter == "r") {
		echo '  <b>R</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=r" class="letterbarlink">R</a> ';
	}

	if ($roscms_SET_letter == "s") {
		echo '  <b>S</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=s" class="letterbarlink">S</a> ';
	}

	if ($roscms_SET_letter == "t") {
		echo '  <b>T</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=t" class="letterbarlink">T</a> ';
	}

	if ($roscms_SET_letter == "u") {
		echo '  <b>U</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=u" class="letterbarlink">U</a> ';
	}

	if ($roscms_SET_letter == "v") {
		echo '  <b>V</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=v" class="letterbarlink">V</a> ';
	}

	if ($roscms_SET_letter == "w") {
		echo '  <b>W</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=w" class="letterbarlink">W</a> ';
	}

	if ($roscms_SET_letter == "x") {
		echo '  <b>X</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=x" class="letterbarlink">X</a> ';
	}

	if ($roscms_SET_letter == "y") {
		echo '  <b>Y</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=y" class="letterbarlink">Y</a> ';
	}

	if ($roscms_SET_letter == "z") {
		echo '  <b>Z</b> ';
	}
	else {
		echo '  <a href="?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;letter=z" class="letterbarlink">Z</a> ';
	}

	echo "</p>";



if (!$roscms_SET_curpos) {
	$roscms_SET_curpos = 0;
}

if ($roscms_SET_letter == "all") {
	$roscms_SET_letter = "%";
}

$query_count_cat=mysql_query("SELECT COUNT('user_id') 
								FROM `users` 
								WHERE `user_account_enabled` = 'yes'
								AND `user_account_hidden` = 'no'
								AND `user_name` LIKE  '" . $roscms_SET_letter . "%'
								ORDER BY `user_name` ASC ;");	
$result_count_cat = mysql_fetch_row($query_count_cat);

	echo "<p align='center'>";
	$j=0;
	for ($i=0; $i < $result_count_cat[0]; $i += $roscms_intern_items_per_page) {
		$j++;
		if ($roscms_SET_curpos == $i) {
			echo "<b>".$j."</b> ";
		}
		else {
			echo "<a href='?page=admin&amp;sec=users&amp;sec2=view&amp;sort=".$rpm_sort."&amp;filt=".$rpm_filt."&amp;opt=".$rpm_opt."&amp;langid=".$rpm_lang_id."&amp;letter=".$roscms_SET_letter."&amp;curpos=".$i."'>".$j."</a> ";
		}
	}
	$j=0;
	echo "</p>";

?> 
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="#5984C3"> 
      <td width="9%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Action</strong></font></div></td>
      <td width="8%" colspan="2" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Info</strong></font></div></td>
      <td width="7%" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>ID</strong></font></div></td>
      <td width="10%" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Nick</strong></font></div></td>
      <td width="13%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Touch</strong></font></div></td>
      <td width="15%" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Fullname</strong></font></div></td>
      <td width="7%" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Occup.</strong></font></div></td>
      <td width="7%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Counter</strong></font></div>
        <div align="center"></div></td>
      <td width="13%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong> 
          Country</strong></font></div></td>
      <td width="10%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Lang</strong></font></div></td>
    </tr>
    <?php
		
		$query_page = mysql_query("SELECT * 
				FROM users
				$ros_cms_intern_users_filt $ros_cms_intern_users_lang  AND `user_name` LIKE  '" . $roscms_SET_letter . "%' 
				ORDER BY '$ros_cms_intern_users_sortby' $ros_cms_intern_users_sort LIMIT ". $roscms_SET_curpos ." , ". $roscms_intern_items_per_page ." ;") ;

/*	if($roscms_intern_account_level==100) {
		$query_page = mysql_query("SELECT * 
				FROM users
				$ros_cms_intern_users_filt $ros_cms_intern_users_lang
				ORDER BY '$ros_cms_intern_users_sortby' $ros_cms_intern_users_sort LIMIT ". $roscms_SET_curpos ." , ". $roscms_intern_items_per_page ." ;") ;
	}
	else {
		$query_page = mysql_query("SELECT * 
				FROM users
				$ros_cms_intern_users_filt AND user_account_hidden != 0 $ros_cms_intern_users_lang
				ORDER BY '$ros_cms_intern_users_sortby' $ros_cms_intern_users_sort LIMIT ". $roscms_SET_curpos ." , ". $roscms_intern_items_per_page ." ;") ;
	}*/

	$color1=$roscms_intern_color1;
	$color2=$roscms_intern_color2;
	$colorcounter="0";
	//$farbe="#CCCCC";
	
	while($result_page = mysql_fetch_array($query_page)) { // users
?>
    <tr> 
      <td width="9%" valign="middle" bgcolor="<?php
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
        <div align="center"><a href="<?php echo "?page=user&amp;sec=profil&amp;sec2=".$result_page['user_id']; ?>"><img src="images/view.gif" alt="Profil" width="19" height="18" border="0"></a>&nbsp; 
          <?php
		if ($result_page['user_email'] != "") {
		?>
          <a href="mailto:<?php echo $result_page['user_email']; ?>"><img src="images/mail.gif" alt="<?php echo $result_page['user_email']; ?>" width="19" height="18" border="0"></a><?php echo "&nbsp;"; } if ($result_page['user_website'] != "") { ?><a href="<?php echo $result_page['user_website']; ?>" target="_blank"><img src="images/home.gif" alt="<?php echo $result_page['user_website']; ?>" width="19" height="18" border="0"></a> 
          <?php } ?>
        </div></td>
      <td width="4%" valign="middle" bgcolor="<?php echo $farbe; ?>">
<div align="center"> 
          <?php
		 if($result_page['user_account_enabled'] == "yes") { ?>
          <a href="<?php if($roscms_intern_account_level>50) { echo '?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;user_active=0&amp;user_active_set='.$result_page['user_id'] ; } else { echo '#'; } ?>"><img src="images/active.gif" alt="active" width="19" height="18" border="0"></a> 
          <?php
		 }
		 else { ?>
          <a href="<?php if($roscms_intern_account_level>50) { echo '?page=admin&amp;sec=users&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;user_active=1&amp;user_active_set='.$result_page['user_id'] ; } else { echo '#'; } ?>"><img src="images/notactive.gif" alt="NOT active" width="19" height="18" border="0"></a> 
          <?php } ?>
        </div></td>
      <td width="4%" valign="middle" bgcolor="<?php echo $farbe; ?>"> 
        <div align="center"> 
          <?php
		 if($result_page['user_account_hidden'] == "no") { ?>
          <img src="images/visible.gif" alt="visible" width="19" height="18" border="0"> 
          <?php
		 }
		 else { ?>
          <img src="images/notvisible.gif" alt="NOT visible" width="19" height="18" border="0"> 
          <?php } ?>
        </div>
        <div align="center"> </div></td>
      <td width="7%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"><?php echo "<b>".$result_page['user_id']."</b>"; ?></font></div></td>
      <td width="10%" valign="middle" bgcolor="<?php echo $farbe; ?>"><font face="Arial, Helvetica, sans-serif"> 
        <?php 
			echo $result_page['user_name'];
		?>
        </font></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>" title="<?php echo $result_page['user_timestamp_touch']; ?>"><div align="center"><font face="Arial, Helvetica, sans-serif"><?php echo $result_page['user_timestamp_touch2']; ?></font></div></td>
      <td width="15%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <font face="Arial, Helvetica, sans-serif"> 
        <?php 
			echo $result_page['user_fullname'];
		?>
        </font></td>
      <td width="7%" valign="middle" bgcolor="<?php echo $farbe; ?>" title="<?php echo $result_page['user_occupation']; ?>"><font face="Arial, Helvetica, sans-serif"> 
        <?php 
			echo substr(htmlentities($result_page['user_occupation'], ENT_QUOTES), 0, 10);
		?>
        </font></td>
      <td width="7%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="right"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['user_login_counter'];
		?>
          </font></div>
        <div align="center"><font face="Arial, Helvetica, sans-serif"> </font></div></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['user_country'];;
		?>
          </font></div></td>
      <td width="10%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['user_language'];
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
