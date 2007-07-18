<?php
	/* RosCMS install script */

	// This function was taken from the sourceStream() function of "Database.php" of MediaWiki 1.10.0 and modified
	function import_sql_dump( $file )
	{
		$ret = TRUE;
		$fp = fopen( $file, "r" );
		
		while( !feof( $fp ) )
		{
			$line = trim( fgets( $fp, 1024 ) );
			$sl = strlen( $line ) - 1;
			
			// Skip empty and comment lines
			if( $sl < 0 || ( $line{0} == "-" && $line{1} == "-" ) )
				continue;
			
			// If this lines ends with a semicolon, it terminates the command
			if( $line{$sl} == ";" )
			{
				$done = true;
				$line = substr( $line, 0, $sl );
			}
			
			$cmd .= "$line\n";
			
			if( $done )
			{
				if( !mysql_query( $cmd ) )
				{
					$ret = FALSE;
					echo "<br>" . mysql_errno() . ": " . mysql_error();
				}
				
				$done = false;
				$cmd = "";
			}
		}
		
		fclose( $fp );
		
		if( $ret )
			echo "OK";
		
		return $ret;
	}

	$rpm_ready = "";
	if (array_key_exists("ready", $_GET)) $rpm_ready=htmlspecialchars($_GET["ready"]);
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd">
<html>
<head>
	<title>RosCMS Install Script</title>
</head>
<body>
<?php
	if ($rpm_ready == "step2") {
?>
	<h1>RosCMS - Install Script - Step2</h1>
	<form id="form1" method="post" action="install.php?ready=step3">
		<h3>Database Information</h3>
		
		<table>
			<tr>
				<td>Database Host Name:</td>
				<td><input name="dbHost" type="text" id="dbHost" value="localhost" /></td>
			</tr>
			<tr>
				<td>Database User Name:</td>
				<td><input name="dbUser" type="text" id="dbUser" value="root" /></td>
			</tr>
			<tr>
				<td>Database User Password:</td>
				<td><input name="dbPass" type="text" id="dbPass" /></td>
			</tr>
			<tr>
				<td>Database  Name:</td>
				<td><input name="dbName" type="text" id="dbName" value="domainname" /></td>
			</tr>
		</table>
		
		<h3>Server Information</h3>
		<p>Complete Server Path: <input name="path_server" type="text" id="path_server" value="http://localhost/domain.com/" size="50" /></p>
		
		<p>&nbsp;</p>
		
		<input type="submit" name="Submit" value="Install RosCMS ..." />
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

		
		echo "Connecting to the MySQL Server... ";		
		$connect = mysql_connect($dbHost, $dbUser, $dbPass) or die("Cannot connect to the MySQL Server!");
		echo "OK<br>";
		
		echo "Creating the Database... ";
		mysql_query("CREATE DATABASE `". mysql_real_escape_string($dbName) ."`") or die( mysql_errno() . ": " . mysql_error() );
		echo "OK<br>";
		
		echo "Selecting the Database... ";
		mysql_select_db($dbName, $connect) or die("Cannot find and select <b>$dbName</b>!");
		echo "OK<br>";
		
		echo "Importing the SQL Data... ";
		if( !import_sql_dump( "roscms.sql" ) )
			exit;
		
		/* TODO: The complete server path has to be saved somewhere */
		
		echo "<p>&nbsp;</p>";
		echo "<p><b>RosCMS was set up successfully!</b></p>";
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
?>
		<p>Your PHP installation is configured correctly. Click <i>Next</i> to continue!</p>
		
		<p><a href="install.php?ready=step2"><b>Next &gt;</b></a></p>
<?php
	}
?>
</body>
</html>