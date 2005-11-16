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


	if ($rpm_sec2 && $rpm_sec2 != "") {
	
?>


<table width="460" border="0" cellpadding="0" cellspacing="0">
  <tr> 
    <td width="" colspan="3"><span class="contentSmallTitle"><?php echo $roscms_langres['User_Profil']; ?></span></td>
  </tr>
  <tr> 
    <td colspan="2" bgcolor="#F9F8F8"><table width="100%" border="0">
        <tr> 
          <td width="135" valign="top"><b><img src="images/userpic_default.jpg" width="124" height="165" border="1"></b></td>
          <td valign="top"><table width="100%" border="0" cellpadding="4">
              <tr> 
                <td width="84" valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Nick:</font></td>
                <td width="" colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"><b><?php
				$query_usraccount= @mysql_query("SELECT * 
								FROM `users` 
								WHERE `user_id` = ".mysql_escape_string($rpm_sec2)." LIMIT 0 , 1");
				$result_usraccount=@mysql_fetch_array($query_usraccount);

				
				 echo $result_usraccount['user_name']; ?></b></font></td>
              </tr>
              <tr> 
                <td valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Group(s):</font></td>
                <td colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"><?php 
				
		$profil_sadmin = false;
		$profil_admin = false;
		$profil_dev = false;
		$profil_team = false;
		$profil_trans = false;
		$profil_user = false;
						
		// check usergroup membership
		$roscms_login_usrgrp_member_query = @mysql_query("SELECT * 
															FROM `usergroup_members` 
															WHERE `usergroupmember_userid` = ".mysql_escape_string($rpm_sec2).";");		
		while($roscms_login_usrgrp_member_list = @mysql_fetch_array($roscms_login_usrgrp_member_query)) {
			switch ($roscms_login_usrgrp_member_list['usergroupmember_usergroupid']) { // Membership
				case "user": // normal user
					$profil_user = true;
					break;
				case "translator": // translator
				case "test": // RosCMS test user
					$profil_trans = true;
					break;
				case "moderator": // moderator
				case "mediateam": // mediateam (add here all other team's but not the 'web-team' => admin-group)
					$profil_team = true;
					break;
				case "developer": // developer
					$profil_dev = true;
					break;
				case "ros_admin": // administrator
					$profil_admin = true;
					break;
				case "ros_sadmin": // super administrator (if someone is member of this group he is also a member of the normal admin group!)
					$profil_admin = true;
					$profil_sadmin = true;
					break;
				default: // nothing
					break;
			}
		}

				
		if ($profil_user == true) echo "User";
		if ($profil_trans == true) echo "<br>Translator";
		if ($profil_team == true) echo "<br>Team";
		if ($profil_dev == true) echo "<br>Developer";
		if ($profil_admin == true) echo "<br>Administrator";
		if ($profil_sadmin == true) echo "<br>Super Administrator";

				
				?></font></td>
              </tr>
              <tr> 
                <td valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Realname:</font></td>
                <td colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"> 
                  <?php
				
				echo $result_usraccount['user_fullname'];

				
				?>
                  </font></td>
              </tr>
              <tr> 
                <td valign="top"><font size="2" face="Arial, Helvetica, sans-serif">E-Mail:</font></td>
                <td colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"><?php echo $result_usraccount['user_email']; ?></font></td>
              </tr>
              <tr> 
                <td valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Homepage: 
                  </font></td>
                <td colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"><?php echo $result_usraccount['user_website']; ?></font></td>
              </tr>
              <tr> 
                <td valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Language: 
                  </font></td>
                <td colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"><?php echo $result_usraccount['user_language']; ?></font></td>
              </tr>
              <tr> 
                <td valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Country:</font></td>
                <td colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"><?php echo $result_usraccount['user_country']; ?></font></td>
              </tr>
              <tr> 
                <td valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Timezone:</font></td>
                <td colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"><?php echo $result_usraccount['user_timezone']; ?></font></td>
              </tr>
              <tr> 
                <td valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Occupation:</font></td>
                <td colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"><?php echo $result_usraccount['user_occupation']; ?></font></td>
              </tr>
              <tr> 
                <td valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Description:</font></td>
                <td colspan="2" valign="top"> <font size="2" face="Arial, Helvetica, sans-serif"><?php 
					
					include("../editor/bbcode/cbparser.php");
					echo bb2html(stripslashes($result_usraccount['user_description']),'');
				
				?></font></td>
              </tr>
            </table> </td>
        </tr>
      </table> </tr>
  <tr bgcolor=#AEADAD> 
    <td><img src="images/line.gif" width="1" height="1"></td>
  </tr>
</table>

<?php
	}
	else {
		die("no parameter");
	}
?>