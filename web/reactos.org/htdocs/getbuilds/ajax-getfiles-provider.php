<?php
/*
  PROJECT:    ReactOS Website
  LICENSE:    GPL v2 or any later version
  FILE:       web/reactos.org/htdocs/getbuilds/ajax-getfiles-provider.php
  PURPOSE:    Easily download prebuilt ReactOS Revisions
  COPYRIGHT:  Copyright 2007 Colin Finck <mail@colinfinck.de>
*/
   
	// This "ajax-getfiles.php" script has to be uploaded to the server, which contains the ISO files.
	// Therefore it has an own configuration and doesn't use "config.inc.php".

	// Configuration
	$ISO_DIR = ".";
	$MAX_FILES_PER_REV = 4;


	// Functions
	function fsize_str( $size )
	{
		if( $size > 1000000 )
		{
			$size = $size / 1048576;
			$unit = " MB";
		}
		else if( $size > 1000 )
		{
			$size = $size / 1024;
			$unit = " KB";
		}
		else
			$unit = " Bytes";
		
		return number_format( $size, 2, ".", ",") . $unit;
	}
	
	// Entry point
	if( !isset( $_GET["rev"] ) )
		die("No revision specified!");
	
	$revfiles = 0;
	$dir = opendir( $ISO_DIR ) or die("opendir failed!");
	header("Content-type: text/xml; charset=utf-8");
?>
<files>
<?php
	while( $fname = readdir($dir) )
	{
		if( strpos( $fname, "-" . $_GET["rev"] . "-" ) !== false )
		{
			$fsize = fsize_str( filesize( "$ISO_DIR/$fname" ) );
			$fdate = date( "Y-m-d H:i", filemtime( "$ISO_DIR/$fname" ) );
?>
<file>
	<name><?php echo $fname; ?></name>
	<size><?php echo $fsize; ?></size>
	<date><?php echo $fdate; ?></date>
</file>
<?php
			$revfiles++;
			
			if( $revfiles == $MAX_FILES_PER_REV )
				break;
		}
	}
	
	closedir($dir);
?>
</files>
