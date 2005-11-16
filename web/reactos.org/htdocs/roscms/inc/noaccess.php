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
	if ( !defined('ROSCMS_SYSTEM') )
	{
		if ( !defined('ROSCMS_SYSTEM_LOG') ) {
			define ("ROSCMS_SYSTEM_LOG", "Hacking attempt");
		}
		$seclog_section="roscms_interface";
		$seclog_level="50";
		$seclog_reason="Hacking attempt: noaccess.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}

?>


 
  <table border="0" cellspacing="0" cellpadding="0" width="320">
    <tr> 
      <td colspan="3"><span class="contentSmallTitle">ReactOS Global Login System</span></td>
    </tr>
    <tr> 
      <td colspan="2" bgcolor="#F9F8F8"> <table width="342" border="0" cellpadding="4">
          <tr> 
            <td width="20"> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td width="300"> <div align="left"><font size="2" face="Arial, Helvetica, sans-serif"> 
                <?php
	if ($rpm_sec == "standard") {
?>
                <strong>Standard Login</strong> 
                <?php } else { echo '<a href="?page=login&amp;sec=standard">Standard Login</a>'; } ?>
                </font></div></td>
          </tr>
          <tr> 
            <td width="20"> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td width="300"> <div align="left"><font size="2" face="Arial, Helvetica, sans-serif"> 
                <?php
	if ($rpm_sec == "security") {
?>
                <strong>High Security Login</strong> 
                <?php } else { echo '<a href="?page=login&amp;sec=security">High Security Login</a>'; } ?>
                </font></div></td>
          </tr>
          <tr> 
            <td> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td> <div align="left"><font size="2" face="Arial, Helvetica, sans-serif"> 
                <a href="?page=register">Register an Account</a> </font></div></td>
          </tr>
          <tr> 
            <td> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td> <div align="left"><font size="2" face="Arial, Helvetica, sans-serif"> 
                <a href="#">Help</a> </font></div></td>
          </tr>
        </table></tr>
    <tr bgcolor=#AEADAD> 
      <td><img src="images/line.gif" width="1" height="1"></td>
    </tr>
  </table><br>
<div class="contentSmall"><span class="contentSmallTitle">Login required</span> 
  <p>You have no access to this part of the homepage because you are not logged in!</p>
  <p>Please use the <a href="?page=login">login function</a> to get access or 
    <a href="?page=register">register an account</a> for free! </p>
  <p><a href="?page=login"><img src="../media/pictures/2005/myreactos_info.jpg" alt="ReactOS Homepage - Global Login System" width="500" height="242" border="0"></a><br>
    <a href="?page=login">ReactOS Homepage - Global Login System</a></p>
  <p>&nbsp;</p>
</div>