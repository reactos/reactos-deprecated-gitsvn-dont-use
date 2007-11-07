#!/bin/bash
#
# RosBE-Builder
# Copyright 2007 Colin Finck <mail@colinfinck.de>
# partially based on the BuildMingwCross script (http://www.mingw.org/MinGWiki/index.php/BuildMingwCross)
#
# Released under GNU GPL v2 or any later version.

# Constants
ROSBE_VERSION="0.3.7-SVN"
KNOWN_ROSBE_VERSIONS="0.3.6 0.3.7-SVN"
DEFAULT_INSTALL_DIR="/usr/RosBE"
NEEDED_TOOLS="bison flex gcc g++ grep makeinfo"		# GNU Make has a special check

# Get the absolute path to the script directory
cd `dirname $0`
SCRIPTDIR="$PWD"
SOURCEDIR="$SCRIPTDIR/sources"

# Functions
boldmsg()
{
	echo -e "\e[1m$1\e[0m"
}

greenmsg()
{
	echo -e "\e[32m$1\e[0m"
}

redmsg()
{
	echo -e "\e[31m$1\e[0m"
}

checkrun()
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


echo "*******************************************************************************"
echo "*         ReactOS Build Environment for Unix-based Operating Systems          *"
echo "*                                 Builder Tool                                *"
echo "*                      by Colin Finck <mail@colinfinck.de>                    *"
echo "*                                                                             *"
echo "*                                Version $ROSBE_VERSION                                *"
echo "*******************************************************************************"

echo
echo "This script compiles and installs a complete Build Environment for building ReactOS."
echo

# Check if we're running as root
if [ ! "`whoami`" = "root" ]; then
	boldmsg "User is not \"root\""

	echo "It is recommended to run this script as the \"root\" user. Otherwise you probably cannot create the necessary directories."
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

# Select the installation directory
boldmsg "Installation Directory"

echo "In which directory do you want to install it?"
echo "Enter the path to the directory here or simply press ENTER to install it into the default directory."

createdir=false
installdir=""
update=false

while [ "$installdir" = "" ]; do
	read -p "[$DEFAULT_INSTALL_DIR] " installdir
	echo

	if [ "$installdir" = "" ]; then
		installdir=$DEFAULT_INSTALL_DIR
	fi

	# Check if the installation directory already exists
	if [ -f "$installdir" ]; then
		echo "The directory \"$installdir\" is a file! Please enter another directory!"
		echo
		installdir=""

	elif [ -d "$installdir" ]; then
		# Check if the directory is empty
		if [ ! "`ls $installdir`" = "" ]; then
			if [ -f "$installdir/RosBE-Version" ]; then
				# Allow the user to update an already installed RosBE version
				choice=""
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

							showchoice "R" "r R c C"
						else
							echo "An older version of the Build Environment has been found in this directory."
							echo "Please choose one of the following options:"
							echo
							echo " (U)pdate the existing Build Environment"
							echo " (R)einstall all new components of the Build Environment"
							echo " (C)hoose a different installation directory"
							echo

							showchoice "U" "u U r R c C"
						fi

						echo
						break
					fi
				done

				if [ "$choice" = "U" ] || [ "$choice" = "u" ]; then
					update=true
				elif [ "$choice" = "C" ] || [ "$choice" = "c" ]; then
					echo "Please enter another directory!"
					installdir=""
				fi
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
		createdir=true
		echo "The directory \"$installdir\" does not exist. The installation script will create it for you."
		echo
	fi
done

# Ready to start
boldmsg "Ready to start"

echo "Ready to build and install the ReactOS Build Environment."
echo "Press ENTER to continue or any other key to exit."
read answer

if [ "$answer" != "" ]; then
	exit 1
fi

