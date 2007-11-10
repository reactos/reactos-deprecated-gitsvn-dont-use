ReactOS Build Environment 1.0


The ReactOS Build Environment v1.0 contains a complete build environment for ReactOS based on GCC 4.1.3/MinGW.

While installing the ReactOS Build Environment you are asked to provide the directory where your ReactOS sources are located or should be checked out to (ie.reactos\), you must enter the complete path to this folder (ie. C:\projects\reactos). The path is now required for the ReactOS Build Environment to work correctly, so if you change where your sources are located you must also change the entry in the shortcuts in the start menu (the 'Start in:' entry). If you did not checkout a Source tree yet, start RosBE and type “svn create”. This will take a while.

To build ReactOS with the ReactOS Build Environment you run "GCC 4.1.3 ReactOS Build Environment" from the start
menu. Now you have a choice of using either the built in commands (which are displayed onscreen with information
about their use) or you can build as you normally would using the standard MinGW/RBuild commands.

The ReactOS Build Environment v1.0 contains the following packages:

binutils-2.18.50-20071015
   patched with:
   - binutils-2.18.50.10-15-2007-snapshot-hpoussin.patch
mingw-runtime-3.13
w32api-3.10
mingw32-make-3.81-2
nasm-0.99.06-win32
mingw-4.1.3-20071015-prerelease
   patched with:
   - “GCC-v4.1-r129382-prerelease-w3seek.patch”
   - “GCC-v4.1-r129382-virtual-stdcall-bug27067.patch”
   - “GCC-v4.1-r129490-CreateFileMapping-Vista-bug30335.patch”

Websites:

MinGW - Minimalist GNU for Windows
http://www.mingw.org

GCC, the GNU Compiler Collection
http://www.gcc.org

NASM, the Netwide Assembler
http://www.kernel.org/pub/software/devel/nasm
http://nasm.sourceforge.net

NSIS (Nullsoft Scriptable Install System)
http://nsis.sourceforge.net

Subversion Client
http://subversion.tigris.org

Inspired by blight's ReactOS Build Environment v0.2-3.4.5
Inspired by Dazzle from TinyKRNL (http://www.tinykrnl.org)
Icon made by ROSFan
