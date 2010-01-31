#!/bin/bash
#
# ReactOS Build Environment for Unix-based Operating Systems - Builder Tool for the Base package
# Copyright 2007-2010 Colin Finck <colin@reactos.org>
# partially based on the BuildMingwCross script (http://www.mingw.org/MinGWiki/index.php/BuildMingwCross)
#
# Released under GNU GPL v2 or any later version.


# RosBE Setup Variables
rs_host_cflags="-pipe -O2"
rs_needed_tools="bison flex gcc g++ grep makeinfo"        # GNU Make has a special check
rs_target="mingw32"
rs_target_cflags="-pipe -gstabs+ -O2 -march=pentium -mtune=i686"

# Get the absolute path to the script directory
cd `dirname $0`
rs_scriptdir="$PWD"
rs_workdir="$rs_scriptdir/sources"
rs_sourcedir="$rs_scriptdir/sources"

# RosBE-Unix Constants
DEFAULT_INSTALL_DIR="/usr/local/RosBE"
KNOWN_ROSBE_VERSIONS="0.3.6 1.1 1.4 1.4.2 1.5-RC2"
ROSBE_VERSION="1.5-RC2"
TARGET_ARCH="i386"

source "$rs_scriptdir/scripts/rosbelibrary.sh"
source "$rs_scriptdir/scripts/setuplibrary.sh"


echo "*******************************************************************************"
echo "*         ReactOS Build Environment for Unix-based Operating Systems          *"
echo "*                      Builder Tool for the Base package                      *"
echo "*                      by Colin Finck <colin@reactos.org>                     *"
echo "*                                                                             *"
echo "*                                 Version $ROSBE_VERSION                                 *"
echo "*******************************************************************************"

echo
echo "This script compiles and installs a complete Build Environment for building"
echo "ReactOS."
echo

if [ "$1" = "-h" ] || [ "$1" = "-?" ] || [ "$1" = "--help" ]; then
	echo "Syntax: ./RosBE-Builder.sh [installdir]"
	echo
	echo " installdir - Optional parameter to specify an installation directory. If you"
	echo "              do this, the script will check whether this directory does not"
	echo "              yet exist and in this case, it will perform an unattended"
	echo "              installation to this directory."
	echo
	echo "Usually, you just call the script without any parameters and it will guide you"
	echo "through all possible installation options."
	exit 0
fi

check_root
rs_check_requirements

reinstall=false
update=false

# Select the installation directory
rs_boldmsg "Installation Directory"

if [ "$1" = "" ]; then
	installdir=""

	echo "In which directory do you want to install it?"
	echo "Enter the path to the directory here or simply press ENTER to install it into the default directory."

	while [ "$installdir" = "" ]; do
		read -p "[$DEFAULT_INSTALL_DIR] " installdir
		echo

		if [ "$installdir" = "" ]; then
			installdir=$DEFAULT_INSTALL_DIR
		fi

		# Make sure we have the absolute path to the installation directory
		installdir=`eval echo $installdir`

		# Check if the installation directory already exists
		if [ -f "$installdir" ]; then
			echo "The directory \"$installdir\" is a file! Please enter another directory!"
			echo
			installdir=""
		elif [ -d "$installdir" ]; then
			# Check if the directory is empty
			if [ ! "`ls $installdir`" = "" ]; then
				if [ -f "$installdir/RosBE-Version" ]; then
					choice=""

					# Allow the user to update an already installed RosBE version
					installed_version=`cat "$installdir/RosBE-Version"`

					for known_version in $KNOWN_ROSBE_VERSIONS; do
						if [ "$known_version" = "$installed_version" ]; then
							if [ "$installed_version" = "$ROSBE_VERSION" ]; then
								echo "The current version of the Build Environment is already installed in this directory."
								echo "Please choose one of the following options:"
								echo
								echo " (R)einstall all components of the Build Environment"
								echo " (C)hoose a different installation directory"
								echo

								rs_showchoice "R" "r R c C"
							else
								echo "An older version of the Build Environment has been found in this directory."
								echo "Please choose one of the following options:"
								echo
								echo " (U)pdate/(r)einstall the Build Environment"
								echo " (C)hoose a different installation directory"
								echo

								rs_showchoice "U" "u U r R c C"
							fi

							break
						fi
					done

					if [ "$choice" = "" ]; then
						# We found no known version, so this could be for example a Release Candidate.
						# Only offer the reinstallation option.
						echo "Another version of the Build Environment is already installed in this directory."
						echo "Please choose one of the following options:"
						echo
						echo " (R)einstall all components of the Build Environment"
						echo " (C)hoose a different installation directory"
						echo

						rs_showchoice "R" "r R c C"
					fi

					case "$choice" in
						"U"|"u")
							# We don't support update in this version due to lots of changes
							# To reenable updating, change this to "update=true"
							reinstall=true
							;;
						"R"|"r")
							reinstall=true
							;;
						"C"|"c")
							echo "Please enter another directory!"
							installdir=""
							;;
					esac
				else
					echo "The directory \"$installdir\" is not empty. Do you really want to continue? (yes/no)"
					read -p "[no] " answer
					echo

					if [ "$answer" != "yes" ]; then
						echo "Please enter another directory!"
						installdir=""
					fi
				fi
			fi
		else
			echo "The directory \"$installdir\" does not exist. The installation script will create it for you."
			echo
		fi
	done

	# Ready to start
	rs_boldmsg "Ready to start"

	echo "Ready to build and install the ReactOS Build Environment."
	echo "Press Return to continue or Ctrl+C to exit."
	read
