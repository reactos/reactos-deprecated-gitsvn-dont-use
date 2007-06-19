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
	
	function ArraySort(&$array, $elem)
	{
		global $sort_elem;
		
		$sort_elem = $elem;
		usort( $array, "ArraySort_callback" );
	}
	
	function ArraySort_callback($a, $b)
	{
		global $sort_elem;
		
		return strnatcasecmp( $a[$sort_elem], $b[$sort_elem] );
	}
	
	
	// Entry point
	if( !isset( $_GET["from"] ) || !isset( $_GET["to"] ) )
		die("No revision range specified!");
	
	$files = array();
	$i = 0;
	$dir = opendir( $ISO_DIR ) or die("opendir failed!");
?>
<files>
<?php
	while( $fname = readdir($dir) )
	{
		for( $j = $_GET["from"]; $j <= $_GET["to"]; $j++ )
		{
			if( strpos( $fname, "-$j-" ) !== false )
			{
				$files[$i]["name"] = $fname;
				$files[$i]["size"] = fsize_str( filesize( "$ISO_DIR/$fname" ) );
				$files[$i]["date"] = date( "Y-m-d H:i", filemtime( "$ISO_DIR/$fname" ) );
				
				$i++;
				break;
			}
		}
	}
	
	closedir($dir);
	ArraySort( $files, "name" );
	
	for( $j = 0; $j < $i; $j++ )
	{
?>
<file>
	<name><?php echo $files[$j]["name"]; ?></name>
	<size><?php echo $files[$j]["size"]; ?></size>
	<date><?php echo $files[$j]["date"]; ?></date>
</file>
<?php
	}
?>
</files>
