 <div class="contentSmall"> 
  <p><span class="contentSmallTitle">Editor Test Page</span> </p>
  <p><strong>Rich Text Editors </strong></p>
  <ul>
    <li><strong>FCKeditor</strong> (GNU GPL): <a href="http://www.fckeditor.net/" target="_blank">download</a> 
      <ul>
        <li>many configuration settings</li>
        <li>different languages available (auto detect)</li>
        <li>see example 1 &amp; 2 (basic and default toolbar settings) </li>
      </ul>
    </li>
  </ul>
</div
>

<div class="contentSmall">
  <p><span class="contentSmallTitle">Rich Text Editor Example 1 (for normal user 
    (compatibility/packages section; comment function), translators)</span></p>
<!-- START Demo 2 Code -->

<?php
	include("../editor/FC/fckeditor.php") ;
	// Automatically calculates the editor base path based on the _samples directory.
	// This is usefull only for these samples. A real application should use something like this:
	// $oFCKeditor->BasePath = '/FCKeditor/' ;	// '/FCKeditor/' is the default value.
	$sBasePath = $_SERVER['PHP_SELF'] ;
	$sBasePath = substr( $sBasePath, 0, strpos( $sBasePath, "_samples" ) ) ;
	
	$oFCKeditor = new FCKeditor('FCKeditor') ;
	//$oFCKeditor->BasePath	= $sBasePath ;
	$oFCKeditor->ToolbarSet = "Basic";
	$oFCKeditor->BasePath = '../editor/FC/';
	
	$oFCKeditor->Value		= '<p><center><b>RosCMS System</b></center></p><p>Rich Text Editor Example 2.</p><p><a href="http://www.reactos.org" target="_blank">http://www.reactos.org</a></p>' ;
	$oFCKeditor->Create() ;
?>
<!-- END Demo 2 Code -->

</div>
<div class="contentSmall">
  <p><span class="contentSmallTitle">Rich Text Editor Example 2 (for admins)</span></p>
  <!-- START Demo 3 Code -->
  <?php


	$oFCKeditor = new FCKeditor('FCKeditora') ;
	$oFCKeditor->BasePath = '../editor/FC/';
	$oFCKeditor->Value		= '<p><center><b>RosCMS System</b></center></p><p>Rich Text Editor Example 3.</p><p><a href="http://www.reactos.org" target="_blank">http://www.reactos.org</a></p>' ;
	$oFCKeditor->Create() ;
?>
  <!-- END Demo 3 Code -->
</div>

