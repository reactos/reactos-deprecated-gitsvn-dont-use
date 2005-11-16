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
	$seclog_reason="Hacking attempt: admin_subsys_deluser.php";
	define ("ROSCMS_SYSTEM", "Hacking attempt");
	include('securitylog.php'); // open security log
	die("Hacking attempt");
}

require_once("subsys_bugzilla.php");
require_once("subsys_phpbb.php");
require_once("subsys_wiki.php");

?>

<div class="contentSmall"> <span class="contentSmallTitle">Admin Interface - Delete a user</span> 
<?php
$can_delete = TRUE;
if (! subsys_wiki_can_delete(1602))
  {
    $can_delete = FALSE;
  }

if (! $can_delete)
  {
    echo "<br>\nCan't delete this user<br>\n";
  }
subsys_wiki_can_delete(4);
subsys_wiki_can_delete(48);
subsys_wiki_can_delete(71);
subsys_wiki_can_delete(73);
subsys_wiki_can_delete(84);
subsys_wiki_can_delete(93);
subsys_wiki_can_delete(107);
subsys_wiki_can_delete(110);
subsys_wiki_can_delete(115);
subsys_wiki_can_delete(116);
subsys_wiki_can_delete(123);
subsys_wiki_can_delete(1020);
subsys_wiki_can_delete(1098);
subsys_wiki_can_delete(1403);
subsys_wiki_can_delete(1422);
subsys_wiki_can_delete(1430);
subsys_wiki_can_delete(1560);
subsys_wiki_can_delete(1562);
subsys_wiki_can_delete(1565);
subsys_wiki_can_delete(1566);
subsys_wiki_can_delete(1570);
subsys_wiki_can_delete(1573);
subsys_wiki_can_delete(1575);
subsys_wiki_can_delete(1578);
subsys_wiki_can_delete(1591);
subsys_wiki_can_delete(1602);
subsys_wiki_can_delete(1604);
subsys_wiki_can_delete(1610);
subsys_wiki_can_delete(1611);
subsys_wiki_can_delete(1613);
subsys_wiki_can_delete(1624);
subsys_wiki_can_delete(1629);
subsys_wiki_can_delete(1631);
subsys_wiki_can_delete(1643);
subsys_wiki_can_delete(1652);
subsys_wiki_can_delete(1656);
subsys_wiki_can_delete(1657);
subsys_wiki_can_delete(1869);
?>
</div>
