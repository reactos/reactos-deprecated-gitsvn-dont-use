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
<div class="navTitle"><?php echo $roscms_langres['myReactOS']; ?></div>
	<ol>
		<li><a href="?page=user"><?php echo $roscms_langres['Overview']; ?></a></li>
		<li><a href="?page=user&amp;sec=account&amp;sec2=edit"><?php echo $roscms_langres['Account_Edit']; ?></a></li>
		<li><a href="?page=user&amp;sec=accountlist"><?php echo $roscms_langres['Accountlist']; ?></a></li>
		<li><a href="?page=user&amp;sec=gls"><?php echo $roscms_langres['Login_System']; ?></a></li>
<?php
		/*
			<li><a href="?page=user&sec=help"><?php echo $roscms_langres['Documentation']; ?></a></li>
		*/
		if ($roscms_intern_usrgrp_sadmin == true ||
			$roscms_intern_usrgrp_admin == true ||
			$roscms_intern_usrgrp_dev == true ||
			$roscms_intern_usrgrp_team == true ||
			$roscms_intern_usrgrp_trans == true)
		{
?>
			<li><a href="?page=home">RosCMS Interface</a></li>
<?php
		}
?>
	</ol>
</div>
<p></p>
