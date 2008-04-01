<?php
/**
 *
 * @addtogroup SpecialPage
 */

/**
 * constructor
 */
function wfSpecialUserlogout() {
	global $wgUser

	$wgUser->logout();
	header("Location: /roscms/?page=logout");
}
