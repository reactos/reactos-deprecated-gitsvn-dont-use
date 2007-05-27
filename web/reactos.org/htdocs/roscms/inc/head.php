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

	// To prevent hacking activity:
	if ( !defined('ROSCMS_SYSTEM') )
	{
		if ( !defined('ROSCMS_SYSTEM_LOG') ) {
			define ("ROSCMS_SYSTEM_LOG", "Hacking attempt");
		}
		$seclog_section="roscms_interface";
		$seclog_level="50";
		$seclog_reason="Hacking attempt: head.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		require('securitylog.php'); // open security log
		die("Hacking attempt");
	}
	
	// This page was generated in ...
	$roscms_gentime = microtime(); 
	$roscms_gentime = explode(' ',$roscms_gentime); 
	$roscms_gentime = $roscms_gentime[1] + $roscms_gentime[0]; 
	$roscms_pg_start = $roscms_gentime; 

function create_head($page_title, $logo, $roscms_langres)
{
	require_once('roscms_config.php');

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd">

<html lang="de">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=<?php echo $roscms_langres['charset']; ?>" />
	<title>ReactOS Homepage - <?php echo $page_title ; ?></title>
	<meta http-equiv="Pragma" content="no-cache" >
	<meta name="Copyright" content="ReactOS Foundation" >
	<meta name="generator" content="RosCMS" >
	<meta name="Content-language" content="<?php echo $roscms_langres['lang_code']; ?>">
	<meta name="Robots" content="noindex,nofollow" >
	<link rel="SHORTCUT ICON" href="../favicon.ico" >
	<link href="<?php echo $roscms_intern_path_server."roscms/"; ?>style.css" type="text/css" rel="stylesheet" />
	
<?php 
	if ($logo == "normal") {
?>
	<style type="text/css">
		#top {
			background: url('images/logo.jpg') no-repeat scroll 35px 0;
		}
	</style>
<?php
	}
	if ($logo == "myreactos") {
?>
	<style type="text/css">
		#top {
			background: url('images/logo_myreactos.jpg') no-repeat scroll 35px 0;
		}
	</style>
<?php
	}


?>
</head>
<body>
<div id="top">
  <div id="topMenu"> 
    <!-- 
       Use <p> to align things for links/lynx, then in the css make it
	   margin: 0; and use text-align: left/right/etc;.
   -->
    <p align="center"> 
		<a href="<?php echo $roscms_intern_path_server; ?>?page=index"><?php echo $roscms_langres['Home']; ?></a> <font color="#ffffff">|</font> 
		<a href="<?php echo $roscms_intern_path_server; ?>?page=about"><?php echo $roscms_langres['Info']; ?></a> <font color="#ffffff">|</font> 
		<a href="<?php echo $roscms_intern_path_server; ?>?page=community"><?php echo $roscms_langres['Community']; ?></a> <font color="#ffffff">|</font> 
		<a href="<?php echo $roscms_intern_path_server; ?>?page=dev"><?php echo $roscms_langres['Dev']; ?></a> <font color="#ffffff">|</font> 
		<a href="<?php echo $roscms_intern_path_server."roscms/"; ?>?page=user"><?php echo $roscms_langres['myReactOS']; ?></a> </p>
 </div>
</div>


<?php /*<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html><head><title>ReactOS Package Manager - Online</title>

<link rel="stylesheet" type="text/css" href="reactos.basic.css"><!-- reactos.css -->
<META content="ReactOS Package Manager - CMS System" name=GENERATOR>
<META content="Klemens Friedl" name=AUTHOR>
</head>  
<body>*/


require("./inc/inc_account_check.php");

?>

<!-- Start of Navigation Bar -->

<?php
} // End of function create_head
?>
