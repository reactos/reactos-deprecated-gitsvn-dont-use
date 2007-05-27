<?php

	/* RosCMS install script */

	/* @TODO: fix this script */

	$rpm_ready = "";
	if (array_key_exists("ready", $_GET)) $rpm_ready=htmlspecialchars($_GET["ready"]);

	
	if ($rpm_ready == "step2") {
?>
	<h1>RosCMS - Install Script - Step2</h1>
	<form id="form1" method="post" action="install.php?ready=step3">
	  <h3>Database Information       </h3>
	  <p>Database Host Name: 
	    <input name="dbHost" type="text" id="dbHost" value="localhost" />
      </p>
	  <p>Database User Name:
        <input name="dbUser" type="text" id="dbUser" value="root" />
</p>
	  <p>Database User Password:
        <input name="dbPass" type="text" id="dbPass" />
</p>
	  <p>Database  Name:
        <input name="dbName" type="text" id="dbName" value="domainname" />
</p>
	  <h3>Server Information </h3>
	  <p>Complete Server Path: 
	    <input name="path_server" type="text" id="path_server" value="http://localhost/domain.com/" size="50" />
	  </p>
	  <p>&nbsp;</p>
	  <p>
	    <label>
        <input type="submit" name="Submit" value="Install RosCMS ..." />
        </label>
      </p>
	</form>
	
<?php	
	}
	else if ($rpm_ready == "step3") {
		echo "<h1>RosCMS - Install Script - Step3</h1>";
		
		$dbHost = "localhost";
		$dbUser = "root";
		$dbPass = "";
		$dbName = "roscms";
		$path_server = "";
		
		if (array_key_exists("dbHost", $_POST)) $dbHost=htmlspecialchars($_POST["dbHost"]);
		if (array_key_exists("dbUser", $_POST)) $dbUser=htmlspecialchars($_POST["dbUser"]);
		if (array_key_exists("dbPass", $_POST)) $dbPass=htmlspecialchars($_POST["dbPass"]);
		if (array_key_exists("dbName", $_POST)) $dbName=htmlspecialchars($_POST["dbName"]);
		if (array_key_exists("path_server", $_POST)) $path_server=htmlspecialchars($_POST["path_server"]);

				
		$connect = @mysql_connect($dbHost, $dbUser, $dbPass) or die("ERROR: Cannot connect to the database!");
		
		$query_temp1 = mysql_query("CREATE DATABASE `". mysql_real_escape_string($dbName) ."` ;") ;
		
		$selectDB = @mysql_select_db($dbName, $connect) or die("Cannot find and select <b>". $dbName ."</b>!");
		
		$query_temp2 = mysql_query(file_get_contents("install_sql.txt")) ;
		
		echo "<p>".$query_temp2."</p>";
		
		
	
	}
	else {
?>
		<h1>RosCMS - Install Script - Step1</h1>
		<p><i>&copy; Klemens Friedl 2005-2007, GNU GPL 2 license</i></p>
		<p>&nbsp;</p>
<?php
		if (get_magic_quotes_gpc()) {
			die("<p>ERROR: Disable 'magic quotes' in php.ini (=Off)</p>");
		}
		else {
?>
			<p><a href="install.php?ready=step2"><b>Next &gt;</b></a></p>
			<p>&nbsp;</p>
<?php
		}
	}
?>