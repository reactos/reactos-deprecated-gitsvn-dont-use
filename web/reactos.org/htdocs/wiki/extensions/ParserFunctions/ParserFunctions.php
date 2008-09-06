<?php

if ( !defined( 'MEDIAWIKI' ) ) {
	die( 'This file is a MediaWiki extension, it is not a valid entry point' );
}

$wgExtensionFunctions[] = 'wfSetupParserFunctions';
$wgExtensionCredits['parserhook'][] = array(
	'name' => 'ParserFunctions',
	'version' => '1.1.1',
	'url' => 'http://meta.wikimedia.org/wiki/ParserFunctions',
	'author' => 'Tim Starling',
	'description' => 'Enhance parser with logical functions',
	'descriptionmsg' => 'pfunc_desc',
);

$wgExtensionMessagesFiles['ParserFunctions'] = dirname(__FILE__) . '/ParserFunctions.i18n.php';
$wgHooks['LanguageGetMagic'][]       = 'wfParserFunctionsLanguageGetMagic';
$wgHooks['ParserLimitReport'][]      = 'wfParserFunctionsLimitReport';

$wgMaxIfExistCount = 100;

class ExtParserFunctions {
	var $mExprParser;
	var $mTimeCache = array();
	var $mTimeChars = 0;
	var $mMaxTimeChars = 6000; # ~10 seconds

	function registerParser( &$parser ) {
		if ( defined( get_class( $parser ) . '::SFH_OBJECT_ARGS' ) ) {
			// These functions accept DOM-style arguments
			$parser->setFunctionHook( 'if', array( &$this, 'ifObj' ), SFH_OBJECT_ARGS );
			$parser->setFunctionHook( 'ifeq', array( &$this, 'ifeqObj' ), SFH_OBJECT_ARGS );
			$parser->setFunctionHook( 'switch', array( &$this, 'switchObj' ), SFH_OBJECT_ARGS );
			$parser->setFunctionHook( 'ifexist', array( &$this, 'ifexistObj' ), SFH_OBJECT_ARGS );
			$parser->setFunctionHook( 'ifexpr', array( &$this, 'ifexprObj' ), SFH_OBJECT_ARGS );
			$parser->setFunctionHook( 'iferror', array( &$this, 'iferrorObj' ), SFH_OBJECT_ARGS );
		} else {
			$parser->setFunctionHook( 'if', array( &$this, 'ifHook' ) );
			$parser->setFunctionHook( 'ifeq', array( &$this, 'ifeq' ) );
			$parser->setFunctionHook( 'switch', array( &$this, 'switchHook' ) );
			$parser->setFunctionHook( 'ifexist', array( &$this, 'ifexist' ) );
			$parser->setFunctionHook( 'ifexpr', array( &$this, 'ifexpr' ) );
			$parser->setFunctionHook( 'iferror', array( &$this, 'iferror' ) );
		}

		$parser->setFunctionHook( 'expr', array( &$this, 'expr' ) );
		$parser->setFunctionHook( 'time', array( &$this, 'time' ) );
		$parser->setFunctionHook( 'timel', array( &$this, 'localTime' ) );
		$parser->setFunctionHook( 'rel2abs', array( &$this, 'rel2abs' ) );
		$parser->setFunctionHook( 'titleparts', array( &$this, 'titleparts' ) );

		return true;
	}

	function clearState(&$parser) {
		$this->mTimeChars = 0;
		$parser->pf_ifexist_count = 0;
		$parser->pf_ifexist_breakdown = array();
		return true;
	}

	function &getExprParser() {
		if ( !isset( $this->mExpr ) ) {
			if ( !class_exists( 'ExprParser' ) ) {
				require( dirname( __FILE__ ) . '/Expr.php' );
			}
			$this->mExprParser = new ExprParser;
		}
		return $this->mExprParser;
	}

	function expr( &$parser, $expr = '' ) {
		try {
			return $this->getExprParser()->doExpression( $expr );
		} catch(ExprError $e) {
			return $e->getMessage();
		}
	}

