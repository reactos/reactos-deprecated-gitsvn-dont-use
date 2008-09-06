<?php

if ( !defined( 'MEDIAWIKI' ) ) {
	die( 'This file is a MediaWiki extension, it is not a valid entry point' );
}

// Character classes
define( 'EXPR_WHITE_CLASS', " \t\r\n" );
define( 'EXPR_NUMBER_CLASS', '0123456789.' );

// Token types
define( 'EXPR_WHITE', 1 );
define( 'EXPR_NUMBER', 2 );
define( 'EXPR_NEGATIVE', 3 );
define( 'EXPR_POSITIVE', 4 );
define( 'EXPR_PLUS', 5 );
define( 'EXPR_MINUS', 6 );
define( 'EXPR_TIMES', 7 );
define( 'EXPR_DIVIDE', 8 );
define( 'EXPR_MOD', 9 );
define( 'EXPR_OPEN', 10 );
define( 'EXPR_CLOSE', 11 );
define( 'EXPR_AND', 12 );
define( 'EXPR_OR', 13 );
define( 'EXPR_NOT', 14 );
define( 'EXPR_EQUALITY', 15 );
define( 'EXPR_LESS', 16 );
define( 'EXPR_GREATER', 17 );
define( 'EXPR_LESSEQ', 18 );
define( 'EXPR_GREATEREQ', 19 );
define( 'EXPR_NOTEQ', 20 );
define( 'EXPR_ROUND', 21 );

class ExprError extends Exception {
	public function __construct($msg, $parameter = ''){
		wfLoadExtensionMessages( 'ParserFunctions' );
		$this->message = '<strong class="error">' . wfMsgForContent( "pfunc_expr_$msg", htmlspecialchars( $parameter ) ) . '</strong>';
	}
}

class ExprParser {
	var $maxStackSize = 100;

	var $precedence = array(
		EXPR_NEGATIVE => 10,
		EXPR_POSITIVE => 10,
		EXPR_NOT => 9,
		EXPR_TIMES => 8,
		EXPR_DIVIDE => 8,
		EXPR_MOD => 8,
		EXPR_PLUS => 6,
		EXPR_MINUS => 6,
		EXPR_ROUND => 5,
		EXPR_EQUALITY => 4,
		EXPR_LESS => 4,
		EXPR_GREATER => 4,
		EXPR_LESSEQ => 4,
		EXPR_GREATEREQ => 4,
		EXPR_NOTEQ => 4,
		EXPR_AND => 3,
		EXPR_OR => 2,
		EXPR_OPEN => -1,
		EXPR_CLOSE => -1
	);

	var $names = array(
		EXPR_NEGATIVE => '-',
		EXPR_POSITIVE => '+',
		EXPR_NOT => 'not',
		EXPR_TIMES => '*',
		EXPR_DIVIDE => '/',
		EXPR_MOD => 'mod',
		EXPR_PLUS => '+',
		EXPR_MINUS => '-',
		EXPR_ROUND => 'round',
		EXPR_EQUALITY => '=',
		EXPR_LESS => '<',
		EXPR_GREATER => '>',
		EXPR_LESSEQ => '<=',
		EXPR_GREATEREQ => '>=',
		EXPR_NOTEQ => '<>',
		EXPR_AND => 'and',
		EXPR_OR => 'or',
	);


	var $words = array(
		'mod' => EXPR_MOD,
		'and' => EXPR_AND,
		'or' => EXPR_OR,
		'not' => EXPR_NOT,
		'round' => EXPR_ROUND,
		'div' => EXPR_DIVIDE
	);

