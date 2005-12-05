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
		echo "Admin Interface - Content";
	}
	elseif ($rpm_page == "dev") {
		echo "Dev Interface - Content";
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
  <?php if(!isset($_POST['content_rad_opt']) || $_POST['content_rad_opt'] == "preview") { ?>
  <ul>
    <li><strong><a href="?page=<?php echo $rpm_page; ?>&sec=content&sec2=view&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id."#".$rpm_db_id ; ?>">Content</a></strong> 
      <ul>
        <li>View content</li>
      </ul>
    </li>
  </ul>
  <?php

	$query_content = mysql_query("SELECT * 
			FROM content
			WHERE content_id = '$rpm_db_id'
			ORDER BY 'content_lang' ASC") ;

	$farbe=$roscms_intern_color1;
	
	$result_content = mysql_fetch_array($query_content);
	$roscms_intern_editor_content = "";
	$roscms_intern_editor_content = $result_content['content_text'];
?>
  <form name="cms_content" method="post" action="<?php //echo $_SERVER['PHP_SELF'];
		echo '?page='.$rpm_page.'&amp;sec=content&amp;sec2=save&amp;db_id='.$rpm_db_id;
	 ?>">
    <table width="600" border="0" cellpadding="1" cellspacing="1">
      <tr bgcolor="#5984C3"> 
        <td width="15%" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Content 
            ID </strong></font></div></td>
        <td width="85%" bgcolor="#E2E2E2"> <div align="left"><font face="Arial, Helvetica, sans-serif"> 
            [#cont_
            <input name="txt_contentid" type="text" id="txt_contentid" value="<?php echo $result_content['content_name']; ?>" size="50" maxlength="50">
            ] &nbsp; (e.g. &quot;media&quot;)</font></div></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Language</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><div align="left"> 
            <select id="txt_langa" size="1" name="txt_langa" class="selectbox">
              <?php 
				//echo $result_page['page_language'];
				$cmsros_intern_temp_lang_short=$result_content['content_lang'];
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
              <optgroup label="other"> 
              <option value="xhtml"<?php if ($result_content['content_lang'] == "xhtml") { echo ' selected="selected"'; } ?>>XHTML</option>
              <option value="html"<?php if ($result_content['content_lang'] == "html") { echo ' selected="selected"'; } ?>>HTML</option>
              </optgroup>
            </select>
          </div></td>
      </tr>
      <?php 
	if ($result_content['content_editor'] != "richtext" && $result_content['content_editor'] != "bbcode") {
?>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Content</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><textarea name="textarea_content" cols="60" rows="20" id="textarea_content"><?php
				//echo htmlentities(ereg_replace("&amp;(#[0-9]{4};)", "&\\1", $roscms_intern_editor_content), ENT_QUOTES, 'UTF-8'); 
				echo ereg_replace("&amp;(#[0-9]{4};)", "&\\1", htmlentities($roscms_intern_editor_content, ENT_QUOTES, 'UTF-8'))

				//echo htmlentities($roscms_intern_editor_content);
				
			?></textarea></td>
      </tr>
      <?php
	}
?>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Revision</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_content['content_version'];
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Visible</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			if ($result_content['content_visible'] == 1) {
				echo ' <input name="content_visible" type="radio" value="yes" checked> yes &nbsp; <input type="radio" name="content_visible" value="no"> no';
			}
			else {
				echo ' <input name="content_visible" type="radio" value="yes"> yes &nbsp; <input type="radio" name="content_visible" value="no" checked> no';
			}	
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Active</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			if ($result_content['content_active'] == 1) {
				echo ' <input name="content_active" type="radio" value="yes" checked> yes &nbsp; <input type="radio" name="content_active" value="no"> no';
			}
			else {
				echo ' <input name="content_active" type="radio" value="yes"> yes &nbsp; <input type="radio" name="content_active" value="no" checked> no';
			}	
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Editor</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"> <select id="txt_extra" size="1" name="txt_extra" class="selectbox">
            <optgroup label="current"> 
            <?php   
				echo '<option value="'.$result_content["content_editor"].'"';
				echo ' selected="selected">';
				
				if ($result_content["content_editor"]=="") {
					echo "plain text";
				}
				else {
					echo $result_content["content_editor"];
				}
				echo '</option>';
				?>
            </optgroup>
            <optgroup label="extra option"> 
            <option value="">plain text</option>
            <option value="richtext">richtext</option>
            <option value="bbcode">bbcode</option>
            </optgroup>
          </select> </td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Date</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_content['content_date'];
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Time</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_content['content_time'];
		?>
          </font></td>
      </tr>
    </table>
    <p> 
      <?php
		if ($result_content['content_editor']=="richtext") {

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
			$oFCKeditor->Value		= $roscms_intern_editor_content;
			$oFCKeditor->Create() ;
		}
		if ($result_content['content_editor']=="bbcode") {
			include("../editor/bbcode/bbcodeeditor.php");
			include("../editor/bbcode/cbparser.php");
			if (isset($_POST['content_rad_opt'])) {
				echo "<p><fieldset><legend>Preview</legend>".bb2html(stripslashes(@$_POST['post']),'')."</fieldset></p>";
			}
			else {
				echo "<p><fieldset><legend>Preview</legend>".bb2html(stripslashes($roscms_intern_editor_content),'')."</fieldset></p>";
				//echo "<p><hr>".bb2html(stripslashes($roscms_intern_editor_content),'')."<hr></p>";
			}
		}
		
		$roscms_intern_content_name=$result_content['content_name'];
	?>
    </p>
    <p> 
      <?php if ($rpm_opt=="translate") { ?>
      <input name="content_rad_opt" type="radio" value="translate" checked>
      Translate 
    <p><b><blink>Please check, if you selected the right language, before you 
      click "submit"!</blink></b></p>
    <input type="submit" name="Submit" value="Submit">
    <p>&nbsp;</p>
    <?php
	  } else { ?>
    <input name="content_rad_opt" type="radio" value="insert" <?php if($roscms_intern_account_id != $result_content['content_usrname_id'] || date("Y-m-d") != $result_content['content_date']) { echo "checked"; } ?>>
    Save (new version) &nbsp; 
    <?php 
		if(($roscms_intern_usrgrp_admin == true || ($roscms_intern_account_id == $result_content['content_usrname_id'] && date("Y-m-d") == $result_content['content_date'])) AND $roscms_intern_content_name != "" ) {
	?>
    <input name="content_rad_opt" type="radio" value="update" <?php if($roscms_intern_account_id == $result_content['content_usrname_id'] && date("Y-m-d") == $result_content['content_date']) { echo "checked"; } ?>>
    Update 
    <?php } ?>
    <?php 
				if($result_content['content_editor'] == "bbcode") {
			?>
    <input name="content_rad_opt" type="radio" value="preview" checked>
    Preview 
    <?php } ?></p>
    <p>
      <?php 
			if ($roscms_intern_usrgrp_sadmin == true ||
				$roscms_intern_usrgrp_admin == true ||
				$roscms_intern_usrgrp_dev == true ||
				$roscms_intern_usrgrp_team == true ||
				$roscms_intern_usrgrp_trans == true) {
			?>
      <input type="submit" name="Submit" value="Submit">
      <?php  } else { ?>
      <img src="images/lock.gif" alt="Locked" width="19" height="18"> (you need 
      a higher account level to save the content to the database)</p>
    <?php
			 } 
		}
	?>
    <p><strong>Info:</strong> for each [#inc_xyz] tag the RosCMS will include 
      the code that is linked with this tag from database.<br>
      <br>
      <strong>Hints:</strong></p>
    <ul>
      <li>use &amp;amp; instead of &quot;&amp;&quot; in links, e.g. http://www.reactos.org/?page=support&amp;amp;lang=en</li>
      <li> &quot;&lt;placeholder&gt;&quot; =&gt; &amp;lt;placeholder&amp;gt; </li>
      <li>ReactOS Homepage URL: [#roscms_path_homepage]</li>
    </ul>
  </form>
  <?php
	}
	elseif(isset($_POST['content_rad_opt']) && $_POST['content_rad_opt'] != "preview") {
?>
  <ul>
    <li><strong><a href="?page=<?php echo $rpm_page; ?>&sec=content&sec2=view&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id."#".$rpm_db_id ; ?>">Content</a></strong> 
      <ul>
        <li>Save content</li>
      </ul>
    </li>
  </ul>
  <?php 

		$content_contentid="";
		$content_langa="";
		$content_vis="";
		$content_act="";
		$content_extra="";
		$content_savemode="";

		if (array_key_exists("txt_contentid", $_POST)) $content_contentid=$_POST['txt_contentid'];
		if (array_key_exists("txt_langa", $_POST)) $content_langa=$_POST['txt_langa'];
		if (array_key_exists("content_visible", $_POST)) $content_vis=$_POST['content_visible'];
		if (array_key_exists("content_active", $_POST)) $content_act=$_POST['content_active'];
		if (array_key_exists("txt_extra", $_POST)) $content_extra=$_POST['txt_extra'];
		if (array_key_exists("content_rad_opt", $_POST)) $content_savemode=$_POST['content_rad_opt'];

		//echo "<br>".$content_act;
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
		
		//die($content_data);

		//echo "<p><fieldset><legend>Preview</legend>".$content_data."</fieldset></p>";


		if ($content_savemode == "update") {
			$content_description = date("Y-m-d H:i:s")." [".$roscms_intern_account_id."] ";
			
			$content_posta="UPDATE `content` SET `content_name` = '". mysql_escape_string($content_contentid) ."',
				`content_lang` = '". mysql_escape_string($content_langa) ."',
				`content_editor` = '". mysql_escape_string($content_extra) ."',
				`content_text` = '". mysql_real_escape_string($content_data)  ."',
				`content_visible` = '". mysql_escape_string($content_vis) ."',
				`content_active` = '". mysql_escape_string($content_act) ."',
				`content_description` = '". mysql_escape_string($content_description) ."'
				WHERE `content_id` = '$rpm_db_id' LIMIT 1 ;";
				//`content_active` = '1',
				//`content_usrname_id` = '$roscms_intern_account_id',
				//`content_date` = CURDATE( ) ,
				//`content_time` = CURTIME( )
			$content_post_lista=mysql_query($content_posta);
		}
		elseif ($content_savemode == "translate") {
			$content_version="1";
			$content_postb="INSERT INTO `content` ( `content_id` , `content_name` , `content_lang` , `content_editor` , `content_text` , `content_version` , `content_active` , `content_visible` , `content_date` , `content_time` , `content_usrname_id`)
				VALUES ('', '". mysql_escape_string($content_contentid) ."', '". mysql_escape_string($content_langa) ."', '". mysql_escape_string($content_extra) ."', '". mysql_real_escape_string($content_data) ."', '1', '". mysql_escape_string($content_act) ."', '". mysql_escape_string($content_vis) ."', CURDATE( ), CURTIME( ), '". mysql_escape_string($roscms_intern_account_id) ."');";
			$content_post_listb=mysql_query($content_postb);
		}	
		else {
			// content Version:
			$query_content_save_version = mysql_query("SELECT * 
										FROM `content` 
										WHERE `content_name` 
										LIKE '$content_contentid'
										ORDER BY `content_version` DESC LIMIT 1;");
			$result_content_save_version = mysql_fetch_array($query_content_save_version);
			$content_version=$result_content_save_version['content_version'];
			$content_version++;
			
			$query_content_save_equal = mysql_query("SELECT * 
													FROM `content` 
													WHERE `content_name` = '$content_contentid'  AND `content_lang` = '$content_langa' ;") ;
			while($result_content_savea = mysql_fetch_array($query_content_save_equal)) {
				// Set all versions not active:
				$content_na=$result_content_savea['content_id'];
				$content_postc="UPDATE `content` SET `content_active` = '0' WHERE `content_id` = '$content_na' LIMIT 1 ;";
				$content_post_listc=mysql_query($content_postc);
			}
		
			$content_postb="INSERT INTO `content` ( `content_id` , `content_name` , `content_lang` , `content_editor` , `content_text` , `content_version` , `content_active` , `content_visible` , `content_date` , `content_time` , `content_usrname_id`)
				VALUES ('', '". mysql_escape_string($content_contentid) ."', '". mysql_escape_string($content_langa) ."', '". mysql_escape_string($content_extra) ."', '". mysql_real_escape_string($content_data) ."', '". mysql_escape_string($content_version) ."', '". mysql_escape_string($content_act) ."', '". mysql_escape_string($content_vis) ."', CURDATE( ), CURTIME( ), '". mysql_escape_string($roscms_intern_account_id) ."');";
			$content_post_listb=mysql_query($content_postb);
		}	
		
	
	
		$query_content = mysql_query("SELECT content_text, content_id, content_name
				FROM content
				WHERE content_id = '$rpm_db_id'") ;
		$result_content = mysql_fetch_array($query_content);


		if ($content_savemode == "update") {
			echo "<p>The content '".$result_content['content_name']."' (id='".$rpm_db_id."') has been saved!</p>";
			echo "<p><a href=".$_SERVER['HTTP_REFERER'].">Back to the 'content edit' page</a></p>";
		}
		else {		
			$query_content_new_revision_preview = mysql_query("SELECT * 
														FROM `content` 
														WHERE `content_name` LIKE '$content_contentid'
														AND `content_lang` = '$content_langa'
														AND `content_version` = ". $content_version ."
														LIMIT 1;");
			$result_content_new_revision_preview = mysql_fetch_array($query_content_new_revision_preview);
			
			echo "<p>A new version of content '".$result_content_new_revision_preview['content_name']."' (old id='".$rpm_db_id."', new id='". $result_content_new_revision_preview["content_id"] ."') has been saved!</p>";		
			echo "<p><a href='?page=". $rpm_page ."&amp;sec=content&amp;sec2=edit&amp;sort=". $rpm_sort ."&amp;filt=". $rpm_filt ."&amp;langid=". $rpm_lang_id ."&amp;db_id=". $result_content_new_revision_preview['content_id'] ."'>Go to the 'content edit' page (revision ". $result_content_new_revision_preview["content_id"] .")</a></p>";
			echo "<p><a href='".$_SERVER['HTTP_REFERER']."'>Back to the 'content edit' page (revision ". $result_content['content_id'] .")</a></p>";
		}
		echo "<p>&nbsp;</p><p><fieldset><legend>Preview</legend><br>".$result_content['content_text']."</fieldset></p>";

}
?>
</div>

