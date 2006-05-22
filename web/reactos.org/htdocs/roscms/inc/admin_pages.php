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
		$seclog_section="roscms_admin_interface";
		$seclog_level="50";
		$seclog_reason="Hacking attempt: admin_pages.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}

	include("db/connect_db.inc.php"); // database connection script
	
	if ($rpm_page != "admin" && $rpm_page != "dev") {
		die("");
	}
?> 
<div class="contentSmall"> <span class="contentSmallTitle"><?php echo $roscms_langres['ContTrans_Interface_Pages']; ?></span> 
  <ul>
    <li><strong><?php echo $roscms_langres['ContTrans_Pages']; ?></strong></li>
  </ul>


  <p><?php echo $roscms_langres['ContTrans_Action'].": "; ?><a href="?page=<?php echo $rpm_page; ?>&amp;sec=pages&amp;sec2=edit&amp;opt=insert&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id ; ?>&amp;db_id=new"><?php echo $roscms_langres['ContTrans_NewPage']; ?></a> | <a href="?page=<?php echo $rpm_page; ?>&sec=pages&sec2=view"><?php echo $roscms_langres['ContTrans_resetfilters']; ?></a></p>
<?php
	if($roscms_intern_usrgrp_sadmin == true) {
                $rpm_page_active = "";
                $rpm_page_active_set="";
		if(array_key_exists("page_active", $_GET)) $rpm_page_active=$_GET["page_active"];
		if(array_key_exists("page_avtive_set", $_GET)) $rpm_page_active_set=$_GET["page_active_set"];
	
		if ($rpm_page_active != "" AND $rpm_page_active_set != "") {
			$page_postc="UPDATE `pages` SET `page_active` = '$rpm_page_active' WHERE `page_id` = '$rpm_page_active_set' LIMIT 1 ;";
			$page_post_listc=mysql_query($page_postc);
		}
	}


	if ($rpm_lang_id == "") {
		$rpm_lang_id="all";
	}
	echo '<p>'.$roscms_langres['ContTrans_Language'].': ';
	if ($rpm_lang_id == "all") {	
		echo '<b>'.$roscms_langres['ContTrans_All'].'</b>';
		$ros_cms_intern_pages_lang = "";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=all">'.$roscms_langres['ContTrans_All'].'</a>';
	}
	echo ' | ';
	if ($rpm_lang_id == "nolang") {	
		echo '<b>'.$roscms_langres['ContTrans_International'].'</b>';
		$ros_cms_intern_pages_lang = "AND page_language = 'all'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=nolang">'.$roscms_langres['ContTrans_International'].'</a>';
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
			$ros_cms_intern_pages_lang = "AND page_language = '".$roscms_sel_lang."'";
		}
		else {
			echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$roscms_sel_lang.'">'.$myrow_lang[1].'</a>';
		}
	}


	if ($rpm_filt == "") {
		$rpm_filt="active";
	}
	echo '<p>'.$roscms_langres['ContTrans_Filter'].': ';
	if ($rpm_filt == "active") {	
		echo '<b>'.$roscms_langres['ContTrans_activepages'].'</b>';
		$ros_cms_intern_pages_filt = "WHERE page_active = '1'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=active&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_activepages'].'</a>';
	}
	echo ' | ';
	if ($rpm_filt == "all") {	
		echo '<b>'.$roscms_langres['ContTrans_allpages'].'</b>';
		$ros_cms_intern_pages_filt = "WHERE `page_name` != ''";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=all&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_allpages'].'</a>';
	}
	echo ' | ';
	if ($rpm_filt == "user") {	
		echo '<b>'.$roscms_langres['ContTrans_currentuser'].'</b>';
		$ros_cms_intern_pages_filt = "WHERE page_usrname_id = '".$roscms_intern_account_id."'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=user&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_currentuser'].'</a>';
	}
	echo ' | ';
	if ($rpm_filt == "normalpages") {	
		echo '<b>'.$roscms_langres['ContTrans_normalpages'].'</b>';
		$ros_cms_intern_pages_filt = "WHERE pages_extra = ''";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=normalpages&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_normalpages'].'</a>';
	}
	echo ' | ';
	if ($rpm_filt == "dynamicpages") {	
		echo '<b>'.$roscms_langres['ContTrans_dynamicpages'].'</b>';
		$ros_cms_intern_pages_filt = "WHERE pages_extra != ''";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=dynamicpages&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_dynamicpages'].'</a>';
	}
	if ($rpm_filt == "history") {	
		echo ' | <b>'.$roscms_langres['ContTrans_history'].'</b>';
		$ros_cms_intern_pages_filt = "WHERE page_name = '".$rpm_opt."'";
	}
	echo '</p>';

	if ($rpm_sort == "") {
		$rpm_sort="id";
	}
	echo '<p>'.$roscms_langres['ContTrans_Sortedby'].': ';
	if ($rpm_sort == "id") {	
		echo '<b>'.$roscms_langres['ContTrans_pageid'].'</b>';
		$ros_cms_intern_pages_sortby="page_name";
		$ros_cms_intern_pages_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort=id&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_pageid'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "date") {	
		echo '<b>'.$roscms_langres['ContTrans_date'].'</b>';
		$ros_cms_intern_pages_sortby="page_date";
		$ros_cms_intern_pages_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort=date&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_date'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "user") {	
		echo '<b>'.$roscms_langres['ContTrans_user'].'</b>';
		$ros_cms_intern_pages_sortby="page_usrname_id";
		$ros_cms_intern_pages_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort=user&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_user'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "active") {	
		echo '<b>'.$roscms_langres['ContTrans_active'].'</b>';
		$ros_cms_intern_pages_sortby="page_active";
		$ros_cms_intern_pages_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort=active&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_active'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "visible") {	
		echo '<b>'.$roscms_langres['ContTrans_visible'].'</b>';
		$ros_cms_intern_pages_sortby="page_visible";
		$ros_cms_intern_pages_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort=visible&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_visible'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "version") {	
		echo '<b>'.$roscms_langres['ContTrans_version'].'</b>';
		$ros_cms_intern_pages_sortby="page_version";
		$ros_cms_intern_pages_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort=version&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_version'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "extra") {	
		echo '<b>'.$roscms_langres['ContTrans_extra'].'</b>';
		$ros_cms_intern_pages_sortby="pages_extra";
		$ros_cms_intern_pages_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort=extra&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_extra'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "language") {	
		echo '<b>'.$roscms_langres['ContTrans_language'].'</b>';
		$ros_cms_intern_pages_sortby="page_language";
		$ros_cms_intern_pages_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort=language&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_language'].'</a>';
	}
	echo '</p>';

