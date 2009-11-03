# Shared functions for RosBE-Unix components
# Part of RosBE for Unix-based Operating Systems
# Copyright 2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

# Check whether the given architecture is supported and if so, change the _ROSBE_ARCH variable appropriately
#   Parameter 1: The architecture to set
change_architecture()
{
	local new_arch="$1"

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

# Check whether we're running as root and warn the user in case we don't
check_root()
{
	if [ ! "`whoami`" = "root" ]; then
		rs_boldmsg "User is not \"root\""

		echo "The default installation path for the Build Environment is \"$DEFAULT_INSTALL_DIR\"."
		echo "Therefore it is recommended to run this script as the \"root\" user. Otherwise you probably cannot create the necessary directories."
		echo "An alternative is to specify another installation directory at the installation process."
		echo "Do you really want to continue? (yes/no)"

		local answer
		read -p "[no] " answer

		if [ "$answer" = "yes" ]; then
			echo
		else
			exit 1
		fi
	fi
}

# Set the PATH variable properly for the specified architecture
set_path_variable()
{
	# Set the PATH
	PATH="$_ROSBE_ROSSCRIPTDIR/$_ROSBE_ARCH/bin:$_ROSBE_ROSSCRIPTDIR/bin:$_ROSBE_OLDPATH"
}
