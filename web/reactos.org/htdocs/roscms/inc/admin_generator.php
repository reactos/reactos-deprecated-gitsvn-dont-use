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
?>
<div class="contentSmall"> <span class="contentSmallTitle"><?php
	if ($rpm_page == "admin") {
		echo "Admin Interface - Page Generator";
	}
	elseif ($rpm_page == "dev") {
		echo $roscms_langres['ContTrans_DevPageGenerator'];
	}
  ?></span>
<p><?php echo $roscms_langres['ContTrans_Gen_desc']; ?></p>
    <table border="0" cellspacing="0" cellpadding="0" width="650">
    <tr> 
      <td colspan="3"><span class="contentSmallTitle"><?php echo $roscms_langres['Dev_PageGeneratorOverview']; ?></span></td>
    </tr>
    <tr> 
      <td colspan="2" bgcolor="#F9F8F8"> <table width="650" border="0" cellpadding="4">
          <tr>
            <td width="20" valign="top"><div align="center"><img src="images/dot.gif" vspace="3" /></div></td>
            <td width="300" valign="top"><strong><font face="Arial, Helvetica, sans-serif"><a href="#gensinglepage" title="Generate one static page (if you want to update one specific page)"><?php echo $roscms_langres['ContTrans_Gen_menu3']; ?></a> </font></strong></td>
            <td width="10" valign="top">&nbsp;</td>
            <td valign="top">
            <div align="center"><img src="images/dot.gif" vspace="3" /></div></td>
            <td width="300" valign="top">
            <div align="left">
              <p><font face="Arial, Helvetica, sans-serif"><a href="?page=<?php echo $rpm_page; ?>&amp;sec=generator&amp;sec2=view&amp;sec3=menu&amp;site=index&amp;gen_lang=en" title="View the static homepage in a dynamic way"><strong><?php echo $roscms_langres['ContTrans_Gen_menu4']; ?></strong></a></font></p>
              </div></td>
          </tr>
          <tr>
            <td valign="top"><div align="center"><img src="images/dot.gif" vspace="3" /></div></td>
            <td valign="top"><font face="Arial, Helvetica, sans-serif"><strong><a href="?page=<?php echo $rpm_page; ?>&amp;sec=generator&amp;sec2=output&amp;newcontent=true" title="Generate all UPDATED static pages! (except content that is visible on several pages, e.g. menu bars -&gt; then use the 'generate all pages' link instead!)"><?php echo $roscms_langres['ContTrans_Gen_menu1']; ?></a></strong> <br />
                <?php 
				
				// Languages
				$sql_lang="SELECT * 
							FROM languages
							WHERE lang_level != '0'
							ORDER BY 'lang_level' DESC";
				$sql_query_lang=mysql_query($sql_lang);
				while($myrow_lang=mysql_fetch_row($sql_query_lang)) {
					echo '<a href="?page='.$rpm_page.'&amp;sec=generator&amp;sec2=output&amp;newcontent=true&amp;gen_lang='.$myrow_lang[0].'">'.$myrow_lang[1].'</a> ';
				}
			
			?>
            </font></td>
            <td valign="top">&nbsp;</td>
            <td valign="top"><div align="center"><strong><img src="images/dot.gif" vspace="3" /></strong></div></td>
            <td valign="top"><div align="left"><font face="Arial, Helvetica, sans-serif"><a href="?page=<?php echo $rpm_page; ?>&amp;sec=generator&amp;sec2=output" title="Generate all static pages (only if you need to update all pages, e.g. one time per day)"><strong><?php echo $roscms_langres['ContTrans_Gen_menu2']; ?></strong></a><br />
			<?php 
				
				// Languages
				$sql_lang="SELECT * 
							FROM languages
							WHERE lang_level != '0'
							ORDER BY 'lang_level' DESC";
				$sql_query_lang=mysql_query($sql_lang);
				while($myrow_lang=mysql_fetch_row($sql_query_lang)) {
					echo '<a href="?page='.$rpm_page.'&amp;sec=generator&amp;sec2=output&amp;gen_lang='.$myrow_lang[0].'">'.$myrow_lang[1].'</a> ';
				}
			
			?>
            </font></div></td>
          </tr>
        </table></td>
    </tr>
    <tr bgcolor="#AEADAD"> 
      <td><strong><img src="images/line.gif" width="1" height="1" /></strong></td>
    </tr>
  </table>
  <p>&nbsp; </p>
  <span class="contentSmallTitle"><a name="gensinglepage" id="gensinglepage"></a><?php echo $roscms_langres['ContTrans_Gen_menu3']; ?></span> 
  <p><?php echo $roscms_langres['ContTrans_Gen_menu3_desc']; ?></p>
 <?php
	if($roscms_intern_account_level>50) {
		$rpm_page_active="";
		$rpm_page_active_set="";
		if (array_key_exists("page_active", $_GET)) $rpm_page_active=$_GET["page_active"];
		if (array_key_exists("page_active_set", $_GET)) $rpm_page_active_set=$_GET["page_active_set"];

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
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=all#gensinglepage">'.$roscms_langres['ContTrans_All'].'</a>';
	}
	echo ' | ';
	if ($rpm_lang_id == "nolang") {	
		echo '<b>'.$roscms_langres['ContTrans_International'].'</b>';
		$ros_cms_intern_pages_lang = "AND page_language = 'all'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=nolang#gensinglepage">'.$roscms_langres['ContTrans_International'].'</a>';
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
			echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$roscms_sel_lang.'#gensinglepage">'.$myrow_lang[1].'</a>';
		}
	}


	if ($rpm_filt == "") {
		$rpm_filt="active";
	}
	echo '<p>'.$roscms_langres['ContTrans_Filter'].': ';
	if ($rpm_filt == "active") {	
		echo '<b>'.$roscms_langres['ContTrans_allactivepages'].'</b>';
		$ros_cms_intern_pages_filt = "WHERE page_active = '1'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=active&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_allactivepages'].'</a>';
	}
	echo ' | ';
	if ($rpm_filt == "user") {	
		echo '<b>'.$roscms_langres['ContTrans_currentuser'].'</b>';
		$ros_cms_intern_pages_filt = "WHERE page_active = '1' AND page_usrname_id = '".$roscms_intern_account_id."'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=user&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_currentuser'].'</a>';
	}
	echo ' | ';
	if ($rpm_filt == "normalpages") {	
		echo '<b>'.$roscms_langres['ContTrans_normalpages'].'</b>';
		$ros_cms_intern_pages_filt = "WHERE page_active = '1' AND pages_extra = ''";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=normalpages&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_normalpages'].'</a>';
	}
	echo ' | ';
	if ($rpm_filt == "dynamicpages") {	
		echo '<b>'.$roscms_langres['ContTrans_dynamicpages'].'</b>';
		$ros_cms_intern_pages_filt = "WHERE page_active = '1' AND pages_extra != ''";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=dynamicpages&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_dynamicpages'].'</a>';
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
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort=id&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_pageid'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "date") {	
		echo '<b>'.$roscms_langres['ContTrans_date'].'</b>';
		$ros_cms_intern_pages_sortby="page_date";
		$ros_cms_intern_pages_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort=date&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_date'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "user") {	
		echo '<b>'.$roscms_langres['ContTrans_user'].'</b>';
		$ros_cms_intern_pages_sortby="page_usrname_id";
		$ros_cms_intern_pages_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort=user&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_user'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "active") {	
		echo '<b>'.$roscms_langres['ContTrans_active'].'</b>';
		$ros_cms_intern_pages_sortby="page_active";
		$ros_cms_intern_pages_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort=active&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_active'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "visible") {	
		echo '<b>'.$roscms_langres['ContTrans_visible'].'</b>';
		$ros_cms_intern_pages_sortby="page_visible";
		$ros_cms_intern_pages_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort=visible&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_visible'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "version") {	
		echo '<b>'.$roscms_langres['ContTrans_version'].'</b>';
		$ros_cms_intern_pages_sortby="page_version";
		$ros_cms_intern_pages_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort=version&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_version'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "extra") {	
		echo '<b>'.$roscms_langres['ContTrans_extra'].'</b>';
		$ros_cms_intern_pages_sortby="pages_extra";
		$ros_cms_intern_pages_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort=extra&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_extra'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "language") {	
		echo '<b>'.$roscms_langres['ContTrans_language'].'</b>';
		$ros_cms_intern_pages_sortby="page_language";
		$ros_cms_intern_pages_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=generate&amp;sec2=view&amp;sort=language&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'#gensinglepage">'.$roscms_langres['ContTrans_language'].'</a>';
	}
	echo '</p>';

