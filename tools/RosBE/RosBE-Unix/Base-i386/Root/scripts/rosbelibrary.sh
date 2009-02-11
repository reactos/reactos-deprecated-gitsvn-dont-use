# Shared functions for RosBE-Unix components
# Part of RosBE for Unix-based Operating Systems
# Copyright 2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

# Print a message in a bold font
#  Parameter 1: The message
#  Parameter 2: Optional additional parameters for "echo"
boldmsg()
{
	echo -e $2 "\e[1m$1\e[0m"
}

# Print a message in green color
#  Parameter 1: The message
#  Parameter 2: Optional additional parameters for "echo"
greenmsg()
{
	echo -e $2 "\e[32m$1\e[0m"
}

# Print a message in red color
#  Parameter 1: The message
#  Parameter 2: Optional additional parameters for "echo"
redmsg()
{
	echo -e $2 "\e[31m$1\e[0m"
}

# Allow a choice between several options
# If a valid choice was done, the result is stored in $choice
#   Parameter 1 - The default value
#   Parameter 2 - All possible values
showchoice()
{
	default_value="$1"
	valid_choices="$2"

	breakloop=false
	choice=""

	while true; do
		read -p "Your choice [$default_value]: " choice

		if [ "$choice" = "" ]; then
			choice="$default_value"
			break
		fi

		for valid_choice in $valid_choices; do
			if [ "$choice" = "$valid_choice" ]; then
				breakloop=true
				break
			fi
		done

		if $breakloop; then
			break
		fi
	done
}

# Change the _ROSBE_ARCH variable appropriately
change_architecture()
{
	new_arch="$1"

	if [ -f "$_ROSBE_ROSSCRIPTDIR/$new_arch/version.sh" ]; then
		# Architecture parameter seems valid
		_ROSBE_ARCH="$new_arch"
	else
		echo "Unsupported architecture specified, falling back to i386!"
		echo
		_ROSBE_ARCH="i386"
	fi

	export _ROSBE_ARCH
}

# Set the PATH variable properly for the specified architecture
set_path_variable()
{
	# Set the PATH
	PATH="$_ROSBE_ROSSCRIPTDIR/$_ROSBE_ARCH/bin:$_ROSBE_ROSSCRIPTDIR/bin:$_ROSBE_OLDPATH"
}

# Check for several requirements, which need to be met in order to run the installation script properly
setup_check_requirements()
{
	# Check whether we're running as root
	if [ ! "`whoami`" = "root" ]; then
		boldmsg "User is not \"root\""

		echo "The default installation path for the Build Environment is \"$DEFAULT_INSTALL_DIR\"."
		echo "Therefore it is recommended to run this script as the \"root\" user. Otherwise you probably cannot create the necessary directories."
		echo "An alternative is to specify another installation directory at the installation process."
		echo "Do you really want to continue? (yes/no)"
		read -p "[no] " answer

		if [ "$answer" = "yes" ]; then
			echo
		else
			exit 1
		fi
	fi

	# Test if the script directory is writeable
	if [ ! -w "$SCRIPTDIR" ]; then
		redmsg "The script directory \"$SCRIPTDIR\" is not writeable, aborted!"
		exit 1
	fi

	# Test if the script directory contains spaces
	case "$SCRIPTDIR" in
	*" "*)
		redmsg "The script directory \"$SCRIPTDIR\" contains spaces!"
		redmsg "Therefore some build tools cannot be compiled properly."
		echo
		redmsg "Please move \"$SCRIPTDIR\" to a directory, which does not contain spaces."

		exit 1;;
	esac

	# Check if all necessary tools exist
	boldmsg "Checking for the needed tools..."

	toolmissing=false
	for tool in $NEEDED_TOOLS; do
		echo -n "Checking for $tool... "

		if which "$tool" >& /dev/null; then
			greenmsg "OK"
		else
			redmsg "MISSING"
			toolmissing=true
		fi
	done

	# Special check for GNU Make
	# For example FreeBSD's "make" is not GNU Make, so we have to define a variable
	echo -n "Checking for GNU Make... "

	if make -v 2>&1 | grep "GNU Make" >& /dev/null; then
		makecmd="make"
		greenmsg "OK"
	elif gmake -v 2>&1 | grep "GNU Make" >& /dev/null; then
		makecmd="gmake"
		greenmsg "OK"
	else
		redmsg "MISSING"
		toolmissing=true
	fi

	if $toolmissing; then
		echo "At least one needed tool is missing, aborted!"
		exit 1
	fi

	echo
}

# Check whether the previous command finished with errorlevel 0
# If it did so, print a green "OK" and delete the debug logfile for the command.
# If that wasn't the case, print a red "FAILED" and give information about the debug logfile.
setup_check_run()
{
	if [ $? -ne 0 ]; then
		redmsg "FAILED"

		if [ "$1" = "" ]; then
			echo "If you did not do something wrong, please contact the ReactOS Team."
		else
			echo "Please take a look at the log file \"$SCRIPTDIR/$1.log\""
			echo "If you did not do something wrong, please save the log file and contact the ReactOS Team."
		fi

		echo "Aborted!"
		exit 1
	else
		greenmsg "OK"

		if [ "$1" != "" ]; then
			rm "$SCRIPTDIR/$1.log"
		fi
	fi
}
