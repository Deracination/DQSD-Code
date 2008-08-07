@rem ****************************************************************************************************
@rem * A basic batch file install that copies the DQSD files to %ProgramFiles% and does the necessary   *
@rem * registry edits and DLL registration. It's not smart, or even vaguely robust, but it should work. *
@rem * Note: MUST run as administrator in order to be able to perform all install steps.                *
@rem ****************************************************************************************************
@echo off

rem Using setlocal to make sure vars don't end up out in the environment...
setlocal

:SetVARS
set _REGSVR=%WinDir%\System32\regsvr32.exe
set _DQSD_CLSID={EC9FE983-E520-4D8F-B1A7-ACBCA0439C70}
set _DQSD_INSTDIR=%PROGRAMFILES%\Quick Search Deskbar
set _CSS_Save=%_DQSD_INSTDIR%\CSS_SAVE

:PreReqChecks
rem Make sure we're on a 64-bit OS!
if NOT "%PROCESSOR_ARCHITECTURE%"=="AMD64" goto WrongArchitecture 
rem Check for pre-existing DQSD directory
if exist "%_DQSD_INSTDIR%" goto PreExisting 
rem Check to see if we have sufficient privilege to create a directory
mkdir "%_DQSD_INSTDIR%" 2>NUL & If errorlevel 1 goto NotAdmin

rem Check OS version
echo Checking Windows version...
call :CheckOSVer
echo Running on Windows %WINVER%

rem Check if running in debug mode
if /I "%1" equ "/d" set _DEBUG=TRUE
if "%_DEBUG%" neq "TRUE" goto DisplayOK

:DebugShowVars
echo RegSVR is: %_REGSVR%
echo CSS_Save is: %_CSS_Save%
echo DQSD_CLSID is: %_DQSD_CLSID%
echo DQDD_INSTDIR is: %_DQSD_INSTDIR%

:DisplayOK
echo This batch file will install the x64 version of Dave's Quick Search Deskbar to: 
echo.
echo     %_DQSD_INSTDIR%
echo. 
echo And, if you're running Vista, it will copy the Vista StyleSheet into place
echo so you don't have a funny blue DQSD in your taskbar. :-)
echo.
echo Please make sure you are running as an administrator, or most of the install
echo steps will fail.
echo.
echo If this is OK, then...
echo. 
pause
echo. 

:CopyFiles
if NOT exist DQSDTools64.dll goto FileError
if "%_DEBUG%" equ "TRUE" echo "We are in %CD%" && pause
echo Copying DQSD Files to %_DQSD_INSTDIR%...
xcopy *.* "%_DQSD_INSTDIR%" /i /e /q

:RegisterDLL
rem Register the DQSD DLL
echo Registering the DQSD DLL...
cd /d "%_DQSD_INSTDIR%"
if NOT exist DQSDTools64.dll goto CopyError
%_REGSVR% /s DQSDTools64.dll

:UpdateRegistry
rem Add additional registry values
echo Adding necessary registry entries...
reg.exe ADD "HKCR\CLSID\%_DQSD_CLSID%" /f /ve /d "Quick Search"
reg.exe ADD "HKCR\CLSID\%_DQSD_CLSID%" /f /v InstallDir /d "%_DQSD_INSTDIR%"
reg.exe ADD "HKCR\CLSID\%_DQSD_CLSID%" /f /v HelpText /d "Dave's Quick Search Deskbar"

reg.exe ADD "HKCR\CLSID\%_DQSD_CLSID%\SecureFiles" /f /v "%_DQSD_INSTDIR%\search.htm" /t REG_DWORD /d 0

:BackupDefaultTheme
if NOT "%WINVER%"=="Vista" GoTo Finish

rem Backup any existing CSS files
if NOT exist "%_CSS_Save%" mkdir "%_CSS_Save%"
copy theme.css "%_CSS_Save%"
copy search.css "%_CSS_Save%"
copy dqsd.png "%_CSS_Save%"

:InstallVistaTheme
rem Copy the Vista CSS files over the top of the existing
if NOT exist themes\vista\vistatoolbar1_vista.bmp goto CopyError
xcopy themes\vista\*.* . /q /y
goto Finish

:NotAdmin
Echo.
Echo.
Echo This installation script must be run as an administrator. If you're 
Echo   trying to install on Windows Vista, or Windows 2008, you'll need to
Echo   open a CMD window "As Administrator" and run the script from there. 
Echo.
Echo.
Goto End

:PreExisting
echo. 
echo.
echo Found previous version of DQSD. This batch file isn't smart enough 
echo   to save any customizations.  Please save off any copies of 
echo   localprefs.js or other custom files to a safe place. Then delete
echo   the current installation of DQSD and re-run the setupx64.cmd. 
echo.
echo.
goto End

:FileError
echo. 
echo.
echo Failed to find necessary files in the current directory to copy. 
echo   Please check your environment and be sure you are running this 
echo   batch file from the source directory, and that the source
echo   has been unzipped. We're just trying to keep this simple.
echo. 
echo.
goto End

:CopyError
echo. 
echo.
echo Failed to find necessary files in the target directory. Something 
echo   didn't copy correctly.  We're exiting now so you can check the 
echo   Variables in your environment and figure out what echo went wrong. 
echo. 
echo.
goto End

:WrongArchitecture
echo. 
echo.
echo This version of DQSD can only be installed on an x64 version of Windows, 
echo   and this installer has detected that you are not running a 64-bit version. 
echo. 
echo. 
echo Please go to www.dqsd.net and download the current version for 32-bit Windows
echo. 
echo. 
goto End

:Finish
echo. 
echo.
echo DQSD for x64 has been installed on your computer. 
echo Please report any anomalies to the DQSD Users list or the DSQD Developers list. 
echo. 
echo We hope you enjoy DQSD and if you do, please tell your friends about it. 
echo.
echo. 

:End
exit /B

rem * Subroutines
:CheckOSVer
rem These files differ between Win2008 and Windows Vista, VER does not differentiate between server/workstation
if exist %windir%\System32\oobefldr.dll (set WINVER=Vista) && goto :Eof
if exist %windir%\System32\oobe.exe  (set WINVER=Server 2008) && goto :Eof

rem For downlevel platforms, use VER command to check actual version
ver | find "2003" > nul
if %ERRORLEVEL% equ 0 (set WINVER=Server 2003) && goto :Eof

ver | find "XP" > nul
if %ERRORLEVEL% equ 0 (set WINVER=XP) && goto :Eof

goto :Eof
