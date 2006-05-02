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
		$seclog_reason="Hacking attempt: register.php";
		define ("ROSCMS_SYSTEM", "Hacking attempt");
		include('securitylog.php'); // open security log
		die("Hacking attempt");
	}


$reg_newusrsecid="";
$reg_authcode="";
$reg_txtusername="";
$reg_txtemail="";
$reg_txtnew_password="";
$reg_txtpassword_confirm="";
$reg_cbo_lang="";
if (array_key_exists("newusrsecid", $_POST)) $reg_newusrsecid=$_POST["newusrsecid"];
if (array_key_exists("authcode", $_POST)) $reg_authcode=$_POST["authcode"];
if (array_key_exists("txtusername", $_POST)) $reg_txtusername=$_POST["txtusername"];
if (array_key_exists("txtemail", $_POST)) $reg_txtemail=$_POST["txtemail"];
/*if (array_key_exists("txtnew_password", $_POST)) $reg_txtnew_password=$_POST["txtnew_password"];
if (array_key_exists("txtpassword_confirm", $_POST)) $reg_txtpassword_confirm=$_POST["txtpassword_confirm"];
if (array_key_exists("cbo_lang", $_POST)) $reg_cbo_lang=$_POST["cbo_lang"];

if (($reg_newusrsecid != md5($reg_authcode) 
		|| $reg_newusrsecid == ""
		|| $reg_txtemail == ""
		|| $reg_txtnew_password == ""
		|| $reg_txtnew_password != $reg_txtpassword_confirm
		|| $reg_cbo_lang == "")
		&& $rpm_sec == "usrdata2")
	{
		$rpm_sec = "usrdata";
	}

$reg_realname="";
$reg_occupation="";
$reg_interests="";
$reg_website="";
$reg_description="";

if (array_key_exists("realname", $_POST)) $reg_realname=$_POST["realname"];
if (array_key_exists("occupation", $_POST)) $reg_occupation=$_POST["occupation"];
if (array_key_exists("interests", $_POST)) $reg_interests=$_POST["interests"];
if (array_key_exists("website", $_POST)) $reg_website=$_POST["website"];
if (array_key_exists("description", $_POST)) $reg_description=$_POST["description"];

//	echo "  GATEWAY_INTERFACE: $GATEWAY_INTERFACE <br>  SERVER_NAME: $SERVER_NAME <br>  SERVER_SOFTWARE: $SERVER_SOFTWARE <br>  SERVER_PROTOCOL: $SERVER_PROTOCOL <br>  REQUEST_METHOD: $REQUEST_METHOD <br>  QUERY_STRING: $QUERY_STRING <br>  DOCUMENT_ROOT: $DOCUMENT_ROOT <br>  HTTP_ACCEPT: $HTTP_ACCEPT <br>  HTTP_ACCEPT_CHARSET: $HTTP_ACCEPT_CHARSET <br>  HTTP_ENCODING: $HTTP_ENCODING <br>  HTTP_ACCEPT_LANGUAGE: $HTTP_ACCEPT_LANGUAGE<br>  HTTP_CONNECTION: $HTTP_CONNECTION <br>  HTTP_HOST: $HTTP_HOST <br>  HTTP_REFERER: $HTTP_REFERER <br>  HTTP_USER_AGENT: $HTTP_USER_AGENT <br>	  REMOTE_ADDR: $REMOTE_ADDR <br>  REMOTE_PORT: $REMOTE_PORT <br>  SCRIPT_FILENAME: $SCRIPT_FILENAME <br>  SERVER_ADMIN: $SERVER_ADMIN <br>  SERVER_PORT: $SERVER_PORT <br>  SERVER_SIGNATURE: $SERVER_SIGNATURE <br>  PATH_TRANSLATED: $PATH_TRANSLATED <br>  SCRIPT_NAME: $SCRIPT_NAME <br>  REQUEST_URI: $REQUEST_URI <br>";

*/

/*if(!session_is_registered('roshpuser') || $_SESSION['roshpuser'] == "") {
	// Temp
}
else {
	header("location:../support/?page=user");
	die;
}*/
?>