else
	installdir=`eval echo $1`

	if [ -e "$installdir" ]; then
		rs_redmsg "Installation directory \"$installdir\" already exists, aborted!"
		exit 1
	fi

	echo "Using \"$installdir\""
	echo
fi

if $update; then
	# No update supported for RosBE-Unix 1.5 due to lots of changes.
	# Add this part back from older versions once we need to update again.
	exit 1
else
	rs_process_binutils=true
	rs_process_buildtime=true
	rs_process_cpucount=true
	rs_process_gcc=true
	rs_process_getincludes=true
	rs_process_gmp=true
	rs_process_make=true
	rs_process_mingw_runtime_dev=true
	rs_process_mpfr=true
	rs_process_nasm=true
	rs_process_scut=true
	rs_process_w32api=true

	if $reinstall; then
		# Empty the directory if we're reinstalling
		rs_mkdir_empty "$installdir"
	else
		# Otherwise just create the directory if it does not exist
		rs_mkdir_if_not_exists "$installdir"
	fi
fi

# Test if the installation directory is writable
if [ ! -w "$installdir" ]; then
	rs_redmsg "Installation directory \"$installdir\" is not writable, aborted!"
	exit 1
fi

rs_prefixdir="$installdir/$TARGET_ARCH"
rs_supportprefixdir="$installdir/support"
rs_mkdir_if_not_exists "$installdir/bin"


##### BEGIN almost shared buildtoolchain/RosBE-Unix building part #############
rs_boldmsg "Building..."

rs_mkdir_if_not_exists "$rs_prefixdir/$rs_target"
rs_mkdir_if_not_exists "$rs_supportprefixdir"

# Use -march=native if the host compiler supports it
echo -n "Checking if the host compiler supports -march=native... "

if `gcc -march=native -o "$rs_workdir/dummy" "$rs_scriptdir/tools/dummy.c" >& /dev/null`; then
	echo "yes"
	rs_host_cflags+=" -march=native"
	rm "$rs_workdir/dummy"
else
	echo "no"
fi

rs_extract_module "mingw_runtime_dev" "$rs_prefixdir/$rs_target"
rs_extract_module "w32api" "$rs_prefixdir/$rs_target"

if $rs_process_buildtime; then
	rs_do_command gcc -s -o "$installdir/bin/buildtime" "$rs_scriptdir/tools/buildtime.c"
fi

if $rs_process_cpucount; then
	rs_do_command gcc -s -o "$installdir/bin/cpucount" "$rs_scriptdir/tools/cpucount.c"
fi

rs_cpucount=`$installdir/bin/cpucount -x1`

if $rs_process_getincludes; then
	rs_do_command gcc -s -o "$installdir/bin/getincludes" "$rs_scriptdir/tools/getincludes.c"
fi

if $rs_process_scut; then
	rs_do_command gcc -s -o "$installdir/bin/scut" "$rs_scriptdir/tools/scut.c"
fi

