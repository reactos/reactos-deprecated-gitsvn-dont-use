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
	$seclog_section="roscms_interface";
	$seclog_level="70";
	$seclog_reason="Hacking attempt: user_login.php";
	define ("ROSCMS_SYSTEM", "Hacking attempt");
	require('securitylog.php'); // open security log
	die("Hacking attempt");
}

require_once("utils.php");
require_once("subsys_login.php");
	
function create_login_page($message)
{
	global $rpm_page_title;
	global $rpm_logo;
	global $roscms_langres;
	global $rpm_page;
	global $rpm_sec;

	$random_string="";
	$random_string_security="";

	if ($rpm_sec == "security") { 
		$random_string_security = make_key();
		setcookie("roscms_sec_key", $random_string_security, 0, "/", cookie_domain());

		echo '<script language="JavaScript" src="js/md5.js"></script>';
	} 
	else {
		$random_string_security = "";
		setcookie("roscms_sec_key", $random_string_security, time() - 3600, "/", cookie_domain());
	}

	create_head($rpm_page_title, $rpm_logo, $roscms_langres);
	create_structure($rpm_page);

	$target_clean = '';
	if (isset($_REQUEST['target'])) {
		$target_tainted = $_REQUEST['target'];
		if (preg_match('/^(\/[a-zA-Z0-9!$%&,\'()*+\-.\/:;=?@_~]+)$/',
		    $target_tainted, $matches)) {
			$target_clean = $matches[1];
		}
	}
?>

<table border="0" cellspacing="0" cellpadding="0" width="320">
  <tr> 
      <td colspan="3"><span class="contentSmallTitle">ReactOS Global Login System</span></td>
    </tr>
    <tr> 
      <td colspan="2" bgcolor="#F9F8F8"> <table width="342" border="0" cellpadding="4">
          <tr> 
            <td width="20"> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td width="300"> <div align="left"><font size="2" face="Arial, Helvetica, sans-serif"> 
<?php
	if ($rpm_sec == "" || $rpm_sec == "standard") {
?>
                <strong>Standard Login</strong> 
<?php
	}
	else {
		echo '<a href="?page=login&amp;sec=standard';
		if ($target_clean != '') {
			echo '&amp;target=' . urlencode($target_clean);
		}
		echo '">Standard Login</a>';
	}
?>
                </font></div></td>
          </tr>
          <tr> 
            <td width="20"> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td width="300"> <div align="left"><font size="2" face="Arial, Helvetica, sans-serif"> 
<?php
	if ($rpm_sec == "security") {
?>
              <strong>High Security Login</strong> 
<?php
	}
	else {
		echo '<a href="?page=login&amp;sec=security';
		if ($target_clean != '') {
			echo '&amp;target=' . urlencode($target_clean);
		}
		echo '">High Security Login</a>';
	}
?>
              </font></div></td>
          </tr>
          <tr> 
            <td> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td> <div align="left"><font size="2" face="Arial, Helvetica, sans-serif"> 
                <a href="?page=register">Register an Account</a> </font></div></td>
          </tr>
          <tr> 
            <td> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td> <div align="left"><font size="2" face="Arial, Helvetica, sans-serif"> 
                <a href="#">Help</a> </font></div></td>
          </tr>
        </table></tr>
    <tr bgcolor=#AEADAD> 
      <td><img src="images/line.gif" width="1" height="1"></td>
    </tr>
  </table>
<br><form action="<?php $_SERVER['PHP_SELF']; ?>" method="post">
    <?php
    if ($target_clean != '' ) {
      echo "<input type=\"hidden\" name=\"target\" " .
           "value=\"$target_clean\">\n";
    }
    ?>
    <table width="500" border="0" cellpadding="5" cellspacing="1" align="center">
      <tr> 
        <td colspan="2" align="center" bgcolor="#5984C3"> <b><font color="#FFFFFF" size="4"><?php
	if ($rpm_sec == "security") { echo "Security "; }
?>Login</font></b></td>
      </tr>
    <?php
	if ($message != "") {
    ?>
      <tr> 
        <td colspan="2" align="center" bgcolor="red"><?php echo $message; ?></td>
      </tr>
    <?php
	}
    ?>
      <tr> 
        <td width="120" bgcolor="#E2E2E2"> 
          <div align="right">Username:</div></td>
        <td width="380" bgcolor="#E2E2E2"> 
          <input name="rosusername" type="text" class="input" id="rosusername" <?php 
			if (isset($_POST['rosusername'])) {
				echo 'value="' . $_POST['rosusername'] .  '"';
			}
			else if (isset($_COOKIE['roscms_loginname'])) {
				echo 'value="' . $_COOKIE['roscms_loginname'] .  '"';
			}
		  ?> size="50" maxlength="50">
        </td>
      </tr>
      <tr> 
        <td bgcolor="#EEEEEE"> <div align="right">Password:</div></td>
        <td bgcolor="#EEEEEE"> <input name="rospassword" type="password" class="input" id="rospassword" size="50" maxlength="50"> 
        </td>
      </tr>
      <tr bgcolor="#E2E2E2"> 
        <td>&nbsp;</td>
        <td bgcolor="#E2E2E2"><a href="?page=getpwd">Did you forget your password?</a></td>
      </tr>
<?php if ($rpm_sec == "standard" || $rpm_sec == "") { ?>
      <tr bgcolor="#EEEEEE"> 
        <td colspan="2"> 
          <fieldset>
          <legend>&nbsp;<b><font color="#5984C3">Options</font></b>&nbsp;</legend>
          
        <input name="loginoption1" type="checkbox" id="loginoption1" value="save" <?php 
			if (isset($_COOKIE['roscms_loginname'])) {
				echo 'checked';
			}
		?>>Save username <sup>1</sup><br>
          
        <input name="loginoption2" type="checkbox" id="loginoption2" value="notimeout">Log me on automatically (non-expiring session) <sup>1, 2</sup>
          </fieldset></td>
      </tr>
      <tr bgcolor="#E2E2E2"> 
      <td colspan="2"> <address>
        <sup>1</sup> cookie support required<br>
        <sup>2</sup> This options is a possible security risk! Only use this option 
        if you know what you are doing! <br>
        <br>
        Always use the high security login on public available pc's (e.g. internet 
        caf&eacute;'s, office, school, etc.)! 
        </address></td>
      </tr>
<?php } ?>
      <tr bgcolor="#5984C3"> 
        <td colspan="2" align="center"> 
          <input type="submit" name="submit" value="Login" class="button" <?php if ($rpm_sec == "security") { echo 'onclick="rospassword.value = calcMD5(\''.$random_string_security.'\' + calcMD5(rospassword.value))"'; } ?>>
        <input name="logintype" type="hidden" id="logintype" value="<?php if ($rpm_sec == "security") { echo 'security'; } else { echo 'standard'; } ?>"> </td>
      </tr>
    </table>
  </form>
<?php
}

