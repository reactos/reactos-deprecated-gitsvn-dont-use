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
<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Languages</span> 
  <ul>
    <li><strong><a href="?page=admin&sec=languages&sec2=view">Languages</a></strong> 
      <ul>
        <li>View languages</li>
      </ul>
    </li>
  </ul>
  <?php

$query_page = mysql_query("SELECT * 
		FROM languages
		WHERE lang_id = '$rpm_db_id'") ;

$result_page = mysql_fetch_array($query_page);
?>
  <form name="cms_pages" method="post" action="<?php 
	if($roscms_intern_account_level<=20) { echo "#"; } else { 
	echo '?page=admin&amp;sec=languages&amp;sec2=save&amp;db_id='.$rpm_db_id;  } ?>">
    <table width="600" border="0" cellpadding="1" cellspacing="1">
      <tr bgcolor="#5984C3"> 
        <td width="25%" bgcolor="#5984C3"> 
          <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Language 
            ID </strong></font></div></td>
        <td width="75%" bgcolor="#E2E2E2"> 
          <div align="left"> 
            <input name="textfield_lang_a" type="text" id="textfield_lang_a" value="<?php echo $result_page['lang_id']; ?>" size="10" maxlength="10">
          </div></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="#5984C3"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Language</strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><div align="left"><font face="Arial, Helvetica, sans-serif"> 
            </font><font face="Arial, Helvetica, sans-serif"> 
            <input name="textfield_lang_b" type="text" id="textfield_lang_b" value="<?php echo $result_page['lang_name']; ?>" size="25" maxlength="100">
            </font><font face="Arial, Helvetica, sans-serif"> </font> </div></td>
      </tr>
    </table>
    <p> 
      <input type="submit" name="Submit" value="Save">
      <?php if($roscms_intern_account_level<=50) { ?>
      <img src="images/lock.gif" alt="Locked" width="19" height="18"> (you need 
      a higher account level to save the content to the database)</p>
    <?php } ?>
  </form>
  <p>&nbsp;</p>
</div>
