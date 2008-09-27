<?php
/**
 *
 * @addtogroup SpecialPage
 */

define("ROSCMS_PATH", "$IP/../roscms/");
require_once($roscms_path . "logon/subsys_login.php");

/**
 * constructor
 */
function wfSpecialUserlogin() {
	if( session_id() == '' ) {
		wfSetupSession();
	}
	
	$target = "/wiki";
	roscms_subsys_login('wiki', ROSCMS_LOGIN_REQUIRED, $target);
	
	/* We should never end up here, but in case we do... */
	header("Location: $target");
	exit;
}

?>
