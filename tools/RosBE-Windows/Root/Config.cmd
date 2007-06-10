@echo off

echo ARCH
echo Right now
grep \"ARCH\" config.rbuild|cut -d "\"" -f 4

echo SARCH
echo Right now
grep \"SARCH\" config.rbuild|cut -d "\"" -f 4
SET /P X= (), (xbox) 
sed "s/\"SARCH\" value=\"\"/\"SARCH\" value=\"%X%\"/g;s/\"SARCH\" value=\"xbox\"/\"SARCH\" value=\"%X%\"/g" config.rbuild > config2.rbuild

echo OARCH
echo Right now
grep \"OARCH\" config.rbuild|cut -d "\"" -f 4
SET /P XX= 
sed "s/\"OARCH\" value=\"\"/\"OARCH\" value=\"%X%\"/g" config2.rbuild > config21.rbuild

echo OPTIMIZE
echo Right now
grep \"OPTIMIZE\" config.rbuild|cut -d "\"" -f 4
SET /P Y= (0), (1), (2), (3), (4), (5) 
sed "s/\"OPTIMIZE\" value=\"[0-5]\"/\"OPTIMIZE\" value=\"%Y%\"/g" config21.rbuild > config3.rbuild

echo MP
echo Right now
grep \"MP\" config.rbuild|cut -d "\"" -f 4
SET /P Z= (0), (1)
sed "s/\"MP\" value=\"[0-1]\"/\"MP\" value=\"%Z%\"/g" config3.rbuild > config4.rbuild

echo KDBG
echo Right now
grep \"KDBG\" config.rbuild|cut -d "\"" -f 4
SET /P A= (0), (1)
sed "s/\"KDBG\" value=\"[0-1]\"/\"KDBG\" value=\"%A%\"/g" config4.rbuild > config5.rbuild

echo DBG
echo Right now
grep \"DBG\" config.rbuild|cut -d "\"" -f 4
SET /P B= (0), (1)
sed "s/\"DBG\" value=\"[0-1]\"/\"DBG\" value=\"%B%\"/g" config5.rbuild > config6.rbuild

echo GDB
echo Right now
grep \"GDB\" config.rbuild|cut -d "\"" -f 4
SET /P C= (0), (1)
sed "s/\"GDB\" value=\"[0-1]\"/\"GDB\" value=\"%C%\"/g" config6.rbuild > config7.rbuild

echo NSWPAT
echo Right now
grep \"NSWPAT\" config.rbuild|cut -d "\"" -f 4
SET /P D= (0), (1)
sed "s/\"NSWPAT\" value=\"[0-1]\"/\"NSWPAT\" value=\"%D%\"/g" config7.rbuild > config8.rbuild

echo NTLPC
echo Right now
grep \"NTLPC\" config.rbuild|cut -d "\"" -f 4
SET /P E= (0), (1)
sed "s/\"NTLPC\" value=\"[0-1]\"/\"NTLPC\" value=\"%E%\"/g" config8.rbuild > config9.rbuild

echo _WINKD_
echo Right now
grep \"_WINKD_\" config.rbuild|cut -d "\"" -f 4
SET /P F= (0), (1)
sed "s/\"_WINKD_\" value=\"[0-1]\"/\"_WINKD_\" value=\"%F%\"/g" config9.rbuild > config10.rbuild

copy config10.rbuild config.tmp
del *.rbuild
copy config.tmp config.rbuild
del *.tmp
