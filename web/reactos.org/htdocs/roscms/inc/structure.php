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

	// To prevent hacking activity:
	if ( !defined('ROSCMS_SYSTEM') )
	{
		if ( !defined('ROSCMS_SYSTEM_LOG') ) {
			define ("ROSCMS_SYSTEM_LOG", "Hacking attempt");
		}
		$seclog_section="roscms_general";
		$seclog_level="50";
		$seclog_reason="Hacking attempt: structur.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}


function create_structure($page )
{
	include('roscms_config.php');

	global $roscms_intern_account_level;
	global $roscms_intern_login_check;
	global $roscms_intern_login_check_username;
	global $roscms_intern_account_group;
	global $roscms_intern_usrgrp_sadmin;
	global $roscms_intern_usrgrp_admin;
	global $roscms_intern_usrgrp_dev;
	global $roscms_intern_usrgrp_team;
	global $roscms_intern_usrgrp_trans;
	global $roscms_intern_usrgrp_user;
	global $roscms_langres;
	global $rpm_sec;
?>


<table border="0" width="100%" cellpadding="0" cellspacing="0">
  <tr valign="top"> 
    <td width="147" id="leftNav"> <div class="navTitle"><?php echo $roscms_langres['Navigation']; ?></div>
      <ol>
        
      <li><a href="<?php echo $roscms_intern_path_server; ?>?page=index"><?php echo $roscms_langres['Home']; ?></a></li>
        <li><a href="<?php echo $roscms_intern_path_server; ?>?page=about"><?php echo $roscms_langres['Info']; ?></a></li>
        <li><a href="<?php echo $roscms_intern_path_server; ?>?page=community"><?php echo $roscms_langres['Community']; ?></a></li>
        <li><a href="<?php echo $roscms_intern_path_server; ?>?page=dev"><?php echo $roscms_langres['Dev']; ?></a></li>
        <li><a href="<?php echo $roscms_intern_path_server."roscms/"; ?>?page=user"><?php echo $roscms_langres['myReactOS']; ?></a></li>
      </ol></div>
      <p></p>
<?php 

/*
	if ($page == "home" || $page == "trans" || $page == "team" || $page == "dev" || $page == "admin" || $page == "data") { // RosCMS menu
		include("inc/inc_menu_roscms.php");
	}
*/

	if ($page == "user") {
		require("./inc/inc_menu_user.php");
	}
	if ($page == "trans") {
		require("./inc/inc_menu_translator.php");
	}
	if ($page == "team") {
		require("./inc/inc_menu_team.php");
	}
	if ($page == "dev") {
		require("./inc/inc_menu_dev.php");
	}
	
	if ($page == "data" || $page == "admin") {
		require("./inc/menu_main.php");
	}


	if ($page != "404") {
		require("./inc/member_bar.php");
	}
	
?>
<div class="navTitle">Language</div>   
      <ol>
        <li> 
          <div align="center"> 
		  <?php 
		    $rpm_page="";
		  	$rpm_lang="";
			if (array_key_exists("page", $_GET)) $rpm_page=htmlspecialchars($_GET["page"]);
		  	if (array_key_exists("lang", $_GET)) $rpm_lang=htmlspecialchars($_GET["lang"]);
			
			if ($rpm_lang == '' && isset($_COOKIE['roscms_usrset_lang'])) {
				$rpm_lang = $_COOKIE['roscms_usrset_lang'];
				if (substr($rpm_lang, -1) == '/') {
					$rpm_lang = substr($rpm_lang, strlen($rpm_lang) - 1);
				}
				$rpm_lang = check_lang($rpm_lang);
			}

		  ?>
            <select id="select" size="1" name="select" class="selectbox" style="width:140px" onchange="window.open(this.options[this.selectedIndex].value,'_main')">
			<optgroup label="current language"> 
			  <?php 
			$query_roscms_lang = mysql_query("SELECT * 
														FROM `languages` 
														WHERE `lang_id` = '". mysql_real_escape_string($rpm_lang) ."' ;");
			$result_roscms_lang = mysql_fetch_array($query_roscms_lang);
			
			echo '<option value="#">'.$result_roscms_lang[1].'</option>';
              ?>
              </optgroup>
			  <optgroup label="most popular">
              <option value="<?php echo $roscms_intern_path_server.$roscms_intern_path_gererator; ?>?page=<?php echo $rpm_page; ?>&amp;lang=en">English</option>
              <option value="<?php echo $roscms_intern_path_server.$roscms_intern_path_gererator; ?>?page=<?php echo $rpm_page; ?>&amp;lang=de">Deutsch (German)</option>
              <option value="<?php echo $roscms_intern_path_server.$roscms_intern_path_gererator; ?>?page=<?php echo $rpm_page; ?>&amp;lang=fr">Franais (French)</option>
              <option value="<?php echo $roscms_intern_path_server.$roscms_intern_path_gererator; ?>?page=<?php echo $rpm_page; ?>&amp;lang=ru">Russian (Russian)</option>
              </optgroup>
            </select>
          </div>
        </li>
      </ol>
      <p></p>
      <div class="navTitle">Latest Update</div>
      <ol>
        <li> 
          <div align="center"><?php echo date("Y-m-d"); $zeit = localtime(time() , 1); echo " " . sprintf("%02d",$zeit['tm_hour']).":".sprintf("%02d",$zeit['tm_min']); ?></div>
        </li>
      </ol></div>
	  <p></p>
      </td>
	  
<!-- End of Navigation Bar -->
	  
	  <td id="content">
<?php
	if ($page == "admin" || $page == "home" || $page == "dev" || $page == "team" || $page == "trans") { 
		//
	}
	elseif ($page == "user") { 
?>
		<div class="contentSmall"> <span class="contentSmallTitle"><?php echo $roscms_langres['myReactOS']; ?></span> 
			  <p><?php echo $roscms_langres['myReactOS_description']; ?></p>
	    </div>
<?php
	}
	elseif ($page == "login" || $page == "noaccess" || $page == "register") { 
?>
		<div class="contentSmall"> <span class="contentSmallTitle">ReactOS Homepage - Global Login System</span> 
			  <p>The useful ReactOS Homepage Global Login System provide you with one login, access to myReactOS, wiki, forum and bugzilla.</p>
	    </div>
<?php
	}
} // End of function create_structure
?>