	function ifexpr( &$parser, $expr = '', $then = '', $else = '' ) {
		try{
			if($this->getExprParser()->doExpression( $expr )) {
				return $then;
			} else {
				return $else;
			}
		} catch (ExprError $e){
			return $e->getMessage();
		}
	}

	function ifexprObj( $parser, $frame, $args ) {
		$expr = isset( $args[0] ) ? trim( $frame->expand( $args[0] ) ) : '';
		$then = isset( $args[1] ) ? $args[1] : '';
		$else = isset( $args[2] ) ? $args[2] : '';
		$result = $this->ifexpr( $parser, $expr, $then, $else );
		if ( is_object( $result ) ) {
			$result = trim( $frame->expand( $result ) );
		}
		return $result;
	}

	function ifHook( &$parser, $test = '', $then = '', $else = '' ) {
		if ( $test !== '' ) {
			return $then;
		} else {
			return $else;
		}
	}

	function ifObj( &$parser, $frame, $args ) {
		$test = isset( $args[0] ) ? trim( $frame->expand( $args[0] ) ) : '';
		if ( $test !== '' ) {
			return isset( $args[1] ) ? trim( $frame->expand( $args[1] ) ) : '';
		} else {
			return isset( $args[2] ) ? trim( $frame->expand( $args[2] ) ) : '';
		}
	}

	function ifeq( &$parser, $left = '', $right = '', $then = '', $else = '' ) {
		if ( $left == $right ) {
			return $then;
		} else {
			return $else;
		}
	}

	function ifeqObj( &$parser, $frame, $args ) {
		$left = isset( $args[0] ) ? trim( $frame->expand( $args[0] ) ) : '';
		$right = isset( $args[1] ) ? trim( $frame->expand( $args[1] ) ) : '';
		if ( $left == $right ) {
			return isset( $args[2] ) ? trim( $frame->expand( $args[2] ) ) : '';
		} else {
			return isset( $args[3] ) ? trim( $frame->expand( $args[3] ) ) : '';
		}
	}

	function iferror( &$parser, $test = '', $then = '', $else = false ) {
		if ( preg_match( '/<(strong|span) class="error"/', $test ) ) {
			return $then;
		} elseif ( $else === false ) {
			return $test;
		} else {
			return $else;
		}
	}

	function iferrorObj( &$parser, $frame, $args ) {
		$test = isset( $args[0] ) ? trim( $frame->expand( $args[0] ) ) : '';
		$then = isset( $args[1] ) ? $args[1] : false;
		$else = isset( $args[2] ) ? $args[2] : false;
		$result = $this->iferror( $parser, $test, $then, $else );
		if ( $result === false ) {
			return '';
		} else {
			return trim( $frame->expand( $result ) );
		}
	}

	function switchHook( &$parser /*,...*/ ) {
		$args = func_get_args();
		array_shift( $args );
		$primary = trim(array_shift($args));
		$found = false;
		$parts = null;
		$default = null;
		$mwDefault =& MagicWord::get( 'default' );
		foreach( $args as $arg ) {
			$parts = array_map( 'trim', explode( '=', $arg, 2 ) );
			if ( count( $parts ) == 2 ) {
				# Found "="
				if ( $found || $parts[0] == $primary ) {
					# Found a match, return now
					return $parts[1];
				} else {
					if ( $mwDefault->matchStartAndRemove( $parts[0] ) ) {
						$default = $parts[1];
					} # else wrong case, continue
				}
			} elseif ( count( $parts ) == 1 ) {
				# Multiple input, single output
				# If the value matches, set a flag and continue
				if ( $parts[0] == $primary ) {
					$found = true;
				}
			} # else RAM corruption due to cosmic ray?
		}
		# Default case
		# Check if the last item had no = sign, thus specifying the default case
		if ( count( $parts ) == 1) {
			return $parts[0];
		} elseif ( !is_null( $default ) ) {
			return $default;
		} else {
			return '';
		}
	}

