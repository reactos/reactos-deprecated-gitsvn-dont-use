#!/bin/bash
#
# ReactOS Build Environment for Unix-based Operating Systems - Packaging tool for the Base package
# Copyright 2009 Colin Finck <colin@reactos.org>
#
# Released under GNU GPL v2 or any later version.

#
# Functions
#
check_file()
{
	if ! [ -f "$1" ]; then
		echo "File \"$1\" is required, but does not exist!"
		exit 1
	fi
}


# The real entry point
cd `dirname $0`

#
# Prerequisites
#
# Check the parameters
if [ "$2" = "" ]; then
	echo "makepackage - Package a RosBE-Unix version"
	echo "Syntax: ./makepackage.sh <package> <version>"
	echo
	echo " package - Package name (i.e. \"Base-i386\")"
	echo " version - Version number of the Build Environment to package (i.e. \"1.4\")"
	exit 1
fi

package_name="$1"
package_version="$2"

# Set the full package name
full_package_name="RosBE-Unix-$package_version"

case "$package_name" in
	"Base-i386")
		# Add no suffix
		;;
	*)
		# Add the package name as the suffix
		full_package_name+="-$package_name"
		;;
esac

# Check if the required directory structure exists
if ! [ -d "../Tools/.svn" ]; then
	echo "This script needs to be run in a working copy of the whole \"RosBE\" tree!"
	exit 1
fi

# Build the source file list
sources="binutils gcc "
tools=""

case "$package_name" in
	"Base-i386")
		sources+="gmp make mingw_runtime_dev mpfr nasm w32api"
		tools+="cpucount scut"
esac

# Check if all required files exist
for source in $sources; do
	check_file "$package_name/sources/$source.tar.bz2"
done

# Check if all required tools exist
for tool in $tools; do
	check_file "../Tools/$tool.c"
done 

# Check if other needed files exist
check_file "$package_name/README.pdf"


#
# The Process
#
# Copy the directory and delete some stuff
echo "Copying the \"$package_name\" directory..."
rm -rf "$full_package_name"
cp -R "$package_name" "$full_package_name"

find "$full_package_name" -type d -name ".svn" -exec rm -rf {} ";" >& /dev/null
find "$full_package_name" -type f -name "SVN-Readme.txt" -exec rm {} ";"
rm "$full_package_name/README.odt"

# Copy the shared tools
for tool in $tools; do
	echo "Copying $tool.c..."
	cp "../Tools/$tool.c" "$full_package_name/tools/$tool.c"
done

# Finally create a package out of this
echo "Creating the package..."
tar cjf "$full_package_name.tar.bz2" "$full_package_name"

echo
echo "Done!"
