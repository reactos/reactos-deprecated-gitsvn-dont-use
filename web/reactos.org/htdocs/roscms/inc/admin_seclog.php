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
	if ( !defined('ROSCMS_SYSTEM') OR !defined('ROSCMS_SYSTEM_ADMIN') )
	{
		if ( !defined('ROSCMS_SYSTEM_LOG') ) {
			define ("ROSCMS_SYSTEM_LOG", "Hacking attempt");
		}
		$seclog_section="roscms_admin_interface";
		$seclog_level="50";
		$seclog_reason="Hacking attempt: admin_secloc.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}



	include("db/connect_db.inc.php"); // database connection script
	include("inc_accountlevel_check.php"); // check if the account is valid (login, etc.)
?>
 
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Security 
  Log </span> 
  <ul>
    <li><strong>Security Log</strong></li>
  </ul>
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="#5984C3"> 
      <td width="10%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Action</strong></font></div></td>
      <td width="30%"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Section</strong></font></div></td>
      <td width="10%">
<div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Priority 
          </strong></font></div></td>
      <td width="35%"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Reason</strong></font></div></td>
      <td width="15%"> 
        <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Date</strong></font></div></td>
    </tr>
    <?php

	$query_page = mysql_query("SELECT * 
			FROM roscms_security_log
			ORDER BY 'roscms_sec_log_id' DESC") ;

	$farbe1=$roscms_intern_color1;
	$farbe2=$roscms_intern_color2;
	$zaehler="0";
	//$farbe="#CCCCC";
	
	while($result_page = mysql_fetch_array($query_page)) { // Pages
?>
    <tr> 
      <td width="10%" valign="top" bgcolor="<?php
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
        <div align="center"><a href="?page=admin&amp;sec=seclog&amp;sec2=edit&amp;db_id=<?php echo $result_page['roscms_sec_log_id']; ?>"><img src="images/view.gif" alt="View" width="19" height="18" border="0"></a> 
          <?php if($roscms_intern_account_level<=10) { ?>
          <img src="images/lock.gif" alt="Locked" width="19" height="18"> 
          <?php } ?>
        </div></td>
      <td width="30%" valign="top" bgcolor="<?php echo $farbe; ?>"> 
        <div align="left"><font face="Arial, Helvetica, sans-serif"> <?php echo "<b>".$result_page['roscms_sec_log_section']."</b>"; ?></font></div></td>
      <td width="10%" valign="top" bgcolor="<?php echo $farbe; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['roscms_sec_log_priority'];
		?>
          </font></div></td>
      <td width="35%" valign="top" bgcolor="<?php echo $farbe; ?>"> <font face="Arial, Helvetica, sans-serif"> 
        <?php 
			echo $result_page['roscms_sec_log_reason'];
		?>
        </font></td>
      <td width="15%" valign="top" bgcolor="<?php echo $farbe; ?>"> 
        <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['roscms_sec_log_date'];
		?>
          </font></div></td>
    </tr>
    <?php	
	}	// end while
?>
  </table>
<?php
	include("inc/inc_description_table.php");
?>
</div>
