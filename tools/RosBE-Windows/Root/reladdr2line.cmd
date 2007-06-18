@echo off
for /f "tokens=2" %%i in ('"objdump -p %1 2>NUL | findstr ImageBase"') do SET baseaddr=0x%%i
set /a baseaddr += 0x%2
for /f %%i in ('"%ROSBEBASEDIR%\Tools\echoh %baseaddr%"') do set relbase=%%i
addr2line --exe=%1 %relbase% 2>NUL
