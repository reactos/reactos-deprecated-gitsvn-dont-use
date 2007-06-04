<?php
/**
 *
 * @addtogroup SpecialPage
 */

/**
 * constructor
 */
function wfSpecialUserlogout() {
	global $wgUser, $wgOut;

	if (wfRunHooks('UserLogout', array(&$wgUser))) {

		$wgUser->logout();

		wfRunHooks('UserLogoutComplete', array(&$wgUser));

		header("Location: /roscms/?page=logout");
	}
}

?>