if $update; then
	setvalues=false

	process_cpucount=false
	process_mingwruntime=false
	process_w32api=false
	process_binutils=false
	process_gcc=false
	process_make=false
	process_nasm=false
	process_buildtime=false

	# Logic behind this update part:
	#   - KNOWN_ROSBE_VERSIONS contains all versions from the oldest to the newest one (in this order!)
	#   - setvalues will be set to true, when the iterator reaches the installed version
	#   - Then the processing settings of the installed and the later versions will be applied.
	#     (i.e. if gcc changed from 0.3.7 to 0.3.8, but not from 0.3.6 to 0.3.7, the gcc build will correctly be reprocessed
	#      if the user has an installed RosBE 0.3.6 and wants to update to 0.3.8)
	for known_version in $KNOWN_ROSBE_VERSIONS; do
		if [ "$known_version" = "$installed_version" ]; then
			setvalues=true
		fi

		if $setvalues; then
			case "$known_version" in
				"0.3.6")
					# TODO: Define what has to be reprocessed, when updating from 0.3.6 to the next version
					process_cpucount=true
					;;
			esac
		fi
	done
else
	process_cpucount=true
	process_mingwruntime=true
	process_w32api=true
	process_binutils=true
	process_gcc=true
	process_make=true
	process_nasm=true
	process_buildtime=true

	# Create the directory if necessary
	if $createdir; then
		if ! mkdir -p "$installdir"; then
			redmsg "Could not create \"$installdir\", aborted!"
			exit 1
		fi
	fi
fi

# Test if the installation directory is writeable
if [ ! -w "$installdir" ]; then
	redmsg "Installation directory \"$installdir\" is not writeable, aborted!"
	exit 1
fi

#
# Build the tools
#
boldmsg "Building..."
PATH="$installdir/bin:$PATH"
mkdir -p "$installdir/bin" >& /dev/null
mkdir -p "$installdir/mingw32" >& /dev/null

# cpucount
if $process_cpucount; then
	echo -n "Compiling cpucount... "
	gcc -o "$installdir/bin/cpucount" "$SCRIPTDIR/tools/cpucount.c"
	checkrun
fi

CPUCOUNT=`$installdir/bin/cpucount`
cd "$installdir/mingw32"

# mingw-runtime
if $process_mingwruntime; then
	echo -n "Extracting mingw-runtime... "
	tar -xjf "$SOURCEDIR/mingw-runtime.tar.bz2" >& "$SCRIPTDIR/tar.log"
	checkrun "tar"
fi

# w32api
if $process_w32api; then
	echo -n "Extracting w32api... "
	tar -xjf "$SOURCEDIR/w32api.tar.bz2" >& "$SCRIPTDIR/tar.log"
	checkrun "tar"
fi

cd "$SOURCEDIR"

# binutils
if $process_binutils; then
	rm -rf "binutils"
	rm -rf "binutils-build"
	
	echo -n "Extracting binutils... "
	tar -xjf "binutils.tar.bz2" >& "$SCRIPTDIR/tar.log"
	checkrun "tar"
	
	echo -n "Configuring binutils... "
	mkdir "binutils-build"
	cd "binutils-build"
	../binutils/configure --prefix="$installdir" --target=mingw32 --disable-nls --with-gcc \
			--with-gnu-as --with-gnu-ld --disable-shared >& "$SCRIPTDIR/configure.log"
	checkrun "configure"
	
	echo -n "Building binutils... "
	$makecmd -j $CPUCOUNT CFLAGS="-O2 -fno-exceptions" LDFLAGS="-s" >& "$SCRIPTDIR/make.log"
	checkrun "make"
	
	echo -n "Installing binutils... "
	$makecmd install >& "$SCRIPTDIR/make.log"
	checkrun "make"
	
	echo -n "Cleaning up binutils... "
	cd "$SOURCEDIR"
	rm -rf "binutils-build"
	rm -rf "binutils"
	checkrun
fi

# gcc
if $process_gcc; then
	rm -rf "gcc"
	rm -rf "gcc-build"
	
	echo -n "Extracting gcc... "
	tar -xjf "gcc.tar.bz2" >& "$SCRIPTDIR/tar.log"
	checkrun "tar"
	
	echo -n "Configuring gcc... "
	mkdir "gcc-build"
	cd "gcc-build"
	../gcc/configure --prefix="$installdir" --target=mingw32 \
			--with-headers="$installdir/mingw32/include"--with-gcc --with-gnu-ld \
			--with-gnu-as --enable-languages=c,c++ --enable-checking=release \
			--enable-threads=win32 --disable-win32-registry --disable-nls  \
			--disable-shared >& "$SCRIPTDIR/configure.log"
	checkrun "configure"
	
	echo -n "Building gcc... "
	$makecmd -j $CPUCOUNT CFLAGS="-O2" CXXFLAGS="-O2" LDFLAGS="-s" >& "$SCRIPTDIR/make.log"
	checkrun "make"
	
	echo -n "Installing gcc... "
	$makecmd install >& "$SCRIPTDIR/make.log"
	checkrun "make"
	
	echo -n "Cleaning up gcc... "
	cd "$SOURCEDIR"
	rm -rf "gcc-build"
	rm -rf "gcc"
	checkrun
