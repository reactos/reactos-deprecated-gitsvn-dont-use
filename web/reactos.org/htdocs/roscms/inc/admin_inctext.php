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
		$seclog_reason="Hacking attempt: admin_inctext.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}

?>
 
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Include 
  Text </span> 
  <ul>
    <li><strong>Include Text</strong></li>
  </ul>
  <p>Action: <a href="?page=admin&amp;sec=inctext&amp;sec2=edit&amp;opt=insert&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id ; ?>&amp;db_id=new">New 
    IncText</a> | <a href="?page=admin&sec=inctext&sec2=view">reset filters & 
    sort</a></p>
    
  <?php

	if ($rpm_lang_id == "") {
		$rpm_lang_id="all";
	}
	echo '<p>Language: ';
	if ($rpm_lang_id == "all") {	
		echo '<b>All</b>';
		$ros_cms_intern_inctext_lang = "";
	}
	else {
		echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=all">All</a>';
	}
	echo ' | ';
	if ($rpm_lang_id == "nolang") {	
		echo '<b>International</b>';
		$ros_cms_intern_inctext_lang = "AND inc_lang = 'all'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=nolang">International</a>';
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
			$ros_cms_intern_inctext_lang = "AND inc_lang = '".$roscms_sel_lang."'";
		}
		else {
			echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$roscms_sel_lang.'">'.$myrow_lang[1].'</a>';
		}
	}


	if ($rpm_filt == "") {
		$rpm_filt="visible";
	}
	echo '<p>Filter: ';
	if ($rpm_filt == "visible") {	
		echo '<b>visible inctext</b>';
		$ros_cms_intern_inctext_filt = "WHERE inc_vis = '1'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=visible&amp;langid='.$rpm_lang_id.'">visible inctext</a>';
	}
	echo ' | ';
	if ($rpm_filt == "all") {	
		echo '<b>all inctext</b>';
		$ros_cms_intern_inctext_filt = "WHERE `inc_word` != ''";
	}
	else {
		echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=all&amp;langid='.$rpm_lang_id.'">all inctext</a>';
	}
	echo ' | ';
	if ($rpm_filt == "user") {	
		echo '<b>current user</b>';
		$ros_cms_intern_inctext_filt = "WHERE inc_usrname_id = '".$roscms_intern_account_id."'";
	}
	else {
		echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=user&amp;langid='.$rpm_lang_id.'">current user</a>';
	}
	if ($rpm_filt == "history") {	
		echo ' | <b>history</b>';
		$ros_cms_intern_inctext_filt = "WHERE inc_name = '".$rpm_opt."'";
	}
	echo '</p>';

	if ($rpm_sort == "") {
		$rpm_sort="id";
	}
	echo '<p>Sorted by: ';
	if ($rpm_sort == "id") {	
		echo '<b>inctext id</b>';
		$ros_cms_intern_inctext_sortby="inc_word";
		$ros_cms_intern_inctext_sort="ASC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort=id&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">inctext id</a>';
	}
	echo ' | ';
	if ($rpm_sort == "date") {	
		echo '<b>date</b>';
		$ros_cms_intern_inctext_sortby="inc_date";
		$ros_cms_intern_inctext_sort="DESC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort=date&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">date</a>';
	}
	echo ' | ';
	if ($rpm_sort == "user") {	
		echo '<b>user</b>';
		$ros_cms_intern_inctext_sortby="inc_usrname_id";
		$ros_cms_intern_inctext_sort="ASC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort=user&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">user</a>';
	}
	echo ' | ';
	if ($rpm_sort == "level") {	
		echo '<b>level</b>';
		$ros_cms_intern_inctext_sortby="inc_level";
		$ros_cms_intern_inctext_sort="DESC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort=level&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">level</a>';
	}
	echo ' | ';
	if ($rpm_sort == "visible") {	
		echo '<b>visible</b>';
		$ros_cms_intern_inctext_sortby="inc_vis";
		$ros_cms_intern_inctext_sort="DESC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort=visible&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">visible</a>';
	}
	echo ' | ';
	if ($rpm_sort == "language") {	
		echo '<b>language</b>';
		$ros_cms_intern_inctext_sortby="inc_lang";
		$ros_cms_intern_inctext_sort="ASC";
	}
	else {
		echo '<a href="?page=admin&amp;sec=inctext&amp;sec2=view&amp;sort=language&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">language</a>';
	}
	echo '</p>';

