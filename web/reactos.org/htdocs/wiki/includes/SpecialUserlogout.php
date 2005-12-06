<?php
/**
 *
 * @package MediaWiki
 * @subpackage SpecialPage
 */

/**
 * constructor
 */
function wfSpecialUserlogout() {
	global $wgUser, $wgOut, $returnto;

	if (wfRunHooks('UserLogout', array(&$wgUser))) {
		
		$wgUser->logout();

		wfRunHooks('UserLogoutComplete', array(&$wgUser));

		header("Location: /roscms/?page=logout");
	}
}

?>
