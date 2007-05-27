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
		$seclog_reason="Hacking attempt: admin_dyncontent.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}

?>
 
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Dynamic 
  Content</span> 
  <ul>
    <li><strong>Dynamic Content</strong></li>
  </ul>
  <p>Action: <a href="?page=<?php echo $rpm_page; ?>&amp;sec=dyncontent&amp;sec2=edit&amp;sec3=news_page&amp;opt=insert&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id ; ?>&amp;db_id=new">
    <?php if ($roscms_intern_usrgrp_admin == true && $rpm_page == "admin") { ?>
    New News</a> |
	<a href="?page=<?php echo $rpm_page; ?>&amp;sec=dyncontent&amp;sec2=edit&amp;sec3=newsletter&amp;opt=insert&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id ; ?>&amp;db_id=new">New Newsletter</a> | 
	<a href="?page=<?php echo $rpm_page; ?>&amp;sec=dyncontent&amp;sec2=edit&amp;sec3=interview&amp;opt=insert&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id ; ?>&amp;db_id=new">New Interview</a> | 
	<?php /* <a href="?page=<?php echo $rpm_page; ?>&amp;sec=dyncontent&amp;sec2=edit&amp;sec3=blog&amp;opt=insert&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id ; ?>&amp;db_id=new">New Blog</a> |  */ ?>
	<a href="?page=<?php echo $rpm_page; ?>&amp;sec=dyncontent&amp;sec2=edit&amp;opt=insert&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id ; ?>&amp;db_id=new">New Dynamic Content (plain)</a> | 
	<a href="?page=<?php echo $rpm_page; ?>&sec=dyncontent&sec2=view">
    <?php } ?>
    reset filters & sort</a></p>
    
  <?php
	if($roscms_intern_account_level>50) {
		$rpm_dyn_content_active="";
		$rpm_dyn_content_active_set="";
		if(array_key_exists("dyn_content_active", $_GET)) $rpm_dyn_content_active=$_GET["dyn_content_active"];
		if(array_key_exists("dyn_content_active_set", $_GET)) $rpm_dyn_content_active_set=$_GET["dyn_content_active_set"];

		if ($rpm_dyn_content_active != "" AND $rpm_dyn_content_active_set != "") {
			$content_postc="UPDATE `content` SET `dyn_content_active` = '$rpm_dyn_content_active' WHERE `content_id` = '$rpm_dyn_content_active_set' LIMIT 1 ;";
			$content_post_listc=mysql_query($content_postc);
		}
	}


	if ($rpm_lang_id == "") {
		$rpm_lang_id="nolang";
	}
	echo '<p>Language: ';
	if ($roscms_intern_usrgrp_dev == true || $roscms_intern_usrgrp_admin == true) {
		if ($rpm_lang_id == "all") {	
			echo '<b>All</b>';
			$ros_cms_intern_dyn_content_lang = "";
		}
		else {
			echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid=all">All</a>';
		}
		echo ' | ';
	}
	if ($rpm_lang_id == "nolang") {	
		echo '<b>International</b>';
		$ros_cms_intern_dyn_content_lang = "AND dyn_content_lang = 'all'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid=nolang">International</a>';
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
			$ros_cms_intern_dyn_content_lang = "AND dyn_content_lang = '".$roscms_sel_lang."'";
		}
		else {
			echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$roscms_sel_lang.'">'.$myrow_lang[1].'</a>';
		}
	}


	if ($rpm_filt == "") {
		$rpm_filt="active";
	}
	echo '<p>Filter: ';
	if ($rpm_filt == "active") {	
		echo '<b>active content</b>';
		$ros_cms_intern_content_filt = "WHERE dyn_content_active = '1'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=active&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">active content</a>';
	}
	echo ' | ';
	if ($rpm_filt == "all") {	
		echo '<b>all content</b>';
		$ros_cms_intern_content_filt = "WHERE `dyn_content_name` != ''";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=all&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">all content</a>';
	}
	echo ' | ';
	if ($rpm_filt == "user") {	
		echo '<b>current user</b>';
		$ros_cms_intern_content_filt = "WHERE dyn_content_usrname_id = '".$roscms_intern_account_id."'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=user&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">current user</a>';
	}
	if ($rpm_filt == "history") {	
		echo ' | <b>history</b>';
		$ros_cms_intern_content_filt = "WHERE dyn_content_id = '".$rpm_opt."'";
	}
	echo '</p>';


	if ($rpm_filt2 == "") {
		$rpm_filt2="all";
	}
	echo '<p>Sub section: ';
	if ($rpm_filt2 == "all") {	
		echo '<b>all</b>';
		$ros_cms_intern_content_filt2 = "";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2=all&amp;langid='.$rpm_lang_id.'">all</a>';
	}
	echo ' | ';
	if ($rpm_filt2 == "news") {	
		echo '<b>news</b>';
		$ros_cms_intern_content_filt2 = " AND `dyn_content_name` = 'news_page'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2=news&amp;langid='.$rpm_lang_id.'">news</a>';
	}
	echo ' | ';
	if ($rpm_filt2 == "newsletter") {	
		echo '<b>newsletter</b>';
		$ros_cms_intern_content_filt2 = " AND `dyn_content_name` = 'newsletter'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2=newsletter&amp;langid='.$rpm_lang_id.'">newsletter</a>';
	}
	echo ' | ';
	if ($rpm_filt2 == "interview") {	
		echo '<b>interview</b>';
		$ros_cms_intern_content_filt2 = " AND `dyn_content_name` = 'interview'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2=interview&amp;langid='.$rpm_lang_id.'">interview</a>';
	}
