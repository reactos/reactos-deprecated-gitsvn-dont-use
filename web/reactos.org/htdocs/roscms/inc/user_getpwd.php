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
		$seclog_reason="Hacking attempt: getpwd.php";
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
if (array_key_exists("txtusername", $_POST)) $reg_txtusername=$_POST["txtusername"];
if (array_key_exists("txtemail", $_POST)) $reg_txtemail=$_POST["txtemail"];


?>

<div class="contentSmall"> <span class="contentSmallTitle">Did you forget your password?</span> 
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
<?php
	if ($rpm_sec == "usrdata"  || $rpm_sec == "") {
?>
<form name="form1" method="post" action="?page=getpwd&amp;sec=usrdata2">

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


mt_srand((double)microtime()*1000000);
mt_srand((double)microtime()*65000*mt_rand(1,10000));
$reg_radomnumber = mt_rand(1,1000);

mt_srand((double)microtime()*250*mt_rand(1,100));
$reg_radomnumber2 = mt_rand(25,30); // pwd length 25-30 chars

$reg_randompwd=substr(md5($reg_newusrsecid + $reg_radomnumber), 0, $reg_radomnumber2);


// Do NOT remove the comment slashes! This would be a big security risc.
//echo "<a href='?page=getpwd2&amp;usrname=" . $reg_txtusername . "&amp;code=" . $reg_randompwd . "'>Link</a>";

$reg_account_count_query=mysql_query("SELECT * 
										FROM `users` 
										WHERE `user_name` = '". mysql_escape_string($reg_txtusername) ."'");
$reg_account_count_list = mysql_fetch_row($reg_account_count_query);

//echo "<br>=".$reg_account_count_list[0];
$reg_account_counter=$reg_account_count_list[0];
if ($reg_account_counter == "") {
	die("<p><b>There is no account with the same name! Please use the 'back' function in your browser and try it again!</b></p>");
}

$content_postc="UPDATE `users` SET `user_roscms_getpwd_id` = '" . $reg_randompwd . "' WHERE `user_name` = '". mysql_escape_string($reg_txtusername) ."' LIMIT 1 ;";
$content_post_listc=mysql_query($content_postc);



$reg_accounta_query=mysql_query("SELECT * 
										FROM `users` 
										WHERE `user_name` = '". mysql_escape_string($reg_txtusername) ."'");
$reg_accounta_query_list = mysql_fetch_array($reg_accounta_query);


	/* Empfänger */
	$empfaenger = array(htmlentities($reg_accounta_query_list['user_email']));
	
	/* Empfänger CC */
	$empfaengerCC = array("");
	
	/* Empfänger BCC */
	$empfaengerBCC = array("");
	
	/* Absender */
	$absender = "RosCMS<roscms@reactos.org>";
	
	/* Reply */
	$reply = "RosCMS<roscms@reactos.org>";
	
	/* Subject */
	$subject = "ReactOS Homepage - myReactOS Account [New Password - #1]";
	
	/* Message */
	$message = "ReactOS Homepage - myReactOS Account [New Password - #1]\n\n\nUsername: ".$reg_txtusername."\nActivation Code: ".$reg_randompwd."\n\n\nUse this link to get a new password:\nhttp://www.reactos.org/roscms/?page=getpwd2&usrname=" . $reg_txtusername . "&code=" . $reg_randompwd . "\n\n\n\nIf you find any problems, feel free to register them in BugZilla.\n\n\nBest regards,\nThe ReactOS Web Team\n\n\n\nPlease do not reply as this is an auto generated email! If you have not requested a new password, then someone tried to recover your password.";
	
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

	echo "<p>Please check your E-mails! A random generated <b>activation link</b> has been sent to your email address!</p>";

	echo "</div>";

	}
?>
