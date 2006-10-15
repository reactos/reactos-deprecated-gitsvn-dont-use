<?php
    /*
    RosCMS - ReactOS Content Management System
    Copyright (C) 2005  Klemens Friedl <frik85@reactos.org>
	                    Ge van Geldorp <gvg@reactos.org>

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
	include("connect.db.php");


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
	


// Language detection
function check_lang($lang)
{
	if (preg_match('/^([a-zA-Z]+)(-[a-zA-Z]+)?$/', $lang, $matches)) {
		$checked_lang = strtolower($matches[1]);
		switch($checked_lang) {
		case 'de':
		case 'en':
		case 'fr':
		case 'ru':
		case 'es':
		case 'it':
		case 'hu':
		case 'sv':
		case 'lt':
		case 'nl':
		case 'pl':
		case 'no':
		case 'da':
		case 'id':
		case 'zh':
			break;
		default:
			$checked_lang = '';
		}
	}
	else if ($lang == '*') {
		$checked_lang = 'en';
	}
	else {
		$checked_lang = '';
	}

	return $checked_lang;
}

if ($rpm_lang == '' && isset($_COOKIE['roscms_usrset_lang'])) {
	$rpm_lang = $_COOKIE['roscms_usrset_lang'];
	if (substr($rpm_lang, -1) == '/') {
		$rpm_lang = substr($rpm_lang, strlen($rpm_lang) - 1);
	}
	$rpm_lang = check_lang($rpm_lang);
}

if ($rpm_lang == '') {
	/* After parameter and cookie processing, we still don't have a valid
           language. So check whether the HTTP Accept-language header can
           help us. */
	$accept_language = $_SERVER['HTTP_ACCEPT_LANGUAGE'];
	$best_q = 0;
	while (preg_match('/^\s*([^,]+)((,(.*))|$)/',
	                  $accept_language, $matches)) {
		$lang_range = @$matches[1];
		$accept_language = @$matches[4];
		if (preg_match('/^(([a-zA-Z]+)(-[a-zA-Z]+)?)(;q=([0-1](\.[0-9]{1,3})?))?/',
		               $lang_range, $matches)) {
			$lang = check_lang($matches[1]);
			if ($lang != '') {
				$q = @$matches[5];
				if ($q == "") {
					$q = 1;
				}
				else {
					settype($q, 'float');
				}
				if ($best_q < $q) {
					$rpm_lang = $lang;
					$best_q = $q;
				}
			}
		}
	}
}
if ($rpm_lang == '') {
	/* If all else fails, use the default language */
	$rpm_lang = check_lang('*');
}

$roscms_page_lang = $rpm_lang . '/';
$rpm_lang_session = $rpm_lang . '/';


if (isset($_COOKIE['roscms_usrset_lang']) || isset($_REQUEST['lang'])) {
	/* Delete an existing cookie (if any) which uses the full hostname */
	setcookie('roscms_usrset_lang', '', -3600);
	/* Add cookie using just the domain name */
	require_once('inc/utils.php');
	setcookie('roscms_usrset_lang', $rpm_lang, time() + 5 * 30 * 24 * 3600,
	          '/', cookie_domain());
}
	
/*	// Language detection:
	if ($rpm_lang == "") {
		if ($roscms_usrsetting_lang != "") {
			$rpm_lang = substr($roscms_usrsetting_lang, 0, 2);
			$rpm_lang_session= $roscms_usrsetting_lang;
		}
		else {
			if (isset($roscms_referrer) && $roscms_referrer != "") {
				$var = "";
				$var = strchr($roscms_referrer, "/en/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="en/";
					$rpm_lang="en";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/de/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="de/";
					$rpm_lang="de";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/fr/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="fr/";
					$rpm_lang="fr";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/ru/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="ru/";
					$rpm_lang="ru";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/es/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="es/";
					$rpm_lang="es";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/it/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="it/";
					$rpm_lang="it";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/hu/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="hu/";
					$rpm_lang="hu";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/lt/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="lt/";
					$rpm_lang="lt";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/nl/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="nl/";
					$rpm_lang="nl";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/pl/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="pl/";
					$rpm_lang="pl";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/no/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="no/";
					$rpm_lang="no";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/da/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="da/";
					$rpm_lang="da";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/id/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="id/";
					$rpm_lang="id";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/zh/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="zh/";
					$rpm_lang="zh";
				}
				$var = "";
				$var = strchr($roscms_referrer, "/ko/");				
				if ($var != "" AND $rpm_lang == "") {
					$rpm_lang_session="ko/";
					$rpm_lang="ko";
				}
				if ($rpm_lang == "") {
					$rpm_lang_session="en/";
					$rpm_lang="en";
				}
				setcookie("roscms_usrset_lang", $rpm_lang_session, time() + 60*60*24*30*12, "/");
			}
			else {
				$rpm_lang_session="en/";
				$rpm_lang="en";
				$varlang = $rpm_lang_session;
				setcookie("roscms_usrset_lang", $rpm_lang_session, time() + 60*60*24*30*12, "/");
			}
		}
	}*/

	require("inc/lang/en.php"); // preload the english language text
	require("inc/lang/".$rpm_lang.".php"); // load the and overwrite the language text

	/*
	// Format detection:
	if ($rpm_forma == "") {
		if ($_COOKIE['varw3cformat'] == "") {
			$var = "";
			$var = strchr($HTTP_REFERER, "/xhtml/");				
			if ($var != "") {
				$rpm_lang="xhtml";
				$varw3cformat = "xhtml/";
				$varformat = "html";
				session_register("varw3cformat");
				session_register("varformat");
			}
		}
		else {
			$rpm_forma = $_COOKIE['varw3cformat'];
		}
	}*/
	

	

	ini_set ('session.name', 'roscms');


	switch ($rpm_page) {
		case "home":
		default: // Frontpage
			require("inc/login.php");
			$rpm_page_title="RosCMS - Home";
			include("inc/head.php"); // Head
			create_head($rpm_page_title, $rpm_logo, $roscms_langres);
			include("inc/structure.php");  // Layout-Structure
			create_structure($rpm_page);
			include("inc/home.php"); // Content
			include("inc/body.php"); // Body
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
			break;
		case "user": // myReactOS
			require("inc/login.php");
			$rpm_page_title="myReactOS";
			$rpm_logo="myreactos";
			include("inc/head.php");
			include("inc/structure.php");
			include("inc/user.php"); 
			include("inc/body.php");
			break;
		case "dev": // developer interface
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
			break;
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

		case "generate_fast_secret": // Generate the static HTML pages (for direct link, e.g. http://www.reactos.org/roscms/?page=generate"); TODO check why this link doesn't work -> errors, etc.?
			//require("inc/login.php");
			include("inc/generate_page.php"); // static page generator
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
