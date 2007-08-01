<?php
    /*
    RosCMS - ReactOS Content Management System
    Copyright (C) 2005-2007  Klemens Friedl <frik85@reactos.org>
	              2005       Ge van Geldorp <gvg@reactos.org>

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

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);

if (get_magic_quotes_gpc()) {
	die("ERROR: Disable 'magic quotes' in php.ini (=Off)");
}


//global $HTTP_GET_VARS; // set the Get var global


	//include("./inc/db/connect_db.inc.php");
	require("connect.db.php");


	// stop MySQL bug (http://dev.mysql.com/doc/refman/4.1/en/news-4-1-20.html):
	$SQLinjectionprevention ="SET GLOBAL sql_mode='NO_BACKSLASH_ESCAPES';";
	$SQLinjectionprevention_query=mysql_query($SQLinjectionprevention);




/*
 *	ReactOS CMS System - Version 2006-05-22
 *	
 *	(c) by Klemens Friedl <frik85>
 *	
 */

	if ( !defined('ROSCMS_SYSTEM') ) {
		define ("ROSCMS_SYSTEM", "Version 0.1"); // to prevent hacking activity
	}

	$roscms_branch = "website";

	// Global Vars:
	$rpm_page="user";
	$rpm_sec="";
	$rpm_sec2="";
	$rpm_sec3="";
	$rpm_site="";
	$rpm_opt="";
	$rpm_filt="";
	$rpm_filt2="";
	$rpm_lang="";
	$rpm_langid="";
	$rpm_forma="";
	$rpm_skin="";
	$rpm_debug="";
	$rpm_logo="";
	$rpm_db_id="";
	$rpm_newcontent="";
	$rpm_export="";
	
	$varlang="";
	$varw3cformat="";
	$varformat="";
	$page_active="";
	$page_active_set="";
	$rpm_lang_id="";
	$rpm_sort="";
	$roscms_intern_account_level="";
	$roscms_intern_login_check="false";
	
	// Central Color Settings:
	include("colors.php");
	
	$roscms_infotable="";
	
	// this vars will be removed soon
	$roscms_intern_login_check_username="";

	session_start();

	//if (array_key_exists("page", $HTTP_GET_VARS)) $rpm_page=$HTTP_GET_VARS["page"];
	if (array_key_exists("page", $_GET)) $rpm_page=htmlspecialchars($_GET["page"]);
	if (array_key_exists("sec", $_GET)) $rpm_sec=htmlspecialchars($_GET["sec"]);
	if (array_key_exists("sec2", $_GET)) $rpm_sec2=htmlspecialchars($_GET["sec2"]);
	if (array_key_exists("sec3", $_GET)) $rpm_sec3=htmlspecialchars($_GET["sec3"]);
	if (array_key_exists("site", $_GET)) $rpm_site=htmlspecialchars($_GET["site"]);
	if (array_key_exists("opt", $_GET)) $rpm_opt=htmlspecialchars($_GET["opt"]);
	if (array_key_exists("sort", $_GET)) $rpm_sort=htmlspecialchars($_GET["sort"]);
	if (array_key_exists("filt", $_GET)) $rpm_filt=htmlspecialchars($_GET["filt"]);
	if (array_key_exists("filt2", $_GET)) $rpm_filt2=htmlspecialchars($_GET["filt2"]);
	if (array_key_exists("lang", $_GET)) $rpm_lang=htmlspecialchars($_GET["lang"]);
	if (array_key_exists("langid", $_GET)) $rpm_lang_id=htmlspecialchars($_GET["langid"]);
	if (array_key_exists("forma", $_GET)) $rpm_forma=htmlspecialchars($_GET["forma"]);
	if (array_key_exists("skin", $_GET)) $rpm_skin=htmlspecialchars($_GET["skin"]);
	if (array_key_exists("debug", $_GET)) $rpm_debug=htmlspecialchars($_GET["debug"]);
	if (array_key_exists("logo", $_GET)) $rpm_logo=htmlspecialchars($_GET["logo"]);
	if (array_key_exists("db_id", $_GET)) $rpm_db_id=htmlspecialchars($_GET["db_id"]);
	if (array_key_exists("newcontent", $_GET)) $rpm_newcontent=htmlspecialchars($_GET["newcontent"]);
	if (array_key_exists("export", $_GET)) $rpm_export=htmlspecialchars($_GET["export"]);
	
	
	if (array_key_exists('HTTP_REFERER', $_SERVER)) $roscms_referrer=htmlspecialchars($_SERVER['HTTP_REFERER']);
	
	if(isset($_COOKIE['roscms_usrset_lang'])) {
		$roscms_usrsetting_lang=$_COOKIE["roscms_usrset_lang"];
	}
	else {
		$roscms_usrsetting_lang="";
	}


	$roscms_intern_dynamic="true";
	




	require("lang.php"); // lang code outsourced
	require("custom.php"); // custom on-screen information
	
	

	ini_set ('session.name', 'roscms');


	switch ($rpm_page) {
		default: // Frontpage
			require("inc/login.php");
			$rpm_page_title = $roscms_extern_brand ." - Home";
			include("inc/header.php");
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			include("inc/home.php"); // Content
			include("inc/body.php"); // Body
			break;
		case "home":
			require("inc/login.php");
			$rpm_page_title = $roscms_extern_brand ." - Home";
			include("inc/header.php");
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			include("inc/home.php"); // Content
			include("inc/body.php"); // Body
			break;
		case "data": // RosCMS v3 Interface
			require("inc/login.php");
			$rpm_page_title = $roscms_extern_brand ." ".$roscms_extern_version;
			include("inc/header.php");
			include("inc/data.php"); 
			include("inc/footer.php");
			break;
		case "data_out": // data to client
			require("inc/login.php");
			include("inc/tools.php"); 
			include("inc/data_export.php"); 
			break;
		/*case "data_in": // data to server
			require("inc/login.php");
			include("inc/data_import.php"); 
			break;
		case "admin": // Admin interface
			require("inc/login.php");
			if ($rpm_site == "") {
				$rpm_page_title="RosCMS - Admin Interface";
				include("inc/head.php");
				include("inc/structure.php");
				include("inc/admin.php"); 
				include("inc/body.php");
			}
			else {
				include("inc/admin.php"); 
			}
			break;*/
		case "user": // myReactOS
			require("inc/login.php");
			$rpm_page_title="myReactOS";
			$rpm_logo="myreactos";
			include("inc/head.php");
			include("inc/structure.php");
			include("inc/user.php"); 
			include("inc/body.php");
			break;
		/*case "dev": // developer interface
			require("inc/login.php");
			$rpm_page_title="Developer Inteface";
			include("inc/head.php");
			include("inc/structure.php");
			include("inc/dev.php"); 
			include("inc/body.php");
			break;
		case "team": // team interface (UI-Team, etc.)
			require("inc/login.php");
			$rpm_page_title="Team Inteface";
			include("inc/head.php");
			include("inc/structure.php");
			include("inc/team.php"); 
			include("inc/body.php");
			break;
		case "trans": // translator interface (UI-Team, etc.)
			require("inc/login.php");
			$rpm_page_title="Translator Inteface";
			include("inc/head.php");
			include("inc/structure.php");
			include("inc/translator.php"); 
			include("inc/body.php");
			break;*/
		case "login": // Login Page
			$rpm_page_title="Login";
			$rpm_logo="normal";
			include("inc/head.php");
			include("inc/structure.php");
			include("inc/user_login.php"); 
			include("inc/body.php");
			break;
		case "register": // Register Account Page
			$rpm_page_title="Register Account";
			$rpm_logo="normal";
			include("inc/head.php");
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			include("inc/structure.php");
			create_structure($rpm_page);
			include("inc/user_register.php"); 
			include("inc/body.php");
			break;
		case "getpwd": // Did you forget your password? -> request -> email
			$rpm_page_title="Did you forget your password?";
			$rpm_logo="normal";
			include("inc/head.php");
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			include("inc/structure.php");
			create_structure($rpm_page);
			include("inc/user_getpwd.php"); 
			include("inc/body.php");
			break;
		case "getpwd2": // -> link (email) -> new password
			$rpm_page_title="Activate new Password";
			$rpm_logo="normal";
			include("inc/head.php");
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			include("inc/structure.php");
			create_structure($rpm_page);
			include("inc/user_getpwd2.php"); 
			include("inc/body.php");
			break;
		case "logout": // Logout function
			include("inc/logout.php");
			break;
		case "noaccess":
			$rpm_page_title="No Access";
			$rpm_logo="normal";
			include("inc/head.php");
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			include("inc/structure.php");
			create_structure($rpm_page);
			include("inc/noaccess.php"); 
			include("inc/body.php");
			break;
		case "nopermission":
			require("inc/login.php");
			$rpm_page_title="No Permission";
			include("inc/head.php");
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			include("inc/structure.php");
			create_structure($rpm_page);
			include("inc/nopermission.php"); 
			include("inc/body.php");
			break;

			
		case "webstatus": // Website Status
			$rpm_page_title="Website Status";
			include("inc/head.php");
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			include("inc/structure.php");
			create_structure($rpm_page);
			include("inc/website_status.php"); 
			include("inc/body.php");
			break;

		case "404":
			$rpm_page_title="Page not found";
			include("inc/head.php");
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			include("inc/structure.php");
			create_structure($rpm_page);
			include("inc/404.php"); 
			include("inc/body.php");
			break;
			
		case "export":
			if ($rpm_export == "diff_content") {
				include("inc/export/diff_content.php"); 
			}
			break;
	}
?>
