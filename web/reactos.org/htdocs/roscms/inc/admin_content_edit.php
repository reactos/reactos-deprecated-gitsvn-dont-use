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
	
	if ($rpm_page != "admin" && $rpm_page != "dev" && $rpm_page != "team" && $rpm_page != "trans") {
		die("");
	}
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
    <li><strong><a href="?page=<?php echo $rpm_page; ?>&sec=content&sec2=view&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id."#".$rpm_db_id ; ?>"><?php
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
  ?></a></strong> 
      <ul>
        <li><?php echo $roscms_langres['ContTrans_ViewEditContent']; ?></li>
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
      <tr bgcolor="<?php echo $roscms_intern_color0; ?>"> 
        <td width="15%" bgcolor="<?php echo $roscms_intern_color0; ?>"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_ContentID']; ?></strong></font></div></td>
        <td width="85%" bgcolor="#E2E2E2"> <div align="left"><font face="Arial, Helvetica, sans-serif"> 
            <font size="1">[#cont_</font><?php
			if ($rpm_page == "trans") {
				echo '<input name="txt_contentid" type="hidden" id="txt_contentid" value="'.$result_content['content_name'].'"><b>'.$result_content['content_name'].'</b>';
			}
			else {
				echo '<input name="txt_contentid" type="text" id="txt_contentid" value="'.$result_content['content_name'].'" size="50" maxlength="50">';
			}
            ?><font size="1">]</font> &nbsp; (<?php echo $roscms_langres['ContTrans_egAbout']; ?>)</font></div></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Language']; ?></strong></font></div></td>
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
<?php 
			if ($rpm_page != "trans") {
				echo '<option value="all"';
				if ($cmsros_intern_temp_lang_short == "all") {
					echo ' selected="selected"';
				}
				echo ">".$roscms_langres['ContTrans_All']; 
				echo "</option>";
			}


		$sql_lang_usr="SELECT * 
					FROM users
					WHERE user_id = '".mysql_escape_string($roscms_intern_account_id)."'
					LIMIT 1 ;";
		$sql_query_lang_usr=mysql_query($sql_lang_usr);
		$myrow_lang_usr=mysql_fetch_array($sql_query_lang_usr);

		// Languages
		if ($rpm_page == "trans") {
			$sql_langa="SELECT * 
						FROM languages
						WHERE lang_level != '0'
						AND lang_id = '".mysql_escape_string($myrow_lang_usr['user_language'])."'
						ORDER BY 'lang_level' DESC";
			$sql_query_langa=mysql_query($sql_langa);
		}
		else {
			$sql_langa="SELECT * 
						FROM languages
						WHERE lang_level != '0'
						ORDER BY 'lang_level' DESC";
			$sql_query_langa=mysql_query($sql_langa);
		}
		while($myrow_langa=mysql_fetch_row($sql_query_langa)) {
			if ($rpm_page == "trans" && $myrow_langa[0] == "en" && $myrow_lang_usr['user_language'] != "en") {
				// temp
			}
			else {
				echo '<option value="'.$myrow_langa[0].'"';
				if ($myrow_lang[0] == $myrow_langa[0]) {
					echo ' selected="selected"';
				}
				echo '>'.$myrow_langa[1].'</option>';
			}
		}
              ?>
              </optgroup>
			  <?php
			if ($rpm_page != "trans") {
              ?>
              <optgroup label="other"> 
              <option value="xhtml"<?php if ($result_content['content_lang'] == "xhtml") { echo ' selected="selected"'; } ?>>XHTML</option>
              <option value="html"<?php if ($result_content['content_lang'] == "html") { echo ' selected="selected"'; } ?>>HTML</option>
              </optgroup>
			  <?php
			}
              ?>
            </select>
          </div></td>
      </tr>
      <?php 
	if ($result_content['content_editor'] != "richtext" && $result_content['content_editor'] != "bbcode") {
?>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Content']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><textarea name="textarea_content" cols="60" rows="20" id="textarea_content"><?php
				//echo htmlentities(ereg_replace("&amp;(#[0-9]{4};)", "&\\1", $roscms_intern_editor_content), ENT_QUOTES, 'UTF-8'); 
				echo ereg_replace("&amp;(#[0-9]{4};)", "&\\1", htmlentities($roscms_intern_editor_content, ENT_QUOTES, 'UTF-8'));
				//echo ereg_replace("&amp;(#[0-9]{4};)", "&\\1", $roscms_intern_editor_content);
				//echo htmlspecialchars($roscms_intern_editor_content);

				//echo htmlentities($roscms_intern_editor_content);
				
			?></textarea></td>
      </tr>
      <?php
	}
