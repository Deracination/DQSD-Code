@echo off

REM Customize these paths to your local install
set VSINSTALLDIR=C:\Program Files\Microsoft Visual Studio .NET 2003
set NSISINSTALLDIR=C:\Program Files\NSIS
set SDKINCLUDE=C:\Program Files\Microsoft SDKs\Windows\2003SP1\include
set SDKLIB=C:\Program Files\Microsoft SDKs\Windows\2003SP1\lib

REM **** Configure VS command-line env ****
call "%VSINSTALLDIR%\Common7\Tools\vsvars32.bat"

set INCLUDE=%SDKINCLUDE%;%INCLUDE%
set LIB=%SDKLIB%;%LIB%

pushd %~dp0

pushd DQSDSearchWiz
devenv /useenv DQSDSearchWiz.sln /rebuild Release
copy /y Release\DQSDSearchWiz.dll ..\..\DQSDSearchWiz.dll
popd

REM The LZMA compressor should yield the smallest installer with NSIS 2.0
"%NSISINSTALLDIR%\makensis" /X"SetCompressor /SOLID lzma" searchwiz.nsi

popd
