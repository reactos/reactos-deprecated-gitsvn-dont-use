<?php
/*
  PROJECT:    ReactOS Web Test Manager
  LICENSE:    GNU GPLv2 or any later version as published by the Free Software Foundation
  PURPOSE:    Class for submitting Wine Test results
  COPYRIGHT:  Copyright 2008-2009 Colin Finck <colin@reactos.org>
*/

	class WineTest implements Test
	{
		public function GetTestID()
		{
			global $dbh;
			global $user_id;
			
			if(!isset($_POST["revision"]) || !isset($_POST["platform"]))
				return "Necessary sub-information not specified!";
			
			// Add a new Test ID with the given information
			$stmt = $dbh->prepare("INSERT INTO " . DB_TESTMAN . ".winetest_runs (user_id, revision, platform) VALUES (:userid, :revision, :platform)");
			$stmt->bindParam(":userid", $user_id);
			$stmt->bindParam(":revision", $_POST["revision"]);
			$stmt->bindParam(":platform", $_POST["platform"]);
			$stmt->execute() or die("GetTestID(): SQL failed #1");
			
			return $dbh->lastInsertId();
		}
		
		public function GetSuiteID()
		{
			global $dbh;
			
			if(!isset($_POST["module"]) || !isset($_POST["test"]))
				return "Necessary sub-information not specified!";
			
			// Determine whether we already have a suite ID for this combination
			$stmt = $dbh->prepare("SELECT id FROM " . DB_TESTMAN . ".winetest_suites WHERE module = :module AND test = :test");
			$stmt->bindParam(":module", $_POST["module"]);
			$stmt->bindParam(":test", $_POST["test"]);
			$stmt->execute() or die("GetSuiteID(): SQL failed #1");
			$id = $stmt->fetchColumn();
			
			if($id)
				return $id;
			
			// Add this combination to the table and return the ID for it
			$stmt = $dbh->prepare("INSERT INTO " . DB_TESTMAN . ".winetest_suites (module, test) VALUES (:module, :test)");
			$stmt->bindParam(":module", $_POST["module"]);
			$stmt->bindParam(":test", $_POST["test"]);
			$stmt->execute() or die("GetSuiteID(): SQL failed #2");
			
			return $dbh->lastInsertId();
		}
		
		public function Submit()
		{
			global $dbh;
			global $user_id;
			
			if(!isset($_POST["testid"]) || !isset($_POST["suiteid"]) || !isset($_POST["log"]))
				return "Necessary sub-information not specified!";
			
			// Make sure we may add information to the test with this Test ID
			$stmt = $dbh->prepare("SELECT COUNT(*) FROM " . DB_TESTMAN . ".winetest_runs WHERE id = :testid AND finished = 0 AND user_id = :userid");
			$stmt->bindParam(":testid", $_POST["testid"]);
			$stmt->bindParam(":userid", $user_id);
			$stmt->execute() or die("Submit(): SQL failed #1");
			
			if(!$stmt->fetchColumn())
				return "No such test or no permissions!";
			
			// Validate and parse the log
			$line = strrchr($_POST["log"], ":");
			
			if(sscanf($line, ": %u tests executed (%u marked as todo, %u failures), %u skipped.", $count, $todo, $failures, $skipped) != 4)
				return "Log is invalid!";
			
			// Add the information into the DB
			$stmt = $dbh->prepare("INSERT INTO " . DB_TESTMAN . ".winetest_results (test_id, suite_id, log, count, todo, failures, skipped) VALUES (:testid, :suiteid, :log, :count, :todo, :failures, :skipped)");
			$stmt->bindValue(":testid", (int)$_POST["testid"]);
			$stmt->bindValue(":suiteid", (int)$_POST["suiteid"]);
			$stmt->bindParam(":log", $_POST["log"]);
			$stmt->bindParam(":count", $count);
			$stmt->bindParam(":todo", $todo);
			$stmt->bindParam(":failures", $failures);
			$stmt->bindParam(":skipped", $skipped);
			$stmt->execute() or die("Submit(): SQL failed #2");
			
			return "OK";
		}
		
		public function Finish()
		{
			global $dbh;
			global $user_id;
			
			if(!isset($_POST["testid"]))
				return "Necessary sub-information not specified!";
			
			// Mark this test as finished, so no more results can be submitted for it
			$stmt = $dbh->prepare("UPDATE " . DB_TESTMAN . ".winetest_runs SET finished = 1 WHERE id = :testid AND user_id = :userid");
			$stmt->bindParam(":userid", $user_id);
			$stmt->bindParam(":testid", $_POST["testid"]);
			$stmt->execute() or die("Finish(): SQL failed #1");
			
			if(!$stmt->rowCount())
				return "Did not update anything!";
			
			return "OK";
		}
	}
?>
