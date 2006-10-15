<?php
/**
 * RosCms nouveau
 *
 * Translated from gwicke's previous TAL template version to remove
 * dependency on PHPTAL.
 *
 * @todo document
 * @package MediaWiki
 * @subpackage Skins
 */

if( !defined( 'MEDIAWIKI' ) )
	die();

/** */
require_once('includes/SkinTemplate.php');

/**
 * Inherit main code from SkinTemplate, set the CSS and template filter.
 * @todo document
 * @package MediaWiki
 * @subpackage Skins
 */
class SkinRosCms extends SkinTemplate {
	/** Using roscms. */
	function initPage( &$out ) {
		SkinTemplate::initPage( $out );
		$this->skinname  = 'roscms';
		$this->stylename = 'roscms';
		$this->template  = 'RosCmsTemplate';
	}
}
	
class RosCmsTemplate extends QuickTemplate {
	/**
	 * Template filter callback for RosCms skin.
	 * Takes an associative array of data set from a SkinTemplate-based
	 * class, and a wrapper for MediaWiki's localization database, and
	 * outputs a formatted page.
	 *
	 * @access private
	 */
	function execute() {
		global $wgUser, $wgTitle;
echo "<?xml version='1.0'?>\n";?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php $this->text('lang') ?>" lang="<?php $this->text('lang') ?>" dir="<?php $this->text('dir') ?>">
  <head>
    <meta http-equiv="Content-Type" content="<?php $this->text('mimetype') ?>; charset=<?php $this->text('charset') ?>" />
    <?php $this->html('headlinks') ?>
    <title><?php $this->text('pagetitle') ?></title>
    <link href="/style.css" type="text/css" rel="stylesheet" />
    <style type="text/css" media="screen,projection">/*<![CDATA[*/ @import "<?php $this->text('stylepath') ?>/<?php $this->text('stylename') ?>/main.css"; /*]]>*/</style>
    <link rel="stylesheet" type="text/css" media="print" href="<?php $this->text('stylepath') ?>/common/commonPrint.css" />
    <!--[if lt IE 5.5000]><style type="text/css">@import "<?php $this->text('stylepath') ?>/<?php $this->text('stylename') ?>/IE50Fixes.css";</style><![endif]-->
    <!--[if IE 5.5000]><style type="text/css">@import "<?php $this->text('stylepath') ?>/<?php $this->text('stylename') ?>/IE55Fixes.css";</style><![endif]-->
    <!--[if gte IE 6]><style type="text/css">@import "<?php $this->text('stylepath') ?>/<?php $this->text('stylename') ?>/IE60Fixes.css";</style><![endif]-->
    <!--[if IE]><script type="text/javascript" src="<?php $this->text('stylepath') ?>/common/IEFixes.js"></script>
    <meta http-equiv="imagetoolbar" content="no" /><![endif]-->
    <?php if($this->data['jsvarurl'  ]) { ?><script type="text/javascript" src="<?php $this->text('jsvarurl'  ) ?>"></script><?php } ?>
    <script type="text/javascript" src="<?php                                   $this->text('stylepath' ) ?>/common/wikibits.js"></script>
    <?php if($this->data['usercss'   ]) { ?><style type="text/css"><?php              $this->html('usercss'   ) ?></style><?php    } ?>
    <?php if($this->data['userjs'    ]) { ?><script type="text/javascript" src="<?php $this->text('userjs'    ) ?>"></script><?php } ?>
    <?php if($this->data['userjsprev']) { ?><script type="text/javascript"><?php      $this->html('userjsprev') ?></script><?php   } ?>
  </head>
  <body <?php if($this->data['body_ondblclick']) { ?>ondblclick="<?php $this->text('body_ondblclick') ?>"<?php } ?>
        <?php if($this->data['nsclass'        ]) { ?>class="<?php      $this->text('nsclass')         ?>"<?php } ?>>
<div id="top">
  <div id="topMenu"> 
    <!-- 
       Use <p> to align things for links/lynx, then in the css make it
	   margin: 0; and use text-align: left/right/etc;.
   -->
	<p align="center"> 
		<a href="/?page=index">Home</a> <font color="#ffffff">|</font> 
		<a href="/?page=about">Info</a> <font color="#ffffff">|</font> 
		<a href="/?page=community">Community</a> <font color="#ffffff">|</font> 
		<a href="/?page=dev">Development</a> <font color="#ffffff">|</font> 
		<a href="/roscms/?page=user">myReactOS</a> </p>
	 </div>
	</div>

<table border="0" width="100%" cellpadding="0" cellspacing="0">
 <tr valign="top">
    <td width="147" id="leftNav"> 
      <div class="navTitle">Navigation</div>
   <ol>
        <li><a href="/?page=index">Home</a></li>
        <li><a href="/?page=about">Info</a></li>
        <li><a href="/?page=community">Community</a></li>
        <li><a href="/?page=dev">Development</a></li>
	<li><a href="/roscms/?page=user">myReactOS</a></li>
      </ol>
   <p></p>

<div class="navTitle">Wiki</div>
      <ol>
        <?php foreach ($this->data['sidebar'] as $bar => $cont) { ?>
            <?php foreach($cont as $key => $val) { ?>
              <li><a href="<?php echo htmlspecialchars($val['href']) ?>"><?php
                      echo htmlspecialchars($val['text'])?></a></li>
             <?php } ?>
        <?php } ?>
      </ol>
      <p></p>

<div class="navTitle">Wiki Toolbox</div>
  <ol>
    <?php if($this->data['notspecialpage']) { foreach( array( 'whatlinkshere', 'recentchangeslinked' ) as $special ) { ?>
      <li><a href="<?php
             echo htmlspecialchars($this->data['nav_urls'][$special]['href'])
                  ?>"><?php echo $this->msg($special) ?></a></li>
    <?php } } ?>
    <?php if(/*$this->data['feeds'] Let's not do this now*/ 0) { ?>
      <li>
        <?php foreach($this->data['feeds'] as $key => $feed) { ?>
          <span id="feed-<?php echo htmlspecialchars($key) ?>"><a href="<?php
                  echo htmlspecialchars($feed['href']) ?>">
            <?php echo htmlspecialchars($feed['text'])?></a>&nbsp;
          </span>
        <?php } ?>
      </li>
    <?php } ?>
    <?php foreach( array('contributions', 'emailuser', 'upload', 'specialpages', 'preferences') as $special ) { ?>
      <?php if($this->data['nav_urls'][$special]) {?>
        <li>
          <a href="<?php
                echo htmlspecialchars($this->data['nav_urls'][$special]['href'])
                ?>"><?php $this->msg($special) ?></a>
        </li>
      <?php } ?>
    <?php } ?>

  </ol>
  <p></p>

<form name="searchform" action="/wiki/index.php/Special:Search" id="searchform">
 <div class="navTitle">Search</div>   
 <div class="navBox">
  <div style="text-align:center;">
   <input id="searchInput" name="search" type="text"
     accesskey="f" value="" />
   <input type='submit' name="go" class="searchButton" id="searchGoButton"
     value="Go"
     />&nbsp;<input type='submit' name="fulltext"
     class="searchButton"
     value="Search" />
  </div>
 </div>
</form>

<div class="navTitle">Account</div>
      <ol>
        <?php if (0 != $wgUser->getID()) { ?>
          <li><a href="/roscms/?page=logout">Logout <?php echo $wgUser->getName() ?></a></li>
        <?php } else { ?>
          <li><a href="/roscms/?page=login&amp;target=<?php echo urlencode($wgTitle->getLocalURL()); ?>">Login</a></li>
        <?php } ?>
        <li><a href="/roscms/?page=register&amp;target=<?php echo urlencode($wgTitle->getLocalURL()); ?>">Register</a></li>
        <?php if (0 != $wgUser->getID()) { ?>
          <li><a href="/wiki/index.php/Special:Preferences">Preferences</a></li>
        <?php } ?>
      </ol>
      <p></p>
      </td>

<td id="wikiContent">
<div class="contentSmall">
<div class="contentSmallTitle"><a class="roscmscontent" href="http://www.reactos.org/xhtml/en/community.html">ReactOS Community</a> &gt; ReactOS Wiki</span>
</div>
    <div id="globalWrapper">
      <div id="column-content">
	<div id="content">
	  <a name="top" id="contentTop"></a>
	  <?php if($this->data['sitenotice']) { ?><div id="siteNotice"><?php $this->html('sitenotice') ?></div><?php } ?>
	  <h1 class="firstHeading"><?php $this->text('title') ?></h1>
	  <div id="bodyContent">
	    <h3 id="siteSub"><?php $this->msg('tagline') ?></h3>
	    <div id="contentSub"><?php $this->html('subtitle') ?></div>
	    <?php if($this->data['undelete']) { ?><div id="contentSub"><?php     $this->html('undelete') ?></div><?php } ?>
	    <?php if($this->data['newtalk'] ) { ?><div class="usermessage"><?php $this->html('newtalk')  ?></div><?php } ?>
	    <!-- start content -->
	    <?php $this->html('bodytext') ?>
	    <?php if($this->data['catlinks']) { ?><div id="catlinks"><?php       $this->html('catlinks') ?></div><?php } ?>
	    <!-- end content -->
	    <div class="visualClear"></div>
	  </div>
	</div>
      </div>
      <div id="column-one">
	<div id="p-cactions" class="portlet">
	  <h5>Views</h5>
	  <ul>
	    <?php foreach($this->data['content_actions'] as $key => $action) {
	       ?><li id="ca-<?php echo htmlspecialchars($key) ?>"
	       <?php if($action['class']) { ?>class="<?php echo htmlspecialchars($action['class']) ?>"<?php } ?>
	       ><a href="<?php echo htmlspecialchars($action['href']) ?>"><?php
	       echo htmlspecialchars($action['text']) ?></a></li><?php
	     } ?>
	  </ul>
	</div>
	<script type="text/javascript"> if (window.isMSIE55) fixalpha(); </script>
      </div><!-- end of the left (by default at least) column -->
      <div class="visualClear"></div>
      <div id="footer">
    <?php if($this->data['poweredbyico']) { ?><div id="f-poweredbyico"><?php $this->html('poweredbyico') ?></div><?php } ?>
	<?php if($this->data['copyrightico']) { ?><div id="f-copyrightico"><?php $this->html('copyrightico') ?></div><?php } ?>
	<ul id="f-list">
	  <?php if($this->data['lastmod'   ]) { ?><li id="f-lastmod"><?php    $this->html('lastmod')    ?></li><?php } ?>
	  <?php if($this->data['viewcount' ]) { ?><li id="f-viewcount"><?php  $this->html('viewcount')  ?></li><?php } ?>
	  <?php if($this->data['credits'   ]) { ?><li id="f-credits"><?php    $this->html('credits')    ?></li><?php } ?>
	  <?php if($this->data['copyright' ]) { ?><li id="f-copyright"><?php  $this->html('copyright')  ?></li><?php } ?>
	  <?php if($this->data['about'     ]) { ?><li id="f-about"><?php      $this->html('about')      ?></li><?php } ?>
	  <?php if($this->data['disclaimer']) { ?><li id="f-disclaimer"><?php $this->html('disclaimer') ?></li><?php } ?>
	</ul>
      </div>
    </div>
    <?php $this->html('reporttime') ?>
</td>
</tr>
</table>

<!--
     links/lynx/etc.. dont handle css (atleast not external
     files by default) so dont overly depend on it.
 -->
<hr size="1"/>

<address>
 <p align="center">
  ReactOS is a trademark of ReactOS Foundation in the United States and other countries.
 </p>
</address>
  </body>
</html>
<?php
	}
}

?>
