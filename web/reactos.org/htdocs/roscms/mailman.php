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

if ( !defined('ROSCMS_SYSTEM') ) {
	define ("ROSCMS_SYSTEM", "Version 0.1"); // to prevent hacking activity
}


$rpm_page_title = "ReactOS - Mailing lists";
$roscms_langres = 'en';
$rpm_lang = 'en';
$rpm_lang_session = 'en';


include("inc/head.php"); // Head
create_head($rpm_page_title, $rpm_logo, 'en'/*$roscms_langres*/);
include("inc/structure.php");  // Layout-Structure
create_structure($rpm_page);
//include("inc/home.php"); // Content
include("inc/body.php"); // Body

?>
