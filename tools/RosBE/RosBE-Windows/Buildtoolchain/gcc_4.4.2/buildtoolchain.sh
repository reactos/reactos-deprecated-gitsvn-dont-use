#!/bin/bash
#
# ReactOS Build Environment for Windows - Script for building a binutils/GCC/mingw-runtime/w32api toolchain for Windows
# Partly based on RosBE-Unix' "RosBE-Builder.sh"
# Copyright 2009 Colin Finck <colin@reactos.org>
#
# Released under GNU GPL v2 or any later version.

########################################################################################################################
# This script was built for the following toolchain versions:
# - binutils 2.20.51-20091017 (snapshot)
# - gcc 4.4.2
#   patched with:
#      * http://gcc.gnu.org/bugzilla/attachment.cgi?id=18882&action=view (committed in GCC r153606)
# - gmp 4.3.1
# - mingw-runtime 3.16
# - mpfr 2.4.2-rc1
# - w32api 3.13
#
# These tools were compiled under MSYS with "gcc version 3.4.5 (mingw-vista special r3)"
#
# These versions are used in RosBE-Windows 1.5 and RosBE-Unix 1.5.
# Get the toolchain packages from this RosBE-Unix version.
########################################################################################################################


#
# Constants
#
WORKDIR="/c/buildtoolchain"

HOST_CFLAGS="-pipe -fno-common -O3 -march=pentium3 -mfpmath=sse"
HOST_GCC_VERSION="gcc version 3.4.5 (mingw-vista special r3)"
INSTALLDIR="$WORKDIR/mingw"
NEEDED_TOOLS="bison flex gcc g++ grep make makeinfo"
SUPPORTDIR="$WORKDIR/support"
SYSHEADERDIR="/mingw/include"
TARGET="mingw32"
TARGET_CFLAGS="-pipe -gstabs+ -O3 -march=pentium -mtune=i686"


#
# Functions
#
boldmsg()
{
	echo -e $2 "\e[1m$1\e[0m"
}

greenmsg()
{
	echo -e $2 "\e[32m$1\e[0m"
}

redmsg()
{
	echo -e $2 "\e[31m$1\e[0m"
}

check_run()
{
	if [ $? -ne 0 ]; then
		redmsg "FAILED"
		echo "Please take a look at the log file \"$WORKDIR/build.log\""
		echo "Aborted!"
		exit 1
	else
		greenmsg "OK"
		rm "$WORKDIR/build.log"
	fi
}

mkdir_if_not_exists()
{
	if ! [ -d "$1" ]; then
		mkdir -p "$1"
	fi
}

mkdir_empty()
{
	if [ -d "$1" ]; then
		rm -rf "$1"
	fi
	
	mkdir -p "$1"
}

prepare_module()
{
	# Check if the module shall be built and extract it then
	process_it=$1
	module=$2
	
	if [ "$process_it" = "0" ]; then
		return 1
	fi
	
	cd "$WORKDIR"
	rm -rf "$module"
	rm -rf "$module-build"
		
	echo -n "Extracting $module... "
	tar -xjf "$source_path/$module.tar.bz2" >& build.log
	check_run
	
	mkdir "$module-build"
	cd "$module-build"
	
	return 0
}

do_command()
{
	echo -n "Running \"$*\"... "
	$* >& "$WORKDIR/build.log"
	check_run
}

clean_module()
{
	echo "Cleaning up $1..."
	cd "$WORKDIR"
	rm -rf "$1-build"
	rm -rf "$1"
}


#
# Entry point
#
echo "buildtoolchain - Build a binutils/GCC/mingw-runtime/w32api toolchain for Windows"
echo "Script for gcc_4.4.2 (see script header for details)"
echo "by Colin Finck <colin@reactos.org>"
echo

if [ "`uname -o`" != "Msys" ]; then
	echo "You can only run this script under a MSYS environment!"
	exit 1
fi

# We don't want too less parameters
if [ "$1" == "" ]; then
	echo "Syntax: ./buildtoolchain.sh <sources> [w32api] [mingw-runtime] [gmp] [mpfr] [binutils] [gcc]"
	echo
	echo " sources  - Path to the directory containing RosBE-Unix toolchain packages (.tar.bz2 files)"
	echo
	echo "The rest of the arguments are optional. You specify them if you want to prevent a component"
	echo "from being (re)built. Do this by passing 0 as the argument of the appropriate component."
	echo "Pass 1 if you want them to be built."
	echo "By default, all of these components are built, so you don't need to pass any of these parameters."
	exit 1
fi

# Get the absolute path to the script directory
cd `dirname $0`
SCRIPTDIR="$PWD"

# Find out whether the script directory contains spaces
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

# Check for the correct GCC version
echo -n "Checking for the correct GCC version... "

if gcc -v 2>&1 | grep "$HOST_GCC_VERSION" >& /dev/null; then
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