<div class="contentSmall"> <span class="contentSmallTitle">Register Account</span> 
  
  <p>With a valid account you will be able to login to ReactOS Global Login System.</p>
  <p><strong>You do NOT need to register a new account, if you had an account 
    on the old ReactOS website (forums, wiki and/or bugzilla). Then you will get 
    an email with more informations how to use the old account with the new Homepage 
    System.</strong></p>
  <table border="0" cellspacing="0" cellpadding="0" width="320">
    <tr> 
      <td colspan="3"><span class="contentSmallTitle">Register Steps</span></td>
    </tr>
    <tr> 
      <td colspan="2" bgcolor="#F9F8F8"> <table width="342" border="0" cellpadding="4">
          <tr> 
            <td width="20"> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td width="300"> <div align="left"><font size="2" face="Arial, Helvetica, sans-serif"> 
                <?php
	if ($rpm_sec == "" || $rpm_sec == "agreement") {
?>
                <strong>Registration Agreement Terms</strong> 
                <?php } else { echo "Registration Agreement Terms"; } ?>
                </font></div></td>
          </tr>
          <tr> 
            <td width="20"> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td width="300"> <div align="left"><font size="2" face="Arial, Helvetica, sans-serif"> 
                <?php
	if ($rpm_sec == "usrdata") {
?>
                <strong>User Data</strong> 
                <?php } else { echo "User Data"; } ?>
                </font></div></td>
          </tr>
          <tr> 
            <td> <div align="center"><img src="images/dot.gif" vspace="3"></div></td>
            <td> <div align="left"><font size="2" face="Arial, Helvetica, sans-serif"> 
                <?php
	if ($rpm_sec == "usrdata2") {
?>
                <strong>Global Login System</strong> 
                <?php } else { echo "Global Login System"; } ?>
                </font></div></td>
          </tr>
        </table></tr>
    <tr bgcolor=#AEADAD> 
      <td><img src="images/line.gif" width="1" height="1"></td>
    </tr>
  </table>
</div><br>
<?php
	if ($rpm_sec == "" || $rpm_sec == "agreement") {
?><div class="contentSmall"> <span class="contentSmallTitle">Registration Agreement 
  Terms</span> 
  <p>The ReactOS administrator and moderator team of the ReactOS homepage will 
    attempt to remove or edit any generally objectionable material as quickly 
    as possible, it is impossible to review every message/comment/entry/etc.. 
    Therefore you acknowledge that all posts/comment/entry/etc. made to the ReactOS 
    homepage express the views and opinions of the author and not the administrators, 
    moderators or webmaster (except for posts/comment/entry/etc. by these people) 
    and hence will not be held liable.</p>
  <p>You agree not to post any abusive, obscene, vulgar, slanderous, hateful, 
    threatening, sexually-oriented or any other material that may violate any 
    applicable laws. Doing so may lead to you being immediately and permanently 
    banned (and your service provider being informed). The IP address of all posts 
    is recorded to aid in enforcing these conditions. You agree that the webmaster, 
    administrator and moderator team of the ReactOS homepage have the right to 
    remove, edit, move or close any topic/entry/etc. at any time should they see 
    fit. As a user you agree to any information you have entered above being stored 
    in a database. While this information will not be disclosed to any third party 
    without your consent the webmaster, administrator and moderator team cannot 
    be held responsible for any hacking attempt that may lead to the data being 
    compromised.</p>
  <p>The ReactOS Global Login System uses cookies to store information on your 
    local computer. These cookies do not contain any of the information you have 
    entered above; they serve only to improve your viewing pleasure. The e-mail 
    address is used only for confirming your registration details and password 
    (and for sending new passwords should you forget your current one).</p>
  <p><strong>Attention: </strong>The ReactOS Global Login System handles all login 
    data, so you do need to login only one time. All ReactOS website components 
    (forum, wiki, bugzilla, compatibility database, etc.) use the Global Login 
    System!</p>
  <p>By clicking Register below you agree to be bound by these conditions</p>
  <p align="center"><a href="?page=register&amp;sec=usrdata">I <strong>agree</strong> 
    to these terms</a></p>
  <p align="center"><a href="../?page=index">I <strong>do not agree</strong> to 
    these terms</a></p>
  <p>&nbsp;</p>
  </div>

<?php 
	}
