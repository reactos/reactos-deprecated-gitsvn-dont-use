sed "s/\"SARCH\" value=\"\"/\"SARCH\" value=\%1"\"/g;s/\"SARCH\" value=\"xbox\"/\"SARCH\" value=\"%1\"/g" config.rbuild > config2.rbuild
sed "s/\"OPTIMIZE\" value=\"[0-5]\"/\"OPTIMIZE\" value=\"%1\"/g" config.rbuild > config2.rbuild
sed "s/\"MP\" value=\"[0-1]\"/\"MP\" value=\"%1\"/g" config.rbuild > config2.rbuild
sed "s/\"KDBG\" value=\"[0-1]\"/\"KDBG\" value=\"%1\"/g" config.rbuild > config2.rbuild
sed "s/\"DBG\" value=\"[0-1]\"/\"DBG\" value=\"%1\"/g" config.rbuild > config2.rbuild
sed "s/\"GDB\" value=\"[0-1]\"/\"GDB\" value=\"%1\"/g" config.rbuild > config2.rbuild
sed "s/\"NSWPAT\" value=\"[0-1]\"/\"NSWPAT\" value=\"%1\"/g" config.rbuild > config2.rbuild
sed "s/\"NTLPC\" value=\"[0-1]\"/\"NTLPC\" value=\"%1\"/g" config.rbuild > config2.rbuild
sed "s/\"_WINKD_\" value=\"[0-1]\"/\"_WINKD_\" value=\"%1\"/g" config.rbuild > config2.rbuild

grep SARCH config.rbuild|cut -d "\"" -f 4
grep OARCH config.rbuild|cut -d "\"" -f 4
grep OPTIMIZE config.rbuild|cut -d "\"" -f 4
grep MP config.rbuild|cut -d "\"" -f 4
grep KDBG config.rbuild|cut -d "\"" -f 4
grep DBG config.rbuild|cut -d "\"" -f 4
grep GDB config.rbuild|cut -d "\"" -f 4
grep NSWPAT config.rbuild|cut -d "\"" -f 4
grep NTLPC config.rbuild|cut -d "\"" -f 4
grep _WINKD_ config.rbuild|cut -d "\"" -f 4