?>
<table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="<?php echo $roscms_intern_color0; ?>"> 
      <td width="9%"> <div align="center"><strong><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><?php echo $roscms_langres['ContTrans_Action']; ?></font></strong></div></td>
      <td width="8%" colspan="3" bgcolor="<?php echo $roscms_intern_color0; ?>"> <div align="center"><strong><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><?php echo $roscms_langres['ContTrans_Info']; ?></font></strong></div></td>
      <td width="13%" bgcolor="<?php echo $roscms_intern_color0; ?>"> <div align="center"><strong><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><?php echo $roscms_langres['ContTrans_PageID']; ?></font></strong></div></td>
      <td width="13%"> <div align="center"><strong><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><?php echo $roscms_langres['ContTrans_Language']; ?></font></strong></div></td>
      <td width="7%"> <div align="center"><strong><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><?php echo $roscms_langres['ContTrans_Rev']; ?></font></strong></div>
      </td>
      <td width="13%"> <div align="center"><strong><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"> 
          <?php echo $roscms_langres['ContTrans_Change']; ?></font></strong></div></td>
      <td width="13%">
        <div align="center"><strong><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><?php echo $roscms_langres['ContTrans_LatestGen']; ?></font></strong></div></td>
      <td width="23%"> 
        <div align="center"><strong><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><?php echo $roscms_langres['ContTrans_UserGen']; ?></font></strong></div></td>
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
								if ($result_page['page_generate_force'] == "1") {
									$farbe = "#00FF00";
								}
							 ?>" title="RosCMS action buttons:

