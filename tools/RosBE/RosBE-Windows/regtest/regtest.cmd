@echo off
setlocal enabledelayedexpansion 

echo RosBE Regression Test
echo.

set /p "dummyvar=Checking for Bison......" < NUL
bison 2> NUL

if %ERRORLEVEL% == 1 (
  start /min bison parser.y -p parser_ -d -o parser.tab.c
  choice /C:AB /T 3 /D A > NUL
  taskkill /f /im "bison.exe" > NUL 2> NUL
  if !ERRORLEVEL! == 0 (
      echo [broken]
  ) else (
      echo [ok]
  )
) else (
    echo [missing]
)

set /p "dummyvar=Checking for Flex......." <nul 
flex -oparser.yy.c -Pparser_ parser.l
if %ERRORLEVEL% == 0 (
    echo [ok]
) else (
    echo [missing]
)
del parser.yy.c 2> NUL
del parser.tab.c 2> NUL
del parser.tab.h 2> NUL