	function switchObj( $parser, $frame, $args ) {
		if ( count( $args ) == 0 ) {
			return '';
		}
		$primary = trim( $frame->expand( array_shift( $args ) ) );
		$found = false;
		$default = null;
		$lastItemHadNoEquals = false;
		$mwDefault =& MagicWord::get( 'default' );
		foreach ( $args as $arg ) {
			$bits = $arg->splitArg();
			$nameNode = $bits['name'];
			$index = $bits['index'];
			$valueNode = $bits['value'];

			if ( $index === '' ) {
				# Found "="
				$lastItemHadNoEquals = false;
				$test = trim( $frame->expand( $nameNode ) );
				if ( $found ) {
					# Multiple input match
					return trim( $frame->expand( $valueNode ) );
				} else {
					$test = trim( $frame->expand( $nameNode ) );
					if ( $test == $primary ) {
						# Found a match, return now
						return trim( $frame->expand( $valueNode ) );
					} else {
						if ( $mwDefault->matchStartAndRemove( $test ) ) {
							$default = $valueNode;
						} # else wrong case, continue
					}
				}
			} else {
				# Multiple input, single output
				# If the value matches, set a flag and continue
				$lastItemHadNoEquals = true;
				$test = trim( $frame->expand( $valueNode ) );
				if ( $test == $primary ) {
					$found = true;
				}
			}
		}
		# Default case
		# Check if the last item had no = sign, thus specifying the default case
		if ( $lastItemHadNoEquals ) {
			return $test;
		} elseif ( !is_null( $default ) ) {
			return trim( $frame->expand( $default ) );
		} else {
			return '';
		}
	}

	/**
	 * Returns the absolute path to a subpage, relative to the current article
	 * title. Treats titles as slash-separated paths.
	 *
	 * Following subpage link syntax instead of standard path syntax, an
	 * initial slash is treated as a relative path, and vice versa.
	 */
	public function rel2abs( &$parser , $to = '' , $from = '' ) {

		$from = trim($from);
		if( $from == '' ) {
			$from = $parser->getTitle()->getPrefixedText();
		}

		$to = rtrim( $to , ' /' );

		// if we have an empty path, or just one containing a dot
		if( $to == '' || $to == '.' ) {
			return $from;
		}

		// if the path isn't relative
		if ( substr( $to , 0 , 1) != '/' &&
		 substr( $to , 0 , 2) != './' &&
		 substr( $to , 0 , 3) != '../' &&
		 $to != '..' )
		{
			$from = '';
		}
		// Make a long path, containing both, enclose it in /.../
		$fullPath = '/' . $from . '/' .  $to . '/';

		// remove redundant current path dots
		$fullPath = preg_replace( '!/(\./)+!', '/', $fullPath );

		// remove double slashes
		$fullPath = preg_replace( '!/{2,}!', '/', $fullPath );

		// remove the enclosing slashes now
		$fullPath = trim( $fullPath , '/' );
		$exploded = explode ( '/' , $fullPath );
		$newExploded = array();

		foreach ( $exploded as $current ) {
			if( $current == '..' ) { // removing one level
				if( !count( $newExploded ) ){
					// attempted to access a node above root node
					wfLoadExtensionMessages( 'ParserFunctions' );
					return '<strong class="error">' . wfMsgForContent( 'pfunc_rel2abs_invalid_depth', $fullPath ) . '</strong>';
				}
				// remove last level from the stack
				array_pop( $newExploded );
			} else {
				// add the current level to the stack
				$newExploded[] = $current;
			}
		}

		// we can now join it again
		return implode( '/' , $newExploded );
	}

	function incrementIfexistCount( $parser, $frame ) {
		// Don't let this be called more than a certain number of times. It tends to make the database explode.
		global $wgMaxIfExistCount;
		$parser->pf_ifexist_count++;
		if ( $frame ) {
			$pdbk = $frame->getPDBK( 1 );
			if ( !isset( $parser->pf_ifexist_breakdown[$pdbk] ) ) {
				$parser->pf_ifexist_breakdown[$pdbk] = 0;
			}
			$parser->pf_ifexist_breakdown[$pdbk] ++;
		}
		return $parser->pf_ifexist_count <= $wgMaxIfExistCount;
	}

