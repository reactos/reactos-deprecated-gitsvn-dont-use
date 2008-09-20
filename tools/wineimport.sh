#!/bin/bash
#
# Copyright (C) 2006-2008 Hervé Poussineau (hpoussin@reactos.org)
# Copyright (C) 2008 Pierre Schweitzer (pschweitzer@reactos.org)

# ****************************************************************************
# Internal functions
int_help()
{
	echo "Syntax:"
	echo "$0 download {wine_lib} [{wine_lib} ...]"
	echo "$0 createrbuild {wine_lib} [{wine_lib} ...]"
	echo "$0 link {path_to_reactos} {wine_lib}"
	echo "$0 merge {path_to_reactos} {wine_lib} [{wine_lib} ...]"
	echo "$0 make {path_to_reactos} {wine_lib} [{wine_lib} ...]"
	echo "$0 fullprocessing {path_to_reactos} {wine_lib} [{wine_lib} ...]"
	exit 0
}

int_download()
{
	if [ "$2" == "" ]; then
		int_help
	fi
	export CVSROOT=":pserver:cvs:cvs@cvs.winehq.org:/home/wine"
	cvs login &>/dev/null
	if [ $? = 127 ]; then
		echo "Error when executing cvs. Try to download the lastest version using"
		echo "sudo apt-get install cvs"
	fi
	while [ "$2" != "" ]; do
		echo "Downloading $2 ..."
		if [ ! -e "wine/dlls/$2/Makefile.in" ]; then
			cvs -z 3 checkout wine/dlls/$2 #&>/dev/null
		fi
		if [ ! -e "wine/dlls/$2/Makefile.in" ]; then
			echo "Error when downloading $2. Are you sure it does exist?"
			exit 0
		fi
		cvs -z 3 update -dPA wine/dlls/$2 #&>/dev/null
		shift
	done
	cvs logout &>/dev/null
	exit 0
}

# ****************************************************************************
# Stubs for unimplemented functions
int_createbuild()
{
	echo "createbuild isn't implemented!"
	int_help
}
int_link()
{
	echo "link isn't implemented!"
	int_help
}
int_merge()
{
	echo "merge isn't implemented!"
	int_help
}
int_make()
{
	echo "make isn't implemented!"
	int_help
}
int_fullprocessing()
{
	echo "fullprocessing isn't implemented!"
	int_help
}

# ****************************************************************************
# Script

WINE_TMPFILE1="tmpfile1.wine"

if [ "$1" == "download" ]; then
	int_download "$@"
elif [ "$1" == "createrbuild" ]; then
	int_createrbuild
elif [ "$1" == "link" ]; then
	int_link
elif [ "$1" == "merge" ]; then
	int_merge
elif [ "$1" == "make" ]; then
	int_make
elif [ "$1" == "fullprocessing" ]; then
	int_fullprocessing	
else
	int_help
fi