fi

# make
if $process_make; then
	rm -rf "make"
	rm -rf "make-build"
	
	echo -n "Extracting make... "
	tar -xjf "make.tar.bz2" >& "$SCRIPTDIR/tar.log"
	checkrun "tar"
	
	echo -n "Configuring make... "
	mkdir "make-build"
	cd "make-build"
	../make/configure --prefix="$installdir" --target=mingw32 \
			--disable-dependency-tracking --disable-nls \
			--enable-case-insensitive-file-system --disable-job-server --disable-rpath \
			--program-prefix=mingw32- >& "$SCRIPTDIR/configure.log"
	checkrun "configure"
	
	echo -n "Building make... "
	$makecmd -j $CPUCOUNT CFLAGS="-s -O2 -mms-bitfields" >& "$SCRIPTDIR/make.log"
	checkrun "make"
	
	echo -n "Installing make... "
	$makecmd install >& "$SCRIPTDIR/make.log"
	checkrun "make"
	
	echo -n "Cleaning up make... "
	cd "$SOURCEDIR"
	rm -rf "make-build"
	rm -rf "make"
	checkrun
fi

# nasm
if $process_nasm; then
	rm -rf "nasm"
	rm -rf "nasm-build"
	
	echo -n "Extracting nasm... "
	tar -xjf "nasm.tar.bz2" >& "$SCRIPTDIR/tar.log"
	checkrun "tar"
	
	echo -n "Configuring nasm... "
	mkdir "nasm-build"
	cd "nasm-build"
	../nasm/configure --prefix="$installdir" --target=mingw32  >& "$SCRIPTDIR/configure.log"
	checkrun "configure"
	
	echo -n "Building nasm... "
	$makecmd -j $CPUCOUNT >& "$SCRIPTDIR/make.log"
	checkrun "make"
	
	echo -n "Installing nasm... "
	$makecmd install >& "$SCRIPTDIR/make.log"
	checkrun "make"
	
	echo -n "Cleaning up nasm... "
	cd "$SOURCEDIR"
	rm -rf "nasm-build"
	rm -rf "nasm"
	checkrun
fi

# Final actions
echo
boldmsg "Final actions"

if $process_buildtime; then
	echo -n "Compiling buildtime... "
	gcc -o "$installdir/bin/buildtime" "$SCRIPTDIR/tools/buildtime.c"
	checkrun
fi

echo -n "Removing unneeded files... "
rm -rf "$installdir/mingw32/sys-include"
rm -rf "$installdir/info"
rm -rf "$installdir/man"
rm -rf "$installdir/share"
checkrun

echo -n "Removing debugging symbols... "
ls "$installdir"/bin/* "$installdir"/mingw32/bin/* | grep -v "gccbug" |
while read file; do
	strip "$file"
done
checkrun

echo -n "Copying scripts... "
cp "$SCRIPTDIR"/scripts/* "$installdir"
checkrun

echo -n "Writing version... "
echo "$ROSBE_VERSION" > "$installdir/RosBE-Version"
checkrun
echo

# Finish
boldmsg "Finished successfully!"
echo "To create a shortcut to the Build Environment on the Desktop, please switch back to your"
echo "normal User Account (I assume you ran this script as \"root\")."
echo "Then execute the following command:"
echo
echo "  $installdir/createshortcut.sh"
echo
echo "If you just want to start the Build Environment without using a shortcut, execute the"
echo "following command:"
echo
echo "  $installdir/RosBE.sh <path/to/your/ReactOS/source/directory> [optional color code]"
echo

exit 0
