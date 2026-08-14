@echo off
setlocal
echo Running Sylpheed Windows Installer build...
C:\msys64\usr\bin\bash.exe -lc "export MSYSTEM=MINGW64; source /etc/profile; cd '%~dp0'; ./make_installer.sh"
if %ERRORLEVEL% equ 0 (
    echo.
    echo ==================================================
    echo Installer generated successfully in nsis\ folder!
    echo ==================================================
) else (
    echo.
    echo Error occurred during installer creation.
)
pause
endlocal
