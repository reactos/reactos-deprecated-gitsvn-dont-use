<?php
    /*
    RosCMS - ReactOS Content Management System
    Copyright (C) 2005  Ge van Geldorp <gvg@reactos.org>

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
if ( !defined('ROSCMS_SYSTEM') OR !defined('ROSCMS_SYSTEM_ADMIN') )
{
	if ( !defined('ROSCMS_SYSTEM_LOG') ) {
		define ("ROSCMS_SYSTEM_LOG", "Hacking attempt");
	}
	$seclog_section="roscms_admin_interface";
	$seclog_level="50";
	$seclog_reason="Hacking attempt: admin_secloc.php";
	define ("ROSCMS_SYSTEM", "Hacking attempt");
	include('securitylog.php'); // open security log
	die("Hacking attempt");
}

require_once("subsys_bugzilla.php");
require_once("subsys_phpbb.php");
require_once("subsys_wiki.php");
require_once("subsys_blogs.php");

?>

<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Check Subsystem Consistency</span> 
<?php
$inconsistency_count = 0;
$inconsistency_count += subsys_bugzilla_check();
$inconsistency_count += subsys_phpbb_check();
$inconsistency_count += subsys_wiki_check();
$inconsistency_count += subsys_blogs_check();
?>
<h2>Total</h2>
<?php
$fix_url = htmlentities('?page=admin&sec=subsys&sec2=fix&subsys=all');
if (0 == $inconsistency_count)
  {
    echo "No problems found.<br/>\n";
  }
else if (1 == $inconsistency_count)
  {
    echo '<br />1 problem found. <a href="' . $fix_url .
         '">Fix this</a><br/>' . "\n";
  }
else
  {
    echo "<br />$inconsistency_count problems found." .
         ' <a href="' . $fix_url . '">Fix these</a><br/>' . "\n";
  }
?>
</div>
