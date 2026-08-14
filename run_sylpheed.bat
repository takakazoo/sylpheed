@echo off
setlocal
set PATH=C:\msys64\mingw64\bin;%~dp0libsylph\.libs;%~dp0src\.libs;%PATH%
if "%LANG%"=="" set LANG=ja_JP.UTF-8
start "" "%~dp0src\.libs\sylpheed.exe" %*
endlocal
