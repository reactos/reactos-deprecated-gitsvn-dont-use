@echo off

if not exist reactos.iso goto Err

echo Getting revision number...
for /f "usebackq" %%i IN (`svnversion .`) DO @set REV=%%i
echo Revision: %REV%.

echo Compressing iso files... 

copy reactos.iso bootcd-%REV%-%1.iso >nul
copy reactos-livecd.iso livecd-%REV%-%1.iso >nul

7z a -bd -t7z bootcd-%REV%-%1.7z bootcd-%REV%-%1.iso -mx9
7z a -bd -t7z livecd-%REV%-%1.7z livecd-%REV%-%1.iso -mx9

if exist bootcd-%REV%-%1.iso del bootcd-%REV%-%1.iso
if exist livecd-%REV%-%1.iso del livecd-%REV%-%1.iso

echo done.
echo Uploading rev %REV% BootCD and LiveCD...

echo open svn.reactos.ru >ftp.dat
call login.cmd
echo cd www >>ftp.dat
echo cd iso >>ftp.dat
echo binary >>ftp.dat
echo put bootcd-%REV%-%1.7z >>ftp.dat
echo put livecd-%REV%-%1.7z >>ftp.dat
echo bye >>ftp.dat

ftp -s:ftp.dat
if exist bootcd-%REV%-%1.7z del bootcd-%REV%-%1.7z
if exist livecd-%REV%-%1.7z del livecd-%REV%-%1.7z
echo done.
exit /b0

:Err
echo No ISO file found.
exit /b2