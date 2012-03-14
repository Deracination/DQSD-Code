@ECHO OFF

SET SRCDIR=%~dp0..
SET XCOPY=XCOPY /EXCLUDE:build64.exclude.txt

REM Had to put the output dir in the root, because zip.exe adds the full path to the archive.
REM \DQSDx64 -> DQSDx64 as a root directory in the resulting zip.
SET OUTDIR=\DQSDx64

PUSHD %~dp0

REM Customize VS path to your local VS2008 install!
CALL "%PROGRAMFILES%\Microsoft Visual Studio 9.0\Common7\Tools\vsvars32.bat"

REM Build DQSDTools for x64
PUSHD DQSDTools
  DEVENV DQSDTools.sln /rebuild "Release|x64"
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
ZIP -rm9 %SRCDIR%\dqsdx64.zip %OUTDIR%

POPD