/*	echo ' | ';
	if ($rpm_filt2 == "blog") {	
		echo '<b>blog</b>';
		$ros_cms_intern_content_filt2 = " AND `dyn_content_name` = 'blog'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2=blog&amp;langid='.$rpm_lang_id.'">blog</a>';
	}*/
	/*echo ' | ';
	if ($rpm_filt2 == "dev_page") {	
		echo '<b>dev page</b>';
		$ros_cms_intern_content_filt2 = " AND `dyn_content_name` = 'dev_page'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2=dev_page&amp;langid='.$rpm_lang_id.'">dev page</a>';
	}*/
	echo '</p>';


	if ($rpm_sort == "") {
		$rpm_sort="id";
	}
	echo '<p>Sorted by: ';
	if ($rpm_sort == "id") {	
		echo '<b>content id</b>';
		$ros_cms_intern_content_sortby="dyn_content_id";
		$ros_cms_intern_content_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort=id&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">content id</a>';
	}
	echo ' | ';
	if ($rpm_sort == "nr") {	
		echo '<b>entry nr</b>';
		$ros_cms_intern_content_sortby="dyn_content_nr";
		$ros_cms_intern_content_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort=nr&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">entry nr</a>';
	}
	echo ' | ';
	if ($rpm_sort == "name") {	
		echo '<b>name</b>';
		$ros_cms_intern_content_sortby="dyn_content_name";
		$ros_cms_intern_content_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort=name&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">name</a>';
	}
	echo ' | ';
	if ($rpm_sort == "date") {	
		echo '<b>date</b>';
		$ros_cms_intern_content_sortby="dyn_content_date";
		$ros_cms_intern_content_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort=date&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">date</a>';
	}
	echo ' | ';
	if ($rpm_sort == "user") {	
		echo '<b>user</b>';
		$ros_cms_intern_content_sortby="dyn_content_usrname_id";
		$ros_cms_intern_content_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort=user&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">user</a>';
	}
	echo ' | ';
	if ($rpm_sort == "active") {	
		echo '<b>active</b>';
		$ros_cms_intern_content_sortby="dyn_content_active";
		$ros_cms_intern_content_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort=active&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">active</a>';
	}
	echo ' | ';
	if ($rpm_sort == "visible") {	
		echo '<b>visible</b>';
		$ros_cms_intern_content_sortby="dyn_content_visible";
		$ros_cms_intern_content_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort=visible&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">visible</a>';
	}
	echo ' | ';
	if ($rpm_sort == "version") {	
		echo '<b>version</b>';
		$ros_cms_intern_content_sortby="dyn_content_version";
		$ros_cms_intern_content_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort=version&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">version</a>';
	}
	echo ' | ';
	if ($rpm_sort == "language") {	
		echo '<b>language</b>';
		$ros_cms_intern_content_sortby="dyn_content_lang";
		$ros_cms_intern_content_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort=language&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'">language</a>';
	}
	echo '</p>';

	if ($rpm_page == "trans" || $rpm_page == "team")  {
		echo "<p><i>The <b>dynamic content interface</b> for translators is <b>under heavy construction</b>! </i></p>";
	}
