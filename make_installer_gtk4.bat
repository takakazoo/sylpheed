@echo off
setlocal
echo ========================================================
echo  Sylpheed GTK4 Windows NSIS Installer Generator
echo ========================================================

set MSYS2_BASH=C:\msys64\usr\bin\bash.exe
if not exist "%MSYS2_BASH%" (
    echo [ERROR] MSYS2 bash not found at C:\msys64\usr\bin\bash.exe
    pause
    exit /b 1
)

"%MSYS2_BASH%" -lc "export MSYSTEM=MINGW64; source /etc/profile; cd /c/Users/takakazu/Documents/dev/sylpheed && ./make_installer_gtk4.sh"
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo ========================================================
echo  Installer created successfully: nsis\Sylpheed-GTK4-4.0.0-alpha_setup.exe
echo ========================================================
endlocal
