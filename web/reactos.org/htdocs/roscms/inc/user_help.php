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
 <div class="contentSmall"> <span class="contentSmallTitle">myReactOS - Documentation</span> 
  <p>&lt;placeholder&gt;</p>
  <table border="0" cellspacing="0" cellpadding="0" width="650">
    <tr> 
      <td colspan="3"><span class="contentSmallTitle">Documentation Overview </span></td>
    </tr>
    <tr> 
      <td colspan="2" bgcolor="#F9F8F8"> <table width="650" border="0" cellpadding="4">
          <tr> 
            <td width="20"> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td width="300"> <div align="left"><font face="Arial, Helvetica, sans-serif"><a href="?page=user&sec=help&amp;sec2=overview"> 
                <?php
	if ($rpm_sec2 == "" || $rpm_sec2 == "overview") {
		?><strong>General Information</strong> 
                <?php } else { echo "General Information"; } ?>
                </a></font></div></td>
            <td width="10">&nbsp;</td>
            <td width="20"> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td width="300"><font face="Arial, Helvetica, sans-serif"><a href="?page=user&sec=help&amp;sec2=bbcode"> 
              <?php
	if ($rpm_sec2 == "bbcode") {
		?><strong>BB Code</strong> 
              <?php } else { echo "BB Code"; } ?>
              </a></font></td>
          </tr>
          <tr> 
            <td width="20"> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td width="300"> <div align="left"><font face="Arial, Helvetica, sans-serif"><a href="?page=user&sec=help&amp;sec2=account"><?php
	if ($rpm_sec2 == "account") {
		?><strong>Edit ccount data</strong><?php } else { echo "Edit ccount data"; } ?></a></font></div></td>
            <td width="10">&nbsp;</td>
            <td width="20">&nbsp;</td>
            <td width="300">&nbsp;</td>
          </tr>
          <tr> 
            <td> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td> <div align="left"><font face="Arial, Helvetica, sans-serif"><a href="?page=user&sec=help&amp;sec2=loginsystem"><?php
	if ($rpm_sec2 == "loginsystem") {
		?><strong>Global Login System</strong><?php } else { echo "Global Login System"; } ?></a></font></div></td>
            <td>&nbsp;</td>
            <td>&nbsp;</td>
            <td>&nbsp;</td>
          </tr>
        </table></tr>
    <tr bgcolor=#AEADAD> 
      <td><img src="images/line.gif" width="1" height="1"></td>
    </tr>
  </table>
<?php
	switch ($rpm_sec2) {
		case "bbcode":
			include("inc/user_help_bbcode.php"); 
			break;
		case "overview":
		default:
			include("inc/user_help_overview.php"); 
			break;
	}
?>	
  </div>