	/**
	 * Evaluate a mathematical expression
	 *
	 * The algorithm here is based on the infix to RPN algorithm given in
	 * http://montcs.bloomu.edu/~bobmon/Information/RPN/infix2rpn.shtml
	 * It's essentially the same as Dijkstra's shunting yard algorithm.
	 */
	function doExpression( $expr ) {
		$operands = array();
		$operators = array();

		# Unescape inequality operators
		$expr = strtr( $expr, array( '&lt;' => '<', '&gt;' => '>' ) );

		$p = 0;
		$end = strlen( $expr );
		$expecting = 'expression';

		while ( $p < $end ) {
			if ( count( $operands ) > $this->maxStackSize || count( $operators ) > $this->maxStackSize ) {
				throw new ExprError('stack_exhausted');
			}
			$char = $expr[$p];
			$char2 = substr( $expr, $p, 2 );

			// Mega if-elseif-else construct
			// Only binary operators fall through for processing at the bottom, the rest
			// finish their processing and continue

			// First the unlimited length classes

			if ( false !== strpos( EXPR_WHITE_CLASS, $char ) ) {
				// Whitespace
				$p += strspn( $expr, EXPR_WHITE_CLASS, $p );
				continue;
			} elseif ( false !== strpos( EXPR_NUMBER_CLASS, $char ) ) {
				// Number
				if ( $expecting != 'expression' ) {
					throw new ExprError('unexpected_number');
				}

				// Find the rest of it
				$length = strspn( $expr, EXPR_NUMBER_CLASS, $p );
				// Convert it to float, silently removing double decimal points
				$operands[] = floatval( substr( $expr, $p, $length ) );
				$p += $length;
				$expecting = 'operator';
				continue;
			} elseif ( ctype_alpha( $char ) ) {
				// Word
				// Find the rest of it
				$remaining = substr( $expr, $p );
				if ( !preg_match( '/^[A-Za-z]*/', $remaining, $matches ) ) {
					// This should be unreachable
					throw new ExprError('preg_match_failure');
				}
				$word = strtolower( $matches[0] );
				$p += strlen( $word );

				// Interpret the word
				if ( !isset( $this->words[$word] ) ){
					throw new ExprError('unrecognised_word', $word);
				}
				$op = $this->words[$word];
				// Unary operator
				switch($op){
				case EXPR_NOT:
					if ( $expecting != 'expression' ) {
						throw new ExprError('unexpected_operator', $word);
					}
					$operators[] = $op;
					continue 2;
				}
				// Binary operator, fall through
				$name = $word;
			}

			// Next the two-character operators

			elseif ( $char2 == '<=' ) {
				$name = $char2;
				$op = EXPR_LESSEQ;
				$p += 2;
			} elseif ( $char2 == '>=' ) {
				$name = $char2;
				$op = EXPR_GREATEREQ;
				$p += 2;
			} elseif ( $char2 == '<>' || $char2 == '!=' ) {
				$name = $char2;
				$op = EXPR_NOTEQ;
				$p += 2;
			}

			// Finally the single-character operators

			elseif ( $char == '+' ) {
				++$p;
				if ( $expecting == 'expression' ) {
					// Unary plus
					$operators[] = EXPR_POSITIVE;
					continue;
				} else {
					// Binary plus
					$op = EXPR_PLUS;
				}
			} elseif ( $char == '-' ) {
				++$p;
				if ( $expecting == 'expression' ) {
					// Unary minus
					$operators[] = EXPR_NEGATIVE;
					continue;
				} else {
					// Binary minus
					$op = EXPR_MINUS;
				}
			} elseif ( $char == '*' ) {
				$name = $char;
				$op = EXPR_TIMES;
				++$p;
			} elseif ( $char == '/' ) {
				$name = $char;
				$op = EXPR_DIVIDE;
				++$p;
			} elseif ( $char == '(' )  {
				if ( $expecting == 'operator' ) {
					throw new ExprError('unexpected_operator', '(');
				}
				$operators[] = EXPR_OPEN;
				++$p;
				continue;
			} elseif ( $char == ')' ) {
				$lastOp = end( $operators );
				while ( $lastOp && $lastOp != EXPR_OPEN ) {
					$this->doOperation( $lastOp, $operands );
					array_pop( $operators );
					$lastOp = end( $operators );
				}
				if ( $lastOp ) {
					array_pop( $operators );
				} else {
					throw new ExprError('unexpected_closing_bracket');
				}
				$expecting = 'operator';
				++$p;
				continue;
			} elseif ( $char == '=' ) {
				$name = $char;
				$op = EXPR_EQUALITY;
				++$p;
			} elseif ( $char == '<' ) {
				$name = $char;
				$op = EXPR_LESS;
				++$p;
			} elseif ( $char == '>' ) {
				$name = $char;
				$op = EXPR_GREATER;
				++$p;
			} else {
				throw new ExprError('unrecognised_punctuation', UtfNormal::cleanUp( $char ));
			}

			// Binary operator processing
			if ( $expecting == 'expression' ) {
				throw new ExprError('unexpected_operator', $name);
			}

			// Shunting yard magic
			$lastOp = end( $operators );
			while ( $lastOp && $this->precedence[$op] <= $this->precedence[$lastOp] ) {
				$this->doOperation( $lastOp, $operands );
				array_pop( $operators );
				$lastOp = end( $operators );
			}
			$operators[] = $op;
			$expecting = 'expression';
		}

		// Finish off the operator array
		while ( $op = array_pop( $operators ) ) {
			if ( $op == EXPR_OPEN ) {
				throw new ExprError('unclosed_bracket');
			}
			$this->doOperation( $op, $operands );
		}

		return implode( "<br />\n", $operands );
	}