?>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Rev']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_content['content_version'];
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Visible']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			if ($rpm_page == "trans") {
				echo '<input name="content_visible" type="hidden" id="content_visible" value="yes"> '.$roscms_langres['ContTrans_yes'];
			}
			else {
				if ($result_content['content_visible'] == 1) {
					echo ' <input name="content_visible" type="radio" value="yes" checked> yes &nbsp; <input type="radio" name="content_visible" value="no"> '.$roscms_langres['ContTrans_no'];
				}
				else {
					echo ' <input name="content_visible" type="radio" value="yes"> yes &nbsp; <input type="radio" name="content_visible" value="no" checked> '.$roscms_langres['ContTrans_no'];
				}	
			}
		?>
          
</font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Active']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			if ($rpm_page == "trans") {
				echo '<input name="content_active" type="hidden" id="content_active" value="yes"> '.$roscms_langres['ContTrans_yes'];
			}
			else {
				if ($result_content['content_active'] == 1) {
					echo ' <input name="content_active" type="radio" value="yes" checked> yes &nbsp; <input type="radio" name="content_active" value="no"> '.$roscms_langres['ContTrans_no'];
				}
				else {
					echo ' <input name="content_active" type="radio" value="yes"> yes &nbsp; <input type="radio" name="content_active" value="no" checked> '.$roscms_langres['ContTrans_no'];
				}	
			}
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Editor']; ?></strong></font></div></td>
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
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Type']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif">
<?php

			if ($rpm_page == "trans") {
				echo '<input name="txt_content_type" type="hidden" id="txt_content_type" value="';
				if ($rpm_db_id == "new") {
					echo "default";
				}
				else {
					echo $result_content['content_type'];
				}
				echo'">'.$result_content['content_type'];
			}
			else {
				echo '<input name="txt_content_type" type="text" id="txt_content_type" value="';
				if ($rpm_db_id == "new") {
					echo "default";
				}
				else {
					echo $result_content['content_type'];
				}
				echo '" size="30" maxlength="30">  (&quot;default&quot;, &quot;layout&quot;, ...)';
			}
