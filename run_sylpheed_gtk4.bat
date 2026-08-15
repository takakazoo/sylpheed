@echo off
setlocal
set PATH=C:\msys64\mingw64\bin;%~dp0libsylph\.libs;%~dp0src-gtk4\.libs;%PATH%
if "%LANG%"=="" set LANG=ja_JP.UTF-8
if "%SSL_CERT_FILE%"=="" set SSL_CERT_FILE=C:\msys64\mingw64\etc\ssl\certs\ca-bundle.crt
start "" "%~dp0src-gtk4\.libs\sylpheed-gtk4.exe" %*
endlocal
