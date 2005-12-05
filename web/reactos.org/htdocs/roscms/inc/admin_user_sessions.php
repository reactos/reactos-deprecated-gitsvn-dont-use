<?php
    /*
    RosCMS - ReactOS Content Management System
    Copyright (C) 2005  Klemens Friedl <frik85@reactos.org>
	                    Ge van Geldorp <gvg@reactos.org>

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
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Sessions</span> 
  <ul>
    <li><strong>Sessions</strong></li>
  </ul>
<?php
	if($roscms_intern_usrgrp_sadmin == true) {
                $roscms_deletesesid="";
		if(array_key_exists("deletesesid", $_GET)) $roscms_deletesesid=$_GET["deletesesid"];
	
		if ($roscms_deletesesid != "") {
			$page_postc="DELETE FROM user_sessions WHERE usersession_id = '$roscms_deletesesid' LIMIT 1 ;";
			$page_post_listc=mysql_query($page_postc);
		}
	}
?> 
 
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="#5984C3"> 
      <td width="6%"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Action</strong></font></div></td>
      <td width="8%"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Account</strong></font></div></td>
      <td width="10%"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Expires</strong></font></div></td>
      <td width="15%">
<div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Browser</strong></font></div></td>
      <td width="10%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>IP 
          Address</strong></font></div></td>
    </tr>
    <?php

	$query_page = mysql_query("SELECT *
			FROM user_sessions
			ORDER BY 'usersession_timestamp_touch2' DESC ;") ;

	$farbe1=$roscms_intern_color1;
	$farbe2=$roscms_intern_color2;
	$zaehler="0";
	//$farbe="#CCCCC";
	
	while($result_page = mysql_fetch_array($query_page)) { // Pages
?>
    <tr> 
      <td valign="top" bgcolor="<?php
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
							 ?>" title="RosCMS action buttons:&#10;&#10;* View page&#10;* Delete page&#10;* Locked"> 
        <div align="left"><?php if($roscms_intern_usrgrp_sadmin == true) { ?>
          <a href="?page=admin&amp;sec=sessions&amp;deletesesid=<?php echo $result_page['usersession_id']; ?>"><img src="images/delete.gif" alt="Delete" width="19" height="18" border="0"></a> 
          <?php } ?>
        </div></td>
      <td valign="top" bgcolor="<?php echo $farbe; ?>"> <div align="left"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			$query_usra = mysql_query("SELECT user_id, user_name FROM users WHERE user_id = '".$result_page['usersession_user_id']."'") or die('DB error (show_sessions script)!');
			$result_usra = mysql_fetch_array($query_usra) or die('DB error (show_sessions script)');
			
			echo "<b>".$result_usra['user_name']."</b>"; ?></font></div></td>
      <td valign="top" bgcolor="<?php echo $farbe; ?>"><font face="Arial, Helvetica, sans-serif"><?php echo "<b>".$result_page['usersession_expires']."</b>"; ?></font></td>
      <td valign="top" bgcolor="<?php echo $farbe; ?>"> <div align="left"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['usersession_browseragent'];
		?>
          </font></div></td>
      <td valign="top" bgcolor="<?php echo $farbe; ?>"> <font face="Arial, Helvetica, sans-serif"> 
        <?php 
			echo $result_page['usersession_ipaddress'];
		?>
        </font></td>
    </tr>
    <?php	
	}	// end while
?>
  </table>
</div>
