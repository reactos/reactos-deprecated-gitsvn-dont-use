<?php
/**
 *
 * @package MediaWiki
 * @subpackage SpecialPage
 */

/**
 *
 */
require_once('UserMailer.php');
require_once( '/web/reactos.org/htdocs/roscms/inc/subsys_login.php' );

/**
 * constructor
 */
function wfSpecialUserlogin() {
  $target = "/wiki";
  roscms_subsys_login('wiki', ROSCMS_LOGIN_REQUIRED, $target);

  /* We should never end up here, but in case we do... */
  header("Location: $target");
  exit;
}

?>