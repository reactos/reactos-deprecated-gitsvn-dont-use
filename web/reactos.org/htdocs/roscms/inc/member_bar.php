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

	if($roscms_intern_login_check=="valid" && $roscms_intern_usrgrp_user == true) {
		//require("./inc/inc_accountlevel_check.php");

?><div class="navTitle"><?php echo $roscms_langres['Account']; ?></div>
	<ol>
		<li>
			&nbsp;Nick:&nbsp;<?php echo substr($roscms_intern_login_check_username, 0, 9); ?> 
			<img src="images/info_15x15.gif" alt="<?php echo "Username: ".$roscms_intern_login_check_username; ?>" width="15" height="15">
		</li>
		<li><a href="?page=logout"><?php echo $roscms_langres['Logout']; ?></a></li>
	</ol>
</div>
<p></p><?php
	}
	else {
	/*
		?><form action="?page=login" method="post"><div class="navTitle">Login</div>   
		  <ol>
			<li><?php echo $roscms_langres['Login_Nick']; ?><input type="text" name="rosusername" accesskey="l" size="10"/>
							<input type="hidden" name="fastloginreferrer" value="/home/"/>
			<?php echo $roscms_langres['LoginPwd']; ?><input type="password" name="rospassword" accesskey="2" size="11"/>
		  <div align="center"> 
			<input type="submit" name="submit" value="<?php echo $roscms_langres['Login']; ?>" class="button"> 
			 </div> 
			</li>
		<li><a href="?page=login"><?php echo $roscms_langres['Global_Login_System']; ?></a></li>
		<li><a href="?page=register"><?php echo $roscms_langres['Register_Account']; ?></a></li>
		  </ol></div></form>
		  <p></p><?php
		  */
	}
?>
