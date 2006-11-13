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
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Dynamic Content</span> 
  <?php 
  
	include("roscms_config.php") ;

  if(!isset($_POST['content_rad_opt']) || $_POST['content_rad_opt'] == "preview") { ?>
  <ul>
    <li><strong><a href="?page=admin&sec=dyncontent&sec2=view&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;filt2='.$rpm_filt2.'&amp;langid='.$rpm_lang_id ; ?>">Dynamic Content</a></strong> 
      <ul>
        <li>View dynamic content</li>
      </ul>
    </li>
  </ul>
<?php

	$query_content = mysql_query("SELECT * 
			FROM dyn_content
			WHERE dyn_id = '$rpm_db_id'
			ORDER BY 'dyn_content_lang' ASC") ;

	$farbe=$roscms_intern_color1;
	
	$result_content = mysql_fetch_array($query_content);
	$roscms_intern_editor_content = $result_content['dyn_content_text4'];

	if ($rpm_sec3 == "news_page") {
		echo "<b>News Interface</b>";
	}
	elseif ($rpm_sec3 == "newsletter") {
		echo "<b>Newsletter Interface</b>";
	}
	elseif ($rpm_sec3 == "interview") {
		echo "<b>Interview Interface</b>";
	}
	elseif ($rpm_sec3 == "blog") {
		echo "<b>Blog Interface</b>";
	}
	else {
		echo "<b>New Dynamic Content (plain) Interface</b>";
	}
	
?><form name="cms_content" method="post" action="<?php //echo $_SERVER['PHP_SELF'];
		echo '?page=admin&amp;sec=dyncontent&amp;sec2=save&amp;sec3='.$rpm_sec3.'&amp;db_id='.$rpm_db_id;
	 ?>">
    <table width="600" border="0" cellpadding="1" cellspacing="1">
      <tr bgcolor="#5984C3"> 
        <td bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Section</strong></font></div></td>
        <td bgcolor="#E2E2E2"><input name="txt_contentname" type="text" id="txt_contentname" value="<?php if (!$result_content['dyn_content_name']) { echo $rpm_sec3; } echo $result_content['dyn_content_name']; ?>" size="50" maxlength="100">        </td>
      </tr>
      <tr bgcolor="#5984C3"> 
        <td bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Content 
            ID</strong></font></div></td>
        <td bgcolor="#EEEEEE"> <input name="txt_contentid" type="text" id="txt_contentid" value="<?php 
		
		if ($rpm_db_id == "new") {
			$query_content_count = mysql_query("SELECT COUNT( * ) 
												FROM `dyn_content` 
												WHERE 1 
												AND `dyn_content_name` = '".mysql_real_escape_string($rpm_sec3)."'
												AND `dyn_content_nr` = '1'
												ORDER BY dyn_content_id ASC ;");
			$result_content_count = mysql_fetch_row($query_content_count);
			
			echo ($result_content_count[0]+1);
		}
		else {		
			echo $result_content['dyn_content_id'];
		}
		
		 ?>" size="20" maxlength="20">        </td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Language</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><div align="left"> 
		<?php
			if ($rpm_sec3 == "news_page" || $rpm_sec3 == "newsletter" || $rpm_sec3 == "interview") {
		?>
			<input name="txt_contentnr" type="hidden" value="1" />
		<?php
			}
		?>
            <select id="select" size="1" name="txt_langa" class="selectbox">
              <?php 
				//echo $result_page['page_language'];
				$cmsros_intern_temp_lang_short=$result_content['dyn_content_lang'];
				$sql_lang="SELECT * 
					FROM languages
					WHERE lang_level != '0' AND lang_id = '$cmsros_intern_temp_lang_short'
					ORDER BY 'lang_level' DESC Limit 1";
				$sql_query_lang=mysql_query($sql_lang);
				$myrow_lang=mysql_fetch_row($sql_query_lang); // Languages
			?>
              <optgroup label="languages"> 
              <option value="all"<?php if ($cmsros_intern_temp_lang_short == "all") { echo ' selected="selected"'; } ?>>All</option>
              <?php
			  
		// Languages
		$sql_langa="SELECT * 
					FROM languages
					WHERE lang_level != '0'
					ORDER BY 'lang_level' DESC";
		$sql_query_langa=mysql_query($sql_langa);
		while($myrow_langa=mysql_fetch_row($sql_query_langa)) {
              echo '<option value="'.$myrow_langa[0].'"';
			  if ($myrow_lang[0] == $myrow_langa[0]) {
					echo ' selected="selected"';
			  }
			  echo '>'.$myrow_langa[1].'</option>';
		}
              ?>
              </optgroup>
            </select>
          </div></td>
      </tr>	  
<?php
	if ($rpm_sec3 != "news_page" && $rpm_sec3 != "newsletter"  && $rpm_sec3 != "interview") {
?>
      <tr bgcolor="#5984C3"> 
        <td width="15%" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Content 
            Nr</strong></font></div></td>
        <td width="85%" bgcolor="#E2E2E2"> <input name="txt_contentnr" type="text" id="txt_contentnr" value="<?php 
		
		if ($result_content['dyn_content_nr'] == "") {
			echo "1";
		}
		else {
			echo $result_content['dyn_content_nr'];
		}
		
		 ?>" size="20" maxlength="20"></td>
      </tr>
<?php
	}
?>

      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php 
		if ($rpm_sec3 == "news_page") {
			echo "Title";
		}
		elseif ($rpm_sec3 == "newsletter") {
			echo "Title";
		}
		elseif ($rpm_sec3 == "interview") {
			echo "Title";
		}
		elseif ($rpm_sec3 == "blog") {
			echo "Title";
		}
		else {
			echo "Text1";
		}
		
		?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><input name="txt_content_text1" type="text" id="txt_content_text1" value="<?php echo $result_content['dyn_content_text1']; ?>" size="50" maxlength="100"></td>
      </tr>
<?php
	if ($rpm_sec3 != "news_page" && $rpm_sec3 != "newsletter" && $rpm_sec3 != "interview") {
?>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php 
		if ($rpm_sec3 == "news_page") {
			echo "-";
		}
		elseif ($rpm_sec3 == "newsletter") {
			echo "-";
		}
		elseif ($rpm_sec3 == "interview") {
			echo "-";
		}
		elseif ($rpm_sec3 == "blog") {
			echo "Category";
		}
		else {
			echo "Text2";
		}
		
		?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><input name="txt_content_text2" type="text" id="txt_content_text2" value="<?php echo $result_content['dyn_content_text2']; ?>" size="50" maxlength="100"></td>
      </tr>
<?php
	}
?>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php 
		if ($rpm_sec3 == "news_page") {
			echo "Description";
		}
		elseif ($rpm_sec3 == "newsletter") {
			echo "Description";
		}
		elseif ($rpm_sec3 == "interview") {
			echo "Description";
		}
		elseif ($rpm_sec3 == "blog") {
			echo "Description";
		}
		else {
			echo "Text3";
		}
		
		?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><input name="txt_content_text3" type="text" id="txt_content_text3" value="<?php echo $result_content['dyn_content_text3']; ?>" size="50" maxlength="100"></td>
      </tr>
      <?php 
	if ($result_content['dyn_content_editor'] != "richtext" && $result_content['dyn_content_editor'] != "bbcode") {
?>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php 
		if ($rpm_sec3 == "news_page") {
			echo "Content";
		}
		elseif ($rpm_sec3 == "newsletter") {
			echo "Content";
		}
		elseif ($rpm_sec3 == "interview") {
			echo "Content";
		}
		elseif ($rpm_sec3 == "blog") {
			echo "Content";
		}
		else {
			echo "Text4";
		}
		
		?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"> <textarea name="textarea_content" cols="60" rows="15" id="textarea_content"><?php 
				echo stripslashes($roscms_intern_editor_content);
				
			?></textarea> </td>
      </tr>
      <?php
	}
?>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Revision</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_content['dyn_content_version'];
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Visible</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			if ($result_content['dyn_content_visible'] == "0") {
				echo ' <input name="content_visible" type="radio" value="yes"> yes &nbsp; <input type="radio" name="content_visible" value="no" checked> no';
			}
			else {
				echo ' <input name="content_visible" type="radio" value="yes" checked> yes &nbsp; <input type="radio" name="content_visible" value="no"> no';
			}	
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Active</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			if ($result_content['dyn_content_active'] == "0") {
				echo ' <input name="content_active" type="radio" value="yes"> yes &nbsp; <input type="radio" name="content_active" value="no" checked> no';
			}
			else {
				echo ' <input name="content_active" type="radio" value="yes" checked> yes &nbsp; <input type="radio" name="content_active" value="no"> no';
			}	
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Editor</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"> 
          <select id="txt_extra" size="1" name="txt_extra" class="selectbox">
            <optgroup label="current"> 
            <?php   
				echo '<option value="'.$result_content["dyn_content_editor"].'"';
				echo ' selected="selected">';
				
				if ($result_content["dyn_content_editor"]=="") {
					echo "plain text";
				}
				else {
					echo $result_content["dyn_content_editor"];
				}
				echo '</option>';
				?>
            </optgroup>
            <optgroup label="extra option"> 
            <option value="">plain text</option>
            <option value="richtext">richtext</option>
            <option value="bbcode">bbcode</option>
            </optgroup>
          </select>        </td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>User</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif">
          <?php 
			$accountinfo_query = @mysql_query("SELECT user_name, user_id FROM users WHERE user_id = '".$result_content['dyn_content_usrname_id']."'") or die('DB error (admin interface)!');
			$accountinfo_result = @mysql_fetch_array($accountinfo_query);

			echo $accountinfo_result['user_name'];
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Date/Time</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif">
		<?php 
			echo $result_content['dyn_content_date']." &nbsp; ".$result_content['dyn_content_time'];
		?>
        </font></td>
      </tr>
    </table>
    <p>
      <?php
		if ($result_content['dyn_content_editor']=="richtext") {

			include("../editor/FC/fckeditor.php") ;
			// Automatically calculates the editor base path based on the _samples directory.
			// This is usefull only for these samples. A real application should use something like this:
				// $oFCKeditor->BasePath = '/FCKeditor/' ;	// '/FCKeditor/' is the default value.
			$sBasePath = $_SERVER['PHP_SELF'] ;
			$sBasePath = substr( $sBasePath, 0, strpos( $sBasePath, "_samples" ) ) ;
			
			$oFCKeditor = new FCKeditor('FCKeditor') ;
				//$oFCKeditor->BasePath	= $sBasePath ;
			//$oFCKeditor->ToolbarSet = "Basic";
			$oFCKeditor->BasePath = '../editor/FC/';
			
			$oFCKeditor->Height     = 500;
			$oFCKeditor->Value		= '<link href="'.$roscms_intern_path_server.$roscms_intern_path_gererator.'style_editor.css" type="text/css" rel="stylesheet"/>'.stripslashes($roscms_intern_editor_content);
			$oFCKeditor->Create() ;
		}
		if ($result_content['dyn_content_editor']=="bbcode") {
			include("../editor/bbcode/bbcodeeditor.php");
			include("../editor/bbcode/cbparser.php");
			if (isset($_POST['content_rad_opt'])) {
				echo "<p><fieldset><legend>Preview</legend>".bb2html(stripslashes(@$_POST['post']),'')."</fieldset></p>";
			}
			else {
				echo "<p><fieldset><legend>Preview</legend>".bb2html(stripslashes($roscms_intern_editor_content),'')."</fieldset></p>";
				//echo "<p><hr>".bb2html(stripslashes($result_content['content_text']),'')."<hr></p>";
			}
		}
		
		$roscms_intern_content_name=$result_content['dyn_content_name'];
	?>
    </p>
    <p> 
      <input name="content_rad_opt" type="radio" value="insert" <?php if($roscms_intern_usrgrp_admin != true OR $roscms_intern_content_name == "") { echo "checked"; } ?>>
      Save (new version) &nbsp; 
      <?php 
			if($roscms_intern_usrgrp_admin == true AND $roscms_intern_content_name != "") {
		?>
      <input name="content_rad_opt" type="radio" value="update" <?php if($result_content['dyn_content_editor'] != "bbcode") { echo "checked"; } ?>>
      Update 
      <?php } ?>
      <?php 
			if($result_content['dyn_content_editor'] == "bbcode") {
		?>
      <input name="content_rad_opt" type="radio" value="preview" checked>
      Preview 
      <?php } ?>
    </p>
    <p><?php 
			if($roscms_intern_account_level>20) {
		?>
      <input type="submit" name="Submit" value="Submit"> 
		<?php  } if($roscms_intern_account_level<=20) { ?>
			<img src="images/lock.gif" alt="Locked" width="19" height="18"> (you need 
				  a higher account level to save the content to the database)</p>
		
    <?php } ?>
    <p><strong>Info:</strong> for each [#inc_xyz] tag the RosCMS will include 
      the code that is linked with this tag from database.<br>
      <strong>Hints:</strong> use &amp;amp; instead of &quot;&amp;&quot; in links, 
      e.g. http://www.reactos.org/?page=support&amp;amp;lang=en ; &quot;&lt;placeholder&gt;&quot; 
      =&gt; &amp;lt;placeholder&amp;gt; </p>
    </form>
	
<?php
	}
	elseif(isset($_POST['content_rad_opt']) && $_POST['content_rad_opt'] != "preview") {
?>
  <ul>
    <li><strong><a href="?page=admin&sec=dyncontent&sec2=view&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id ; ?>">Content</a></strong> 
      <ul>
        <li>Save content</li>
      </ul>
    </li>
  </ul>
<?php 
		$content_contentid="";
		$content_contentnr="";
		$content_contentname="";
		$content_langa="";
		$content_vis="";
		$content_act="";
		$content_extra="";
		$content_savemode="";
		
		$content_text1="";
		$content_text2="";
		$content_text3="";


		if (array_key_exists("txt_contentid", $_POST)) $content_contentid=$_POST['txt_contentid'];
		if (array_key_exists("txt_contentnr", $_POST)) $content_contentnr=$_POST['txt_contentnr'];
		if (array_key_exists("txt_contentname", $_POST)) $content_contentname=$_POST['txt_contentname'];
		if (array_key_exists("txt_langa", $_POST)) $content_langa=$_POST['txt_langa'];
		if (array_key_exists("content_visible", $_POST)) $content_vis=$_POST['content_visible'];
		if (array_key_exists("content_active", $_POST)) $content_act=$_POST['content_active'];
		if (array_key_exists("txt_extra", $_POST)) $content_extra=$_POST['txt_extra'];
		if (array_key_exists("content_rad_opt", $_POST)) $content_savemode=$_POST['content_rad_opt'];

		if (array_key_exists("txt_content_text1", $_POST)) $content_text1=$_POST['txt_content_text1'];
		if (array_key_exists("txt_content_text2", $_POST)) $content_text2=$_POST['txt_content_text2'];
		if (array_key_exists("txt_content_text3", $_POST)) $content_text3=$_POST['txt_content_text3'];


		if ($content_langa == "") {
			$content_langa = "all";
		}

		if ($content_vis == "yes") {
			$content_vis="1";
		}
		else {
			$content_vis="0";
		}
		
		if ($content_act == "yes") {
			$content_act="1";
		}
		else {
			$content_act="0";
		}
		
		// Define some vars:
		$roscms_content_save_textarea="";
		$roscms_content_save_richtext="";
		$roscms_content_save_bbcode="";
		if (array_key_exists("textarea_content", $_POST)) $roscms_content_save_textarea=$_POST['textarea_content'];
		if (array_key_exists("FCKeditor", $_POST)) $roscms_content_save_richtext=$_POST['FCKeditor'];
		if (array_key_exists("post", $_POST)) $roscms_content_save_bbcode=$_POST['post'];
		
		$content_data="";
		if ($roscms_content_save_textarea != "") {
			$content_data=$roscms_content_save_textarea;
			echo "<br>Plain Text<br>";
		}
		else if ($roscms_content_save_richtext != "") {
			$roscms_content_save_richtext = str_replace('<link href="'.$roscms_intern_path_server.$roscms_intern_path_gererator.'style_editor.css" type="text/css" rel="stylesheet"/>','',$roscms_content_save_richtext); // remove style sheet
			$content_data=$roscms_content_save_richtext;			
			echo "<br>Rich Text<br>";
		}
		else if ($roscms_content_save_bbcode != "") {
			$content_data=$roscms_content_save_bbcode;			
			echo "<br>BB Code<br>";
		}
		else {
			$content_data="";
			echo "<br>???<br>";
		}

		//echo "<p><fieldset><legend>Preview</legend>".$content_data."</fieldset></p>";


		if ($content_savemode == "update") {
			$content_posta="UPDATE `dyn_content` SET `dyn_content_id` = '". mysql_escape_string($content_contentid) ."',
				`dyn_content_nr` = '". mysql_escape_string($content_contentnr) ."',
				`dyn_content_name` = '". mysql_escape_string($content_contentname) ."',
				`dyn_content_lang` = '". mysql_escape_string($content_langa) ."',
				`dyn_content_editor` = '". mysql_escape_string($content_extra) ."',
				`dyn_content_active` = '". mysql_escape_string($content_vis) ."',
				`dyn_content_visible` = '". mysql_escape_string($content_act) ."',
				`dyn_content_text1` = '". mysql_escape_string($content_text1) ."',
				`dyn_content_text2` = '". mysql_escape_string($content_text2) ."',
				`dyn_content_text3` = '". mysql_escape_string($content_text3) ."',
				`dyn_altered` = '1',
				`dyn_content_text4` = '". mysql_real_escape_string($content_data) ."'
				WHERE `dyn_id` = '$rpm_db_id' LIMIT 1 ;";
			$content_post_lista=mysql_query($content_posta);
			
			$query_content = mysql_query("SELECT dyn_content_text4, dyn_content_id, dyn_content_name
					FROM dyn_content
					WHERE dyn_id = '$rpm_db_id'") ;
			$result_content = mysql_fetch_array($query_content);
	
			echo "<p>The content '".$result_content['dyn_content_name']."' (id='".$rpm_db_id."') has been saved!</p>";
			echo "<p><a href=".$_SERVER['HTTP_REFERER'].">Back to the 'content edit' page</a></p>";
			echo "<p>&nbsp;</p><p><fieldset><legend>&nbsp;Preview&nbsp;</legend><br>".$result_content['dyn_content_text4']."</fieldset></p>";
		}
		else {
			// content Version:
			$query_content_save_version = mysql_query("SELECT * 
										FROM `dyn_content` 
										WHERE `dyn_content_name` = '$content_contentname' AND `dyn_content_nr` = '$content_contentnr' AND `dyn_content_id` = '$content_contentid'
										ORDER BY `dyn_content_version` DESC LIMIT 1;");
			$result_content_save_version = mysql_fetch_array($query_content_save_version);
			$content_version=$result_content_save_version['dyn_content_version'];
			$content_version++;
			
			$query_content_save_equal = mysql_query("SELECT * 
													FROM `dyn_content` 
													WHERE `dyn_content_name` = '$content_contentname' AND `dyn_content_nr` = '$content_contentnr' AND `dyn_content_id` = '$content_contentid' AND `dyn_content_lang` = '$content_langa' ;") ;
			while($result_content_savea = mysql_fetch_array($query_content_save_equal)) {
				// Set all versions not active:
				//$content_na=$result_content_savea['content_id'];
				$content_postc="UPDATE `dyn_content` SET `dyn_content_active` = '0' WHERE `dyn_content_name` = '$content_contentname' AND `dyn_content_nr` = '$content_contentnr' AND `dyn_content_id` = '$content_contentid' LIMIT 1 ;";
				$content_post_listc=mysql_query($content_postc);
			}
		
			/*echo "INSERT INTO `dyn_content` ( `dyn_id`, `dyn_content_id` , `dyn_content_nr` , `dyn_content_name` , `dyn_content_lang` , `dyn_content_editor` , `dyn_content_active` , `dyn_content_visible` , `dyn_content_text1` , `dyn_content_text2` , `dyn_content_text3` , `dyn_content_text4`, `dyn_content_date`, `dyn_content_time`, `dyn_content_usrname_id`)
				VALUES ('', '". mysql_escape_string($content_contentid) ."', '". mysql_escape_string($content_contentnr) ."', '". mysql_escape_string($content_contentname) ."', '". mysql_escape_string($content_langa) ."', '". mysql_escape_string($content_extra) ."', '". mysql_escape_string($content_vis) ."', '". mysql_escape_string($content_act) ."', '". mysql_escape_string($content_text1) ."', '". mysql_escape_string($content_text2) ."', '". mysql_escape_string($content_text3) ."', '". mysql_escape_string($content_data) ."', CURDATE( ), CURTIME( ), '". mysql_escape_string($roscms_intern_account_id) ."');";
			*/
			$content_postb="INSERT INTO `dyn_content` ( `dyn_id`, `dyn_content_id` , `dyn_content_nr` , `dyn_content_name` , `dyn_content_lang` , `dyn_content_editor` , `dyn_content_active` , `dyn_content_visible` , `dyn_content_text1` , `dyn_content_text2` , `dyn_content_text3` , `dyn_content_text4`, `dyn_content_date`, `dyn_content_time`, `dyn_content_usrname_id`)
				VALUES ('', '". mysql_escape_string($content_contentid) ."', '". mysql_escape_string($content_contentnr) ."', '". mysql_escape_string($content_contentname) ."', '". mysql_escape_string($content_langa) ."', '". mysql_escape_string($content_extra) ."', '". mysql_escape_string($content_vis) ."', '". mysql_escape_string($content_act) ."', '". mysql_escape_string($content_text1) ."', '". mysql_escape_string($content_text2) ."', '". mysql_escape_string($content_text3) ."', '". mysql_escape_string($content_data) ."', CURDATE( ), CURTIME( ), '". mysql_escape_string($roscms_intern_account_id) ."');";
			$content_post_listb=mysql_query($content_postb);
			echo "<p>&nbsp;</p><p><fieldset><legend>&nbsp;Preview&nbsp;</legend><br>".$content_data."</fieldset></p>";

		}	
}
?>
</div>

