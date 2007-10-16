<?php
/**
 * RosCMS Skin
 *
 * Translated from gwicke's previous TAL template version to remove
 * dependency on PHPTAL.
 *
 * @todo document
 * @addtogroup Skins
 */

if( !defined( 'MEDIAWIKI' ) )
	die( -1 );

/** */
require_once('includes/SkinTemplate.php');

/**
 * Inherit main code from SkinTemplate, set the CSS and template filter.
 * @todo document
 * @addtogroup Skins
 */
class SkinRosCMS extends SkinTemplate {
	/** Using roscms. */
	function initPage( &$out ) {
		SkinTemplate::initPage( $out );
		$this->skinname  = 'roscms';
		$this->stylename = 'roscms';
		$this->template  = 'RosCMSTemplate';
	}
}

/**
 * @todo document
 * @addtogroup Skins
 */
class RosCMSTemplate extends QuickTemplate {
	/**
	 * Template filter callback for MonoBook skin.
	 * Takes an associative array of data set from a SkinTemplate-based
	 * class, and a wrapper for MediaWiki's localization database, and
	 * outputs a formatted page.
	 *
	 * @access private
	 */
	function execute() {
		global $wgUser, $wgTitle;
		$skin = $wgUser->getSkin();

		// Suppress warnings to prevent notices about missing indexes in $this->data
		wfSuppressWarnings();

?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="<?php $this->text('xhtmldefaultnamespace') ?>" <?php 
	foreach($this->data['xhtmlnamespaces'] as $tag => $ns) {
		?>xmlns:<?php echo "{$tag}=\"{$ns}\" ";
	} ?>xml:lang="<?php $this->text('lang') ?>" lang="<?php $this->text('lang') ?>" dir="<?php $this->text('dir') ?>">
	<head>
		<meta http-equiv="Content-Type" content="<?php $this->text('mimetype') ?>; charset=<?php $this->text('charset') ?>" />
		<?php $this->html('headlinks') ?>
		<title><?php $this->text('pagetitle') ?></title>
		<link href="/style.css" type="text/css" rel="stylesheet" />
		<style type="text/css" media="screen,projection">/*<![CDATA[*/ @import "<?php $this->text('stylepath') ?>/<?php $this->text('stylename') ?>/main.css?<?php echo $GLOBALS['wgStyleVersion'] ?>"; /*]]>*/</style>
		<link rel="stylesheet" type="text/css" <?php if(empty($this->data['printable']) ) { ?>media="print"<?php } ?> href="<?php $this->text('stylepath') ?>/common/commonPrint.css?<?php echo $GLOBALS['wgStyleVersion'] ?>" />
		<link rel="stylesheet" type="text/css" media="handheld" href="<?php $this->text('stylepath') ?>/<?php $this->text('stylename') ?>/handheld.css?<?php echo $GLOBALS['wgStyleVersion'] ?>" />
		<!--[if lt IE 5.5000]><style type="text/css">@import "<?php $this->text('stylepath') ?>/<?php $this->text('stylename') ?>/IE50Fixes.css?<?php echo $GLOBALS['wgStyleVersion'] ?>";</style><![endif]-->
		<!--[if IE 5.5000]><style type="text/css">@import "<?php $this->text('stylepath') ?>/<?php $this->text('stylename') ?>/IE55Fixes.css?<?php echo $GLOBALS['wgStyleVersion'] ?>";</style><![endif]-->
		<!--[if IE 6]><style type="text/css">@import "<?php $this->text('stylepath') ?>/<?php $this->text('stylename') ?>/IE60Fixes.css?<?php echo $GLOBALS['wgStyleVersion'] ?>";</style><![endif]-->
		<!--[if IE 7]><style type="text/css">@import "<?php $this->text('stylepath') ?>/<?php $this->text('stylename') ?>/IE70Fixes.css?<?php echo $GLOBALS['wgStyleVersion'] ?>";</style><![endif]-->
		<!--[if lt IE 7]><script type="<?php $this->text('jsmimetype') ?>" src="<?php $this->text('stylepath') ?>/common/IEFixes.js?<?php echo $GLOBALS['wgStyleVersion'] ?>"></script>
		<meta http-equiv="imagetoolbar" content="no" /><![endif]-->
		
		<?php print Skin::makeGlobalVariablesScript( $this->data ); ?>
                
		<script type="<?php $this->text('jsmimetype') ?>" src="<?php $this->text('stylepath' ) ?>/common/wikibits.js?<?php echo $GLOBALS['wgStyleVersion'] ?>"><!-- wikibits js --></script>
<?php	if($this->data['jsvarurl'  ]) { ?>
		<script type="<?php $this->text('jsmimetype') ?>" src="<?php $this->text('jsvarurl'  ) ?>"><!-- site js --></script>
<?php	} ?>
<?php	if($this->data['pagecss'   ]) { ?>
		<style type="text/css"><?php $this->html('pagecss'   ) ?></style>
<?php	}
		if($this->data['usercss'   ]) { ?>
		<style type="text/css"><?php $this->html('usercss'   ) ?></style>
<?php	}
		if($this->data['userjs'    ]) { ?>
		<script type="<?php $this->text('jsmimetype') ?>" src="<?php $this->text('userjs' ) ?>"></script>
<?php	}
		if($this->data['userjsprev']) { ?>
		<script type="<?php $this->text('jsmimetype') ?>"><?php $this->html('userjsprev') ?></script>
<?php	}
		if($this->data['trackbackhtml']) print $this->data['trackbackhtml']; ?>
		<!-- Head Scripts -->
<?php $this->html('headscripts') ?>
	</head>
<body <?php if($this->data['body_ondblclick']) { echo "ondblclick=\"". $this->text('body_ondblclick') ."\""; } ?>
<?php if($this->data['body_onload']) { echo "onload=\"". $this->text('body_onload') ."\""; } ?>
 class="mediawiki <?php $this->text('nsclass') ?> <?php $this->text('dir') ?> <?php $this->text('pageclass') ?>">

<?php
	$userlang = $_COOKIE["roscms_usrset_lang"];
	if(strlen($userlang) != 2)
		$userlang = "en";
	
	readfile("http://www.reactos.org/$userlang/subsys_extern_menu_top.html");
	readfile("http://www.reactos.org/$userlang/subsys_extern_menu_left.html");
?>

			<div class="navTitle">Wiki</div>
			<ol>
				<?php foreach ($this->data['sidebar'] as $bar => $cont) { ?>
				<?php foreach($cont as $key => $val) { ?>
				<li><a href="<?php echo htmlspecialchars($val['href']) ?>"><?php echo htmlspecialchars($val['text'])?></a></li>
				<?php } ?>
				<?php } ?>
			</ol>
			<p></p>
			
			<div class="navTitle">Wiki Toolbox</div>
			<ol>
				<?php if($this->data['notspecialpage']) { foreach( array( 'whatlinkshere', 'recentchangeslinked', 'permalink' ) as $special ) { ?>
					<li><a href="<?php echo htmlspecialchars($this->data['nav_urls'][$special]['href']) ?>"><?php echo $this->msg($special) ?></a></li>
					<?php } } ?>
					<?php if(/*$this->data['feeds'] Let's not do this now*/ 0) { ?>
						<li>
							<?php foreach($this->data['feeds'] as $key => $feed) { ?>
							<span id="feed-<?php echo htmlspecialchars($key) ?>">
								<a href="<?php echo htmlspecialchars($feed['href']) ?>">
								<?php echo htmlspecialchars($feed['text'])?></a>&nbsp;
							</span>
							<?php } ?>
						</li>
					<?php } ?>
					<?php foreach( array('contributions', 'emailuser', 'upload', 'specialpages', 'preferences') as $special ) { ?>
					<?php if($this->data['nav_urls'][$special]) {?>
						<li>
							<a href="<?php echo htmlspecialchars($this->data['nav_urls'][$special]['href']) ?>">
							<?php $this->msg($special) ?></a>
						</li>
					<?php } ?>
				<?php } ?>
			</ol>
			<p></p>

			<form name="searchform" action="/wiki/index.php/Special:Search" id="searchform">
				<div class="navTitle"><?php $this->msg('search'); ?></div> 
				<div class="navBox">
					<div style="text-align:center;">
						<input id="searchInput" name="search" type="text" accesskey="f" value="" />
						<input type='submit' name="go" class="searchButton" id="searchGoButton" value="<?php $this->msg('searcharticle'); ?>" />&nbsp;
						<input type='submit' name="fulltext" class="searchButton" value="<?php $this->msg('searchbutton'); ?>" />
					</div>
				</div>
			</form>
			<p></p>

			<div class="navTitle">Account</div>
			<ol>
				<?php if (0 != $wgUser->getID()) { ?>
					<li><a href="/roscms/?page=logout">Logout <?php echo $wgUser->getName() ?></a></li>
					<li><a href="/wiki/index.php/User:<?php echo $wgUser->getName(); ?>"><?php $this->msg('mypage'); ?></a></li>
					<li><a href="/wiki/index.php/User talk:<?php echo $wgUser->getName(); ?>"><?php $this->msg('mytalk'); ?></a></li>
					<li><a href="/wiki/index.php/Special:Preferences"><?php $this->msg('preferences'); ?></a></li>
					<li><a href="/wiki/index.php/Special:Watchlist"><?php $this->msg('watchlist'); ?></a></li>
					<li><a href="/wiki/index.php/Special:Contributions/<?php echo $wgUser->getName(); ?>"><?php $this->msg('mycontris'); ?></a></li>
				<?php } else { ?>
					<li><a href="/roscms/?page=login&amp;target=<?php echo urlencode($wgTitle->getLocalURL()); ?>">Login</a></li>
					<li><a href="/roscms/?page=register&amp;target=<?php echo urlencode($wgTitle->getLocalURL()); ?>">Register</a></li>
				<?php } ?>
			</ol>
			<p></p>
			
			<?php if( $this->data['language_urls'] ) { ?>
				<div class="navTitle"><?php $this->msg('otherlanguages'); ?></div>
				<ol>
					<?php foreach($this->data['language_urls'] as $langlink) { ?>
						<li><a href="<?php echo htmlspecialchars($langlink['href']) ?>"><?php echo $langlink['text'] ?></a></li>
					<?php } ?>
				</ol>
				<p></p>
			<?php } ?>
			
			<!-- Google AdSense - start -->
			<script type="text/javascript"><!--
			google_ad_client = "pub-8424466656027272";
			google_ad_width = 120;
			google_ad_height = 600;
			google_ad_format = "120x600_as";
			google_ad_type = "text";
			google_ad_channel ="899341706";
			google_color_border = "5984C3";
			google_color_bg = "EEEEEE";
			google_color_link = "000000";
			google_color_text = "000000";
			google_color_url = "006090";
			//--></script>
			<script type="text/javascript" src="http://pagead2.googlesyndication.com/pagead/show_ads.js"></script>
			<!-- Google AdSense - end -->
		</td>
		<td>
			<div class="contentSmallTitle"><a href="http://www.reactos.org/en/community.html">ReactOS Community</a> &gt; ReactOS Wiki</div>

<div id="wikiContent">
	<div id="globalWrapper">
		<div id="column-content">
			<div id="content">
				<a name="top" id="contentTop"></a>
				<?php if($this->data['sitenotice']) { ?>
					<div id="siteNotice">
						<?php $this->html('sitenotice') ?>
					</div>
				<?php } ?>
				
				<h1 class="firstHeading"><?php $this->data['displaytitle']!=""?$this->html('title'):$this->text('title') ?></h1>
				<div id="bodyContent">
					<h3 id="siteSub"><?php $this->msg('tagline') ?></h3>
					<div id="contentSub"><?php $this->html('subtitle') ?></div>
					<?php if($this->data['undelete']) { ?>
						<div id="contentSub2"><?php     $this->html('undelete') ?></div>
					<?php } ?>
					<?php if($this->data['newtalk'] ) { ?>
						<div class="usermessage"><?php $this->html('newtalk')  ?></div>
					<?php } ?>
			
					<!-- start content -->
					<?php $this->html('bodytext') ?>
					<?php if($this->data['catlinks']) { ?>
						<div id="catlinks"><?php       $this->html('catlinks') ?></div>
					<?php } ?>
					<!-- end content -->
			
					<div class="visualClear"></div>
				</div>
			</div>
		</div>
		
		<div id="p-cactions" class="portlet">
			<h5><?php $this->msg('views') ?></h5>
			<ul>
				<?php foreach($this->data['content_actions'] as $key => $tab) { ?>
					<li id="ca-<?php echo Sanitizer::escapeId($key) ?>"<?php if($tab['class']) { ?> class="<?php echo htmlspecialchars($tab['class']) ?>"<?php } ?>><a href="<?php echo htmlspecialchars($tab['href']) ?>"<?php echo $skin->tooltipAndAccesskey('ca-'.$key) ?>><?php echo htmlspecialchars($tab['text']) ?></a></li>
				<?php } ?>
			</ul>
		</div>
		<div class="visualClear"></div>
			
		<div id="footer">
			<?php if($this->data['poweredbyico']) { ?>
				<div id="f-poweredbyico"><?php $this->html('poweredbyico') ?></div>
			<?php 	} ?>
			<?php if($this->data['copyrightico']) { ?>
				<div id="f-copyrightico"><?php $this->html('copyrightico') ?></div>
			<?php	} ?>
			
			<ul id="f-list">
				<?php
					$footerlinks = array(
						'lastmod', 'viewcount', 'numberofwatchingusers', 'credits', 'copyright',
						'privacy', 'about', 'disclaimer', 'tagline',
					);
					
					foreach( $footerlinks as $aLink ) {
						if( isset( $this->data[$aLink] ) && $this->data[$aLink] ) { ?>
							<li id="<?php echo$aLink?>"><?php $this->html($aLink) ?></li>
						<?php } ?>
					<?php } ?>
			</ul>
		</div>
	</div>
			
	<?php $this->html('bottomscripts'); /* JS call to runBodyOnloadHook */ ?>
	
	<?php $this->html('reporttime') ?>
	<?php if ( $this->data['debug'] ): ?>
	<!-- Debug output:
	<?php $this->text( 'debug' ); ?>
	-->
	<?php endif; ?>
</div>
</td>
</tr>
</table>

<hr size="1"/>
<address>
 <p align="center">
  ReactOS is a trademark of ReactOS Foundation in the United States and other countries.
 </p>
</address>
</body></html>
<?php
	wfRestoreWarnings();
	} // end of execute() method
} // end of class
?>
