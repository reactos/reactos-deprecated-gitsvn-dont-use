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
		$seclog_reason="Hacking attempt: admin_secloc_edit.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}



	include("db/connect_db.inc.php"); // database connection script
	include("inc_accountlevel_check.php"); // check if the account is valid (login, etc.)
?> 
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Security 
  Log</span> 
  <ul>
    <li><strong><a href="?page=admin&sec=seclog&sec2=view">Security Log</a></strong> 
      <ul>
        <li>View log</li>
      </ul>
    </li>
  </ul>
  <?php

$query_page = mysql_query("SELECT * 
		FROM roscms_security_log
		WHERE roscms_sec_log_id = '$rpm_db_id'") ;

$result_page = mysql_fetch_array($query_page);
?>
    
  <table width="600" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="#5984C3"> 
      <td width="25%" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Section</strong></font></div></td>
      <td width="75%" bgcolor="#E2E2E2"> <div align="left"><?php echo $result_page['roscms_sec_log_section']; ?></div></td>
    </tr>
    <tr> 
      <td valign="top" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Priority</strong></font></div></td>
      <td valign="top" bgcolor="#EEEEEE"><div align="left"><font face="Arial, Helvetica, sans-serif"> 
          </font><font face="Arial, Helvetica, sans-serif"> </font><font face="Arial, Helvetica, sans-serif"><?php echo $result_page['roscms_sec_log_priority']; ?> 
          </font> </div></td>
    </tr>
    <?php
	if ($result_page['content_editor']!="richtext") { ?>
    <tr> 
      <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Reason</strong></font></div></td>
      <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"><?php echo $result_page['roscms_sec_log_reason']; ?> 
        </font></td>
    </tr>
    <tr> 
      <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Sec. 
          Guard</strong></font></div></td>
      <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"><?php echo $result_page['roscms_sec_log_user']; ?></font></td>
    </tr>
    <tr> 
      <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Account</strong></font></div></td>
      <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"><?php echo $result_page['roscms_sec_log_useraccount']; ?></font></td>
    </tr>
    <tr> 
      <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>IP</strong></font></div></td>
      <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"><?php echo $result_page['roscms_sec_log_userip']; ?></font></td>
    </tr>
    <tr> 
      <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Referrer</strong></font></div></td>
      <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"><?php echo $result_page['roscms_sec_log_referrer']; ?></font></td>
    </tr>
    <tr> 
      <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Date</strong></font></div></td>
      <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"><?php echo $result_page['roscms_sec_log_date']; ?></font></td>
    </tr>
    <tr> 
      <td valign="top" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Time</strong></font></div></td>
      <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"><?php echo $result_page['roscms_sec_log_time']; ?></font></td>
    </tr>
    <?php } ?>
  </table>

  <p>&nbsp;</p>
  <p><strong>More information:</strong></p>
  <p><font face="Arial, Helvetica, sans-serif"><?php echo $result_page['roscms_sec_log_text']; ?></font></p>
  <p>&nbsp;</p>
</div>
