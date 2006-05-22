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
<div class="contentSmall"> <span class="contentSmallTitle">myReactOS - Edit Profile</span> 
  <ul>
    <li><strong><a href="?page=user&amp;sec=account">Account</a></strong> 
      <ul>
        <li>Edit Profile</li>
      </ul>
    </li>
  </ul>
<?php

require_once("subsys_utils.php");

	$query_contenta = mysql_query("SELECT * FROM users WHERE user_id = '".$roscms_intern_account_id."'") ;

	$farbe=$roscms_intern_color1;
	
	$result_contenta = mysql_fetch_array($query_contenta);
	
	if ($rpm_sec2=="save") {
		$save_account_fullname="";
		$save_account_pwd1="";
		$save_account_pwd2="";
		$save_account_pwd3="";
		$save_account_email="";
		$save_account_hp="";
		$save_account_accup="";
		$save_account_intere="";
		$save_account_textarea_content="";
		$save_account_multi="";
		$save_account_brows="";
		$save_account_ipadd="";
		$save_account_noses="";
		$save_account_txt_langa="";
		$save_account_country="";
		$save_account_timezone="";
		
		if (array_key_exists("fullname", $_POST)) $save_account_fullname=$_POST['fullname'];
		if (array_key_exists("pwd1", $_POST)) $save_account_pwd1=$_POST['pwd1'];
		if (array_key_exists("pwd2", $_POST)) $save_account_pwd2=$_POST['pwd2'];
		if (array_key_exists("pwd3", $_POST)) $save_account_pwd3=$_POST['pwd3'];
		if (array_key_exists("email", $_POST)) $save_account_email=$_POST['email'];
		if (array_key_exists("hp", $_POST)) $save_account_hp=$_POST['hp'];
		if (array_key_exists("accup", $_POST)) $save_account_accup=$_POST['accup'];
		if (array_key_exists("intere", $_POST)) $save_account_intere=$_POST['intere'];
		if (array_key_exists("textarea_content", $_POST)) $save_account_textarea_content=$_POST['textarea_content'];
		if (array_key_exists("multi", $_POST)) $save_account_multi=$_POST['multi'];
		if (array_key_exists("brows", $_POST)) $save_account_brows=$_POST['brows'];
		if (array_key_exists("ipadd", $_POST)) $save_account_ipadd=$_POST['ipadd'];
		if (array_key_exists("noses", $_POST)) $save_account_noses=$_POST['noses'];
		if (array_key_exists("txt_langa", $_POST)) $save_account_txt_langa=$_POST['txt_langa'];
		if (array_key_exists("country", $_POST)) $save_account_country=$_POST['country'];
		if (array_key_exists("timezone", $_POST)) $save_account_timezone=$_POST['timezone'];
		
		if ($save_account_txt_langa) {
			$new_pwd="";
			if ($save_account_pwd1 && ($result_contenta['user_roscms_password'] == md5($save_account_pwd1)) && $save_account_pwd2 == $save_account_pwd3) {		
				echo "New password saved!";
				$new_pwd = "`user_roscms_password` = MD5( '".mysql_real_escape_string($save_account_pwd2)."' ) ,";
			}
			
			if (!$save_account_multi) {
				$save_account_multi = "false";
			}
			if (!$save_account_brows) {
				$save_account_brows = "false";
			}
			if (!$save_account_ipadd) {
				$save_account_ipadd = "false";
			}
			if (!$save_account_noses) {
				$save_account_noses = "false";
			}

			$content_posta="UPDATE `users` SET ". $new_pwd ."
								`user_timestamp_touch2` = NOW( ) ,
								`user_fullname` = '". mysql_real_escape_string($save_account_fullname) ."',
								`user_website` = '". mysql_real_escape_string($save_account_hp) ."',
								`user_email` = '". mysql_real_escape_string($save_account_email) ."',
								`user_language` = '". mysql_real_escape_string($save_account_txt_langa) ."',
								`user_country` = '". mysql_real_escape_string($save_account_country) ."',
								`user_timezone` = '". mysql_real_escape_string($save_account_timezone) ."',
								`user_occupation` = '". mysql_real_escape_string($save_account_accup) ."',
								`user_description` = '". mysql_real_escape_string($save_account_textarea_content) ."',
								`user_setting_multisession` = '". mysql_real_escape_string($save_account_multi) ."',
								`user_setting_browseragent` = '". mysql_real_escape_string($save_account_brows) ."',
								`user_setting_ipaddress` = '". mysql_real_escape_string($save_account_ipadd) ."',
								`user_setting_timeout` = '". mysql_real_escape_string($save_account_noses) . "'";
			
		        if (! preg_match('/^[\\w\\.\\+\\-=]+@[\\w\\.-]+\\.[\\w\\-]+$/',
		                         $save_account_email)) {
                		echo '<p><font color="#FF0000">The email address ' . 
				     htmlspecialchars($save_account_email) .
				     " is not valid.</font></p><br>\n";
			} else {
				$content_posta .= ", `user_email` = '". mysql_real_escape_string($save_account_email) . "'";
			}
			$content_posta .= " WHERE `user_id` ='". mysql_real_escape_string($roscms_intern_account_id) . "'";
			$content_post_lista=mysql_query($content_posta);
			subsys_update_user($roscms_intern_account_id);
		}
	}
	
	$query_content = mysql_query("SELECT * FROM users WHERE user_id = '".$roscms_intern_account_id."'") ;
	$result_content = mysql_fetch_array($query_content);
