@echo off

for /f "usebackq" %%i in (`cpucount -x1`) do set CPUCOUNT=%%i
echo %CPUCOUNT% Processors detected (1 added automatically).
mingw32-make -j %CPUCOUNT% %*