	function ifexist( &$parser, $title = '', $then = '', $else = '' ) {
		return $this->ifexistCommon( $parser, false, $title, $then, $else );
	}

	function ifexistCommon( &$parser, $frame, $title = '', $then = '', $else = '' ) {
		$title = Title::newFromText( $title );
		if ( $title ) {
			/* If namespace is specified as NS_MEDIA, then we want to check the physical file,
			 * not the "description" page.
			 */
			if( $title->getNamespace() == NS_MEDIA ) {
				if ( !$this->incrementIfexistCount( $parser, $frame ) ) {
					return $else;
				}
				$file = wfFindFile($title);
				if ( !$file ) {
					return $else;
				}
				$parser->mOutput->addImage($file->getName());
				return $file->exists() ? $then : $else;
			} elseif( $title->getNamespace() == NS_SPECIAL || $title->isExternal() ) {
				// Specials and interwikis...
				// Currently these always return false, though perhaps
				// they should be able to do some checks?
				//
				// In any case, don't register them in local link tables as below...
				return $else;
			} else {
				$pdbk = $title->getPrefixedDBkey();
				$lc = LinkCache::singleton();
				if ( $lc->getGoodLinkID( $pdbk ) ) {
					return $then;
				} elseif ( $lc->isBadLink( $pdbk ) ) {
					return $else;
				}
				if ( !$this->incrementIfexistCount( $parser, $frame ) ) {
					return $else;
				}

				$id = $title->getArticleID();
				$parser->mOutput->addLink( $title, $id );
				if ( $id ) {
					return $then;
				}
			}
		}
		return $else;
	}

	function ifexistObj( &$parser, $frame, $args ) {
		$title = isset( $args[0] ) ? trim( $frame->expand( $args[0] ) ) : '';
		$then = isset( $args[1] ) ? $args[1] : null;
		$else = isset( $args[2] ) ? $args[2] : null;

		$result = $this->ifexistCommon( $parser, $frame, $title, $then, $else );
		if ( $result === null ) {
			return '';
		} else {
			return trim( $frame->expand( $result ) );
		}
	}

	function time( &$parser, $format = '', $date = '', $local = false ) {
		global $wgContLang, $wgLocaltimezone;
		if ( isset( $this->mTimeCache[$format][$date][$local] ) ) {
			return $this->mTimeCache[$format][$date][$local];
		}

		if ( $date !== '' ) {
			$unix = @strtotime( $date );
		} else {
			$unix = time();
		}

		if ( $unix == -1 || $unix == false ) {
			wfLoadExtensionMessages( 'ParserFunctions' );
			$result = '<strong class="error">' . wfMsgForContent( 'pfunc_time_error' ) . '</strong>';
		} else {
			$this->mTimeChars += strlen( $format );
			if ( $this->mTimeChars > $this->mMaxTimeChars ) {
				wfLoadExtensionMessages( 'ParserFunctions' );
				return '<strong class="error">' . wfMsgForContent( 'pfunc_time_too_long' ) . '</strong>';
			} else {
				if ( $local ) {
					# Use the time zone
					if ( isset( $wgLocaltimezone ) ) {
						$oldtz = getenv( 'TZ' );
						putenv( 'TZ='.$wgLocaltimezone );
					}
					wfSuppressWarnings(); // E_STRICT system time bitching
					$ts = date( 'YmdHis', $unix );
					wfRestoreWarnings();
					if ( isset( $wgLocaltimezone ) ) {
						putenv( 'TZ='.$oldtz );
					}
				} else {
					$ts = wfTimestamp( TS_MW, $unix );
				}
				if ( method_exists( $wgContLang, 'sprintfDate' ) ) {
					$result = $wgContLang->sprintfDate( $format, $ts );
				} else {
					if ( !class_exists( 'SprintfDateCompat' ) ) {
						require( dirname( __FILE__ ) . '/SprintfDateCompat.php' );
					}

					$result = SprintfDateCompat::sprintfDate( $format, $ts );
				}
			}
		}
		$this->mTimeCache[$format][$date][$local] = $result;
		return $result;
	}

