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
    <li><strong>Languages</strong></li>
  </ul>
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr bgcolor="#5984C3"> 
      <td width="10%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Action</strong></font></div></td>
      <td width="13%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Language 
          ID</strong></font></div></td>
      <td width="13%"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Language</strong></font></div></td>
      <td width="13%"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Language 
          Level</strong></font></div></td>
    </tr>
    <?php

	$query_page = mysql_query("SELECT * 
			FROM languages
			ORDER BY 'lang_name' ASC") ;

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
							 ?>" title="RosCMS action buttons:&#10;&#10;* View page&#10;* Delete page&#10;* Locked"> 
        <div align="left"><a href="?page=admin&amp;sec=languages&amp;sec2=edit&amp;db_id=<?php echo $result_page['lang_id']; ?>"><img src="images/view.gif" alt="View" width="19" height="18" border="0"></a> 
          <?php if($roscms_intern_account_level<=10) { ?>
          <img src="images/lock.gif" alt="Locked" width="19" height="18"> 
          <?php } ?>
          <?php if($roscms_intern_account_level==100) { ?>
          <a href="#"><img src="images/delete.gif" alt="Delete" width="19" height="18" border="0"></a> 
          <?php } ?>
        </div></td>
      <td width="13%" valign="top" bgcolor="<?php echo $farbe; ?>"><div align="left"><font face="Arial, Helvetica, sans-serif"> 
          <?php echo "<b>".$result_page['lang_id']."</b>"; ?></font></div></td>
      <td width="13%" valign="top" bgcolor="<?php echo $farbe; ?>"><div align="left"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['lang_name'];
		?>
          </font></div></td>
      <td width="13%" valign="top" bgcolor="<?php echo $farbe; ?>"> <font face="Arial, Helvetica, sans-serif"> 
        <?php 
			echo $result_page['lang_level'];
		?>
        </font></td>
    </tr>
    <?php	
	}	// end while
?>
  </table>
<?php
	include("inc/inc_description_table.php");
?>
</div>
