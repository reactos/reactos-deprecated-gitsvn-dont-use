<?php
    /*
    RosCMS - ReactOS Content Management System
    Copyright (C) 2007  Klemens Friedl <frik85@reactos.org>

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
<div class="navTitle">RosCMS</div>
	<ol>
		<li><a href="?page=data">Pages</a></li>
		<li><a href="?page=admin&amp;sec=users">Users</a></li>
<?php
	global $rpm_page;

	if ($rpm_sec == "users" || $rpm_sec == "usergroups" || $rpm_sec == "usrgrpmbr" || $rpm_sec == "userhistory" || $rpm_sec == "sessions") { 
?>
		<li><a href="?page=admin&amp;sec=usergroups">&nbsp;- Usergroups</a></li>
		<li><a href="?page=admin&amp;sec=usrgrpmbr">&nbsp;- Group Members</a></li>
		<li><a href="?page=admin&amp;sec=sessions">&nbsp;- Sessions</a></li>		
		<li><a href="?page=admin&amp;sec=userhistory">&nbsp;- User History</a></li>
<?php
	}
?>
		<li><a href="?page=admin&sec=help">Help</a></li>
	</ol>
</div>
<p></p>