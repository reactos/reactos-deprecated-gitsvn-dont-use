dnl config.m4 for extension roscms

PHP_ARG_ENABLE(roscms, whether to enable roscms support,
[  --enable-roscms           Enable roscms support])

if test "$PHP_ROSCMS" != "no"; then
  PHP_NEW_EXTENSION(roscms, roscms.c, $ext_shared)
fi
