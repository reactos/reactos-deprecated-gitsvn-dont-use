<?php
/*
  PROJECT:    ReactOS Website
  LICENSE:    GPL v2 or any later version
  FILE:       web/reactos.org/htdocs/getbuilds/config.inc.php
  PURPOSE:    Easily download prebuilt ReactOS Revisions
  COPYRIGHT:  Copyright 2007 Colin Finck <mail@colinfinck.de>
*/
	
	// Configuration
	$AJAX_GETFILES_PROVIDER_URL = "http://svn.reactos.org/scripts/ajax-getfiles-provider.php";
	$ISO_DOWNLOAD_URL = "http://svn.reactos.org/iso/";
	$MAX_FILES_PER_PAGE = 100;			// The same value has to be set in "ajax-getfiles-provider.php"
	$SVN_ACTIVITY_URL = "http://svn.reactos.org/svnact/svn_activity.xml";
?>
