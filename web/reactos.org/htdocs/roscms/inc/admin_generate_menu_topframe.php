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
?><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>RosCMS Generator Menu</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<body bgcolor="#FFFFCC">
<table width="100%" border="0" align="center">
  <tr> 
    <td width="20%"><div align="center"><font size="2" face="Arial, Helvetica, sans-serif"><strong><a href="?page=admin" target="_parent">RosCMS - Generator</a></strong></font></div></td>
    <td width="80%"> 
      <div align="center">
        <p>| <?php 
			
	
	// Languages
	$sql_lang="SELECT * 
				FROM languages
				WHERE lang_level != '0'
				ORDER BY 'lang_level' DESC";
	$sql_query_lang=mysql_query($sql_lang);
	while($myrow_lang=mysql_fetch_row($sql_query_lang)) {
		echo '<a href="?page=admin&amp;sec=generator&amp;sec2=view&amp;site=index&amp;gen_lang='.$myrow_lang[0].'" target="mainFrame">'.$myrow_lang[1].'</a> | ';
	}
			
			?></p>
      </div>
    </td>
  </tr>
</table>
</body>
</html>
