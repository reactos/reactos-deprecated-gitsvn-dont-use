@echo off

for /f "usebackq" %%i in (`cpucount -x1`) do set CPUCOUNT=%%i
echo %CPUCOUNT% Processors detected (Automatically doubled).
mingw32-make -j %CPUCOUNT% %*
