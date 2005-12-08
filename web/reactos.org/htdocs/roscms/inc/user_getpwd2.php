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
		$seclog_level="70";
		$seclog_reason="Hacking attempt: getpwd2.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}


$reg_txtusername="";
$reg_newusrsecid="";
$reg_authcode="";
$reg_txtemail="";
$reg_txtnew_password="";
$reg_txtpassword_confirm="";
$reg_cbo_lang="";
if (array_key_exists("newusrsecid", $_POST)) $reg_newusrsecid=$_POST["newusrsecid"];
if (array_key_exists("authcode", $_POST)) $reg_authcode=$_POST["authcode"];


$roscms_SET_code = "";
$roscms_SET_usrname = "";
if (array_key_exists("code", $_GET)) $roscms_SET_code=htmlspecialchars($_GET["code"]);
if (array_key_exists("usrname", $_GET)) $roscms_SET_usrname=htmlspecialchars($_GET["usrname"]);



$reg_account_count_query=mysql_query("SELECT * 
										FROM `users` 
										WHERE `user_name` = '". mysql_escape_string($roscms_SET_usrname) ."' 
										AND `user_roscms_getpwd_id` = '". mysql_escape_string($roscms_SET_code) ."' ;");
$reg_account_count_list = mysql_fetch_row($reg_account_count_query);

$reg_account_counter=$reg_account_count_list[0];
if ($reg_account_counter == "") {
	die("<p><b>The username or code is invalid! Don't try abuse this helpful function!</b></p>");
	/*
	include("roscms_config.php");
	header("Location: ".$roscms_intern_path_server);
	*/

}


?>

<div class="contentSmall"> <span class="contentSmallTitle">Activate new Password</span> 
  <p>[...]</p>
</div>
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
	if ($rpm_sec == "standard") {
?>
                <strong>Standard Login</strong> 
<?php
	}
	else {
		echo '<a href="?page=login&amp;sec=standard';
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
      <td><img srAc="images/line.gif" width="1" height="1"></td>
    </tr>
  </table>
  
<p>&nbsp;</p>

<div class="contentSmall"> <span class="contentSmallTitle">Global Login System</span> 
  <p>Account activation process</p>  
<?php 



mt_srand((double)microtime()*1000000);
mt_srand((double)microtime()*65000*mt_rand(1,10000));
$reg_radomnumber = mt_rand(1,1000);

mt_srand((double)microtime()*250*mt_rand(1,100));
$reg_radomnumber2 = mt_rand(8,12); // pwd length 8-10 chars

$reg_randompwd=substr(md5($reg_newusrsecid + $reg_radomnumber), 0, $reg_radomnumber2);


$reg_account_count_query=mysql_query("SELECT * 
										FROM `users` 
										WHERE `user_name` = '". mysql_escape_string($roscms_SET_usrname) ."'");
$reg_account_count_list = mysql_fetch_row($reg_account_count_query);

//echo "<br>=".$reg_account_count_list[0];
$reg_account_counter=$reg_account_count_list[0];
if ($reg_account_counter == "") {
	die("<p><b>There is no account with the same name! Please use the 'back' function in your browser and try it again!</b></p>");
}


$content_postc="UPDATE `users` 
				SET `user_roscms_password` = MD5( '". mysql_escape_string($reg_randompwd) ."' ) ,
				`user_roscms_getpwd_id` = '' 
				WHERE `user_name` = '". mysql_escape_string($roscms_SET_usrname) ."' 
				LIMIT 1  ;";
$content_post_listc=mysql_query($content_postc);

$reg_account_query=mysql_query("SELECT * 
										FROM `users` 
										WHERE `user_name` = '". mysql_escape_string($roscms_SET_usrname) ."'");
$reg_account_query_list = mysql_fetch_array($reg_account_query);

//echo "----".$reg_account_query_list['user_id'];
$roscms_intern_TEMP_usrid = "";
$roscms_intern_TEMP_usrid = $reg_account_query_list['user_id'];
$querya = "DELETE FROM user_sessions " .
		 " WHERE usersession_user_id = '" .
						 mysql_escape_string($roscms_intern_TEMP_usrid) . "'";
mysql_query($querya);


echo "<br><br>New Password: <b>" . $reg_randompwd . "</b><br><br><br>";

include("roscms_config.php");

echo "<b><a href='" . $roscms_intern_path_server . "roscms/?page=user&amp;sec=account&amp;sec2=edit'>Login and change your password</a></b><br><br>";

// Do NOT remove the comment slashes! This would be a big security risc.
//echo "<p>usrname=" . $reg_txtusername . " pwd=" . $reg_randompwd ."</p>";



	/* Empfänger */
	$empfaenger = array(htmlentities($reg_account_query_list['user_email']));
	
	/* Empfänger CC */
	$empfaengerCC = array("");
	
	/* Empfänger BCC */
	$empfaengerBCC = array("");
	
	/* Absender */
	$absender = "RosCMS<roscms@reactos.org>";
	
	/* Reply */
	$reply = "RosCMS<roscms@reactos.org>";
	
	/* Subject */
	$subject = "ReactOS Homepage - myReactOS Account [New Password - #2]";
	
	/* Message */
	$message = "ReactOS Homepage - myReactOS Account [New Password - #2]\n\n\nUsername: ".$reg_txtusername."\nNew Password: ".$reg_randompwd." (random generated; please change this password as soon as possibe!)\n\n\nChange your Password:\nGo to the 'myReactOS' page and click on 'account' -> 'edit' or use this deep link: http://www.reactos.org/roscms/?page=user&sec=account&sec2=edit\n\n\n\nIf you find any problems, feel free to register them in BugZilla.\n\n\nBest regards,\nThe ReactOS Web Team\n\n\n\nPlease do not reply as this is an auto generated email! If you have not requested a new password, then someone tried to recover your password.";
	
	/* build the mail header */
	$headers = "";
	$headers .= "From:" . $absender . "\n";
	$headers .= "Reply-To:" . $reply . "\n"; 
	$headers .= "X-Mailer: RosCMS\n"; 
	$headers .= "X-Sender-IP: ".$_SERVER['REMOTE_ADDR']."\n"; 
	$headers .= "Content-type: text/plain\n";
	
	// extract the email adresses
	$empfaengerString = implode(",",$empfaenger);
	$empfaengerCCString = implode(",",$empfaengerCC);
	$empfaengerBCCString = implode(",",$empfaengerBCC);
	
	$headers .= "Cc: " . $empfaengerCCString . "\n";
	$headers .= "Bcc: " . $empfaengerBCCString . "\n";
	
	/* sent the mail */
	@mail($empfaengerString, $subject, $message, $headers);

	echo "<p>The <b>new Password</b> has been sent to your email address! Don't forget to change the password!</p>";

	echo "</div>";

?>
