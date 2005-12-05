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
	$seclog_reason="Hacking attempt: webtransstatus";
	define ("ROSCMS_SYSTEM", "Hacking attempt");
	include('securitylog.php'); // open security log
	die("Hacking attempt");
}
?>
<h1><a href="http://www.reactos.org/?page=dev">ReactOS Development</a> &gt; Website  Status</h1>
<h2>Website Status</h2>
<h3>Website Content Updates</h3>
<table cellpadding="1" cellspacing="1">
  <tr bgcolor="#5984C3">
    <td width="200">
    <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Title</strong></font></div></td>
    <td width="250"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Description</strong></font></div></td>
    <td width="150"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Filename</strong></font></div></td>
    <td width="150">
    <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Language</strong></font></div></td>
    <td width="150"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Date &amp; Time </strong></font></div></td>
  </tr>
  <?php

	$query_updates = mysql_query("SELECT * 
									FROM `content` 
									WHERE `content_active` = 1
									AND `content_visible` = 1
									AND `content_type` = 'default'
									ORDER BY `content_id` DESC 
									LIMIT 0 , 10 ;") ;
	$color="";
	$color1=$roscms_intern_color1;
	$color2=$roscms_intern_color2;
	$colorcounter="0";
	//$farbe="#CCCCC";
	
	while($result_updates = mysql_fetch_array($query_updates)) {
?>
  <tr>
    <td valign="middle" bgcolor="<?php
								$colorcounter++;
								if ($colorcounter == "1") {
									echo $color1;
									$color = $color1;
								}
								elseif ($colorcounter == "2") {
									$colorcounter="0";
									echo $color2;
									$color = $color2;
								}
							 ?>"><font face="Arial, Helvetica, sans-serif" size="2"><b>
      <?php 
							 
			$query_count_title=mysql_query("SELECT COUNT('page_id')
											FROM `pages` 
											WHERE `page_name` LIKE '".$result_updates['content_name']."' ;");	
			$result_count_title = mysql_fetch_row($query_count_title);
			
			if ($result_updates['content_lang']=="all") {
				$updates_lang = "en";
			}
			else {
				$updates_lang = $result_updates['content_lang'];
			}
			
			
			if ($result_count_title[0] == "0" || $result_count_title[0] == "") {
				echo $result_updates['content_name'];
			}
			else { 
				$query_lang_page_name_update = mysql_query("SELECT * 
															FROM `pages` 
															WHERE `page_name` = '". $result_updates['content_name'] ."'
															AND (`page_language` = 'all' OR `page_language` = 'en')
															AND `page_active` = 1
															AND `page_visible` = 1 ;") ;
				$result_lang_page_name_update = mysql_fetch_array($query_lang_page_name_update);
		
				echo $result_lang_page_name_update['page_title'];
			
			}
		?>
    </b></font></td>
    <td valign="middle" bgcolor="<?php echo $color; ?>"><font face="Arial, Helvetica, sans-serif" size="2">
      <?php 
							 
			if ($result_count_title[0] == "0" || $result_count_title[0] == "") {
				// temp
			}
			else { 

		
				echo $result_lang_page_name_update['page_description'];
			}
		?>
    </font></td>
    <td valign="middle" bgcolor="<?php echo $color; ?>"><font face="Arial, Helvetica, sans-serif" size="2"><b>
      <?php 
							 
			if ($result_count_title[0] == "0" || $result_count_title[0] == "") {
				echo $result_updates['content_name'];
			}
			else { 
				echo '<a href="../?page='.$result_updates['content_name'].'&amp;lang='.$updates_lang.'">'.$result_updates['content_name'].'</a>';
			}
		?>
    </b></font></td>
    <td valign="middle" bgcolor="<?php echo $color; ?>"><div align="center"><font face="Arial, Helvetica, sans-serif" size="2">
	<?php 
		$query_lang_name_update = mysql_query("SELECT * 
								FROM `languages` 
								WHERE `lang_id` = '". $updates_lang ."' ;") ;
		$result_lang_name_update = mysql_fetch_array($query_lang_name_update);

		echo $result_lang_name_update['lang_name'];
	?>
    </font></div></td>
    <td valign="middle" bgcolor="<?php echo $color; ?>"><div align="center"><font face="Arial, Helvetica, sans-serif" size="2">
        <?php 
		echo $result_updates['content_date'].' '.$result_updates['content_time'];
	?>
    </font></div></td>
  </tr>
  <?php	
	}	// end while
?>
</table>
<h3>Website Translation Status</h3>
<p>This page show the current website translation status.</p>
<table cellpadding="1" cellspacing="1">
  <tr bgcolor="#5984C3">
    <td width="200"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Title </strong></font></div></td>
    <td width="200">      <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>Filename</strong></font></div></td>
    <td width="100">      <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong>English</strong></font></div></td>
	<?php
		$query_lang_names = mysql_query("SELECT * 
									FROM `languages` 
									WHERE `lang_level` <=9
									ORDER BY `lang_level` DESC ;") ;
		while($result_lang_names = mysql_fetch_array($query_lang_names)) {
	?>
    <td width="100">
    <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $result_lang_names["lang_name"]; ?></strong></font></div></td>
	<?php
		}
	?>
  </tr>
  <?php

	$query_page = mysql_query("SELECT * 
								FROM `content` 
								WHERE `content_active` = 1
								AND `content_visible` = 1
								AND `content_type` = 'default'
								ORDER BY `content_name` ASC ;") ;
	$color="";
	$color1=$roscms_intern_color1;
	$color2=$roscms_intern_color2;
	$colorcounter="0";
	//$farbe="#CCCCC";
	
	while($result_page = mysql_fetch_array($query_page)) {
		if ($result_page['content_lang'] == "all") {
?>
  <tr>
    <td valign="middle" bgcolor="<?php
								$colorcounter++;
								if ($colorcounter == "1") {
									echo $color1;
									$color = $color1;
								}
								elseif ($colorcounter == "2") {
									$colorcounter="0";
									echo $color2;
									$color = $color2;
								}
							 ?>"><font face="Arial, Helvetica, sans-serif" size="2">
      <?php 
							 
			$query_count_title=mysql_query("SELECT COUNT('page_id')
											FROM `pages` 
											WHERE `page_name` LIKE '".$result_page['content_name']."' ;");	
			$result_count_title = mysql_fetch_row($query_count_title);
			if ($result_count_title[0] == "0" || $result_count_title[0] == "") {
				// temp
			}
			else { 
				//echo '<a href="../?page='.$result_page['content_name'].'">'.$result_page['content_name'].'</a>';
				$query_lang_page_name_trans = mysql_query("SELECT * 
															FROM `pages` 
															WHERE `page_name` = '". $result_page['content_name'] ."'
															AND (`page_language` = 'all' OR `page_language` = 'en') 
															AND `page_active` = 1
															AND `page_visible` = 1 ;") ;
				$result_lang_page_name_trans = mysql_fetch_array($query_lang_page_name_trans);
		
				echo "<b>".$result_lang_page_name_trans['page_title']."</b>";
			}
		?>
    </font></td>
    <td valign="middle" bgcolor="<?php echo $color; ?>"><font face="Arial, Helvetica, sans-serif" size="2"><?php 
							 
			$link_current_line = true;
			if ($result_count_title[0] == "0" || $result_count_title[0] == "") {
				echo $result_page['content_name'];
				$link_current_line = false;
			}
			else { 
				//echo '<a href="../?page='.$result_page['content_name'].'">'.$result_page['content_name'].'</a>';
				echo $result_page['content_name'];
				$link_current_line = true;
			}
		?></font></td>
    <td valign="middle" bgcolor="<?php echo $color; ?>"><div align="center"><font face="Arial, Helvetica, sans-serif" size="2"><?php 
		if ($link_current_line == "true") {
			echo '<a href="../?page='.$result_page['content_name'].'&amp;lang=en">done</a>';
		}
		else {
			echo "done";
		}
	?></font></div></td>
	<?php
		$query_lang_name = mysql_query("SELECT * 
									FROM `languages` 
									WHERE `lang_level` <=9
									ORDER BY `lang_level` DESC ;") ;
		while($result_lang_name = mysql_fetch_array($query_lang_name)) {
	?>
    <td valign="middle" bgcolor="<?php echo $color; ?>"><div align="center"><font face="Arial, Helvetica, sans-serif" size="2"><?php 

			$query_count_lang_item=mysql_query("SELECT COUNT('content_id')
												FROM `content` 
												WHERE `content_name` = '".$result_page['content_name']."'
												AND `content_lang` = '". $result_lang_name["lang_id"] ."'
												AND `content_active` = 1
												AND `content_visible` = 1
												AND `content_type` = 'default' ;");	
			$result_count_lang_item = mysql_fetch_row($query_count_lang_item);
			if ($result_count_lang_item[0] != "0" && $result_count_lang_item[0] != "") {
				if ($link_current_line == "true") {
					echo '<a href="../?page='.$result_page['content_name'].'&amp;lang='.$result_lang_name['lang_id'].'">done</a>';
				}
				else {
					echo "done";
				}
			}
	
	?></font></div></td>
	<?php	
		}
	?>
  </tr>
  <?php	
  		}
	}	// end while
?>
</table>
<p>All entries that are marked with &quot;done&quot; are NOT by all means completely translated!</p>
