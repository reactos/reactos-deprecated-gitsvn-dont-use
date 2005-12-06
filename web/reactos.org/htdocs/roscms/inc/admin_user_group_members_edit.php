<?php
if ($rpm_sec2=="delete") {
?>
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Group Members - Delete Membership</span> 
  <ul>
    <li><strong><a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view">Group Members</a></strong> 
      <ul>
        <li>Delete Membership</li>
      </ul>
    </li>
  </ul>
<?php
		$spdas="DELETE FROM `usergroup_members` WHERE `usergroupmember_userid` = ".mysql_real_escape_string($rpm_db_id)." AND `usergroupmember_usergroupid` = '".mysql_real_escape_string($rpm_sec3)."' LIMIT 1;";
		$spdaslists=mysql_query($spdas);
		echo "<p>The group membership '".$rpm_sec3."' for the user '".$rpm_db_id."' has been deleted!</p>";
}
else {
?>

<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Group Members - <?php if ($rpm_sec2=="edit") { echo "Edit"; } elseif ($rpm_sec2=="add") { echo "Add"; } ?> Membership</span> 
  <ul>
    <li><strong><a href="?page=admin&amp;sec=usrgrpmbr&amp;sec2=view">Group Members</a></strong> 
      <ul>
        <li><?php if ($rpm_sec2=="edit") { echo "Edit"; } elseif ($rpm_sec2=="add") { echo "Add"; } ?> Membership</li>
      </ul>
    </li>
  </ul>
<?php

	$query_contenta = mysql_query("SELECT * FROM users WHERE user_id = '".$roscms_intern_account_id."'") ;

	$farbe="#E2E2E2";
	
	$result_contenta = mysql_fetch_array($query_contenta);
	
	if ($rpm_sec2=="save") {
		$save_account_member_usrid="";
		$save_account_member_usrgrp="";
		
		if (array_key_exists("usrid", $_POST)) $save_account_member_usrid=$_POST['usrid'];
		if (array_key_exists("usrgrp", $_POST)) $save_account_member_usrgrp=$_POST['usrgrp'];

		if ($save_account_member_usrgrp == "ros_sadmin" && $roscms_intern_usrgrp_sadmin != true) {
			die("Not possible! You will need a higher account membership level!");
		}
		if ($save_account_member_usrgrp == "ros_admin" && ($roscms_intern_usrgrp_sadmin != true || $roscms_intern_usrgrp_admin != true)) {
			die("Not possible! You will need a higher account membership level!");
		}		

		if ($save_account_member_usrid && $save_account_member_usrgrp && $rpm_sec3) {
			$content_posta="UPDATE `usergroup_members` SET `usergroupmember_usergroupid` = '". mysql_real_escape_string($save_account_member_usrgrp) ."'
								WHERE `usergroupmember_userid` =". mysql_real_escape_string($save_account_member_usrid) ."
								AND CONVERT( `usergroupmember_usergroupid` USING utf8 ) = '". mysql_real_escape_string($rpm_sec3) ."'
								LIMIT 1 ;";
			$content_post_lista=mysql_query($content_posta);
			echo "<p><b>saved</b></p>";
			$rpm_db_id = $save_account_member_usrid;
		}
	}
	if ($rpm_sec2=="insert") {
		$save_account_member_usrid="";
		$save_account_member_usrgrp="";
		$save_account_member_usridname="";
		
		if (array_key_exists("usrid", $_POST)) $save_account_member_usrid=$_POST['usrid'];
		if (array_key_exists("usridname", $_POST)) $save_account_member_usridname=$_POST['usridname'];
		if (array_key_exists("usrgrp", $_POST)) $save_account_member_usrgrp=$_POST['usrgrp'];
		
		if ($save_account_member_usridname == "1") {
			$accountinfo_query = @mysql_query("SELECT user_name, user_id FROM users WHERE user_name = '".mysql_real_escape_string($save_account_member_usrid)."'") or die('DB error (admin group members edit interface)!');
			$accountinfo_result = @mysql_fetch_array($accountinfo_query);
			if ($accountinfo_result['user_id']) {
				$save_account_member_usrid = $accountinfo_result['user_id'];
			}
			else {
				die("<blink>Username not found!</blink>");
			}
		}
		
		if ($save_account_member_usrgrp == "ros_sadmin" && $roscms_intern_usrgrp_sadmin != true) {
			die("Not possible! You will need a higher account membership level!");
		}
		if ($save_account_member_usrgrp == "ros_admin" && ($roscms_intern_usrgrp_sadmin != true || $roscms_intern_usrgrp_admin != true)) {
			die("Not possible! You will need a higher account membership level!");
		}
		
		if ($save_account_member_usrid && $save_account_member_usrgrp && $rpm_sec3) {
			$content_postb="INSERT INTO `usergroup_members` ( `usergroupmember_userid` , `usergroupmember_usergroupid` ) 
							VALUES (
							'". mysql_real_escape_string($save_account_member_usrid) ."', '". mysql_real_escape_string($save_account_member_usrgrp) ."') ;";
			$content_post_listb=mysql_query($content_postb);
			echo "<p><b>saved (".$save_account_member_usrid." -> ".$save_account_member_usrgrp.")</b></p>";
			$rpm_db_id = $save_account_member_usrid;
		}
	}
?>
  <form name="cms_content" method="post" action="?page=admin&amp;sec=usrgrpmbr&amp;sec2=<?php if ($rpm_sec2=="edit") { echo "save"; } elseif ($rpm_sec2=="add") { echo "insert"; } ?>&amp;sec3=<?php echo $rpm_sec3; ?>">
    <table width="600" border="0" cellpadding="1" cellspacing="1">
      <tr> 
        <td width="15%" valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>User 
            ID </strong></font></div></td>
        <td width="85%" valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
		<?php
			if ($rpm_db_id == "new") {
		?>
		  <input name="usrid" type="text" id="usrid" size="30" maxlength="50">
		  <input name="usridname" type="hidden" id="usridname" value="1">
		  <?php
			}
			else {
				$query_usra = mysql_query("SELECT user_id, user_name FROM users WHERE user_id = '". $rpm_db_id ."'") or die('DB error (admin membership script)!');
				$result_usra = mysql_fetch_array($query_usra); // or die('DB error (user_group_members_edit)');		  
				
				echo "<b>".$rpm_db_id."</b> (".$result_usra['user_name'].")"; ?> 
				<input name="usrid" type="hidden" id="usrid" value="<?php echo $rpm_db_id; ?>">
		<?php 
			}
		?>
        </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Usergroup</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <select id="usrgrp" size="1" name="usrgrp" class="selectbox">
            <?php 
				$sql_lang="SELECT * 
							FROM `usergroup_members` 
							WHERE `usergroupmember_userid` = ". $rpm_db_id ."
							AND `usergroupmember_usergroupid` = '". $rpm_sec3 ."'
							LIMIT 1 ";
				$sql_query_lang=mysql_query($sql_lang);
				$myrow_lang=mysql_fetch_array($sql_query_lang); // Languages
			?>
            <optgroup label="usergroups"> 
            <?php
			  
		// User Groups
		$sql_langa="SELECT * 
					FROM `usergroups` 
					ORDER BY `usrgroup_securitylevel` ASC ";
		$sql_query_langa=mysql_query($sql_langa);
		while($myrow_langa=mysql_fetch_array($sql_query_langa)) {
              echo '<option value="'.$myrow_langa['usrgroup_name_id'].'"';
			  //echo "<br>=>".$myrow_lang['usergroupmember_userid']." == ".$myrow_langa['usrgroup_name_id'];
			  if ($myrow_lang['usergroupmember_usergroupid'] == $myrow_langa['usrgroup_name_id']) {
					echo ' selected="selected"';
			  }
			  echo '>'.$myrow_langa['usrgroup_name'].'</option>';
		}
              ?>
            </optgroup>
          </select>
          </font></td>
      </tr>
    </table>
      <br />
      <input type="submit" name="Submit" value="Save">
  </form>
  <p>&nbsp;</p>
    <?php
	if ($rpm_db_id != "new") {

		echo "<p><b>".$result_usra['user_name']." is currently member of the following usergroups:</b></p><ul>";
	
		$query_page = mysql_query("SELECT * 
									FROM `usergroup_members` 
									WHERE `usergroupmember_userid` = $rpm_db_id 
									ORDER BY `usergroupmember_usergroupid` ASC ;") ;
		while($result_page = mysql_fetch_array($query_page)) {
			echo "<li>".$result_page['usergroupmember_usergroupid']."</li>";
		}
		echo "</ul>";
	 }
}
?>
</div>
