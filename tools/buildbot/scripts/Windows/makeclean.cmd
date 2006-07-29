@echo off

echo Cleaning...

if exist makefile.auto del /q /s makefile.auto
rd /q /s obj-i386>nul
rd /q /s output-i386>nul
del /s *.gch>nul
del /s *.o >nul
del *.iso >nul
echo done.
exit /b0