if ((! isset($_POST['rosusername']) || $_POST['rosusername'] == "") &&
    (! isset($_POST['rospassword']) || $_POST['rospassword'] == "")) {
	create_login_page("");
}
elseif(! isset($_POST['rosusername']) || $_POST['rosusername'] == "") {
	create_login_page("Please enter your username!");
}
elseif(! isset($_POST['rospassword']) || $_POST['rospassword'] == "") {
	create_login_page("Please enter your password");
}
else { // login process
	$roscms_currentuser_id = "";
	$roscms_ses_id_key="";
	if (isset($_COOKIE['roscms_sec_key']) &&
	    preg_match('/^([a-z0-9]{32})$/', $_COOKIE['roscms_sec_key'], $matches)) {
		$roscms_ses_id_key = $matches[1];
	}
	else {
		$roscms_ses_id_key="";
	}
	if ($roscms_ses_id_key == "") {
		$random_string = make_key();
	}
	else {
		$random_string = $roscms_ses_id_key;
	}
		
	/* Check username. It should only contain printable ASCII chars */
	if (preg_match('/^([ !-~]+)$/', $_POST['rosusername'], $matches)) {
		$rosusername = $matches[1];
	}
	else {
		create_login_page("You have specified an incorrect or " .
		                  "inactive username,<br>\n" . 
	                          "or an invalid password.");
		exit;
	}
		
	/* Check password. It should only contain printable ASCII chars */
	if (preg_match('/^([ !-~]+)$/', $_POST['rospassword'], $matches)) {
		$rospassword = $matches[1];
	}
	else {
		create_login_page("You have specified an incorrect or " .
		                  "inactive username,<br>\n" . 
	                          "or an invalid password.");
		exit;
	}
	
	$query = mysql_query("SELECT * " .
	                     "  FROM users " .
	                     " WHERE user_name = '" .
                                     mysql_escape_string($rosusername) .
                                     "'")
	         or die('DB error (user login #1)!');
	$result = mysql_fetch_array($query); 
	$roscms_currentuser_id = $result['user_id'];

	$pwdtemp = $result['user_roscms_password'];
	
	if ($roscms_ses_id_key != "") {
		// we simply concatenate the password and random key to create
		// a unique session md5 hash
		// hmac functions are not available on most web servers, but
		// this is near as dammit.
		$pwdtemp = md5($random_string . $pwdtemp);
		$apassword = $rospassword;
	}
	else {
		$apassword = md5($rospassword);
	}
	
	if ($apassword != $pwdtemp) {
		create_login_page("You have specified an incorrect or " .
		                  "inactive username,<br>\n" . 
	                          "or an invalid password.");
		exit;
	}

	$rem_adr = $_SERVER['REMOTE_ADDR'];
	$useragent = $_SERVER['HTTP_USER_AGENT'];
	
	// Query DB table 'users' and read the login_counter and settings of
	// the specific user
	$query = "SELECT user_login_counter, " .
	         "       user_account_enabled, " .
		 "       user_setting_multisession, " .
		 "       user_setting_browseragent, " .
	         "       user_setting_ipaddress, " .
	         "       user_setting_timeout " .
	         "  FROM users  " .
	         " WHERE user_id = $roscms_currentuser_id";
	$login_usr_keya_query = mysql_query($query)
	                        or die('DB error (user login #2)!');
	$login_usr_keya_result = mysql_fetch_array($login_usr_keya_query);
	
	$roscms_currentuser_login_counter = $login_usr_keya_result['user_login_counter'];
	$roscms_currentuser_login_user_account_enabled = $login_usr_keya_result['user_account_enabled'];
	$roscms_currentuser_login_user_setting_multisession = $login_usr_keya_result['user_setting_multisession'];
	$roscms_currentuser_login_user_setting_browseragent = $login_usr_keya_result['user_setting_browseragent'];
	$roscms_currentuser_login_user_setting_ipaddress = $login_usr_keya_result['user_setting_ipaddress'];

	// if the account is NOT enabled; e.g. a reason could be that a member
	// of the admin group has disabled this account because of spamming,
	// etc.
	if ($roscms_currentuser_login_user_account_enabled != "yes") { 
		create_login_page("Account is disabled!<br>\n" .
		                  "System message: " .
		                  $roscms_currentuser_login_user_account_enabled);
		exit;
	}
	
	// if the user account setting is "multisession" (a by user setting),
	// it is set to "false" by default
	if ($roscms_currentuser_login_user_setting_multisession != "true") { // for security reasons here stand "!= true" instead of "== false"
		$query = "SELECT COUNT('usersession_user_id') " .
		         "  FROM user_sessions " .
		         " WHERE usersession_user_id = " .
		                 $roscms_currentuser_id;
		
		$login_usr_session_count_query = mysql_query($query)
		                                 or die('DB error (user login #3)!');
		$login_usr_session_count_result = mysql_fetch_array($login_usr_session_count_query);
		$roscms_currentuser_login_user_lastsession_counter = $login_usr_session_count_result[0];

		if ($roscms_currentuser_login_user_lastsession_counter > 0) {
			create_login_page("Your account settings only allow you to login once.<br>\n" .
			                  "You are already logged in so you cannot login again. <br>\n".
			                  "For more informations please have a look at the <a href=\"http://www.reactos.org/?page=about_userfaq#multilogin\">FAQ</a>");
			exit;
		}
	}

	// At this point, we've passed all checks and we have a valid login
	// Check if there's an existing session, if so, end that session
	if (0 != roscms_subsys_login("roscms", ROSCMS_LOGIN_OPTIONAL, "")) {
		$query = "DELETE FROM user_sessions " .
		         " WHERE usersession_id = '" .
                                 mysql_escape_string($_COOKIE['roscmsusrkey']) . "'";
		mysql_query($query);
	}
	
	// save username
	if (isset($_POST['loginoption1']) && $_POST['loginoption1'] == "save") {
		// save username (cookie)
		setcookie('roscms_loginname', $rosusername,
		          time() + 24 * 3600 * 30 * 5, '/', cookie_domain());
	}
	else {
		// delete username (cookie)
		setcookie('roscms_loginname', '',
		          time() - 3600, '/', cookie_domain());
	}

	if (isset($_POST['loginoption2']) &&
	    $_POST['loginoption2'] == "notimeout" &&
	    $login_usr_keya_result['user_setting_timeout'] == 'true') {
		$expire_value = 'NULL';
		$cookie_time = 0x7fffffff;
	}
	else {
		$expire_value = 'DATE_ADD(NOW(), INTERVAL 30 MINUTE)';
		$cookie_time = time() + 60 * 60;
	}

	// Add an entry to the 'user_sessions' table
	$login_key_post="INSERT INTO user_sessions " .
	                "       (usersession_id, " .
	                "        usersession_user_id, " .
	                "        usersession_expires, " .
	                "        usersession_browseragent, " .
	                "        usersession_ipaddress) " .
	                "VALUES ('$random_string', " .
	                "        $roscms_currentuser_id, " .
	                "        $expire_value, " .
	                "        '" .
	                         mysql_escape_string($useragent) . "', " .
	                "        '" .
	                         mysql_escape_string($rem_adr) . "')";
	$login_key_post_list = mysql_query($login_key_post)
	                       or die('DB error (user login #4)!');

	// save session_id (cookie)
	setcookie('roscmsusrkey', $random_string,
		  $cookie_time, '/', cookie_domain());
	
	$now = explode(' ',microtime());
	$time = $now[1].substr($now[0],2,2);
	settype($time, "double");
	
	// Update the login_counter of the specific user
	$roscms_currentuser_login_counter++;
	$login_usr_key_post = "UPDATE users " .
	                      "   SET user_timestamp_touch = '$time', " .
	                      "       user_login_counter = user_login_counter + 1 " .
	                      " WHERE user_id = $roscms_currentuser_id";
	$login_usr_key_post_list=mysql_query($login_usr_key_post);
	
	$roscms_login_next_page="?page=user";

	$random_string="";

	if (isset($_REQUEST['target'])) {
		header("Location: http://" . $_SERVER['HTTP_HOST'] .
                       $_REQUEST['target']);
		exit;
	}
	header("Location:?page=user");
	
	/*
	create_head($rpm_page_title, $rpm_logo, $roscms_langres);
	create_structure($rpm_page);

	echo '<p align="center">Login successful<br><br><a href="'.$roscms_login_next_page.'">Next</a></p>';
	*/
}

function make_key() {
	$random_string = '';
	for($i=0;$i<32;$i++) {
		// generate a good radom string
		mt_srand((double)microtime()*1000000);
		mt_srand((double)microtime()*65000*mt_rand(1,248374));
		$random_string .= chr(mt_rand(97,122)); //32,126
	} 
	return $random_string;
}

?>  
</div>
<p>&nbsp;</p>

