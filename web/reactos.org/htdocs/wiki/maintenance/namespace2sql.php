<?php
# $Header: /cvsroot/wikipedia/phase3/maintenance/namespace2sql.php,v 1.1 2005/03/20 03:29:12 kateturner Exp $
#
# Print SQL to insert namespace names into database.
# This source code is in the public domain.

require_once( "commandLine.inc" );

for ($i = -2; $i < 16; ++$i) {
	$nsname = wfStrencode( $wgLang->getNsText( $i ) );
	$dbname = wfStrencode( $wgDBname );
	print "INSERT INTO ns_name(ns_db, ns_num, ns_name) VALUES('$dbname', $i, '$nsname');\n";
}

?>
