#!/bin/sh

EXE=
if [ x`uname -o` = xCygwin ] ; then
	EXE=.exe
fi

if [ "x$1" == xclean ] ; then
    rm -rf binutils-configure gcc-configure
    exit 0
fi

if [ "x$1" == x ] ; then
    echo usage: "$0" [install-dir]
    exit 1
fi

idir="$1"
libdir="$idir"/lib

# Build and install binutils
if [ ! -d build/binutils-configure ] ; then
    mkdir -p build/binutils-configure || exit 1
fi
cd build/binutils-configure || exit 1
sh ../../gnu/binutils-2.16.1/configure \
    --prefix="$idir" \
    --target=powerpc-unknown-elf && \
make && \
make -C binutils CFLAGS="-g -DDLLTOOL_PPC" dlltool$EXE && \
make -C binutils CFLAGS="-g -DDLLTOOL_PPC" windres$EXE && \
make install && \
cp binutils/dlltool "$idir"/bin/powerpc-unknown-elf-dlltool && \
cp binutils/windres "$idir"/bin/powerpc-unknown-elf-windres || exit 1
cd ../..
# Copy extra files
cp -r ldscript mingw-crt-headers "$libdir" || exit 1

# Build and install gcc
if [ ! -d build/gcc-configure ] ; then
    mkdir build/gcc-configure || exit 1
fi
export PATH="$idir/bin":$PATH
cd build/gcc-configure || exit 1
sh ../../gnu/gcc-4.1.0/configure \
    --prefix="$idir" \
    --target=powerpc-unknown-elf \
    --disable-ssp \
    --disable-threads \
    --enable-languages=c || exit 1
make
make install || exit 1
cd ../..

# Make elfpe
make -C elfpe && cp elfpe/elfpe ovr/powerpc-unknown-elf-gcc || exit 1
# Make env script
echo '#!/bin/sh' > "$idir"/rosbe
echo 'THISDIR=`dirname $0`' >> "$idir"/rosbe
echo export PATH='"$THISDIR/ovr:$THISDIR/bin:$PATH"' >> "$idir"/rosbe
echo export 'INSTALLDIR="$THISDIR"' >> "$idir"/rosbe
echo export OLDMAKE=`which make` >> "$idir"/rosbe
echo echo "\"Run make in your reactos dir to build\"" >> "$idir"/rosbe
echo exec $SHELL >> "$idir"/rosbe
chmod +x "$idir"/rosbe
echo Run "\"$idir/rosbe\"" to get a shell with the right settings
# Make wrappers
cp -r ovr "$idir"