# Get the absolute path to the source directory
cd "$1"
source_path="$PWD"

# Create all necessary directories
mkdir_if_not_exists "$INSTALLDIR/$TARGET"
mkdir_empty         "$SYSHEADERDIR"
mkdir_if_not_exists "$SUPPORTDIR"

# Process each module
boldmsg "Building..."


if [ "$2" != "0" ]; then
	echo -n "Extracting w32api... "
	
	cd "$INSTALLDIR/$TARGET"
	tar -xjf "$source_path/w32api.tar.bz2" >& "$WORKDIR/build.log"
	check_run
fi

if prepare_module "$3" "mingw-runtime"; then
	export CFLAGS="$TARGET_CFLAGS"
	export C_INCLUDE_PATH="$INSTALLDIR/$TARGET/include"
	
	do_command ../mingw-runtime/configure --prefix="$INSTALLDIR/$TARGET" --host="$TARGET" --build="$TARGET"
	do_command make
	do_command make install
	
	# libmingwex.a of the MinGW Runtime 3.15 and later references to _get_output_format, which is exported by msvcr80.dll and later.
	# For older Runtime DLLs (such as the msvcrt.dll we primarily use for ReactOS), MinGW adds a _get_output_format function returning 0 to libmsvcrt.a.
	# As we use MinGW's libmingwex.a, but our own libmsvcrt.a, linking to MinGW's libmingwex.a will fail by default. Therefore we have to create an archive for the compiled
	# _get_output_format stub, copy it to our "lib" directory and include it when linking to libmingwex.a.
	ar r ofmt_stub.a ofmt_stub.o >& /dev/null
	cp ofmt_stub.a "$INSTALLDIR/$TARGET/lib"
	
	clean_module mingw-runtime
	
	export CFLAGS=""
	export C_INCLUDE_PATH=""
fi

if prepare_module "$4" "gmp"; then
	export CFLAGS="$HOST_CFLAGS"
	
	do_command ../gmp/configure ABI=32 --prefix="$SUPPORTDIR" --host="$TARGET" --build="$TARGET" --disable-shared
	do_command make
	do_command make check
	do_command make install
	clean_module gmp
	
	export CFLAGS=""
fi

if prepare_module "$5" "mpfr"; then
	export CFLAGS="$HOST_CFLAGS"
	
	do_command ../mpfr/configure --prefix="$SUPPORTDIR" --host="$TARGET" --build="$TARGET" --with-gmp="$SUPPORTDIR" --disable-shared
	do_command make
	do_command make check
	do_command make install
	clean_module mpfr
	
	export CFLAGS=""
fi

if prepare_module "$6" "binutils"; then
	export CFLAGS="$HOST_CFLAGS"
	
	do_command ../binutils/configure --prefix="$INSTALLDIR" --host="$TARGET" --build="$TARGET" --target="$TARGET" --disable-nls
	do_command make
	do_command make install
	clean_module binutils
	
	export CFLAGS=""
fi

if prepare_module "$7" "gcc"; then
	export STAGE1_CFLAGS="$HOST_CFLAGS"
	export BOOT_CFLAGS="$HOST_CFLAGS"
	export CFLAGS_FOR_TARGET="$TARGET_CFLAGS"
	export CXXFLAGS_FOR_TARGET="$TARGET_CFLAGS"
	export C_INCLUDE_PATH="$INSTALLDIR/$TARGET/include"
	export LIBRARY_PATH="$INSTALLDIR/$TARGET/lib"
	
	do_command ../gcc/configure --prefix="$INSTALLDIR" --host="$TARGET" --build="$TARGET" --target="$TARGET" --with-gmp="$SUPPORTDIR" --with-mpfr="$SUPPORTDIR" --enable-languages=c,c++ --enable-checking=release --enable-version-specific-runtime-libs --enable-threads=win32 --disable-win32-registry --disable-shared --disable-nls
	do_command make profiledbootstrap
	do_command make install
	clean_module gcc
	
	export STAGE1_CFLAGS=""
	export BOOT_CFLAGS=""
	export CFLAGS_FOR_TARGET=""
	export CXXFLAGS_FOR_TARGET=""
	export C_INCLUDE_PATH=""
	export LIBRARY_PATH=""
fi


# Final actions
echo
boldmsg "Final actions"
cd "$INSTALLDIR"

echo "Removing unneeded files..."
rm -rf $TARGET/bin $TARGET/doc $TARGET/share include info man share
rm -f lib/* >& /dev/null
rm -f bin/c++.exe bin/gccbug bin/$TARGET-*

echo "Removing debugging symbols..."
find -name "*.exe" -type f -exec strip -s {} ";"
find -name "*.a" -type f -exec strip -d {} ";"
find -name "*.o" -type f -exec strip -d {} ";"

echo "Finished!"
