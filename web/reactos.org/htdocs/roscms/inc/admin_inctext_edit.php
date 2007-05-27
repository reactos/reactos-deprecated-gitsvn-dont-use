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
if(!isset($_POST['testaaaa'])) {	
?>
<div class="contentSmall"><h1>Admin Interface - Include Text</h1>
  <ul>
    <li><strong><a href="?page=<?php echo $rpm_page; ?>&sec=inctext&sec2=view&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id."#".$rpm_db_id ; ?>">Admin Interface - Include Text</a></strong> 
      <ul>
        <li>Add/edit IncText</li>
      </ul>
    </li>
  </ul>
  <?php 

	$query_inctext = mysql_query("SELECT * 
			FROM include_text
			WHERE inc_id = '$rpm_db_id'
			ORDER BY 'inc_lang' ASC") ;

	$farbe=$roscms_intern_color1;
	
	$result_inctext = mysql_fetch_array($query_inctext);
?>
  <form name="cms_inctext" method="post" action="<?php //echo $_SERVER['PHP_SELF'];
		echo '?page='.$rpm_page.'&amp;sec=inctext&amp;sec2=save&amp;db_id='.$rpm_db_id;
	 ?>">
    <table width="600" border="0" cellpadding="1" cellspacing="1">
      <tr bgcolor="<?php echo $roscms_intern_color0; ?>">
        <td width="15%" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>ID</strong></font></div></td>
        <td width="85%" bgcolor="#E2E2E2"><div align="left"><font face="Arial, Helvetica, sans-serif"> <font size="1">[#cont_</font>
                <?php
			if ($rpm_page == "trans") {
				echo '<input name="txt_inctextid" type="hidden" id="txt_inctextid" value="'.$result_inctext['inc_word'].'"><b>'.$result_inctext['inc_word'].'</b>';
			}
			else {
				echo '<input name="txt_inctextid" type="text" id="txt_inctextid" value="'.$result_inctext['inc_word'].'" size="50" maxlength="50">';
			}
            ?>
          <font size="1">]</font> &nbsp; (<?php echo $roscms_langres['ContTrans_egAbout']; ?>)</font></div></td>
      </tr>
      <tr>
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Lang</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><div align="left">
            <input name="txt_langa" type="text" id="txt_langa" value="<?php echo $result_inctext['inc_lang']; ?>" />
        </div></td>
      </tr>
      <tr>
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Visible</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif">
          <?php 
			if ($rpm_page == "trans") {
				echo '<input name="inc_visible" type="hidden" id="inc_visible" value="yes"> '.$roscms_langres['ContTrans_yes'];
			}
			else {
				if ($result_inctext['inc_vis'] == 1) {
					echo ' <input name="inc_visible" type="radio" value="yes" checked> yes &nbsp; <input type="radio" name="inc_visible" value="no"> '.$roscms_langres['ContTrans_no'];
				}
				else {
					echo ' <input name="inc_visible" type="radio" value="yes"> yes &nbsp; <input type="radio" name="inc_visible" value="no" checked> '.$roscms_langres['ContTrans_no'];
				}	
			}
		?>
        </font></td>
      </tr>
      <tr>
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Extra</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><input id="txt_extra" name="txt_extra" type="text" />        </td>
      </tr>


      <tr>
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_User']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif">
          <?php 
		
				if ($result_inctext['inc_usrname_id']) {
					$query_user_name = mysql_query("SELECT * 
																FROM `users` 
																WHERE `user_id` = ". $result_inctext['inc_usrname_id'] ." ;") ;
					$result_user_name = mysql_fetch_array($query_user_name);
			
					echo "<b>".$result_user_name['user_name']."</b> (".$result_user_name['user_fullname'].") <a href='?page=user&amp;sec=profil&amp;sec2=".$result_inctext['inc_usrname_id']."' target='_blank'>[Profile]</a>";
				}
		?>
        </font></td>
      </tr>
      <tr>
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Date']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif">
          <?php 
			echo $result_inctext['inc_date'];
		?>
        </font></td>
      </tr>
      <tr>
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Time']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif">
          <?php 
			echo $result_inctext['inc_time'];
		?>
          <input name="testaaaa" type="hidden" id="testaaaa" value="hidden" />
          <input name="testbbbb" type="hidden" id="testaaaa" value="<?php echo $result_inctext['inc_word']; ?>" />
        </font></td>
      </tr>
    </table>
    <p>
      <textarea name="txt_text" cols="80" rows="10" id="txt_text"><?php 
			echo $result_inctext['inc_text'];
		?></textarea>
    </p>
    <p>Click &quot;Submit&quot; to save the changes to the database, the best suitable save-setting already choosen. </p>

    <input type="submit" name="Submit" value="Submit">
  </form>
    <p>&nbsp;</p>
  <?php
	}
	elseif(isset($_POST['testaaaa'])) {
?>
  <ul>
    <li><strong><a href="?page=<?php echo $rpm_page; ?>&sec=inctext&sec2=view&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id."#".$rpm_db_id ; ?>">IncText</a></strong> 
      <ul>
        <li><?php echo $roscms_langres['ContTrans_SaveContent']; ?></li>
      </ul>
    </li>
  </ul>
  <?php 

		$inc_inctextid="";
		$inc_langa="";
		$inc_vis="";
		$inc_extra="";
		$inc_savemode="";
		$inc_text="";
		
		if (array_key_exists("txt_inctextid", $_POST)) $inc_inctextid=$_POST['txt_inctextid'];
		if (array_key_exists("txt_langa", $_POST)) $inc_langa=$_POST['txt_langa'];
		if (array_key_exists("inc_visible", $_POST)) $inc_vis=$_POST['inc_visible'];
		if (array_key_exists("inc_extra", $_POST)) $inc_extra=$_POST['inc_extra'];
		if (array_key_exists("txt_text", $_POST)) $inc_text=$_POST['txt_text'];
		if (array_key_exists("testbbbb", $_POST)) $inc_updateornot=$_POST['testbbbb'];


		//echo "<br>".$inc_act;
		if ($inc_langa == "") {
			$inc_langa = "all";
		}

		if ($inc_vis == "yes") {
			$inc_vis="1";
		}
		else {
			$inc_vis="0";
		}
		
		if ($inc_updateornot == "") {
			$inc_savemode = "save";
		}
		else {
			$inc_savemode = "update";
		}
		
		
		if ($inc_savemode == "update") {			
			echo "<p>Update entry</p>";
			$inc_posta="UPDATE `include_text` SET `inc_word` = '". mysql_escape_string($inc_inctextid) ."',
				`inc_lang` = '". mysql_escape_string($inc_langa) ."',
				`inc_extra` = '". mysql_escape_string($inc_extra) ."',
				`inc_text` = '". mysql_real_escape_string($inc_text)  ."',
				`inc_vis` = '". mysql_escape_string($inc_vis) ."',
				`inc_altered` = '1'
				WHERE `inc_id` = '". mysql_escape_string($rpm_db_id) ."' LIMIT 1 ;";

			$inc_post_lista=mysql_query($inc_posta);
			//echo "<p>SQL: <br/>".$inc_posta;
		}
		else {
			echo "<p>Save entry</p>";
			$inc_postb="INSERT INTO `include_text` ( `inc_id` , `inc_level` , `inc_word` , `inc_text` , `inc_lang` , `inc_extra` , `inc_vis` , `inc_seclevel` , `inc_usrname_id` , `inc_date` , `inc_time` , `inc_altered`)
				VALUES ('', '15', '". mysql_escape_string($inc_inctextid) ."', '". mysql_escape_string($inc_text) ."', '". mysql_escape_string($inc_langa) ."', '". mysql_real_escape_string($inc_extra) ."', '". mysql_escape_string($inc_vis) ."', '50', '". mysql_escape_string($roscms_intern_account_id) ."', CURDATE( ), CURTIME( ), '1');";
			$inc_post_listb=mysql_query($inc_postb);
			//echo "<p>SQL: <br/>".$inc_postb;
			
			/*INSERT INTO `include_text` ( `inc_id` , `inc_level` , `inc_word` , `inc_text` , `inc_lang` , `inc_extra` , `inc_vis` , `inc_seclevel` , `inc_usrname_id` , `inc_date` , `inc_time` , `inc_altered` ) 
			VALUES (
			NULL , '0', 'test', 'abc', 'all', 'php_template', '1', '50', '1', CURDATE( ) , CURTIME( ) , '1'
			);*/
		}	
		
	
	
		echo "<p><b><a href='?page=". $rpm_page ."&amp;sec=inctext'>Go on to IncText</a></p>";
}
?>
</div>

