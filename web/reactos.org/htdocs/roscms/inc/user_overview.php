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
<table width="460" border="0" cellpadding="0" cellspacing="0">
  <tr> 
    <td width="" colspan="3"><span class="contentSmallTitle"><?php echo $roscms_langres['User_Profil']; ?></span></td>
  </tr>
  <tr> 
    <td colspan="2" bgcolor="#F9F8F8"><table width="100%" border="0">
        <tr> 
          <td width="15
		  " valign="top">&nbsp;</td>
          <td valign="top"><table width="100%" border="0" cellpadding="4">
              <tr> 
                <td width="84" valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Nick:</font></td>
                <td width="" colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"><b><?php echo $roscms_intern_login_check_username; ?></b></font></td>
              </tr>
              <tr> 
                <td valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Group(s):</font></td>
                <td colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"><?php 
				
		if ($roscms_intern_usrgrp_user == true) echo "User";
		if ($roscms_intern_usrgrp_trans == true) echo "<br>Translator";
		if ($roscms_intern_usrgrp_team == true) echo "<br>Team";
		if ($roscms_intern_usrgrp_dev == true) echo "<br>Developer";
		if ($roscms_intern_usrgrp_admin == true) echo "<br>Administrator";
		if ($roscms_intern_usrgrp_sadmin == true) echo "<br>Super Administrator";

				
				?></font></td>
              </tr>
              <tr> 
                <td valign="top"><font size="2" face="Arial, Helvetica, sans-serif">Realname:</font></td>
                <td colspan="2" valign="top"><font size="2" face="Arial, Helvetica, sans-serif"> 
                  <?php
				
				$query_usraccount= mysql_query("SELECT * 
												FROM `users` 
												WHERE `user_id` = ".$roscms_intern_account_id." LIMIT 1 ;");
				$result_usraccount=mysql_fetch_array($query_usraccount);
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
					
					//include("../editor/bbcode/cbparser.php");
					//echo @bb2html($result_usraccount['user_description'],'');
					echo $result_usraccount['user_description'];
				
				?></font></td>
              </tr>
            </table> </td>
        </tr>
      </table> </tr>
  <tr bgcolor=#AEADAD> 
    <td><img src="images/line.gif" width="1" height="1"></td>
  </tr>
</table>
