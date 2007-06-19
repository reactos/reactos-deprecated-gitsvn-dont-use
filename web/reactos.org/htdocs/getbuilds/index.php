<?php
/*
  PROJECT:    ReactOS Website
  LICENSE:    GPL v2 or any later version
  FILE:       web/reactos.org/htdocs/getbuilds/index.php
  PURPOSE:    Easily download prebuilt ReactOS Revisions
  COPYRIGHT:  Copyright 2007 Colin Finck <mail@colinfinck.de>
*/
	
	require_once("config.inc.php");
	require_once("lang/en.inc.php");
	
	// Get the latest SVN revision
	$fp = fopen( $SVN_ACTIVITY_URL, "r" );
	
	do
	{
		$line = fread( $fp, 1024 );
		
		$firstpos = strpos( $line, "<id>" );
		
		if( $firstpos > 0 )
		{
			$lastpos = strpos( $line, "</id>" );
			$rev = substr( $line, $firstpos + 4, ($lastpos - $firstpos - 4) );
			break;
		}
	} while( $line != "" );
	
	fclose( $fp );
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
	<meta http-equiv="content-type" content="text/html; charset=utf-8">
	<title><?php echo $getbuilds_langres["title"]; ?></title>
	<link rel="stylesheet" type="text/css" href="getbuilds.css">
	<script type="text/javascript">
	<?php require_once("getbuilds.js.php"); ?>
	</script>
</head>
<body onload="ajaxGet('getfiles', 'rev=<?php echo $rev; ?>', <?php echo $rev; ?>);">

<h2><?php echo $getbuilds_langres["title"]; ?></h2>

<div class="bubble_bg">
	<div class="rounded_ll">
	<div class="rounded_lr">
	<div class="rounded_ul">
	<div class="rounded_ur">
	
	<div class="bubble">
		<h3><?php echo $getbuilds_langres["overview"]; ?></h3>
		
		<?php echo $getbuilds_langres["latestrev"]; ?>: <strong><?php echo $rev; ?></strong>
		<ul class="web">
			<li><a href="http://svn.reactos.org/svn/reactos"><?php echo $getbuilds_langres["browsesvn"]; ?></a></li>
			<li><a href="http://cia.vc/stats/project/ReactOS"><?php echo $getbuilds_langres["cia"]; ?></a></li>
		</ul>
		
		<?php echo $getbuilds_langres["buildbot_status"]; ?>: Not yet implemented
		<ul class="web">
			<li><a href="http://www.reactos.org:8010"><?php echo $getbuilds_langres["buildbot_web"]; ?></a></li>
			<li><a href="http://svn.reactos.org/iso"><?php echo $getbuilds_langres["browsebuilds"]; ?></a></li>
		</ul>
	</div>

	</div>
	</div>
	</div>
	</div>
</div>

<div class="bubble_bg">
	<div class="rounded_ll">
	<div class="rounded_lr">
	<div class="rounded_ul">
	<div class="rounded_ur">
	
	<div class="bubble">
		<h3><?php echo $getbuilds_langres["downloadrev"]; ?></h3>
		
		<noscript>
			<?php echo $getbuilds_langres["js_disclaimer"]; ?>
		</noscript>
		
		<script type="text/javascript">
			document.write(
				'<div id="showrev">' +
					'<?php echo $getbuilds_langres["showrevfiles"]; ?>: ' +
					'<img src="images/leftarrow.gif" alt="&lt;" title="<?php echo $getbuilds_langres["prevrev"]; ?>" onclick="prevrev();"> ' +
					'<input type="text" id="revnum" value="<?php echo $rev; ?>" size="6" onkeyup="checkrevnum(this);"> ' +
					'<img src="images/rightarrow.gif" alt="&gt;" title="<?php echo $getbuilds_langres["nextrev"]; ?>" onclick="nextrev();">&nbsp;&nbsp; ' +
					
					'<button type="submit" onclick="showrev();"><strong><?php echo $getbuilds_langres["showrev"]; ?></strong></button>' +
					
					'<span id="ajaxloadinginfo">' +
						'<img src="images/ajax_loading.gif"> <strong><?php echo $getbuilds_langres["gettinglist"]; ?>...</strong>' +
					'</span>' +
				'</div>' +
				
				'<div id="filetable">' +
					'<table class="datatable" cellspacing="0" cellpadding="1">' +
						'<thead>' +
							'<tr class="head">' +
								'<th class="fname"><?php echo $getbuilds_langres["filename"]; ?></th>' +
								'<th class="fsize"><?php echo $getbuilds_langres["filesize"]; ?></th>' +
								'<th class="fdate"><?php echo $getbuilds_langres["filedate"]; ?></th>' +
							'</tr>' +
						'</thead>' +
						'<tbody>' +
							'<tr class="odd">' +
								'<td><?php echo $getbuilds_langres["pleasewait"]; ?>...</td>' +
								'<td>&nbsp;</td>' +
								'<td>&nbsp;</td>' +
							'</tr>' +
						'</tbody>' +
					'</table>' +
				'</div>'
			);
		</script>
	</div>
	
	</div>
	</div>
	</div>
	</div>
</div>

</body>
</html>