?>
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="<?php echo $roscms_intern_color0; ?>"> 
      <td width="9%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Action']; ?></strong></font></div></td>
      <td width="8%" colspan="3" bgcolor="<?php echo $roscms_intern_color0; ?>"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Info']; ?></strong></font></div></td>
      <td width="13%" bgcolor="<?php echo $roscms_intern_color0; ?>"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_PageID']; ?></strong></font></div></td>
      <td width="13%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Language']; ?></strong></font></div></td>
      <td width="26%" bgcolor="<?php echo $roscms_intern_color0; ?>"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Content']; ?></strong></font></div></td>
      <td width="7%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Rev']; ?></strong></font></div>
        <div align="center"></div></td>
      <td width="13%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong> 
          <?php echo $roscms_langres['ContTrans_Date']; ?></strong></font></div></td>
      <td width="10%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_User']; ?></strong></font></div></td>
    </tr>
    <?php

	if($roscms_intern_account_level==100) {
		$query_page = mysql_query("SELECT * 
				FROM pages
				$ros_cms_intern_pages_filt $ros_cms_intern_pages_lang
				ORDER BY '$ros_cms_intern_pages_sortby' $ros_cms_intern_pages_sort") ;
	}
	else {
		$query_page = mysql_query("SELECT * 
				FROM pages
				$ros_cms_intern_pages_filt AND page_visible != 0 $ros_cms_intern_pages_lang
				ORDER BY '$ros_cms_intern_pages_sortby' $ros_cms_intern_pages_sort") ;
	}

	$farbe1=$roscms_intern_color1;
	$farbe2=$roscms_intern_color2;
	$zaehler="0";
	//$farbe="#CCCCC";
	
	while($result_page = mysql_fetch_array($query_page)) { // Pages
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
							 ?>" title="RosCMS action buttons:&#10;&#10;* View page&#10;* Delete page&#10;* Locked"> 
        <div align="center"><a href="?page=<?php echo $rpm_page; ?>&amp;sec=pages&amp;sec2=edit&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id.'&amp;db_id='.$result_page['page_id']; ?>"><img src="images/view.gif" alt="<?php echo $roscms_langres['ContTrans_Edit']; ?>" width="19" height="18" border="0"></a> 
          <?php if($roscms_intern_usrgrp_sadmin == true) { ?>
          <script type="text/javascript">
			<!--
				function DeletePage() {
					var chk = window.confirm("Do you really want to delete this page?");
					if (chk == true) {
						parent.location.href = "?page=<?php echo $rpm_page; ?>&amp;sec=pages&amp;sec2=delete&amp;db_id=<?php echo $result_page['page_id']; ?>";
					}
				}
			-->
			</script>
          <a href="javascript:DeletePage()"><img src="images/delete.gif" alt="Delete" width="19" height="18" border="0"></a> 
          <?php } ?>
          <a href="<?php echo "?page=".$rpm_page."&amp;sec=pages&amp;sec2=view&amp;sort=version&amp;filt=history&amp;opt=".$result_page['page_name']."&amp;langid=".$rpm_lang_id; ?>"><img src="images/history.gif" alt="<?php echo $roscms_langres['ContTrans_filter_history']; ?>" width="19" height="18" border="0"></a> 
        </div></td>
      <td width="3%" valign="middle" bgcolor="<?php echo $farbe; ?>"><div align="center"> 
          <?php
		 if($result_page['page_active'] == "1") { ?>
          <a href="<?php if($roscms_intern_usrgrp_admin == true) { echo '?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;page_active=0&amp;page_active_set='.$result_page['page_id'] ; } else { echo '#'; } ?>"><img src="images/active.gif" alt="active" width="19" height="18" border="0"></a> 
          <?php
		 }
		 else { ?>
          <a href="<?php if($roscms_intern_usrgrp_admin == true) { echo '?page='.$rpm_page.'&amp;sec=pages&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;page_active=1&amp;page_active_set='.$result_page['page_id'] ; } else { echo '#'; } ?>"><img src="images/notactive.gif" alt="NOT active" width="19" height="18" border="0"></a> 
          <?php } ?>
        </div></td>
      <td width="3%" valign="middle" bgcolor="<?php echo $farbe; ?>"><div align="center"> 
          <?php
		 if($result_page['page_visible'] == "1") { ?>
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
          <?php } ?>
        </div></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>" title="<?php echo $result_page['page_title']; ?>"> 
        <div align="left"><font face="Arial, Helvetica, sans-serif"><?php echo "<b>".$result_page['page_name']."</b>"; ?></font></div></td>
      <td width="13%" valign="middle" bgcolor="<?php if ($result_page['page_language'] != "") { echo $farbe; } else { echo "#FF0000"; } ?>"><font face="Arial, Helvetica, sans-serif"> 
        <div align="center"> 
          <?php 
		  	$cmsros_intern_temp_lang_short=$result_page['page_language'];
			$sql_lang="SELECT * 
				FROM languages
				WHERE lang_level != '0' AND lang_id = '$cmsros_intern_temp_lang_short'
				ORDER BY 'lang_level' DESC";
			$sql_query_lang=mysql_query($sql_lang);
			$myrow_lang=mysql_fetch_row($sql_query_lang); // Languages
			if ($myrow_lang[1] != "") {
				echo $myrow_lang[1];
			}
			else if ($result_page['page_language'] != "all") {
				echo $result_page['page_language'];
			}
		?>
        </div>
        </font></td>
      <td width="26%" valign="middle" bgcolor="<?php echo $farbe; ?>" title="<?php 
			if ($result_page['page_visible'] != 1) { echo "NOT visible!\n\n"; }
			echo substr(htmlentities($result_page['page_text'], ENT_QUOTES), 0, 200)."...";
		?>"> <font face="Arial, Helvetica, sans-serif"> 
        <pre><?php 
			echo substr(htmlentities($result_page['page_text'], ENT_QUOTES), 0, 40)."..." ;
		?></pre>
        </font></td>
      <td width="7%" valign="middle" bgcolor="<?php echo $farbe; ?>"> 
        <div align="right"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['page_version'];
		?>
          </font></div>
        <div align="center"><font face="Arial, Helvetica, sans-serif"> </font></div></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>"> 
        <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['page_date']." ".$result_page['page_time'];;
		?>
          </font></div></td>
      <td width="10%" valign="middle" bgcolor="<?php echo $farbe; ?>"> 
        <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			$accountinfo_query = @mysql_query("SELECT user_name, user_id FROM users WHERE user_id = '".$result_page['page_usrname_id']."'") or die('DB error (admin interface)!');
			$accountinfo_result = @mysql_fetch_array($accountinfo_query);

			$roscms_intern_accountuser = $accountinfo_result['user_name'];
			if ($roscms_intern_accountuser && $roscms_intern_accountuser != "") {
				echo "<b>".$roscms_intern_accountuser."</b><br><a href='?page=user&amp;sec=profil&amp;sec2=".$result_page['page_usrname_id']."' target='_blank'>".$roscms_langres['ContTrans_Profile']."</a>";
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