?>
<?php
	if ($rpm_sec == "usrdata") {
?>
<div class="contentSmall"> <span class="contentSmallTitle">User Data</span> 
  <p>Input the registration information ...</p>
  <p> Items marked with a * are required unless stated otherwise. </p>
    <form name="form1" method="post" action="?page=register&amp;sec=usrdata2">

    <table width="100%" border="0" cellpadding="1" cellspacing="1">
      <tr bgcolor="#5984C3"> 
        <td colspan="2"> <span class="contentSmallTitle">Registration Information</span></td>
      </tr>
      <tr bgcolor="#5984C3"> 
        <td width="40%" bgcolor="#E2E2E2"> <div align="left"><font face="Arial, Helvetica, sans-serif">Username: 
            * </font></div></td>
        <td width="60%" bgcolor="#E2E2E2"> <div align="left"><font face="Arial, Helvetica, sans-serif"> 
            <input name="txtusername" type="text" class="post" id="txtusername" style="width:200px" value="<?php echo $reg_txtusername; ?>" size="25" maxlength="25" />
            </font></div></td>
      </tr>
      <tr bgcolor="#EEEEEE"> 
        <td valign="top"> <div align="left"><font face="Arial, Helvetica, sans-serif">E-mail 
            address: *</font></div></td>
        <td valign="top" bgcolor="#EEEEEE"> <div align="left"> 
            <input name="txtemail" type="text" class="post" id="txtemail" style="width:200px" value="<?php echo $reg_txtemail; ?>" size="25" maxlength="255" />
          </div></td>
      </tr>
    </table>
<p>&nbsp;</p>
    <?php

$array = array(
	"1" => array("1" => "??#??+?#?#+#???#+#####+#???#", // A
				 "2" => "??#??+?#?#+#???#+#####+#???#",
				 "3" => "?###+#???#+#???#+#####+#???#",
				 "4" => "A"),
	"2" => array("1" => "####+#???#+####+#???#+####", // B
				 "2" => "###+#??#+###+#??#+###",
				 "3" => "###+#??#+###+#??#+###",
				 "4" => "B"),
	"3" => array("1" => "?####+#+#+#+?####", // C
				 "2" => "?####+#+#+#+?####",
				 "3" => "?####+#+#+#+?####",
				 "4" => "C"),
	"4" => array("1" => "####+#???#+#???#+#???#+####", // D
				 "2" => "####+#???#+#???#+#???#+####",
				 "3" => "####+#???#+#???#+#???#+####",
				 "4" => "D"),
	"5" => array("1" => "#####+##+####+##+#####", // E
				 "2" => "#####+##+####+##+#####",
				 "3" => "#####+##+####+##+#####",
				 "4" => "E"),
	"6" => array("1" => "#####+##+####+##+##", // F
				 "2" => "#####+##+####+##+##",
				 "3" => "#####+##+####+##+##",
				 "4" => "F"),
	"7" => array("1" => "?####+#+#??##+#???#+?###", // G
				 "2" => "?####+#+#??##+#???#+?###",
				 "3" => "?####+#+#??##+#???#+?###",
				 "4" => "G"),
	"8" => array("1" => "#???#+#???#+#####+#???#+#???#", // H
				 "2" => "#???#+#???#+#####+#???#+#???#",
				 "3" => "#???#+#???#+#####+#???#+#???#",
				 "4" => "H"),
	"9" => array("1" => "##+##+##+##+##", // I
				 "2" => "##+##+##+##+##",
				 "3" => "##+##+##+##+##",
				 "4" => "I"),
	"10" => array("1" => "???#+???#+???#+#??#+?##", // J
				 "2" => "???#+???#+???#+#??#+?##",
				 "3" => "???#+???#+???#+#??#+?##",
				 "4" => "J"),
	"11" => array("1" => "#??#+#?#+##+#?#+#??#", // K
				 "2" => "#??#+#?#+##+#?#+#??#",
				 "3" => "#??#+#?#+##+#?#+#??#",
				 "4" => "K"),
	"12" => array("1" => "##+##+##+##+######", // L
				 "2" => "##+##+##+##+######",
				 "3" => "##+##+##+##+######",
				 "4" => "L"),
	"13" => array("1" => "#???#+##?##+#?#?#+#???#+#???#", // M
				 "2" => "#???#+##?##+#?#?#+#???#+#???#",
				 "3" => "#???#+##?##+#?#?#+#???#+#???#",
				 "4" => "M"),
	"14" => array("1" => "#???#+##??#+#?#?#+#??##+#???#", // N
				 "2" => "#???#+##??#+#?#?#+#??##+#???#",
				 "3" => "#???#+##??#+#?#?#+#??##+#???#",
				 "4" => "N"),
	"15" => array("1" => "?####+#????#+#????#+#????#+?####", // O
				 "2" => "?####+#????#+#????#+#????#+?####",
				 "3" => "?####+#????#+#????#+#????#+?####",
				 "4" => "O"),
	"16" => array("1" => "####+#???#+####+#+#", // P
				 "2" => "####+#???#+####+#+#",
				 "3" => "####+#???#+####+#+#",
				 "4" => "P"),
	"17" => array("1" => "?####+#????#+#????#+#??###+?######", // Q
				 "2" => "?####+#????#+#????#+#??###+?######",
				 "3" => "?####+#????#+#????#+#??###+?#####'",
				 "4" => "Q"),
	"18" => array("1" => "###+#??#+###+#??#+#??#", // R
				 "2" => "###+#??#+###+#??#+#??#",
				 "3" => "###+#??#+###+#??#+#??#",
				 "4" => "R"),
	"19" => array("1" => "?####+#???+?###+????#+####", // S
				 "2" => "?####+#???+?###+????#+####",
				 "3" => "?####+#???+?###+????#+####",
				 "4" => "S"),
	"20" => array("1" => "#####+??#+??#+??#+??#", // T
				 "2" => "#####+??#+??#+??#+??#",
				 "3" => "#####+??#+??#+??#+??#",
				 "4" => "T"),
	"21" => array("1" => "#???#+#???#+#???#+#???#+?###", // U
				 "2" => "#???#+#???#+#???#+#???#+?###",
				 "3" => "#???#+#???#+#???#+#???#+?###",
				 "4" => "U"),
	"22" => array("1" => "#???#+#???#+?#?#+?#?#+??#", // V
				 "2" => "#???#+#???#+?#?#+?#?#+??#",
				 "3" => "#???#+#???#+?#?#+?#?#+??#",
				 "4" => "V"),
	"23" => array("1" => "#????#+#????#+#????#+#?##?#+?#??#", // W
				 "2" => "#????#+#????#+#????#+#?##?#+?#??#",
				 "3" => "#????#+#????#+#????#+#?##?#+?#??#",
				 "4" => "W"),
	"24" => array("1" => "#???#+?#?#+??#+?#?#+#???#", // X
				 "2" => "#???#+?#?#+??#+?#?#+#???#",
				 "3" => "#???#+?#?#+??#+?#?#+#???#",
				 "4" => "X"),
	"25" => array("1" => "#???#+?#?#+??#+??#+??#", // Y
				 "2" => "#???#+?#?#+??#+??#+??#",
				 "3" => "#???#+?#?#+??#+??#+??#",
				 "4" => "Y"),
	"26" => array("1" => "#####+???#+??#+?#+#####", // Z
				 "2" => "#####+???#+?###+?#+#####",
				 "3" => "#####+???#+?###+?#+#####",
				 "4" => "Z"),
	"27" => array("1" => "?###+#???#+#???#+#???#+?###?.", // 0
				 "2" => "?###+#???#+#???#+#???#+?###?.",
				 "3" => "?###+#???#+#???#+#???#+?###?.",
				 "4" => "0"),
	"28" => array("1" => "?#+##+?#+?#+###?.", // 1
				 "2" => "?#+##+?#+?#+###?.",
				 "3" => "?#+##+?#+?#+###?.",
				 "4" => "1"),
	"29" => array("1" => "?###+#???#+???#+??#+#####??.", // 2
				 "2" => "?###+#???#+???#+??#+#####??.",
				 "3" => "?###+#???#+???#+??#+#####??.",
				 "4" => "2"),
	"30" => array("1" => "####+????#+??##+????#+####?.", // 3
				 "2" => "####+????#+??##+????#+####?.",
				 "3" => "####+????#+??##+????#+####?.",
				 "4" => "3"),
	"31" => array("1" => "??#+?#+#??#+#####+???#?.", // 4
				 "2" => "??#+?#+#??#+#####+???#?.",
				 "3" => "??#+?#+#??#+#####+???#?.",
				 "4" => "4"),
	"32" => array("1" => "#####+#+####+????#+####?.", // 5
				 "2" => "#####+#+####+????#+####?.",
				 "3" => "#####+#+####+????#+####?.",
				 "4" => "5"),
	"33" => array("1" => "?###+#+####+#???#+?###?.", // 6
				 "2" => "?###+#+####+#???#+?###?.",
				 "3" => "?###+#+####+#???#+?###?.",
				 "4" => "6"),
	"34" => array("1" => "#####+???#+??#+??#+??#?.", // 7
				 "2" => "######+???#+??#+??#+??#?.",
				 "3" => "#####+???#+??#+??#+??#?.",
				 "4" => "7"),
	"35" => array("1" => "?###+#???#+?###+#???#+?###?.", // 8
				 "2" => "?###+#???#+?###+#???#+?###?.",
				 "3" => "?###+#???#+?###+#???#+?###?.",
				 "4" => "8"),
	"36" => array("1" => "?###+#???#+?####+????#+?###?.", // 9
				 "2" => "?###+#???#+?####+????#+?###?.",
				 "3" => "?###+#???#+?####+????#+?###?.",
				 "4" => "9"),
);



?>

  <table width="100%" border="0" cellpadding="0" cellspacing="0">
    <tr> 
      <td colspan="16"> <span class="contentSmallTitle">Code Validator</span></td>
    </tr>
    <tr bgcolor="#666666"> 
      <td width="12%" bgcolor="#666666">&nbsp;</td>
      <?php
		$securityergges="";
	for($x=0;$x<7;$x++){
		echo '<td width="12.5%"> 
        <p><font face="Courier New, Courier, mono">';
		
		mt_srand((double)microtime()*1000000);
		mt_srand((double)microtime()*65000*mt_rand(1,10000));
		$radom1 = mt_rand(1,36);
		//echo $radom1."<br>"; 
		mt_srand((double)microtime()*1000000);
		mt_srand((double)microtime()*65000*mt_rand(1,10000));
		$radom2 = mt_rand(1,3);
		//echo $radom2."<br>"; 
		$securityerg = $array[$radom1][$radom2];
		$securityergges = $securityergges.$array[$radom1][4];
		//$securityerg = $array[$radom1][1];
		$securityerg = str_replace("?","&nbsp;",$securityerg);
		//$securityerg = str_replace("#","R",$securityerg);
		mt_srand((double)microtime()*1000000);
		mt_srand((double)microtime()*89000*mt_rand(1,45510));
		$radom3 = mt_rand(1,36);
		$securityerg = str_replace("#",$array[$radom3][4],$securityerg);
		$securityerg = str_replace("+","<br>",$securityerg);
		echo $securityerg;
		echo '</font></p></td>';
	}
	?>
    </tr>
    <tr>
      <td colspan="16"><table width="100%" border="0" cellpadding="1" cellspacing="1">
          <tr bgcolor="#EEEEEE"> 
            <td width="40%" bgcolor="#EEEEEE">Identify Code:<br>
              <font size="1">(all letters are upper case and all numbers are marked 
              with a small dot)</font></td>
            <td width="59%"><font face="Arial, Helvetica, sans-serif"> 
              <input name="authcode" type="text" class="post" id="authcode" style="width:200px" size="25" maxlength="7" />
              <input name="newusrsecid" type="hidden" id="newusrsecid" value="<?php echo md5($securityergges); ?>">
              </font></td>
          </tr>
        </table> </td>
    </tr>
  </table>
    <input type="submit" name="Submit" value="Submit">
  </form>
</div>
<?php 
	}
