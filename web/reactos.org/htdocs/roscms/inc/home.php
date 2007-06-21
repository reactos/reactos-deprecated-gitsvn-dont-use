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
<?php require("inc/data_menu.php"); ?>
<p>&nbsp;</p>
<h2>Welcome</h2>
<?php
	if (strpos($roscms_security_memberships, "|translator|") === false) {
		//
	}
	else {
		?>
		<h4>RosCMS v3 Information</h4>
		<p>
			We are currently testing a new version 3 of our Content Management System RosCMS on the ReactOS-Website.<br><br>
			
			If you want to edit something, please use the old interfaces until RosCMS v3 is fully ready:
			<ul>
				<?php 
					if ($roscms_intern_usrgrp_trans == true) { ?><li><a href="?page=trans">Translator Interface</a></li><?php }
					if ($roscms_intern_usrgrp_team == true) { ?><li><a href="?page=team">Team Interface</a></li><?php }
					if ($roscms_intern_usrgrp_dev == true) { ?><li><a href="?page=dev">Developer Interface</a></li><?php }
					if ($roscms_intern_usrgrp_admin == true) { ?><li><a href="?page=admin">Admin Interface</a></li><?php }
				?>
			</ul>
		</p>
		
		<h4>Translator Information</h4>
		<p>Language maintainer contact information [...]</p>
		<?php
	}
?>
<p>&nbsp;</p>
<p>&nbsp;</p>


