<?php
/*
  PROJECT:    ReactOS Web Test Manager
  LICENSE:    GNU GPLv2 or any later version as published by the Free Software Foundation
  PURPOSE:    Web Service for receiving test results from "rosautotest"
  COPYRIGHT:  Copyright 2008-2009 Colin Finck <colin@reactos.org>
*/

	require_once("../config.inc.php");
	
	// All classes are autoloaded through this magic function
	function __autoload($class)
	{
		require_once("lib/$class.class.php");
	}
	
	// What one of these classes has to look like
	interface Test
	{
		public function GetTestID();
		public function Submit();
		public function Finish();
	}
	
	
	// Entry point
	if(!isset($_POST["username"]) || !isset($_POST["password"]) || !isset($_POST["testtype"]))
		die("Necessary information not specified!");

	// Check the login credentials
	try
	{
		$dbh = new PDO("mysql:host=" . DB_HOST, DB_USER, DB_PASS);
	}
	catch(PDOException $e)
	{
		// Give no exact error message here, so no server internals are exposed
		die("Could not establish the DB connection");
	}
		
	$stmt = $dbh->prepare("SELECT user_id FROM " . DB_ROSCMS . ".users WHERE user_name = :username AND user_roscms_password = MD5(:password) AND user_account_enabled = 'yes'");
	$stmt->bindParam(":username", $_POST["username"]);
	$stmt->bindParam(":password", $_POST["password"]);
	$stmt->execute() or die("SQL failed #1");
	$user_id = (int)$stmt->fetchColumn();
	
	if(!$user_id)
		die("Invalid Login credentials!");
	
	// Check if the user is permitted to submit test results
	$stmt = $dbh->prepare("SELECT COUNT(*) FROM " . DB_TESTMAN . ".permitted_users WHERE user_id = :userid");
	$stmt->bindParam(":userid", $user_id);
	$stmt->execute() or die("SQL failed #2");
	
	if(!$stmt->fetchColumn())
		die("User is not permitted to submit test results");
	
	switch($_POST["testtype"])
	{
		case "wine":
			$t = new WineTest();
			break;
		
		default:
			die("Invalid test type!");
	}
	
	// What shall we do?
	switch($_POST["action"])
	{
		case "gettestid":  die($t->GetTestID());
		case "getsuiteid": die($t->GetSuiteID());
		case "submit":     die($t->Submit());
		case "finish":     die($t->Finish());
		
		default:
			die("Invalid action");
	}
?>
