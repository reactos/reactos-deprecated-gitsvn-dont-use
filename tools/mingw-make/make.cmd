@echo off
for /f "usebackq" %%i IN (`cpucount`) DO @set CPUCOUNT=%%i
echo %CPUCOUNT% Processors detected.
mingw32-make -j %CPUCOUNT% %*