	function doOperation( $op, &$stack ) {
		switch ( $op ) {
			case EXPR_NEGATIVE:
				if ( count( $stack ) < 1 ) throw new ExprError('missing_operand', $this->names[$op]);
				$arg = array_pop( $stack );
				$stack[] = -$arg;
				break;
			case EXPR_POSITIVE:
				if ( count( $stack ) < 1 ) throw new ExprError('missing_operand', $this->names[$op]);
				break;
			case EXPR_TIMES:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				$stack[] = $left * $right;
					break;
			case EXPR_DIVIDE:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				if ( $right == 0 ) throw new ExprError('division_by_zero', $this->names[$op]);
				$stack[] = $left / $right;
				break;
			case EXPR_MOD:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				if ( $right == 0 ) throw new ExprError('division_by_zero', $this->names[$op]);
				$stack[] = $left % $right;
				break;
			case EXPR_PLUS:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				$stack[] = $left + $right;
				break;
			case EXPR_MINUS:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				$stack[] = $left - $right;
				break;
			case EXPR_AND:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				$stack[] = ( $left && $right ) ? 1 : 0;
				break;
			case EXPR_OR:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				$stack[] = ( $left || $right ) ? 1 : 0;
				break;
			case EXPR_EQUALITY:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				$stack[] = ( $left == $right ) ? 1 : 0;
				break;
			case EXPR_NOT:
				if ( count( $stack ) < 1 ) throw new ExprError('missing_operand', $this->names[$op]);
				$arg = array_pop( $stack );
				$stack[] = (!$arg) ? 1 : 0;
				break;
			case EXPR_ROUND:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$digits = intval( array_pop( $stack ) );
				$value = array_pop( $stack );
				$stack[] = round( $value, $digits );
				break;
			case EXPR_LESS:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				$stack[] = ( $left < $right ) ? 1 : 0;
				break;
			case EXPR_GREATER:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				$stack[] = ( $left > $right ) ? 1 : 0;
				break;
			case EXPR_LESSEQ:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				$stack[] = ( $left <= $right ) ? 1 : 0;
				break;
			case EXPR_GREATEREQ:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				$stack[] = ( $left >= $right ) ? 1 : 0;
				break;
			case EXPR_NOTEQ:
				if ( count( $stack ) < 2 ) throw new ExprError('missing_operand', $this->names[$op]);
				$right = array_pop( $stack );
				$left = array_pop( $stack );
				$stack[] = ( $left != $right ) ? 1 : 0;
				break;
			default:
				// Should be impossible to reach here.
				throw new ExprError('unknown_error');
		}
	}
}
