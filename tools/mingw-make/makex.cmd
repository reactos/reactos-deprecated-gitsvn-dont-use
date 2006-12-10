@echo off

for /f "usebackq" %%i in (`cpucount -x2`) do set CPUCOUNT=%%i
echo %CPUCOUNT% Processors detected (Automatically doubled).
rem mingw32-make -j %CPUCOUNT% %*