?>
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="#5984C3"> 
      <td width="9%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Action</strong></font></div></td>
      <td width="8%" colspan="3" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Info</strong></font></div></td>
      <td width="3%" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong> 
          ID</strong></font></div></td>
      <td width="6%" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Nr</strong></font></div></td>
      <td width="13%" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Name</strong></font></div></td>
      <td width="7%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Lang</strong></font></div></td>
      <td width="20%" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Content</strong></font></div></td>
      <td width="7%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Rev.</strong></font></div>
        <div align="center"></div></td>
      <td width="13%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong> 
          Date</strong></font></div></td>
      <td width="10%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>User</strong></font></div></td>
    </tr>
    <?php

	if($roscms_intern_account_level>=50) {
		$query_content = mysql_query("SELECT * 
				FROM dyn_content
				$ros_cms_intern_content_filt $ros_cms_intern_content_filt2 $ros_cms_intern_dyn_content_lang
				ORDER BY '$ros_cms_intern_content_sortby' $ros_cms_intern_content_sort") ;
	}
	else {
		$query_content = mysql_query("SELECT * 
				FROM dyn_content
				$ros_cms_intern_content_filt $ros_cms_intern_content_filt2 AND dyn_dyn_content_visible != 0 $ros_cms_intern_dyn_content_lang
				ORDER BY '$ros_cms_intern_content_sortby' $ros_cms_intern_content_sort") ;
	}

	$farbe1=$roscms_intern_color1;
	$farbe2=$roscms_intern_color2;
	$zaehler="0";
	//$farbe="#CCCCC";
	
	while($result_content = mysql_fetch_array($query_content)) { // content
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
        <div align="center">
		  <?php
		  if (($rpm_page == "trans" || $rpm_page == "team") && $rpm_lang_id == "nolang") { ?>
		<a href="?page=<?php echo $rpm_page; ?>&amp;sec=dyncontent&amp;sec2=edit&amp;opt=translate&amp;<?php echo 'sec3='.$result_content['dyn_content_name'].'&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'&amp;db_id='.$result_content['dyn_id']; ?>"><img src="images/tool.gif" alt="Translate" width="19" height="18" border="0"></a> 
		<?php } else { ?>
		<a href="?page=<?php echo $rpm_page; ?>&amp;sec=dyncontent&amp;sec2=edit&amp;<?php echo 'sec3='.$result_content['dyn_content_name'].'&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'&amp;db_id='.$result_content['dyn_id']; ?>"><img src="images/view.gif" alt="View" width="19" height="18" border="0"></a> 
          <?php } ?>
          <a href="<?php echo "?page=".$rpm_page."&amp;sec=dyncontent&amp;sec2=view&amp;sort=version&amp;filt=history&amp;opt=".$result_content['dyn_content_id']."&amp;filt2=".$rpm_filt2."&amp;langid=".$result_content['dyn_content_lang']; ?>"><img src="images/history.gif" alt="Filter: history" width="19" height="18" border="0"></a> 
        </div></td>
      <td width="3%" valign="middle" bgcolor="<?php echo $farbe; ?>"><div align="center"> 
          <?php
		 if($result_content['dyn_content_active'] == "1") { ?>
          <a href="<?php if($roscms_intern_account_level>50) { echo '?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'&amp;dyn_content_active=0&amp;dyn_content_active_set='.$result_content['dyn_id'] ; } else { echo '#'; } ?>"><img src="images/active.gif" alt="active" width="19" height="18" border="0"></a> 
          <?php
		 }
		 else { ?>
          <a href="<?php if($roscms_intern_account_level>50) { echo '?page='.$rpm_page.'&amp;sec=dyncontent&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id.'&amp;dyn_content_active=1&amp;dyn_content_active_set='.$result_content['dyn_id'] ; } else { echo '#'; } ?>"><img src="images/notactive.gif" alt="NOT active" width="19" height="18" border="0"></a> 
          <?php } ?>
        </div></td>
      <td width="3%" valign="middle" bgcolor="<?php echo $farbe; ?>"><div align="center"> 
          <?php
		 if($result_content['dyn_content_visible'] == "1") { ?>
          <img src="images/visible.gif" alt="visible" width="19" height="18" border="0"> 
          <?php
		 }
		 else { ?>
          <img src="images/notvisible.gif" alt="NOT visible" width="19" height="18" border="0"> 
          <?php } ?>
        </div></td>
      <td width="3%" valign="middle" bgcolor="<?php echo $farbe; ?>"><div align="center"> 
          <?php if($roscms_intern_account_level<=10) { ?>
          <img src="images/lock.gif" alt="Locked" width="19" height="18"> 
          <?php } else if ($result_content['dyn_content_editor'] == "richtext") { ?>
          <img src="images/richtexteditor.gif" alt="Rich Text Editor" width="19" height="18"> 
          <?php } else if ($result_content['dyn_content_editor'] == "bbcode") { ?>
          <img src="images/bbcode.gif" alt="bbcode Editor" width="19" height="18"> 
          <?php } ?>
        </div></td>
      <td width="3%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="left"><font face="Arial, Helvetica, sans-serif"><?php echo $result_content['dyn_content_id']; ?></font></div></td>
      <td width="6%" valign="middle" bgcolor="<?php echo $farbe; ?>"><font face="Arial, Helvetica, sans-serif"><?php echo $result_content['dyn_content_nr']; ?></font></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>"><font face="Arial, Helvetica, sans-serif"><b><?php echo $result_content['dyn_content_name']; ?></b></font></td>
      <td width="7%" valign="middle" bgcolor="<?php if ($result_content['dyn_content_lang'] != "") { echo $farbe; } else { echo "#FF0000"; } ?>"> 
        <div align="center"> <font face="Arial, Helvetica, sans-serif"> 
          <?php 
		  	$cmsros_intern_temp_lang_short=$result_content['dyn_content_lang'];
			$sql_lang="SELECT * 
				FROM languages
				WHERE lang_level != '0' AND lang_id = '$cmsros_intern_temp_lang_short'
				ORDER BY 'lang_level' DESC";
			$sql_query_lang=mysql_query($sql_lang);
			$myrow_lang=mysql_fetch_row($sql_query_lang); // Languages
			if ($myrow_lang[1] != "") {
				echo $myrow_lang[1];
			}
			else if ($result_content['dyn_content_lang'] != "all") {
				echo $result_content['dyn_content_lang'];
			}
		?>
          </font></div></td>
      <td width="20%" valign="middle" bgcolor="<?php echo $farbe; ?>" title="<?php 
			if ($result_content['dyn_content_visible'] != 1) { echo "NOT visible!\n\n"; }
			echo substr(htmlentities($result_content['dyn_content_text1'], ENT_QUOTES), 0, 200)."...";
		?>"> <pre><font face="Arial, Helvetica, sans-serif"><?php 
			echo substr(htmlentities($result_content['dyn_content_text1'], ENT_QUOTES), 0, 40)."..." ;
		?></font></pre></td>
      <td width="7%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="right"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_content['dyn_content_version'];
		?>
          </font></div>
        <div align="center"><font face="Arial, Helvetica, sans-serif"> </font></div></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_content['dyn_content_date']." ".$result_content['dyn_content_time'];;
		?>
          </font></div></td>
      <td width="10%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			$accountinfo_query = @mysql_query("SELECT user_name, user_id FROM users WHERE user_id = '".$result_content['dyn_content_usrname_id']."'") or die('DB error (admin interface)!');
			$accountinfo_result = @mysql_fetch_array($accountinfo_query);
			
			$roscms_intern_accountuser = $accountinfo_result['user_name'];
			if ($roscms_intern_accountuser && $roscms_intern_accountuser != "") {
				echo "<b>".$roscms_intern_accountuser."</b><br><a href='?page=user&amp;sec=profil&amp;sec2=".$result_content['dyn_content_usrname_id']."' target='_blank'>Profile</a>";
			}
			else {
				echo "<b>RosCMS</b>";
			}

		?>
          </font></div></td>
    </tr>
    <?php	
	}	// end while
?>
  </table>
  <?php
	include("inc/inc_description_table.php");
?>
</div>
