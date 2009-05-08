#!/bin/bash
#
# ReactOS Build Environment for Windows - Script for building a binutils/GCC/mingw-runtime/w32api toolchain for Windows
# Partly based on RosBE-Unix' "RosBE-Builder.sh"
# Copyright 2009 Colin Finck <colin@reactos.org>
#
# Released under GNU GPL v2 or any later version.

##################################################################################
# This script was tested with the following toolchain versions:
# - binutils v2.19.51-20090127 (snapshot) using the corresponding ReactOS patches
# - gcc v4.1.3-20071015 (snapshot) using the corresponding ReactOS patches
# - mingw-runtime 3.13
# - w32api 3.10
##################################################################################

#
# Constants
#
CFLAGS="-march=i686 -O2"
CXXFLAGS="$CFLAGS"
INSTALLDIR="/c/buildtoolchain/mingw"
LDFLAGS="-s"
MINGWDIR="/mingw"
NEEDED_TOOLS="bison flex gcc g++ grep make makeinfo"
TARGET="mingw32"
TEMPDIR="/c/buildtoolchain"
WIN32_INSTALLDIR="c:/buildtoolchain/mingw"

# Make sure we build an entirely native compiler, since MSYS might report subtle differences (i.e. "i686-pc-mingw32" instead of "mingw32")
BUILD="$TARGET"
HOST="$TARGET"

# The following variables need to be accessed by Make and its subprocesses.
export CFLAGS
export CXXFLAGS
export LDFLAGS

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
		echo "Please take a look at the log file \"$TEMPDIR/build.log\""
		echo "Aborted!"
		exit 1
	else
		greenmsg "OK"
		rm "$TEMPDIR/build.log"
	fi
}

#
# Entry point
#
echo "buildtoolchain - Build a binutils/GCC/mingw-runtime/w32api toolchain for Windows"
echo "by Colin Finck <colin@reactos.org>"
echo

if [ "`uname -o`" != "Msys" ]; then
	echo "You can only run this script under a MSYS environment!"
	exit 1
fi

# We don't want too less nor too many parameters
if [ "$1" == "" ] || [ "$4" != "" ]; then
	echo "Syntax: ./buildtoolchain.sh <sources> [binutils] [gcc]"
	echo
	echo " sources  - Path to the directory containing RosBE-Unix toolchain packages (.tar.bz2 files)"
	echo " binutils - optional; whether binutils shall be built (0/1, default is 1)"
	echo " gcc      - optional; whether gcc shall be built (0/1, default is 1)"
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

if $toolmissing; then
	echo "At least one needed tool is missing, aborted!"
	exit 1
fi

echo

# Get the absolute path to the source directory
cd "$1"
source_path="$PWD"

# What do we need to process?
if [ "$2" == "0" ]; then
	process_binutils=false
else
	process_binutils=true
fi

if [ "$3" == "0" ]; then
	process_gcc=false
else
	process_gcc=true
fi

# Create all necessary directories
if ! [ -d "$INSTALLDIR/$TARGET" ]; then
	mkdir -p "$INSTALLDIR/$TARGET"
fi

if [ -d "$MINGWDIR" ]; then
	rm -rf "$MINGWDIR"
fi

mkdir -p "$MINGWDIR"

if ! [ -d "$TEMPDIR" ]; then
	mkdir -p "$TEMPDIR"
fi

cd "$MINGWDIR"

boldmsg "Building..."

# mingw-runtime
echo -n "Extracting mingw-runtime... "
tar -xjf "$source_path/mingw-runtime.tar.bz2" >& "$TEMPDIR/build.log"
check_run

# w32api
echo -n "Extracting w32api... "
tar -xjf "$source_path/w32api.tar.bz2" >& "$TEMPDIR/build.log"
check_run

# Copy the same stuff to the target directory (needed for building gcc and later getting the files located properly)
echo "Copying files... "
cp -R $MINGWDIR/* "$INSTALLDIR/$TARGET"

cd "$TEMPDIR"

# binutils
if $process_binutils; then
	rm -rf "binutils"
	rm -rf "binutils-build"

	echo -n "Extracting binutils... "
	tar -xjf "$source_path/binutils.tar.bz2" >& build.log
	check_run

	echo -n "Configuring binutils... "
	mkdir "binutils-build"
	cd "binutils-build"
	../binutils/configure --prefix="$INSTALLDIR" --build="$BUILD" --host="$HOST" --target="$TARGET" --disable-nls --disable-shared >& "$TEMPDIR/build.log"
	check_run

	echo -n "Building binutils... "
	make >& "$TEMPDIR/build.log"
	check_run

	echo -n "Installing binutils... "
	make install >& "$TEMPDIR/build.log"
	check_run

	echo "Cleaning up binutils..."
	cd "$TEMPDIR"
	rm -rf "binutils-build"
	rm -rf "binutils"
fi

# gcc
if $process_gcc; then
	rm -rf "gcc"
	rm -rf "gcc-build"

	echo -n "Extracting gcc... "
	tar -xjf "$source_path/gcc.tar.bz2" >& build.log
	check_run

	echo -n "Configuring gcc... "
	mkdir "gcc-build"
	cd "gcc-build"
	
	../gcc/configure --prefix="$WIN32_INSTALLDIR" --build="$BUILD" --host="$HOST" --target="$TARGET" --with-ld=/bin/ld \
			--enable-languages=c,c++ --enable-checking=release \
			--enable-threads=win32 --disable-win32-registry --disable-nls  \
			--disable-shared >& "$TEMPDIR/build.log"
	check_run
	
	echo -n "Building gcc... "
	make >& "$TEMPDIR/build.log"
	check_run

	echo -n "Installing gcc... "
	make install >& "$TEMPDIR/build.log"
	check_run

	echo "Cleaning up gcc..."
	cd "$TEMPDIR"
	rm -rf "gcc-build"
	rm -rf "gcc"
fi

# Final actions
echo
boldmsg "Final actions"
cd "$INSTALLDIR"

echo "Removing unneeded files..."
mv $TARGET/bin/* bin
mv $TARGET/include/* include
mv $TARGET/lib/* lib
rm -rf $TARGET info man share
rm -f bin/c++.exe bin/gccbug bin/$TARGET-*

echo "Removing debugging symbols..."
find -name "*.exe" -type f -exec strip -s {} ";"

echo "Finished!"
