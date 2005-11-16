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
?> <div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Pages - Delete</span> 
<ul>
<li><strong><a href="?page=admin&sec=pages&sec2=view">Pages</a></strong> 
  <ul>
	<li>Delete page</li>
  </ul>
</li>
</ul>

<?php 
	if ($roscms_intern_account_level==100) {
		$spdas="DELETE FROM `pages` WHERE `page_id` = $rpm_db_id LIMIT 1;";
		$spdaslists=mysql_query($spdas);
	
		echo "<p>The page (id='".$rpm_db_id."') has been deleted!</p>";
	}
	else {
		echo "<p>No access!</p>"; ?>
		<script language="JavaScript">
			<!--
				window.setTimeout('window.location.href="?page=noaccess"','1')
			-->
		</script>
<?php
	}
?>
?>
<p><a href="<?php echo $HTTP_REFERER; ?>">Back</a></p>
</div>
