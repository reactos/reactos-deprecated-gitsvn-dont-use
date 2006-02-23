Test Application: default_switch
Author: Arty
Purpose: Check for reverse engineerability of the C construct:

switch( x ) {
default:
	...
	break;
}

Status: Disassembly of test code reveals that distinguishing a default-only
switch from unadorned code is impossible.  Therefore, this construction cannot
have arrived by reverse engineering.