* Generate static page
* View page in dynamc mode
* Delete static pagse"> 
        <div align="center">
          <strong><?php 
		  	$cmsros_intern_temp_lang_short=$result_page['page_language'];
			$sql_lang="SELECT * 
				FROM languages
				WHERE lang_level != '0' AND lang_id = '$cmsros_intern_temp_lang_short'
				ORDER BY 'lang_level' DESC";
			$sql_query_lang=mysql_query($sql_lang);
			$myrow_lang=mysql_fetch_row($sql_query_lang); // Languages
		?><a href="?page=<?php echo $rpm_page; ?>&amp;sec=generator&amp;sec2=genpage&amp;site=<?php echo $result_page['page_name']; ?>&amp;gen_lang=allpages&amp;skin=default" target="_blank"><img src="images/genpage.gif" alt="Generate this page (xhtml and html)" width="19" height="18" border="0" /></a> 
        &nbsp; <a href="?page=<?php echo $rpm_page; ?>&amp;sec=generator&amp;sec2=view&amp;site=<?php echo $result_page['page_name']; ?>&amp;gen_lang=<?php echo $rpm_lang; ?>&amp;forma=xhtml&amp;skin=default" target="_blank"><img src="images/view.gif" alt="View page dynamic" width="19" height="18" border="0" /></a>        </strong></div></td>
      <td width="3%" valign="middle" bgcolor="<?php echo $farbe; ?>"><div align="center"> 
          <strong><?php
		 if($result_page['page_active'] == "1") { ?>
          <img src="images/active.gif" alt="active" width="19" height="18" border="0" />
          <?php
		 }
		 else { ?>
          <img src="images/notactive.gif" alt="NOT active" width="19" height="18" border="0" />
          <?php } ?>
        </strong></div></td>
      <td width="3%" valign="middle" bgcolor="<?php echo $farbe; ?>"><div align="center"> 
          <strong><?php
		 if($result_page['page_visible'] == "1") { ?>
          <img src="images/visible.gif" alt="visible" width="19" height="18" border="0" /> 
          <?php
		 }
		 else { ?>
          <img src="images/notvisible.gif" alt="NOT visible" width="19" height="18" border="0" /> 
          <?php } ?>
        </strong></div></td>
      <td width="3%" valign="middle" bgcolor="<?php echo $farbe; ?>"><div align="center"> 
          <strong><?php if($roscms_intern_account_level<=10) { ?>
          <img src="images/lock.gif" alt="Locked" width="19" height="18" /> 
          <?php } ?>
        </strong></div></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="left"><strong><font face="Arial, Helvetica, sans-serif"><?php echo "<b>".$result_page['page_name']."</b>"; ?></font></strong></div></td>
      <td width="13%" valign="middle" bgcolor="<?php if ($result_page['page_language'] != "") { echo $farbe; } else { echo "#FF0000"; } ?>"><strong><font face="Arial, Helvetica, sans-serif"> 
        </font></strong><font face="Arial, Helvetica, sans-serif"><div align="center"> 
          <strong><?php 
			if ($myrow_lang[1] != "") {
				echo $myrow_lang[1];
			}
			else if ($result_page['page_language'] != "all") {
				echo $result_page['page_language'];
			}
		?>
        </strong></div>
        </font></td>
      <td width="7%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="right"><strong><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['page_version'];
		?>
          </font></strong></div>
      </td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['page_date']." ".$result_page['page_time'];;
		?>
          </font></div></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>">
        <div align="center"><font face="Arial, Helvetica, sans-serif">
          <?php 
			echo "<b>".date("Y-m-d",$result_page['page_generate_timestamp'])."</b><br>".date("H:i:s",$result_page['page_generate_timestamp']);
		?>
      </font></div></td>
      <td width="23%" valign="middle" bgcolor="<?php echo $farbe; ?>"> 
        <div align="center"><strong><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			$accountinfo_query = @mysql_query("SELECT user_name, user_id FROM users WHERE user_id = '".$result_page['page_generate_usrid']."'") or die('DB error (admin interface)!');
			$accountinfo_result = @mysql_fetch_array($accountinfo_query);

			echo "<b>".$accountinfo_result['user_name']."</b><br><a href='?page=user&amp;sec=profil&amp;sec2=".$result_page['page_usrname_id']."' target='_blank'>".$roscms_langres['ContTrans_Profile']."</a>";
			
		?> </font></strong></div></td>
    </tr>
    <?php	
	}	// end while
?>
  </table>
    <strong><?php
	include("inc/inc_description_table.php");
?>
</strong><p>&nbsp;</p>
</div>
