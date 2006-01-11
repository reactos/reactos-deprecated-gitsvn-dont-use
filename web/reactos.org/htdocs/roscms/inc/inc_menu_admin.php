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
<div class="navTitle">Admin Interface</div>
	<ol>
		<li><a href="?page=admin&amp;sec=generate">Page Generator</a></li>
<?php
	if ($rpm_sec == "" || $rpm_sec == "pages" || $rpm_sec == "content" || $rpm_sec == "inctext" || $rpm_sec == "dyncontent" || $rpm_sec == "languages" || $rpm_sec == "generate") {
?>
		<li><a href="?page=admin&sec=pages&sec2=view">&nbsp;- Pages</a></li>
		<li><a href="?page=admin&sec=content&sec2=view">&nbsp;- Content</a></li>
		<li><a href="?page=admin&amp;sec=inctext&amp;sec2=view">&nbsp;- Include Text</a></li>
		<li><a href="?page=admin&amp;sec=dyncontent&amp;sec2=view">&nbsp;- Dynamic Content</a></li>
		<li><a href="?page=admin&sec=languages&sec2=view">&nbsp;- Languages</a></li>
<?php
	}
?>
		<li><a href="?page=admin&amp;sec=users">User Accounts</a></li>
<?php
	if ($rpm_sec == "users" || $rpm_sec == "usergroups" || $rpm_sec == "usrgrpmbr" || $rpm_sec == "userhistory" || $rpm_sec == "sessions") { 
?>
		<li><a href="?page=admin&amp;sec=usergroups">&nbsp;- Usergroups</a></li>
		<li><a href="?page=admin&amp;sec=usrgrpmbr">&nbsp;- Group Members</a></li>
		<li><a href="?page=admin&amp;sec=sessions">&nbsp;- Sessions</a></li>		
		<li><a href="?page=admin&amp;sec=userhistory">&nbsp;- User History</a></li>
<?php
	}
?>
		<li><a href="?page=admin&amp;sec=subsys">Subsystems</a></li>
<?php
	if ($rpm_sec == "subsys") { 
?>
		<li><a href="?page=admin&amp;sec=subsys&amp;sec2=rosdb">&nbsp;- RosDB</a></li>
		<li><a href="?page=admin&amp;sec=subsys&amp;sec2=forum">&nbsp;- phpbb</a></li>
		<li><a href="?page=admin&amp;sec=subsys&amp;sec2=bugdb">&nbsp;- bugzilla</a></li>
		<li><a href="?page=admin&amp;sec=subsys&amp;sec2=wiki">&nbsp;- MediaWiki</a></li>
<?php
	}
?>
		<li><a href="?page=admin&amp;sec=stats">Statistics</a></li>
		<li><a href="?page=admin&amp;sec=seclog&amp;sec2=view">Security Logs</a></li>
		<li><a href="?page=admin&sec=help">Documentation</a></li>
	</ol>
</div>
<p></p>