	function localTime( &$parser, $format = '', $date = '' ) {
		return $this->time( $parser, $format, $date, true );
	}

	/**
	 * Obtain a specified number of slash-separated parts of a title,
	 * e.g. {{#titleparts:Hello/World|1}} => "Hello"
	 *
	 * @param Parser $parser Parent parser
	 * @param string $title Title to split
	 * @param int $parts Number of parts to keep
	 * @param int $offset Offset starting at 1
	 * @return string
	 */
	public function titleparts( $parser, $title = '', $parts = 0, $offset = 0) {
		$parts = intval( $parts );
		$offset = intval( $offset );
		$ntitle = Title::newFromText( $title );
		if ( $ntitle instanceof Title ) {
			$bits = explode( '/', $ntitle->getPrefixedText(), 25 );
			if ( count( $bits ) <= 0 ) {
				 return $ntitle->getPrefixedText();
			} else {
				if ( $offset > 0 ) {
					--$offset;
				}
				if ( $parts == 0 ) {
					return implode( '/', array_slice( $bits, $offset ) );
				} else {
					return implode( '/', array_slice( $bits, $offset, $parts ) );
				}
			}
		} else {
			return $title;
		}
	}

	function afterTidy( &$parser, &$text ) {
		global $wgMaxIfExistCount;
		if ( $parser->pf_ifexist_count > $wgMaxIfExistCount ) {
			if ( is_callable( array( $parser->mOutput, 'addWarning' ) ) ) {
				wfLoadExtensionMessages( 'ParserFunctions' );
				$warning = wfMsg( 'pfunc_ifexist_warning', $parser->pf_ifexist_count, $wgMaxIfExistCount );
				$parser->mOutput->addWarning( $warning );
				$cat = Title::makeTitleSafe( NS_CATEGORY, wfMsgForContent( 'pfunc_max_ifexist_category' ) );
				if ( $cat ) {
					$parser->mOutput->addCategory( $cat->getDBkey(), $parser->getDefaultSort() );
				}
			}
		}
		return true;
	}
}

function wfSetupParserFunctions() {
	global $wgParser, $wgExtParserFunctions, $wgHooks;

	$wgExtParserFunctions = new ExtParserFunctions;

	// Check for SFH_OBJECT_ARGS capability
	if ( defined( 'MW_SUPPORTS_PARSERFIRSTCALLINIT' ) ) {
		$wgHooks['ParserFirstCallInit'][] = array( &$wgExtParserFunctions, 'registerParser' );
	} else {
		if ( class_exists( 'StubObject' ) && !StubObject::isRealObject( $wgParser ) ) {
			$wgParser->_unstub();
		}
		$wgExtParserFunctions->registerParser( $wgParser );
	}

	$wgHooks['ParserClearState'][] = array( &$wgExtParserFunctions, 'clearState' );
	$wgHooks['ParserAfterTidy'][] = array( &$wgExtParserFunctions, 'afterTidy' );
}

function wfParserFunctionsLanguageGetMagic( &$magicWords, $langCode ) {
	require_once( dirname( __FILE__ ) . '/ParserFunctions.i18n.magic.php' );
	foreach( efParserFunctionsWords( $langCode ) as $word => $trans )
		$magicWords[$word] = $trans;
	return true;
}

function wfParserFunctionsLimitReport( $parser, &$report ) {
	global $wgMaxIfExistCount;
	if ( isset( $parser->pf_ifexist_count ) ) {
		$report .= "#ifexist count: {$parser->pf_ifexist_count}/$wgMaxIfExistCount\n";
	}
	return true;
}