if rs_prepare_module "gmp"; then
	rs_do_command ../gmp/configure --prefix="$rs_supportprefixdir" --disable-shared --disable-werror
	rs_do_command $rs_makecmd -j $rs_cpucount
	rs_do_command $rs_makecmd check
	rs_do_command $rs_makecmd install
	rs_clean_module "gmp"
fi

if rs_prepare_module "mpfr"; then
	export CFLAGS="$rs_host_cflags"

	rs_do_command ../mpfr/configure --prefix="$rs_supportprefixdir" --with-gmp="$rs_supportprefixdir" --disable-shared
	rs_do_command $rs_makecmd -j $rs_cpucount
	rs_do_command $rs_makecmd check
	rs_do_command $rs_makecmd install
	rs_clean_module "mpfr"

	unset CFLAGS
fi

if rs_prepare_module "binutils"; then
	export CFLAGS="$rs_host_cflags"
	
	rs_do_command ../binutils/configure --prefix="$rs_prefixdir" --target="$rs_target" --disable-nls --disable-werror
	rs_do_command $rs_makecmd -j $rs_cpucount
	rs_do_command $rs_makecmd install
	rs_clean_module "binutils"
	
	unset CFLAGS
fi

if rs_prepare_module "gcc"; then
	export CFLAGS="$rs_host_cflags"
	export CFLAGS_FOR_TARGET="$rs_target_cflags"
	export CXXFLAGS_FOR_TARGET="$rs_target_cflags"
	
	rs_do_command ../gcc/configure --prefix="$rs_prefixdir" --target="$rs_target" --with-gmp="$rs_supportprefixdir" --with-mpfr="$rs_supportprefixdir" --with-pkgversion="RosBE-Unix" --enable-languages=c,c++ --enable-checking=release --enable-version-specific-runtime-libs --disable-shared --disable-nls --disable-werror
	rs_do_command $rs_makecmd -j $rs_cpucount
	rs_do_command $rs_makecmd install
	rs_clean_module "gcc"
	
	unset CFLAGS
	unset CFLAGS_FOR_TARGET
	unset CXXFLAGS_FOR_TARGET
fi

if rs_prepare_module "make"; then
	export CFLAGS="$rs_host_cflags"

	rs_do_command ../make/configure --prefix="$installdir" --disable-dependency-tracking --disable-nls --enable-case-insensitive-file-system --disable-job-server --disable-rpath --disable-werror
	rs_do_command $rs_makecmd -j $rs_cpucount
	rs_do_command $rs_makecmd install
	rs_clean_module "make"

	unset CFLAGS
fi

# NASM doesn't compile in a dedicated build directory, so just extract it
if rs_extract_module "nasm" "$rs_workdir"; then
	export CFLAGS="$rs_host_cflags"

	cd "nasm"
	rs_do_command ./configure --prefix="$rs_prefixdir"
	rs_do_command $rs_makecmd -j $rs_cpucount
	rs_do_command $rs_makecmd install
	rs_clean_module "nasm"

	unset CFLAGS
fi

# Final actions
echo
rs_boldmsg "Final actions"
cd "$installdir"

echo "Removing unneeded files..."
rm -rf share
rm -rf $rs_prefixdir/include $rs_prefixdir/info $rs_prefixdir/man $rs_prefixdir/share
rm -f $rs_prefixdir/lib/* >& /dev/null

# Keep the "include" and "lib" directories of the support files in case a subsequent RosBE-Unix package needs them
rm -rf $rs_supportprefixdir/info $rs_supportprefixdir/share

echo "Removing debugging symbols..."
find -executable -type f -exec strip -s {} ";" >& /dev/null
##### END almost shared buildtoolchain/RosBE-Unix building part ###############


echo "Copying scripts..."
cp -R "$rs_scriptdir/scripts/"* "$installdir"

echo "Writing version..."
echo "$ROSBE_VERSION" > "$installdir/RosBE-Version"
echo

# Finish
rs_boldmsg "Finished successfully!"
echo "To create a shortcut to the Build Environment on the Desktop, please switch back to your"
echo "normal User Account (I assume you ran this script as \"root\")."
echo "Then execute the following command:"
echo
echo "  $installdir/createshortcut.sh"
echo
echo "If you just want to start the Build Environment without using a shortcut, execute the"
echo "following command:"
echo
echo "  $installdir/RosBE.sh [source directory] [color code] [architecture]"
echo
echo "All parameters for that script are optional."
