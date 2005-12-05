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
		$seclog_section="roscms_admin_interface";
		$seclog_level="50";
		$seclog_reason="Hacking attempt: user_accounts.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}

	
?> 
<h1>myReactOS - User Accounts </h1>
<p>myReactOS user accounts sorted by account name</p>
  
<?php

// Setting:
$roscms_intern_items_per_page = 50;


$roscms_SET_curpos = "";
if (array_key_exists("curpos", $_GET)) $roscms_SET_curpos=htmlspecialchars($_GET["curpos"]);
if (!$roscms_SET_curpos) {
	$roscms_SET_curpos = 0;
}

$query_count_cat=mysql_query("SELECT COUNT('user_id') 
								FROM `users` 
								WHERE `user_account_enabled` = 'yes'
								AND `user_account_hidden` = 'no'
								ORDER BY `user_name` ASC ;");	
$result_count_cat = mysql_fetch_row($query_count_cat);

	echo "<p align='center'>";
	$j=0;
	for ($i=0; $i < $result_count_cat[0]; $i += $roscms_intern_items_per_page) {
		$j++;
		if ($roscms_SET_curpos == $i) {
			echo "<b>".$j."</b> ";
		}
		else {
			echo "<a href='?page=user&amp;sec=accountlist&amp;curpos=".$i."'>".$j."</a> ";
		}
	}
	$j=0;
	echo "</p>";

?> 
<table width="100%" border="0" cellpadding="1" cellspacing="1">
  <tr bgcolor="#5984C3"> 
    <td bgcolor="#5984C3"> <div align="center"></div>
    <div align="center"><font color="<?php echo $roscms_intern_color0; ?>" face="Arial, Helvetica, sans-serif"><strong>Nick</strong></font></div></td>
    <td> <div align="center"><font color="<?php echo $roscms_intern_color0; ?>" face="Arial, Helvetica, sans-serif"></font></div>
      <div align="center"><font color="<?php echo $roscms_intern_color0; ?>" face="Arial, Helvetica, sans-serif"><strong>Fullname</strong></font></div></td>
    <td bgcolor="#5984C3"> <div align="center"><font color="<?php echo $roscms_intern_color0; ?>" face="Arial, Helvetica, sans-serif"><strong>Occupation</strong></font></div>
      <div align="center"><font color="<?php echo $roscms_intern_color0; ?>" face="Arial, Helvetica, sans-serif"></font></div>
      <div align="center"></div></td>
    <td width="13%"> <div align="center"><font color="<?php echo $roscms_intern_color0; ?>" face="Arial, Helvetica, sans-serif"><strong> 
        Country</strong></font></div></td>
    <td width="10%"> <div align="center"><font color="<?php echo $roscms_intern_color0; ?>" face="Arial, Helvetica, sans-serif"><strong>Lang</strong></font></div></td>
  </tr>
  <?php

		$query_page = mysql_query("SELECT * 
									FROM `users` 
									WHERE `user_account_enabled` = 'yes'
									AND `user_account_hidden` = 'no'
									ORDER BY `user_name` ASC
									LIMIT " . $roscms_SET_curpos . " , " . $roscms_intern_items_per_page . " ;") ;

	$color1=$roscms_intern_color1;
	$color2=$roscms_intern_color2;
	$zaehler="0";
	//$color="#CCCCC";
	
	while($result_page = mysql_fetch_array($query_page)) { // users
?>
  <tr> 
    <td valign="middle" bgcolor="<?php
								$zaehler++;
								if ($zaehler == "1") {
									echo $color1;
									$color = $color1;
								}
								elseif ($zaehler == "2") {
									$zaehler="0";
									echo $color2;
									$color = $color2;
								}
							 ?>"> 
      <div align="center"></div>
      <div align="center"></div>
      <font face="Arial, Helvetica, sans-serif"> 
      <?php 
			echo "<a href='?page=user&sec=profil&sec2=".$result_page['user_id']."'>".$result_page['user_name']."</a>";
		?>
      </font></td>
    <td valign="middle" bgcolor="<?php echo $color; ?>"> 
      <div align="center"></div>
      <font face="Arial, Helvetica, sans-serif"> 
      <?php 
			echo $result_page['user_fullname'];
		?>
      </font></td>
    <td valign="middle" bgcolor="<?php echo $color; ?>"><font face="Arial, Helvetica, sans-serif"> 
      <?php 
			echo substr(htmlentities($result_page['user_occupation'], ENT_QUOTES), 0, 10);
		?>
      </font> <div align="right"><font face="Arial, Helvetica, sans-serif"> </font></div>
      <div align="center"><font face="Arial, Helvetica, sans-serif"> </font></div></td>
    <td width="13%" valign="middle" bgcolor="<?php echo $color; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
        <?php 
			echo $result_page['user_country'];;
		?>
        </font></div></td>
    <td width="10%" valign="middle" bgcolor="<?php echo $color; ?>"> <div align="center"><font face="Arial, Helvetica, sans-serif"> 
        <?php 
			echo $result_page['user_language'];
		?>
        </font></div></td>
  </tr>
  <?php	
	}	// end while
?>
</table>
<p align="center"><b><?php

	echo ($roscms_SET_curpos+1)." to ";

	if (($roscms_SET_curpos + $roscms_intern_items_per_page) > $result_count_cat[0]) {
		echo $result_count_cat[0];
	}
	else {
		echo ($roscms_SET_curpos + $roscms_intern_items_per_page);
	}
		
	echo " of ".$result_count_cat[0]; 
	
?></b></p>

