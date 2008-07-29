@Echo off
REM ****************************************************************************************************
REM *                                                                                                  *
REM * Filename: setupx64.cmd                                                                           *
REM *                                                                                                  *
REM * A basic batch file install that copies the DQSD files to %ProgramFiles% and does the necessary   *
REM * registry edits and DLL registration. It's not smart, or even vaguely robust, but it should work. *
REM *                                                                                                  *
REM * ModHist: 14 Aug, 2005 -- Initial (Charlie Russel)                                                *
REM *        : 15 Aug, 2005 -- fix up path problem (charlie). Release to beta.                         *
REM *        : 29 Dec, 2007 -- change to setlocal(charlie).                                            *
REM *        : 21 Jul, 2008 -- Change for v4, and add Vista Check for CSS change                       *
REM *        : 25 Jul, 2008 -- Change to use REG ADD, updated registry entries.                        *
REM *        : 27 Jul, 2008 -- Fix bug of not saving off default theme files                           *
REM *        : 28 Jul, 2008 -- fix bugs, VAR order,vista theme (charlie), rename cmd file (Kim)        *
REM *                                                                                                  *
REM ****************************************************************************************************

@REM using setlocal to make sure variables don't end up out in the environment...
setlocal

@REM First, make sure we're on a 64-bit OS!
if NOT "%PROCESSOR_ARCHITECTURE%"=="AMD64" goto WrongArchitecture

:SetVARS
SET _REGSVR=%WinDir%\System32\regsvr32
SET _FileCheck=%WinDir%\System32\SystemPropertiesAdvanced.exe
SET _DQSD_CLSID={EC9FE983-E520-4D8F-B1A7-ACBCA0439C70}
SET _DQSD_INSTDIR=%PROGRAMFILES%\Quick Search Deskbar
SET _CSS_Save=%_DQSD_INSTDIR%\CSS_SAVE

IF /I "%1" equ "/d" SET _DEBUG=TRUE

if "%_DEBUG%" equ "TRUE" goto ShowVars
rem echo if "%_DEBUG%" equals "TRUE" why are we here?
goto DisplayOK

:ShowVars
echo RegSVR is: %_REGSVR%
echo FileCheck is: %_FileCheck%
echo CSS_Save is: %_CSS_Save%
echo DQSD_CLSID is: %_DQSD_CLSID%
echo DQDD_INSTDIR is: %_DQSD_INSTDIR%

:DisplayOK
Echo This batch file will install the x64 version of Dave's Quick Search Deskbar to: 
Echo.
Echo     %_DQSD_INSTDIR%
Echo. 
Echo. And, if you're running Vista, it will copy the Vista StyleSheet into place
Echo.  so you don't have a funny blue DQSD in your taskbar. :-)
Echo.
Echo   If this is OK, then...
Echo. 
pause
echo. 

:CopyFiles
if exist "%_DQSD_INSTDIR%"\DQSDTools*.dll goto PreExisting
if NOT exist DQSDTools64.dll goto FileError
if "%_DEBUG%" equ "TRUE" echo "We are in %CD%" && pause
xcopy *.* "%_DQSD_INSTDIR%" /i /e 

:RegisterDLL
REM Register the DQSD DLL
cd "%_DQSD_INSTDIR%"
if NOT exist DQSDTools64.dll goto CopyError
%_REGSVR% /s DQSDTools64.dll

:UpdateRegistry
REM Add additional registry values
reg.exe ADD "HKCR\CLSID\%_DQSD_CLSID%" /f /ve /d "Quick Search"
reg.exe ADD "HKCR\CLSID\%_DQSD_CLSID%" /f /v InstallDir /d "%_DQSD_INSTDIR%"
reg.exe ADD "HKCR\CLSID\%_DQSD_CLSID%" /f /v HelpText /d "Dave's Quick Search Deskbar"

reg.exe ADD "HKCR\CLSID\%_DQSD_CLSID%\SecureFiles" /f /v "%_DQSD_INSTDIR%\search.htm" /t REG_DWORD /d 0

:CheckVista
if NOT exist %_FileCheck% goto EchoDone

:BackupDefaultTheme
@REM Copy the Vista CSS files over the top of the existing
if NOT exist themes\vista\vistatoolbar1_vista.bmp goto CopyError
if NOT exist "%_CSS_Save%" mkdir "%_CSS_Save%"
copy theme.css "%_CSS_Save%"
copy search.css "%_CSS_Save%"
copy dqsd.png "%_CSS_Save%"

:InstallVistaTheme
copy themes\vista\*.* /y
copy theme_vista.css theme.css /y

goto EchoDone

:PreExisting
echo. 
echo.
echo Found previous version of DQSD. This batch file isn't smart enough to save any customizations.
echo Please save off any copies of localprefs.js or other custom files to a safe place. Then delete
echo the current installation of DQSD and re-run the setupx64.cmd. 
echo.
echo.
goto End

:FileError
echo. 
echo.
echo Failed to find necessary files in the current directory to copy. Please check your environment
echo and be sure you are running this batch file from the source directory, and that the source has been
echo has been unzipped. We're just trying to keep this simple.
echo. 
echo.
goto End

:CopyError
echo. 
echo.
echo Failed to find necessary files in the target directory. Something didn't copy correctly.
echo We're exiting now so you can check the Variables in your environment and figure out what 
echo went wrong. 
echo. 
echo.
goto End

:WrongArchitecture
echo. 
echo.
echo This version of DQSD can only be installed on an x64 version of Windows, and 
echo this installer has detected that you are not running a 64-bit version. 
echo. 
echo. 
echo Please go to www.dqsd.net and download the current version for 32-bit Windows
echo. 
echo. 

:EchoDone
echo. 
echo.
echo. 
echo.
echo. 
echo.
echo. 
echo.
echo DQSD for x64, version 4.x has been installed on your computer. 
echo. Note that this is an experimental version of 64-bit DQSD. Please report
echo. any anomalies to the DQSD Users list or the DSQD Developers list. 
echo. 
echo.


:unsetVARS
ENDLOCAL
echo.
echo.
echo. 
echo. 
echo.
echo. 
echo. 
echo Done! We hope you enjoy DQSD and if you do, please tell your friends about it. 
echo.
echo. 

:End