?>
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="#5984C3"> 
      <td width="9%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Action</strong></font></div></td>
      <td width="8%" colspan="2" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Info</strong></font></div></td>
      <td width="13%" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>IncText 
          ID</strong></font></div></td>
      <td width="13%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Language</strong></font></div></td>
      <td width="26%" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>inctext</strong></font></div></td>
      <td width="7%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Ext.</strong></font></div>
        <div align="center"></div></td>
      <td width="13%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong> 
          Date</strong></font></div></td>
      <td width="10%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>User</strong></font></div></td>
    </tr>
    <?php

	if($roscms_intern_account_level==100) {
		$query_inctext = mysql_query("SELECT * 
				FROM include_text
				$ros_cms_intern_inctext_filt $ros_cms_intern_inctext_lang
				ORDER BY '$ros_cms_intern_inctext_sortby' $ros_cms_intern_inctext_sort") ;
		/*$query_inctext = mysql_query("SELECT * 
				FROM inctext
				$ros_cms_intern_inctext_filt $ros_cms_intern_inctext_lang
				ORDER BY '$ros_cms_intern_inctext_sortby' $ros_cms_intern_inctext_sort") ;*/
	}
	else {
		$query_inctext = mysql_query("SELECT * 
				FROM include_text
				$ros_cms_intern_inctext_filt AND inc_vis != 0 AND `inc_extra` = '' $ros_cms_intern_inctext_lang
				ORDER BY '$ros_cms_intern_inctext_sortby' $ros_cms_intern_inctext_sort") ;
	}

	$farbe1=$roscms_intern_color1;
	$farbe2=$roscms_intern_color2;
	$zaehler="0";
	//$farbe="#CCCCC";
	
	while($result_inctext = mysql_fetch_array($query_inctext)) { // inctext
?>
    <tr> 
      <td width="8%" valign="middle" bgcolor="<?php
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
							 ?>" title="RosCMS action buttons:&#10;&#10;* View inctext&#10;* Delete inctext&#10;* Locked"> 
        <div align="center"><a href="?page=admin&amp;sec=inctext&amp;sec2=edit&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id.'&amp;db_id='.$result_inctext['inc_id']; ?>"><img src="images/view.gif" alt="View" width="19" height="18" border="0"></a> 

        </div></td>
      <td width="5%" valign="middle" bgcolor="<?php echo $farbe; ?>"> 
        <div align="center"> 
          <?php
		 if($result_inctext['inc_vis'] == "1") { ?>
          <img src="images/visible.gif" alt="visible" width="19" height="18" border="0"> 
          <?php
		 }
		 else { ?>
          <img src="images/notvisible.gif" alt="NOT visible" width="19" height="18" border="0"> 
          <?php } ?>
        </div>
        <div align="center"> </div></td>
      <td width="5%" valign="middle" bgcolor="<?php echo $farbe; ?>">
<div align="center"> 
          <?php if($roscms_intern_account_level<=10) { ?>
          <img src="images/lock.gif" alt="Locked" width="19" height="18"> 
          <?php } else if (0/*$result_inctext['inc_editor'] == "richtext"*/) { ?>
          <img src="images/richtexteditor.gif" alt="Rich Text Editor" width="19" height="18"> 
          <?php } ?>
        </div></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="left"><font face="Arial, Helvetica, sans-serif"><?php echo "[#inc_<b>".$result_inctext['inc_word']."</b>]"; ?></font></div></td>
      <td width="13%" valign="middle" bgcolor="<?php if ($result_inctext['inc_lang'] != "") { echo $farbe; } else { echo "#FF0000"; } ?>"> 
        <div align="center"> <font face="Arial, Helvetica, sans-serif"> 
          <?php 
		  	$cmsros_intern_temp_lang_short=$result_inctext['inc_lang'];
			$sql_lang="SELECT * 
				FROM languages
				WHERE lang_level != '0' AND lang_id = '$cmsros_intern_temp_lang_short'
				ORDER BY 'lang_level' DESC";
			$sql_query_lang=mysql_query($sql_lang);
			$myrow_lang=mysql_fetch_row($sql_query_lang); // Languages
			if ($myrow_lang[1] != "") {
				echo $myrow_lang[1];
			}
			else if ($result_inctext['inc_lang'] != "all") {
				echo $result_inctext['inc_lang'];
			}
		?>
          </font></div></td>
      <td width="26%" valign="middle" bgcolor="<?php echo $farbe; ?>" title="<?php 
			if ($result_inctext['inc_vis'] != 1) { echo "NOT visible!\n\n"; }
			echo substr(htmlentities($result_inctext['inc_text'], ENT_QUOTES), 0, 200)."...";
		?>"> <pre><font face="Arial, Helvetica, sans-serif"><?php 
			echo substr(htmlentities($result_inctext['inc_text'], ENT_QUOTES), 0, 40)."..." ;
		?></font></pre></td>
      <td width="7%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo substr($result_inctext['inc_extra'], 0, 7);
		?>
          </font></div>
        <div align="center"><font face="Arial, Helvetica, sans-serif"> </font></div></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_inctext['inc_date']." ".$result_inctext['inc_time'];;
		?>
          </font></div></td>
      <td width="10%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_inctext['inc_usrname_id'];
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