?></font></td>
      </tr>
      <tr>
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Description']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif">
<?php
			if ($rpm_page == "trans") {
				echo '<input name="txt_content_desc" type="hidden" id="txt_content_desc" value="'.$result_content['content_description'].'">'.$result_content['content_description'];
			}
			else {
				echo '<input name="txt_content_desc" type="text" id="txt_content_desc" value="'.$result_content['content_description'].'" size="50" maxlength="255">';
			}
        ?></font></td>
      </tr>
      <tr>
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_User']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"><?php 
		
				if ($result_content['content_usrname_id']) {
					$query_user_name = mysql_query("SELECT * 
																FROM `users` 
																WHERE `user_id` = ". $result_content['content_usrname_id'] ." ;") ;
					$result_user_name = mysql_fetch_array($query_user_name);
			
					echo "<b>".$result_user_name['user_name']."</b> (".$result_user_name['user_fullname'].") <a href='?page=user&amp;sec=profil&amp;sec2=".$result_content['content_usrname_id']."' target='_blank'>[Profile]</a>";
				}
		?></font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Date']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_content['content_date'];
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Time']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
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
	

	<p>&nbsp;</p>
	<p>Click &quot;Submit&quot; to save the changes to the database, the best suitable save-setting already choosen. </p>
	<p> 
      <?php if ($rpm_opt == "translate") { ?>
	  
      <input name="content_rad_opt" type="radio" value="translate" checked>
      <?php echo $roscms_langres['ContTrans_Translate']; ?> 
    <p><b><blink><?php echo $roscms_langres['ContTrans_checklang']; ?></blink></b></p>
    <input type="submit" name="Submit" value="Submit">
    <p>&nbsp;</p>
    <?php
	  } else { ?>
    <input name="content_rad_opt" type="radio" value="insert" <?php if($roscms_intern_account_id != $result_content['content_usrname_id'] || date("Y-m-d") != $result_content['content_date']) { echo "checked"; } ?>>
    <?php echo $roscms_langres['ContTrans_SAVE']; ?> &nbsp; 
    <?php 
		if(($roscms_intern_usrgrp_admin == true || ($roscms_intern_account_id == $result_content['content_usrname_id'] && date("Y-m-d") == $result_content['content_date'])) AND $roscms_intern_content_name != "" ) {
	?>
    <input name="content_rad_opt" type="radio" value="update" <?php if($roscms_intern_account_id == $result_content['content_usrname_id'] && date("Y-m-d") == $result_content['content_date']) { echo "checked"; } ?>>
    <?php echo $roscms_langres['ContTrans_UPDATE']; ?> 
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
	<p>&nbsp;</p>
	<p>
	<?php
			$RosCMS_query_translate_content_current = mysql_query("SELECT *
												FROM content 
												WHERE content_name = '". mysql_real_escape_string($result_content['content_name'])  ."' 
												AND `content_lang` = 'all'
												AND `content_active` = 1
												AND `content_visible` = 1
												ORDER BY `content_version` DESC 
												LIMIT 1 ;");
			$RosCMS_result_translate_content_current = mysql_fetch_array($RosCMS_query_translate_content_current);
			$RosCMS_query_translate_content_old = mysql_query("SELECT *
												FROM content 
												WHERE content_name = '". mysql_real_escape_string($result_content['content_name'])  ."' 
												AND `content_lang` = 'all'
												AND `content_active` = 0
												ORDER BY `content_version` DESC 
												LIMIT 1 ;");
			$RosCMS_result_translate_content_old = mysql_fetch_array($RosCMS_query_translate_content_old);
			
		if ($rpm_db_id != "new" && $RosCMS_result_translate_content_old['content_id'] != "" && $RosCMS_result_translate_content_current['content_id'] != "") {
	?>	
			<!-- Diff  -->
			<script src="js/diff.js" language="javascript"></script>
			<b>Latest changes of the english content:<br />
			</b>Note: some characters may appear in utf-8 encoded version, don't copy and paste directly; <a href="#orgcont">use the text box below</a> to copy&amp;paste content !!!
	<table width="100%" border="1" cellpadding="0" cellspacing="0" bordercolor="#666666" bgcolor="#CCCCCC">
			  <tr>
				<td><font size="1"><div id="diff2"></div></font></td>
			  </tr>
	</table>
			<div id="d1" style="display:none;"><?php echo $RosCMS_result_translate_content_old['content_text']; ?></div>
			<div id="d2" style="display:none;"><?php echo $RosCMS_result_translate_content_current['content_text']; ?></div>
			<script type="text/javascript">
				document.getElementById('diff2').innerHTML = StringDiff( 
					document.getElementById('d1').innerHTML, 
					document.getElementById('d2').innerHTML 
				);
			</script>
	<?php 
		}
		if ($rpm_db_id != "new") { ?>
			<br /><b><a name="orgcont" id="orgcont"></a>Current english original content:</b><br />
			<textarea name="txtorginal" cols="80" rows="5" id="txtorginal" readonly="readonly"><?php 
			echo ereg_replace("&amp;(#[0-9]{4};)", "&\\1", htmlentities($RosCMS_result_translate_content_current['content_text'], ENT_QUOTES, 'UTF-8')); ?>
			</textarea>
	<?php
		}
	?>
	</p>	    <table width="100%" border="1" cellpadding="5" cellspacing="0" bordercolor="#666666">
      <tr>
        <td><p><strong><font size="1">Hints:</font></strong></p>
            <ul>
              <li><font size="1">use &amp;amp; instead of &quot;&amp;&quot; in links, e.g. http://www.xyz.org/?page=support&amp;amp;lang=en</font></li>
              <li> <font size="1">&quot;&lt;placeholder&gt;&quot; =&gt; &amp;lt;placeholder&amp;gt; </font></li>
              <li><font size="1"> Homepage URL: [#roscms_path_homepage]</font></li>
          </ul></td>
      </tr>
    </table>
  </form>
  <?php
	}
	elseif(isset($_POST['content_rad_opt']) && $_POST['content_rad_opt'] != "preview") {
?>
  <ul>
    <li><strong><a href="?page=<?php echo $rpm_page; ?>&sec=content&sec2=view&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id."#".$rpm_db_id ; ?>">Content</a></strong> 
      <ul>
        <li><?php echo $roscms_langres['ContTrans_SaveContent']; ?></li>
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
		$content_typea="";
		$content_desca="";

		if (array_key_exists("txt_contentid", $_POST)) $content_contentid=$_POST['txt_contentid'];
		if (array_key_exists("txt_langa", $_POST)) $content_langa=$_POST['txt_langa'];
		if (array_key_exists("content_visible", $_POST)) $content_vis=$_POST['content_visible'];
		if (array_key_exists("content_active", $_POST)) $content_act=$_POST['content_active'];
		if (array_key_exists("txt_extra", $_POST)) $content_extra=$_POST['txt_extra'];
		if (array_key_exists("content_rad_opt", $_POST)) $content_savemode=$_POST['content_rad_opt'];
		if (array_key_exists("txt_content_type", $_POST)) $content_typea=$_POST['txt_content_type'];
		if (array_key_exists("txt_content_desc", $_POST)) $content_desca=$_POST['txt_content_desc'];


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
			$content_description = date("Y-m-d H:i:s")." [".$roscms_intern_account_id."] | ".$content_desca;
			
			$content_posta="UPDATE `content` SET `content_name` = '". mysql_escape_string($content_contentid) ."',
				`content_lang` = '". mysql_escape_string($content_langa) ."',
				`content_editor` = '". mysql_escape_string($content_extra) ."',
				`content_text` = '". mysql_real_escape_string($content_data)  ."',
				`content_visible` = '". mysql_escape_string($content_vis) ."',
				`content_active` = '". mysql_escape_string($content_act) ."',
				`content_description` = '". mysql_escape_string($content_description) ."',
				`content_altered` = '1',
				`content_type` = '". mysql_escape_string($content_typea) ."'
				WHERE `content_id` = '$rpm_db_id' LIMIT 1 ;";
				//`content_active` = '1',
				//`content_usrname_id` = '$roscms_intern_account_id',
				//`content_date` = CURDATE( ) ,
				//`content_time` = CURTIME( )

			$content_post_lista=mysql_query($content_posta);
		}
		elseif ($content_savemode == "translate") {
			$RosCMS_query_translate_content = mysql_query("SELECT COUNT('content_id') 
				FROM content 
				WHERE content_name = '". mysql_real_escape_string($content_contentid)  ."' 
				AND content_lang = '". mysql_real_escape_string($content_langa)  ."' 
				AND content_active = '1' 
				AND content_visible = '1' ;");
			$RosCMS_result_translate_content = mysql_fetch_row($RosCMS_query_translate_content);
			
			if ($RosCMS_result_translate_content[0] <= 0) {
				$content_version="1";
				$content_postb="INSERT INTO `content` ( `content_id` , `content_name` , `content_lang` , `content_editor` , `content_text` , `content_version` , `content_active` , `content_visible` , `content_date` , `content_time` , `content_usrname_id`)
					VALUES ('', '". mysql_escape_string($content_contentid) ."', '". mysql_escape_string($content_langa) ."', '". mysql_escape_string($content_extra) ."', '". mysql_real_escape_string($content_data) ."', '1', '". mysql_escape_string($content_act) ."', '". mysql_escape_string($content_vis) ."', CURDATE( ), CURTIME( ), '". mysql_escape_string($roscms_intern_account_id) ."');";
				$content_post_listb=mysql_query($content_postb);
			}
			else {
			
			}
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
			$roscms_TEMP_cont_name = $result_content['content_name'];
			echo "<p>The content '".$result_content['content_name']."' (id='".$rpm_db_id."') has been saved!</p>";
			echo "<p><a href=".$_SERVER['HTTP_REFERER'].">Back to the 'content edit' page</a></p>";
			echo "<p>&nbsp;</p><p><fieldset><legend>Preview</legend><br>".$result_content['content_text']."</fieldset></p>";
		}
		else {
			if ($content_savemode == "translate" && $RosCMS_result_translate_content[0] > 0) {
				$RosCMS_query_translate_content2 = mysql_query("SELECT * 
					FROM content 
					WHERE content_name = '". mysql_real_escape_string($content_contentid)  ."' 
					AND content_lang = '". mysql_real_escape_string($content_langa)  ."' 
					AND content_active = '1' 
					AND content_visible = '1' 
					LIMIT 1 ;");
				$RosCMS_result_translate_content2 = mysql_fetch_row($RosCMS_query_translate_content2);
				
				echo "<p>This content (".$RosCMS_result_translate_content2['content_name']." - ".$RosCMS_result_translate_content2['content_lang'].") has already been translated to your favorite language.<br /><b><a href='?page=". $rpm_page ."&amp;sec=content&amp;sec2=edit&amp;sort=". $rpm_sort ."&amp;filt=". $rpm_filt ."&amp;langid=". $rpm_lang_id ."&amp;db_id=". $RosCMS_result_translate_content2['content_id'] ."'>Click here to view/edit the entry</b> (revision ". $RosCMS_result_translate_content2["content_id"] .")</a></p>";
			}
			else {		
				$query_content_new_revision_preview = mysql_query("SELECT * 
															FROM `content` 
															WHERE `content_name` LIKE '$content_contentid'
															AND `content_lang` = '$content_langa'
															AND `content_version` = ". $content_version ."
															LIMIT 1;");
				$result_content_new_revision_preview = mysql_fetch_array($query_content_new_revision_preview);
				$roscms_TEMP_cont_name = $result_content_new_revision_preview['content_name'];
				echo "<p>".$roscms_langres['ContTrans_Save1']." '".$result_content_new_revision_preview['content_name']."' (old id='".$rpm_db_id."', new id='". $result_content_new_revision_preview["content_id"] ."') ".$roscms_langres['ContTrans_Save2']."</p>";		
				echo "<p><b><a href='?page=". $rpm_page ."&amp;sec=content&amp;sec2=edit&amp;sort=". $rpm_sort ."&amp;filt=". $rpm_filt ."&amp;langid=". $rpm_lang_id ."&amp;db_id=". $result_content_new_revision_preview['content_id'] ."'>".$roscms_langres['ContTrans_Save3']."</b> (revision ". $result_content_new_revision_preview["content_id"] .")</a></p>";
				echo "<p><a href='".$_SERVER['HTTP_REFERER']."'>".$roscms_langres['ContTrans_Save4']." (revision ". $result_content['content_id'] .")</a></p>";
				echo "<p>&nbsp;</p><p><fieldset><legend>".$roscms_langres['ContTrans_Preview']."</legend><br>".$result_content_new_revision_preview['content_text']."</fieldset></p>";
			}
		}
		
	
	$query_TEMP_content = mysql_query("SELECT COUNT('page_id') 
									FROM `pages` 
									WHERE `page_name` = '". $roscms_TEMP_cont_name ."'
									AND `page_active` = 1
									AND `page_visible` = 1 ;") ;
	$result_TEMP_content = mysql_fetch_row($query_TEMP_content);
	
	// Update the "page" generator info:
	if ($result_TEMP_content[0] == "0" || $result_TEMP_content[0] == "") {
		// temp
	}
	else { 
		$content_postc="UPDATE `pages` SET `page_generate_force` = '1' WHERE `page_name` = '". $roscms_TEMP_cont_name ."' ;";
		$content_post_listc=mysql_query($content_postc);
	}
}
?>
</div>

