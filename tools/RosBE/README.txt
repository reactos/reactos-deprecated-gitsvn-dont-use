                     ReactOS Build Environment 0.3.5b2


    The ReactOS Build Environment v0.3.5b2 contains a complete
build environment for ReactOS based on GCC 4.1.2/MinGW.

    While installing the ReactOS Build Environment you are asked to
provide the directory where your ReactOS sources are located (ie.
reactos\), you must enter the complete path to them (ie. C:\projects\reactos).
The path is now required for the ReactOS Build Environment to work correctly
so if you change where your sources are located you must also change the entry
in the shortcuts in the start menu (the 'Start in:' entry).

    To build ReactOS with the ReactOS Build Environment you run
"GCC 4.1.2 ReactOS Build Environment" from the start menu. Now you have
a choice of using either the built in commands (which are displayed onscreen
with information about their use) or you can build as you normally would
using the standard MinGW/RBuild commands.

The ReactOS Build Environment v0.3.5b2 contains the following
packages:

binutils-2.17.50-20060824-1.tar.gz
mingw-runtime-3.11.tar.gz
w32api-3.8.tar.gz
mingw32-make-3.81-1.tar.gz
nasm-0.98.39-win32.zip
gcc-4.1.2-20070122-Snapshot-Patched.7z
    - Patched to fix a GCC bug concerning
      decorating virtual methods with stdcall
      in C++, see GCC issue:
      http://gcc.gnu.org/bugzilla/show_bug.cgi?id=27067
    - Patched to run on Vista under UAC, see GCC issue:
      http://gcc.gnu.org/bugzilla/show_bug.cgi?id=30335

Websites:

MinGW - Minimalist GNU for Windows
http://www.mingw.org/

GCC, the GNU Compiler Collection
http://www.gcc.org/

NASM, the Netwide Assembler
http://www.kernel.org/pub/software/devel/nasm/
http://nasm.sourceforge.net/

NSIS (Nullsoft Scriptable Install System)
http://nsis.sourceforge.net/

The GnuWin32 Project (tee/test and their dependencies were obtained here)
http://gnuwin32.sourceforge.net/

Inspired by blight's ReactOS Build Environment v0.2-3.4.4
Inspired by Dazzle from TinyKRNL (http://www.tinykrnl.org/)