?>
  <form name="cms_content" method="post" action="<?php echo '?page=user&amp;sec=account&amp;sec2=save&amp;db_id='.$roscms_intern_account_id;
	 ?>">
    <table width="600" border="0" cellpadding="1" cellspacing="1">
      <tr bgcolor="#5984C3"> 
        <td width="15%" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Nick</strong></font></div></td>
        <td width="85%" bgcolor="#E2E2E2"> <div align="left"><font face="Arial, Helvetica, sans-serif"><?php echo $result_content['user_name']; ?></font></div></td>
      </tr>
      <tr bgcolor="#5984C3">
        <td bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Old 
            Pwd</strong></font></div></td>
        <td bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif">
          <input name="pwd1" type="password" id="pwd1" size="32" maxlength="32">
          * </font></td>
      </tr>
      <tr bgcolor="#5984C3"> 
        <td bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Password</strong></font></div></td>
        <td bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <input name="pwd2" type="password" id="pwd2" size="32" maxlength="32">
          * </font></td>
      </tr>
      <tr bgcolor="#5984C3"> 
        <td bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Pwd 
            again </strong></font></div></td>
        <td bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
          <input name="pwd3" type="password" id="pwd3" size="32" maxlength="32">
          * </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Fullname</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <input name="fullname" type="text" id="fullname" value="<?php echo $result_content['user_fullname']; ?>" size="50" maxlength="100">
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>E-Mail</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
          <input name="email" type="text" id="email" value="<?php echo $result_content['user_email']; ?>" size="50" maxlength="150">
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Homepage</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <input name="hp" type="text" id="hp" value="<?php echo $result_content['user_website']; ?>" size="50" maxlength="150">
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Occupation</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><p><font face="Arial, Helvetica, sans-serif"> 
            <input name="accup" type="text" id="accup" value="<?php echo $result_content['user_occupation']; ?>" size="50" maxlength="50">
            </font></p></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Description</strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><textarea name="textarea_content" cols="60" rows="5" id="textarea_content"><?php 
				echo $result_content['user_description'];
				?></textarea><br><font face="Arial, Helvetica, sans-serif">(255 character 
          limit; bb-code is allowed)</font> </td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Options</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <fieldset>
          <legend>&nbsp;Global Login System&nbsp;</legend>
          <input name="multi" type="checkbox" id="multi" value="true" <?php
			$user_account_multisession=$result_content['user_setting_multisession'];
			if ($user_account_multisession == "true") { echo "checked"; } ?>>
          multisession<br>
          <input name="brows" type="checkbox" id="brows" value="true" <?php
			$user_account_browseragent=$result_content['user_setting_browseragent'];
			if ($user_account_browseragent == "true") { echo "checked"; } ?>>
          check browseragent<br>
          <input name="ipadd" type="checkbox" id="ipadd" value="true" <?php
			$user_account_ipaddress=$result_content['user_setting_ipaddress'];
			if ($user_account_ipaddress == "true") { echo "checked"; } ?>>
          check ipaddress<br>
          <input name="noses" type="checkbox" id="noses" value="true" <?php
			$user_account_timeout=$result_content['user_setting_timeout'];
			if ($user_account_timeout == "true") { echo "checked"; } ?>>
          no session timeout 
          </fieldset>
          <br>
          <fieldset>
          <legend>&nbsp;Location&nbsp;</legend>
          <table width="100%" border="0">
            <tr> 
              <td width="100"><font face="Arial, Helvetica, sans-serif">Language:</font></td>
              <td><font face="Arial, Helvetica, sans-serif"> 
                <select id="select2" size="1" name="txt_langa" class="selectbox">
                  <?php 
				//echo $result_page['page_language'];
				$cmsros_intern_temp_lang_short=$result_content['user_language'];
				$sql_lang="SELECT * 
					FROM languages
					WHERE lang_level != '0' AND lang_id = '$cmsros_intern_temp_lang_short'
					ORDER BY 'lang_level' DESC Limit 1";
				$sql_query_lang=mysql_query($sql_lang);
				$myrow_lang=mysql_fetch_row($sql_query_lang); // Languages
			?>
                  <optgroup label="languages"> 
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
                </font></td>
            </tr>
            <tr> 
              <td><font face="Arial, Helvetica, sans-serif">Country:</font></td>
              <td><font face="Arial, Helvetica, sans-serif"><font face="Arial, Helvetica, sans-serif"> 
                <input name="country" type="text" id="country" value="<?php echo $result_content['user_country']; ?>">
                </font></font></td>
            </tr>
            <tr> 
              <td><font face="Arial, Helvetica, sans-serif">Timezone:</font></td>
              <td><font face="Arial, Helvetica, sans-serif"> 
                <input name="timezone" type="text" id="timezone2" value="<?php echo $result_content['user_timezone']; ?>">
                </font></td>
            </tr>
          </table>
          </fieldset>
          </font></td>
      </tr>
    </table>
    <br>
    <address>* only required if you want to set a new password</address>
    <p><input type="submit" name="Submit" value="Update"> 
  </form>
</div>

