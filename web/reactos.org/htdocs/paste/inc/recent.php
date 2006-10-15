<?php

    /*
    ReactOS Paste Service
    Copyright (C) 2006  Klemens Friedl <frik85@reactos.org>

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
<h1>Recent Pastes</h1> 
<h2>Recent Pastes</h2>
<p>Pastes expire by default after 7 days, if desired even earlier. </p>
<table width="100%" border="0" cellpadding="1" cellspacing="1">
  <tr bgcolor="#5984C3">
    <td width="25%" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Date</strong></font></div></td>
    <td width="10%" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Language</strong></font></div></td>
    <td width="25%" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Nick</strong></font></div></td>
    <td width="40%" bgcolor="#5984C3"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Description</strong></font></div></td>
  </tr>
<?php

	$farbe1="#E2E2E2";
	$farbe2="#EEEEEE";
	$zaehler="0";
	
	$query_content = mysql_query("SELECT * 
									FROM `paste_service` 
									WHERE `paste_days` <=7
									AND `paste_public` = 1
									ORDER BY `paste_datetime` DESC 
									LIMIT 0 , 30 ;") ;	
									
	while($result_content = mysql_fetch_array($query_content)) { 
		//echo "<p>".stripDate($result_content['paste_datetime'])."</p>";
		if (compareDate((date('Y')."-".date('m')."-".date('d')),stripDate($result_content['paste_datetime'])) > 7) {
			//echo "break!";
			break;
		}
		if (compareDate((date('Y')."-".date('m')."-".date('d')),stripDate($result_content['paste_datetime'])) <= $result_content['paste_days']) {

?>
  <tr>
    <td valign="middle" bgcolor="<?php
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
							 ?>"><div align="left"><font size="2" face="Verdana, Arial, Helvetica, sans-serif"><?php echo $result_content['paste_datetime']; ?></font></div></td>
    <td valign="middle" bgcolor="<?php echo $farbe; ?>"><div align="left"><font size="2" face="Verdana, Arial, Helvetica, sans-serif"><?php echo $result_content['paste_lang']; ?></font></div></td>
    <td valign="middle" bgcolor="<?php echo $farbe; ?>"><font size="2" face="Verdana, Arial, Helvetica, sans-serif">
      <div align="left"><?php echo $result_content['paste_nick']; ?>
      </div></td>
    <td valign="middle" bgcolor="<?php echo $farbe; ?>" ><div align="left"><font size="2" face="Verdana, Arial, Helvetica, sans-serif"><a href="<?php echo $ros_paste_SET_path_ex . $result_content['paste_id']."/"; ?>">
      <?php 
	
		if ($result_content['paste_desc']) {
			echo $result_content['paste_desc'];
		}
		else {
			echo "No description";
		}
	
	 ?>
    </a></font></div></td>
  </tr>
  <?php	
		}
	}	// end while
?>
</table>
<p>&nbsp;</p>
