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
	$seclog_reason="Hacking attempt: admin_subsys_fix.php";
	define ("ROSCMS_SYSTEM", "Hacking attempt");
	include('securitylog.php'); // open security log
	die("Hacking attempt");
}

require_once("subsys_bugzilla.php");
require_once("subsys_phpbb.php");
require_once("subsys_wiki.php");

?>

<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Fix Subsystem Consistency</span> 
<?php
if (isset($_GET['subsys']))
  {
    $subsys = $_GET['subsys'];
  }
else
  {
    $subsys = 'all';
  }
$fix_count = 0;
if ($subsys == 'bugzilla' || $subsys == 'all')
  {
    $fix_count += subsys_bugzilla_fix_all();
  }
if ($subsys == 'phpbb' || $subsys == 'all')
  {
    $fix_count += subsys_phpbb_fix_all();
  }
if ($subsys == 'wiki' || $subsys == 'all')
  {
    $fix_count += subsys_wiki_fix_all();
  }
?>
<h2>Total</h2>
<?php
if (0 == $fix_count)
  {
    echo "No problems fixed.<br/>\n";
  }
else if (1 == $fix_count)
  {
    echo "1 problem fixed.<br/>\n";
  }
else
  {
    echo "$fix_count problems fixed.<br/>\n";
  }
?>
</div>
