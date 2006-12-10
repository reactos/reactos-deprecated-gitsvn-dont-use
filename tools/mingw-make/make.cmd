@echo off

for /f "usebackq" %%i in (`cpucount`) do set CPUCOUNT=%%i
echo %CPUCOUNT% Processors detected.
rem mingw32-make -j %CPUCOUNT% %*
