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
		$seclog_reason="Hacking attempt: admin_content.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}
	
	include("roscms_config.php");
	
	include("inc/tools.php");

	
	if ($rpm_page != "admin" && $rpm_page != "dev" && $rpm_page != "team" && $rpm_page != "trans") {
		die("");
	}
	if ($roscms_intern_usrgrp_admin == true || $roscms_intern_usrgrp_dev == true || $roscms_intern_usrgrp_team == true || $roscms_intern_usrgrp_trans == true) {

?>
 
<div class="contentSmall"> <span class="contentSmallTitle"><?php
	if ($rpm_page == "admin") {
		echo "Admin Interface - Content";
	}
	elseif ($rpm_page == "dev") {
		echo $roscms_langres['ContTrans_Interface_Content'];
	}
	elseif ($rpm_page == "trans") {
		echo "Translator Interface - Content";
	}
	elseif ($rpm_page == "team") {
		echo "Team Interface - Content";
	}
	else {
		echo $rpm_page." Interface - Content";
	}
  ?></span> 
  <ul>
    <li><strong><?php echo $roscms_langres['ContTrans_Contents']; ?></strong></li>
  </ul>

<?php 

	$roscms_temp_diff="";
	if (array_key_exists("diff", $_GET)) $roscms_temp_diff=htmlspecialchars($_GET["diff"]);

	if ($roscms_temp_diff != "") {
		$query_diff_content=mysql_query("SELECT *
											FROM `content`
											WHERE `content_id` = '".mysql_escape_string($roscms_temp_diff)."' ;");
		$result_diff_content = mysql_fetch_array($query_diff_content);

		echo '<p><b><a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">Back to the content list</a></b></p>';
		echo "<h3>".$result_diff_content['content_name']."</h3>";
		echo "<p>ID: <b>".$result_diff_content['content_id']."</b><br />Lang: <b>".$result_diff_content['content_lang']."</b></p>";
		
		
		
?>

<!-- Diff  -->
<script src="js/jsdiff2.js" language="javascript"></script>
<script src="js/diff.js" language="javascript"></script>
<table width="100%"  border="0">
  <tr bgcolor="<?php echo $roscms_intern_color0; ?>">
    <td colspan="2"><div align="center"><?php 
	
	$tlang="";
	
	echo "<select name=\"vendor\" id=\"vendor\" onChange=\"ajat_LoadText('?page=export&amp;export=diff_content&amp;diff2='+this.value,'d1')\">";
	echo '<option value="0" selected >Select a revision</option>';
	
	$query_diff_sbox1 = mysql_query("SELECT * 
								FROM `content` 
								WHERE `content_name` = '".mysql_escape_string($result_diff_content['content_name'])."'
								ORDER BY `content_lang` ASC, `content_id` DESC ;") ;

	while($result_diff_sbox1 = mysql_fetch_array($query_diff_sbox1)) { // Diff ComboBox
		if ($tlang != $result_diff_sbox1['content_lang']) {
			echo '<optgroup label="'.$result_diff_sbox1['content_lang'].'">';
		}
		echo "\n <option value=\"". $result_diff_sbox1['content_id'] ."\">".$result_diff_sbox1['content_id']." &nbsp;(act: ".$result_diff_sbox1['content_active']." | vis: ".$result_diff_sbox1['content_visible'].") - ".$result_diff_sbox1['content_date']." ".$result_diff_sbox1['content_time']."</option>"; 
		$tlang = $result_diff_sbox1['content_lang'];
		if ($tlang != $result_diff_sbox1['content_lang']) {
			echo '</optgroup>';
		}
	}	// end while
	
	echo '</select>';
	
	?>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<button name="bdiff" id="bdiff" type="button" value="Diff" onclick="DoDiff()" >&nbsp;Diff&nbsp;</button>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?php 
	
	$tlang="";
	
	echo "<select name=\"vendor\" id=\"vendor\" onChange=\"ajat_LoadText('?page=export&amp;export=diff_content&amp;diff2='+this.value,'d2')\">";
	echo '<option value="0" selected >Select a revision</option>';
	
	$query_diff_sbox2 = mysql_query("SELECT * 
								FROM `content` 
								WHERE `content_name` = '".mysql_escape_string($result_diff_content['content_name'])."'
								ORDER BY `content_lang` ASC, `content_id` DESC ;") ;

	while($result_diff_sbox2 = mysql_fetch_array($query_diff_sbox2)) { // Diff ComboBox
		if ($tlang != $result_diff_sbox2['content_lang']) {
			echo '<optgroup label="'.$result_diff_sbox2['content_lang'].'">';
		}
		echo "\n <option value=\"". $result_diff_sbox2['content_id'] ."\">".$result_diff_sbox2['content_id']." &nbsp;(act: ".$result_diff_sbox2['content_active']." | vis: ".$result_diff_sbox2['content_visible'].") - ".$result_diff_sbox2['content_date']." ".$result_diff_sbox2['content_time']."</option>"; 
		$tlang = $result_diff_sbox2['content_lang'];
		if ($tlang != $result_diff_sbox2['content_lang']) {
			echo '</optgroup>';
		}
	}	// end while
	
	echo '</select>';
	
	?>
</div></td>
    </tr>
  <tr>
    <td colspan="2">&nbsp;</td>
  </tr>

  <tr bgcolor="#CCCCCC">
    <td colspan="2"><font size="1" face="Verdana, Arial, Helvetica, sans-serif"><div id="diff2"></div>&nbsp;</font></td>
    </tr>
  <tr>
    <td colspan="2">&nbsp;</td>
  </tr>
  <tr bgcolor="#CCCCCC">
    <td colspan="2"><font size="1" face="Verdana, Arial, Helvetica, sans-serif"><div id="diff3"></div>&nbsp;</font></td>
    </tr>
</table>
<div id="d1" style="display:none;"></div>
<div id="d2" style="display:none;"></div>
<script language="JavaScript1.2">
<!--

	var oOld = document.getElementById('d1');
	var oNew = document.getElementById('d2');
	var out = "";

	function DoDiff() {


		// Diff 2: (jsdiff2.js)
		document.getElementById('diff2').innerHTML = diffString_ver2( 
		   document.getElementById('d1').innerHTML, 
		   document.getElementById('d2').innerHTML 
		); 
		
		// Diff 3: (diff.js)
		document.getElementById('diff3').innerHTML = StringDiff( 
		   document.getElementById('d1').innerHTML, 
		   document.getElementById('d2').innerHTML 
		); 
	}
-->
</script>
<script type="text/javascript">
<!--

	function setCursor(mode) {
	  var docBody = document.getElementsByTagName("body")[0];
	  docBody.style.cursor = mode;
	}	


	var xmlhttp=false;
	/* IE 5+ only: */
	/*@cc_on @*/
		/*@if (@_jscript_version >= 5)
			try {
				xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
			} catch (e) {
			try {
				xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (E) {
				xmlhttp = false;
			}
		}
	@end @*/
	
	if (!xmlhttp && typeof XMLHttpRequest != 'undefined') {
		xmlhttp = new XMLHttpRequest();
	}
	
	function ajat_LoadText(serverPage, objID) {
		var obj = document.getElementById(objID);
		xmlhttp.open("GET", serverPage);
		xmlhttp.onreadystatechange = function() {
			setCursor('wait');
			if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
				setCursor('auto');
				obj.innerHTML = xmlhttp.responseText;
			}
		}
		xmlhttp.send(null);
	}
	
//-->
</script>
<?php
	}
	else {

?>
  
  <p><?php echo $roscms_langres['ContTrans_Action'].": "; if (($roscms_intern_usrgrp_admin == true && $rpm_page == "admin") || ($roscms_intern_usrgrp_dev == true && $rpm_page == "dev")) { ?><a href="?page=<?php echo $rpm_page; ?>&amp;sec=content&amp;sec2=edit&amp;opt=insert&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id ; ?>&amp;db_id=new"><?php echo $roscms_langres['ContTrans_NewContent']; ?></a> | <?php } ?><a href="?page=<?php echo $rpm_page; ?>&sec=content&sec2=view"><?php echo $roscms_langres['ContTrans_resetfilters']; ?></a></p>
    
  <?php
	if($roscms_intern_usrgrp_admin == true) {
		$rpm_content_active="";
		$rpm_content_active_set="";
		if(array_key_exists("content_active", $_GET)) $rpm_content_active=$_GET["content_active"];
		if(array_key_exists("content_active_set", $_GET)) $rpm_content_active_set=$_GET["content_active_set"];
	
		if ($rpm_content_active != "" AND $rpm_content_active_set != "") {
			$content_postc="UPDATE `content` SET `content_active` = '$rpm_content_active' WHERE `content_id` = '". $rpm_content_active_set ."' LIMIT 1 ;";
			$content_post_listc=mysql_query($content_postc);
		}
	}


	if ($rpm_lang_id == "") {
		if (($roscms_intern_usrgrp_trans == true || $roscms_intern_usrgrp_team == true) && ($rpm_page == "trans" || $rpm_page == "team")) {
			$rpm_lang_id="nolang";
		}
		else {
			$rpm_lang_id="nolang";
			//$rpm_lang_id="all";
		}
	}
	if ($roscms_intern_usrgrp_admin != true || $roscms_intern_usrgrp_sadmin != true) {
		$sql_lang_usr="SELECT * 
					FROM users
					WHERE user_id = '".mysql_escape_string($roscms_intern_account_id)."'
					LIMIT 1 ;";
		$sql_query_lang_usr=mysql_query($sql_lang_usr);
		$myrow_lang_usr=mysql_fetch_array($sql_query_lang_usr);
		$sql_lang_usr_lang=mysql_query("SELECT * 
					FROM languages
					WHERE lang_id  = '".mysql_escape_string($myrow_lang_usr['user_language'])."'
					LIMIT 1 ;");
		$myrow_lang_usr_lang=mysql_fetch_array($sql_lang_usr_lang);
		
		$ros_cms_intern_content_lang = "AND content_lang = 'all' AND content_type = 'default'";
		echo '<p>'.$roscms_langres['ContTrans_Language'].': <b>'.$myrow_lang_usr_lang['lang_name'].'</b>';
	}
	else {	
	
		echo '<p>'.$roscms_langres['ContTrans_Language'].': ';
		if ($roscms_intern_usrgrp_dev == true || $roscms_intern_usrgrp_admin == true) {
			if ($rpm_lang_id == "all") {	
				echo '<b>'.$roscms_langres['ContTrans_All'].'</b>';
				$ros_cms_intern_content_lang = "";
			}
			else {
				echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=all">'.$roscms_langres['ContTrans_All'].'</a>';
			}
			echo ' | ';
			if ($rpm_lang_id == "layout") {	
				echo '<b>Layout</b>';
				$ros_cms_intern_content_lang = "AND content_type = 'layout'";
			}
			else {
				echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=layout">Layout</a>';
			}
			echo ' | ';
			if ($rpm_lang_id == "error") {	
				echo '<b>Error</b>';
				$ros_cms_intern_content_lang = "AND content_type != 'default' AND content_type != 'layout'";
			}
			else {
				echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=error">Error</a>';
			}
			echo ' | ';
		}
	
			if ($rpm_lang_id == "nolang") {	
				echo '<b>'.$roscms_langres['ContTrans_International'].'</b>';
				if ($roscms_intern_usrgrp_admin == true) {
					$ros_cms_intern_content_lang = "AND content_lang = 'all'";
				}
				if ($roscms_intern_usrgrp_dev == true && $rpm_page == "dev") {
					$ros_cms_intern_content_lang = "AND content_lang = 'all'";
				}
				if (($roscms_intern_usrgrp_team == true && $rpm_page == "team") || ($roscms_intern_usrgrp_trans == true && $rpm_page == "trans")) {
					$ros_cms_intern_content_lang = "AND content_lang = 'all' AND content_type = 'default'";
				}
			}
			else {
				echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid=nolang">'.$roscms_langres['ContTrans_International'].'</a>';
			}
	
		$sql_lang_usr="SELECT * 
					FROM users
					WHERE user_id = '".mysql_escape_string($roscms_intern_account_id)."'
					LIMIT 1 ;";
		$sql_query_lang_usr=mysql_query($sql_lang_usr);
		$myrow_lang_usr=mysql_fetch_array($sql_query_lang_usr);


		// Languages
		$sql_lang="SELECT * 
					FROM languages
					WHERE lang_level != '0'
					ORDER BY 'lang_level' DESC";
		$sql_query_lang=mysql_query($sql_lang);
		while($myrow_lang=mysql_fetch_row($sql_query_lang)) {
			if ($roscms_intern_usrgrp_dev != true || $roscms_intern_usrgrp_admin != true) {
				if ($myrow_lang[0] == $roscms_standard_language) {
					continue;
				}
			}
			$roscms_sel_lang = $myrow_lang[0];
				echo ' | ';
				if ($rpm_lang_id == $roscms_sel_lang) {	
					echo '<b>'.$myrow_lang[1].'</b>';
					$ros_cms_intern_content_lang = "AND content_lang = '".$roscms_sel_lang."'";
				}
				else {
					echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$roscms_sel_lang.'">'.$myrow_lang[1].'</a>';
				}
		}
	}


	if ($rpm_filt == "") {
		if ($roscms_intern_usrgrp_admin == true) {
			$rpm_filt="active";
		}
		if ($roscms_intern_usrgrp_dev == true && $rpm_page == "dev") {
			$rpm_filt="anvc";
		}
		if (($roscms_intern_usrgrp_team == true && $rpm_page == "team") || ($roscms_intern_usrgrp_trans == true && $rpm_page == "trans")) {
			$rpm_filt="anvc";
		}
	}
	echo '<p>'.$roscms_langres['ContTrans_Filter'].': ';
	if (($roscms_intern_usrgrp_admin == true && $rpm_page == "admin") || ($roscms_intern_usrgrp_dev == true && $rpm_page == "dev")) {
		if ($rpm_filt == "active") {	
			echo '<b>'.$roscms_langres['ContTrans_activecontent'].'</b>';
			$ros_cms_intern_content_filt = "WHERE content_active = '1'";
		}
		else {
			echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=active&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_activecontent'].'</a>';
		}
		echo ' | ';
		if ($rpm_filt == "all") {	
			echo '<b>'.$roscms_langres['ContTrans_allcontent'].'</b>';
			$ros_cms_intern_content_filt = "WHERE `content_name` != ''";
		}
		else {
			echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=all&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_allcontent'].'</a>';
		}
		echo ' | ';
	}
	if ($rpm_filt == "anvc") {	
		echo '<b>'.$roscms_langres['ContTrans_activeandvisiblecontent'].'</b>';
		$ros_cms_intern_content_filt = "WHERE content_active = '1' AND content_visible = '1'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=anvc&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_activeandvisiblecontent'].'</a>';
	}
	echo ' | ';
	if ($rpm_filt == "user") {	
		echo '<b>'.$roscms_langres['ContTrans_currentuser'].'</b>';
		$ros_cms_intern_content_filt = "WHERE content_usrname_id = '".$roscms_intern_account_id."'";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt=user&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_currentuser'].'</a>';
	}
	if ($rpm_filt == "history") {	
		echo ' | <b>'.$roscms_langres['ContTrans_history'].'</b>';
		$ros_cms_intern_content_filt = "WHERE content_name = '".$rpm_opt."'";
	}
	echo '</p>';

	if ($rpm_sort == "") {
		$rpm_sort="id";
	}
	echo '<p>'.$roscms_langres['ContTrans_Sortedby'].': ';
	if ($rpm_sort == "id") {	
		echo '<b>'.$roscms_langres['ContTrans_contentid'].'</b>';
		$ros_cms_intern_content_sortby="content_name";
		$ros_cms_intern_content_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort=id&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_contentid'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "date") {	
		echo '<b>'.$roscms_langres['ContTrans_date'].'</b>';
		$ros_cms_intern_content_sortby="content_date";
		$ros_cms_intern_content_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort=date&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_date'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "user") {	
		echo '<b>'.$roscms_langres['ContTrans_user'].'</b>';
		$ros_cms_intern_content_sortby="content_usrname_id";
		$ros_cms_intern_content_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort=user&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_user'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "active") {	
		echo '<b>'.$roscms_langres['ContTrans_active'].'</b>';
		$ros_cms_intern_content_sortby="content_active";
		$ros_cms_intern_content_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort=active&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_active'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "visible") {	
		echo '<b>'.$roscms_langres['ContTrans_visible'].'</b>';
		$ros_cms_intern_content_sortby="content_visible";
		$ros_cms_intern_content_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort=visible&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_visible'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "version") {	
		echo '<b>'.$roscms_langres['ContTrans_version'].'</b>';
		$ros_cms_intern_content_sortby="content_version";
		$ros_cms_intern_content_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort=version&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_version'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "language") {	
		echo '<b>'.$roscms_langres['ContTrans_language'].'</b>';
		$ros_cms_intern_content_sortby="content_lang";
		$ros_cms_intern_content_sort="ASC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort=language&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_language'].'</a>';
	}
	echo ' | ';
	if ($rpm_sort == "editor") {	
		echo '<b>'.$roscms_langres['ContTrans_editor'].'</b>';
		$ros_cms_intern_content_sortby="content_editor";
		$ros_cms_intern_content_sort="DESC";
	}
	else {
		echo '<a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort=editor&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'">'.$roscms_langres['ContTrans_editor'].'</a>';
	}
	echo '</p>';

?>
<p><i><?php echo $roscms_langres['ContTrans_generalinfo']; ?></i></p>
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="<?php echo $roscms_intern_color0; ?>"> 
      <td width="9%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Action']; ?></strong></font></div></td>
      <td width="8%" colspan="3" bgcolor="<?php echo $roscms_intern_color0; ?>"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Info']; ?></strong></font></div></td>
      <td width="13%" bgcolor="<?php echo $roscms_intern_color0; ?>"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_ContentID']; ?></strong></font></div></td>
      <td width="13%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Language']; ?></strong></font></div></td>
      <td width="26%" bgcolor="<?php echo $roscms_intern_color0; ?>"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Content']; ?></strong></font></div></td>
      <td width="7%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Rev']; ?></strong></font></div>
        <div align="center"></div></td>
      <td width="13%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong> 
          <?php echo $roscms_langres['ContTrans_Date']; ?></strong></font></div></td>
      <td width="10%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_User']; ?></strong></font></div></td>
    </tr>
    <?php

	if($roscms_intern_usrgrp_sadmin == true) {
		$query_content = mysql_query("SELECT * 
				FROM content
				$ros_cms_intern_content_filt $ros_cms_intern_content_lang
				ORDER BY '$ros_cms_intern_content_sortby' $ros_cms_intern_content_sort") ;
		/*$query_content = mysql_query("SELECT * 
				FROM content
				$ros_cms_intern_content_filt $ros_cms_intern_content_lang
				ORDER BY '$ros_cms_intern_content_sortby' $ros_cms_intern_content_sort") ;*/
	}
	elseif ($roscms_intern_usrgrp_admin == true) {
		$query_content = mysql_query("SELECT * 
				FROM content
				$ros_cms_intern_content_filt AND content_visible != 0 $ros_cms_intern_content_lang
				ORDER BY '$ros_cms_intern_content_sortby' $ros_cms_intern_content_sort") ;
	}
	elseif ($roscms_intern_usrgrp_dev == true) {
		$query_content = mysql_query("SELECT * 
				FROM content
				$ros_cms_intern_content_filt AND content_visible != 0 $ros_cms_intern_content_lang
				ORDER BY '$ros_cms_intern_content_sortby' $ros_cms_intern_content_sort") ;
	}
	elseif ($roscms_intern_usrgrp_team == true || $roscms_intern_usrgrp_trans == true) {
		$query_content = mysql_query("SELECT * 
				FROM content
				$ros_cms_intern_content_filt AND content_visible != 0 $ros_cms_intern_content_lang
				ORDER BY '$ros_cms_intern_content_sortby' $ros_cms_intern_content_sort") ;
	}
	else {
		die("");
	}

	$farbe1=$roscms_intern_color1;
	$farbe2=$roscms_intern_color2;
	$zaehler="0";
	//$farbe="#CCCCC";
	
	while($result_content = mysql_fetch_array($query_content)) { // content
		if ($rpm_page == "trans" || $rpm_page == "team") {	
			$accountinfo_query2 = @mysql_query("SELECT user_language FROM users WHERE user_id = '".$roscms_intern_account_id."'") or die('DB error (admin interface)!');
			$accountinfo_result2 = @mysql_fetch_array($accountinfo_query2);

			$RosCMS_query_translate_content3 = mysql_query("SELECT *
				FROM content 
				WHERE content_name = '". mysql_real_escape_string($result_content['content_name'])  ."' 
				AND content_lang = '". mysql_real_escape_string($accountinfo_result2['user_language'])  ."' 
				AND content_active = '1' 
				AND content_visible = '1' 
				LIMIT 1 ;");
			$RosCMS_result_translate_content3 = mysql_fetch_array($RosCMS_query_translate_content3);
		}
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
          <a name="<?php echo $result_content['content_id']; ?>"></a>
	<?php
		  if (($roscms_intern_usrgrp_trans == true || $roscms_intern_usrgrp_team == true) && ($rpm_page == "trans" || $rpm_page == "team") && $rpm_lang_id == "nolang") { 
          	if ($RosCMS_result_translate_content3['content_id'] == "") { ?>
			  <a href="?page=<?php echo $rpm_page; ?>&amp;sec=content&amp;sec2=edit&amp;opt=translate&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id.'&amp;db_id='.$result_content['content_id']; ?>"><img src="images/tool.gif" alt="<?php echo $roscms_langres['ContTrans_Translate']; ?>" width="19" height="18" border="0"></a> 
          
	<?php	}
			else { ?>
			  <a href="?page=<?php echo $rpm_page; ?>&amp;sec=content&amp;sec2=edit&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$accountinfo_result2['user_language'].'&amp;db_id='.$RosCMS_result_translate_content3['content_id']; ?>"><img src="images/view.gif" alt="<?php echo $roscms_langres['ContTrans_Edit']; ?>" width="19" height="18" border="0"></a> 
	<?php	}
		  } else { ?>
          <a href="?page=<?php echo $rpm_page; ?>&amp;sec=content&amp;sec2=edit&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id.'&amp;db_id='.$result_content['content_id']; ?>"><img src="images/view.gif" alt="<?php echo $roscms_langres['ContTrans_Edit']; ?>" width="19" height="18" border="0"></a> 
	<?php
		  }
		  if($roscms_intern_usrgrp_sadmin == true) { ?>
          <script type="text/javascript">
			<!--
				function DeleteContent() {
					var chk = window.confirm("Do you really want to delete this content?");
					if (chk == true) {
						//parent.location.href = "?page=admin&amp;sec=content&amp;sec2=delete&amp;db_id=<?php echo $result_content['content_id']; ?>";
						alert("Sorry! This feature has been disabled. Please ask the administrator if you really want to delete a content.");
					}
				}
			-->
			</script>
          <a href="javascript:DeleteContent()"><img src="images/delete.gif" alt="<?php echo $roscms_langres['ContTrans_Delete']; ?>" width="19" height="18" border="0"></a> 
          <?php } 
		  if($roscms_intern_usrgrp_admin == true || $roscms_intern_usrgrp_team == true) {?>
			  <a href="<?php echo "?page=".$rpm_page."&amp;sec=content&amp;sec2=view&amp;sort=version&amp;filt=history&amp;opt=".$result_content['content_name']."&amp;langid=".$result_content['content_lang']; ?>"><img src="images/history.gif" alt="<?php echo $roscms_langres['ContTrans_filter_history']; ?>" width="19" height="18" border="0"></a> 
          <?php } ?>
        </div></td>
      <td width="3%" valign="middle" bgcolor="<?php echo $farbe; ?>"><div align="center"> 
          <?php
		 if($result_content['content_active'] == "1") { ?>
          <a href="<?php if($roscms_intern_usrgrp_admin == true) { echo '?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;content_active=0&amp;content_active_set='.$result_content['content_id'] ; } else { echo '#'; } ?>"><img src="images/active.gif" alt="active" width="19" height="18" border="0"></a> 
          <?php
		 }
		 else { ?>
          <a href="<?php if($roscms_intern_usrgrp_admin == true) { echo '?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;opt='.$rpm_opt.'&amp;langid='.$rpm_lang_id.'&amp;content_active=1&amp;content_active_set='.$result_content['content_id'] ; } else { echo '#'; } ?>"><img src="images/notactive.gif" alt="NOT active" width="19" height="18" border="0"></a> 
          <?php } ?>
        </div></td>
      <td width="3%" valign="middle" bgcolor="<?php echo $farbe; ?>"><div align="center"> 
          <?php
		 if($result_content['content_visible'] == "1") { ?>
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
          <?php } else if ($result_content['content_editor'] == "richtext") { ?>
			  <img src="images/richtexteditor.gif" alt="Rich Text Editor" width="19" height="18"> 
		  <?php } else if ($result_content['content_editor'] == "bbcode") { ?>
			  <img src="images/bbcode.gif" alt="bbcode Editor" width="19" height="18"> 
		  <?php } ?>
        </div></td>
      <td width="13%" valign="middle" bgcolor="<?php
	   
	   		$temp_datecompare = 0;
	   		if ($roscms_intern_usrgrp_admin != true || $roscms_intern_usrgrp_sadmin != true) {
				$RosCMS_query_translate_lang2 = mysql_query("SELECT *
					FROM content 
					WHERE content_name = '". mysql_real_escape_string($result_content['content_name'])  ."' 
					AND content_lang = '".mysql_real_escape_string($myrow_lang_usr['user_language'])."' 
					AND content_active = '1' 
					AND content_visible = '1' 
					LIMIT 1 ;"); 
				$RosCMS_result_translate_lang2 = mysql_fetch_array($RosCMS_query_translate_lang2);
				$temp_date = $RosCMS_result_translate_lang2['content_date'];
				$temp_time = $RosCMS_result_translate_lang2['content_time'];
				$temp_usrid = $RosCMS_result_translate_lang2['content_usrname_id'];
				$temp_lang = $RosCMS_result_translate_lang2['content_lang'];
			}
			else {
				$temp_date = $result_content['content_date'];
				$temp_time = $result_content['content_time'];
				$temp_usrid = $result_content['content_usrname_id'];
				$temp_lang = $result_content['content_lang'];
			}
	   
   
			$RosCMS_query_translate_org = mysql_query("SELECT *
				FROM content 
				WHERE content_name = '". mysql_real_escape_string($result_content['content_name'])  ."' 
				AND content_lang = 'all' 
				AND content_active = '1' 
				AND content_visible = '1' 
				LIMIT 1 ;"); /* $myrow_lang_usr['user_language'] */
			$RosCMS_result_translate_org = mysql_fetch_array($RosCMS_query_translate_org);
			if ($temp_lang) {
				if ($temp_lang == "all") {
					echo $farbe;
				}
				else {
					$temp_datecompare = compareDate($RosCMS_result_translate_org['content_date'], $temp_date);
					if ($temp_datecompare <= 0) {
						echo $roscms_intern_color_new;
					}
					else { 
						echo $roscms_intern_color_old;
					}
				}
			}
			else {
				echo $farbe;
			}
				
	   
	   
	    ?>" title="<?php
		
		echo $RosCMS_result_translate_org['content_date']." (en) vs. \n".$temp_date. " (" .$temp_lang. ")";
		
		?>"> <div align="left"><font face="Arial, Helvetica, sans-serif"><?php echo '<font size="1">[#cont_</font><b>'.$result_content['content_name'].'</b><font size="1">]</font>';
		
		if ($RosCMS_result_translate_org['content_date'] != "" && $temp_date != "" && $temp_datecompare > 0) {
			echo "<br />".$temp_datecompare." day(s) outdated!";
		}
				
		 ?></font></div></td>
      <td width="13%" valign="middle" bgcolor="<?php if ($result_content['content_lang'] != "") { echo $farbe; } else { echo "#FF0000"; } ?>"> <div align="center"> 
          <font face="Arial, Helvetica, sans-serif">
          <?php 
		  
		  	$cmsros_intern_temp_lang_short=$result_content['content_lang'];
			$sql_lang="SELECT * 
				FROM languages
				WHERE lang_level != '0' AND lang_id = '$cmsros_intern_temp_lang_short'
				ORDER BY 'lang_level' DESC";
			$sql_query_lang=mysql_query($sql_lang);
			$myrow_lang=mysql_fetch_row($sql_query_lang); // Languages
			if ($myrow_lang[1] != "") {
				echo $myrow_lang[1];
			}
			else if ($result_content['content_lang'] != "all") {
				echo $result_content['content_lang'];
			}
			echo '<br /><font size="1">'.$result_content['content_type'].'</font>';
		?>
          </font></div></td>
      <td width="26%" valign="middle" bgcolor="<?php echo $farbe; ?>" title="<?php 
			if ($result_content['content_visible'] != 1) { echo "NOT visible!\n\n"; }
			echo substr(htmlentities($result_content['content_text'], ENT_QUOTES), 0, 200)."...";
		?>"> <pre><font face="Arial, Helvetica, sans-serif"><?php 
			echo substr(htmlentities($result_content['content_text'], ENT_QUOTES), 0, 40)."..." ;
		?></font></pre> </td>
      <td width="7%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo '<b>'.$result_content['content_version'].'</b><br /><a href="?page='.$rpm_page.'&amp;sec=content&amp;sec2=view&amp;diff='.$result_content['content_id'].'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id.'"><i>Diff-Tool</i></a>';
		?>
          </font></div>
      <div align="center"><font face="Arial, Helvetica, sans-serif"> </font></div></td>
      <td width="13%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
				echo $temp_date." ".$temp_time;
		?>
          </font></div></td>
      <td width="10%" valign="middle" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			$accountinfo_query = @mysql_query("SELECT user_name, user_id FROM users WHERE user_id = '".$temp_usrid."'") or die('DB error (admin interface)!');
			$accountinfo_result = @mysql_fetch_array($accountinfo_query);
			
			$roscms_intern_accountuser = $accountinfo_result['user_name'];
			if ($roscms_intern_accountuser && $roscms_intern_accountuser != "") {
				echo "<b>".$roscms_intern_accountuser."</b><br><a href='?page=user&amp;sec=profil&amp;sec2=".$result_content['content_usrname_id']."' target='_blank'>".$roscms_langres['ContTrans_Profile']."</a>";
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
	}
?>
</div>
<?php
	}
?>
