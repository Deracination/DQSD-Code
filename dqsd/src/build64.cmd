@ECHO OFF

SET SRCDIR=%~dp0..
SET OUTDIR=\DQSDx64
SET XCOPY=XCOPY /EXCLUDE:build64.exclude.txt

PUSHD %~dp0

REM Customize VS path to your local VS2008 install!
CALL "%PROGRAMFILES%\Microsoft Visual Studio 9.0\Common7\Tools\vsvars32.bat"

REM Build DQSDTools for x64
PUSHD DQSDTools
  DEVENV DQSDTools.vs2008.sln /rebuild "Release|x64"
  COPY /y "x64\Release\DQSDTools.dll" ..\..\DQSDTools64.dll
POPD

REM Recreate output dir
RD /S /Q %OUTDIR%
MD %OUTDIR%

REM Copy distribution
%XCOPY% /S %SRCDIR% %OUTDIR%\

REM Add extra directories we want in the install folder
MD %OUTDIR%\localsearches

REM Create ZIP file
ZIP -rm %SRCDIR%\dqsdx64.zip %OUTDIR%

POPD