?>
<?php
	if ($rpm_sec == "usrdata2") {
	
	?>
<div class="contentSmall"> <span class="contentSmallTitle">Global Login System</span> 
  <p>Account activation process</p>  
<?php 


//echo "<br>ID_system: ".$reg_newusrsecid."<br>";
//echo "ID_user_input: ".md5($reg_authcode)."<br>";
if ($reg_newusrsecid != md5($reg_authcode) && $rpm_sec == "usrdata2") {
	die("<p><b>You typed a NOT valid registration code! Please use the 'back' function in your browser and try it again with a other username!</b></p>");
}

if ("usrdata2" == $rpm_sec) {
	if (! preg_match('/^[\\w\\.\\+\\-=]+@[\\w\\.-]+\\.[\\w\\-]+$/',
	                 $reg_txtemail) && $rpm_sec == "usrdata2") {
		die("<p><b>The email address you provided was not valid. " .
	            "Please use the 'back' function in your browser and try " .
		    "again with a valid email address.</b></p>");
	}

	$query = "SELECT COUNT(*) AS match_count " .
	         "  FROM users " .
	         " WHERE LOWER(user_email) = LOWER('" .
	                 mysql_real_escape_string($reg_txtemail) . "')";
	$email_check_set = mysql_query($query) or die("DB error");
	$email_check_row = mysql_fetch_array($email_check_set);
	if ($email_check_row['match_count'] != 0) {
		die("<p><b>An account has already been registered using this " .
		    "email address.</b></p>");
	}
}

mt_srand((double)microtime()*1000000);
mt_srand((double)microtime()*65000*mt_rand(1,10000));
$reg_radomnumber = mt_rand(1,1000);

mt_srand((double)microtime()*250*mt_rand(1,100));
$reg_radomnumber2 = mt_rand(25,30); // pwd length 25-30 chars

$reg_randompwd=substr(md5($reg_newusrsecid + $reg_radomnumber), 0, $reg_radomnumber2);

$reg_account_count_query=mysql_query("SELECT * 
										FROM `users` 
										WHERE `user_name` = '". mysql_escape_string($reg_txtusername) ."'");
$reg_account_count_list = mysql_fetch_row($reg_account_count_query);

//echo "<br>=".$reg_account_count_list[0];
$reg_account_counter=$reg_account_count_list[0];
if ($reg_account_counter != "") {
	die("<p><b>There is already an account with the same name! Please use the 'back' function in your browser and try it again with another username!</b></p>");
}


//echo  "PWD::".$reg_randompwd;
$reg_account_post="INSERT INTO `users` ( `user_id` , `user_name` , `user_roscms_password` , `user_timestamp_touch` , `user_timestamp_touch2` , `user_login_counter` , `user_account_enabled` , `user_account_hidden` , `user_register` , `user_fullname` , `user_email` , `user_website` , `user_language` , `user_country` , `user_timezone` , `user_occupation` , `user_description` , `user_setting_multisession` , `user_setting_browseragent` , `user_setting_ipaddress` , `user_setting_timeout` ) 
				VALUES ('', '". mysql_escape_string($reg_txtusername) ."', MD5( '". mysql_escape_string($reg_randompwd) ."' ) , '', NOW( ) , '0', 'yes', 'no', NOW( ) , '', '". mysql_escape_string($reg_txtemail) ."', '', 'en', '', '', '', '', 'true', 'false', 'false', 'true');";
$reg_account_list=mysql_query($reg_account_post);


$reg_account_count_query2=mysql_query("SELECT * 
										FROM `users` 
										WHERE `user_name` = '". mysql_escape_string($reg_txtusername) ."'");
$reg_account_count_list2 = mysql_fetch_row($reg_account_count_query2);

//echo "<br>==".$reg_account_count_list2[0];
$reg_account_counter2=$reg_account_count_list2[0];


$reg_account_post2="INSERT INTO `usergroup_members` ( `usergroupmember_userid` , `usergroupmember_usergroupid` ) 
					VALUES ('". mysql_escape_string($reg_account_counter2) ."', 'user');";
$reg_account_list2=mysql_query($reg_account_post2);


require("inc/subsys_utils.php");

subsys_update_user($reg_account_counter2);


	/* Empfänger */
	$empfaenger = array(htmlentities($reg_txtemail));
	
	/* Empfänger CC */
	$empfaengerCC = array("");
	
	/* Empfänger BCC */
	$empfaengerBCC = array("");
	
	/* Absender */
	$absender = "RosCMS<roscms@reactos.org>";
	
	/* Reply */
	$reply = "RosCMS<roscms@reactos.org>";
	
	/* Subject */
	$subject = "ReactOS Homepage - myReactOS Account";
	
	/* Message */
	$message = "ReactOS Homepage - myReactOS Account\n\n\nUsername: ".$reg_txtusername."\nPassword: ".$reg_randompwd."\n\n\nNext step:\nVisit the ReactOS Homepage (http://www.reactos.org) and click on 'myReactOS' (http://www.reactos.org/roscms/?page=login). On the login page, input your account data (see above) and press 'login'.\n\nEdit the account data:\nGo to the 'myReactOS' page and click on 'account' -> 'edit' or use this deep link: http://www.reactos.org/roscms/?page=user&sec=account&sec2=edit\n\n\n\nWe hope you like the new website. If you find any problems, feel free to register them in BugZilla.\n\n\nBest regards,\nThe ReactOS Web Team\n\n\n(please do not reply as this is an auto generated email!)";
	
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
	mail($empfaengerString, $subject, $message, $headers);

	echo "<p>Please check your E-mails! A random generated password has been sent to your email address!</p>";

	echo "</div>";

	}
?>
