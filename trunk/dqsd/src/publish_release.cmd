@ECHO OFF

SETLOCAL

REM Publish files
SET RELEASE=%~1
IF "%RELEASE%"=="" (
  ECHO Usage: %~nx0 ^<release name^>
  GOTO :EOF
)

SET PSCP=%~dp0\pscp.exe

SET SHORT_RELEASE=%RELEASE:.=%
SET SHORT_RELEASE=%SHORT_RELEASE: =-%

ECHO Publishing release %RELEASE%...
ECHO.

PUSHD %~dp0

IF NOT EXIST ..\dqsd.exe (
  ECHO ERROR: Please build before attempting to publish!
  POPD
  GOTO :EOF
)

IF NOT EXIST ..\dqsdx64.zip (
  ECHO ERROR: Please build before attempting to publish!
  POPD
  GOTO :EOF
)

IF NOT EXIST ..\README.md (
  ECHO ERROR: Please put release notes in README.md before attempting to publish!
  POPD
  GOTO :EOF
)

IF EXIST "%RELEASE%" RD /S /Q "%RELEASE%"
MKDIR "%RELEASE%"

COPY ..\dqsd.exe "%RELEASE%\dqsd-%SHORT_RELEASE%.exe"
COPY ..\dqsdx64.zip "%RELEASE%\dqsdx64-%SHORT_RELEASE%.zip"
COPY ..\README.md "%RELEASE%\README.md"

%PSCP% -r "%RELEASE%" kimgrasman@frs.sourceforge.net:/home/pfs/project/dqsd/dqsd/

RD /S /Q "%RELEASE%"
POPD

REM Publish web site
PUSHD %~dp0..\..\dqsdweb

SET TMPDIR=%TEMP%\dqsdweb
IF EXIST %TMPDIR% RD /s /q %TMPDIR%

@ECHO Exporting web from SVN into %TMPDIR%
svn export . %TMPDIR%

@ECHO Publishing web files from %TMPDIR% to Sourceforge
%PSCP% -r %TMPDIR%\* kimgrasman,dqsd@web.sourceforge.net:htdocs

RD /s /q %TMPDIR%

POPD
