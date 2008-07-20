@echo off

REM Customize these paths to your local install
set VSINSTALLDIR=C:\Program Files\Microsoft Visual Studio .NET 2003
set NSISINSTALLDIR=C:\Program Files\NSIS
set SDKINCLUDE=C:\Program Files\Microsoft SDKs\Windows\2003SP1\include
set SDKLIB=C:\Program Files\Microsoft SDKs\Windows\2003SP1\lib

REM **** Build DQSD DLLs ****
call "%VSINSTALLDIR%\Common7\Tools\vsvars32.bat"

set INCLUDE=%SDKINCLUDE%;%INCLUDE%
set LIB=%SDKLIB%;%LIB%

pushd %~dp0

pushd DQSDTools
devenv /useenv DQSDTools.sln /rebuild "Release MinDependency"
copy /y ReleaseMinDependency\DQSDTools.dll ..\..\DQSDTools.dll
popd

REM **** Build installer ****
REM The LZMA compressor should yield the smallest installer with NSIS 2.0
"%NSISINSTALLDIR%\makensis" /X"SetCompressor /FINAL lzma" search.nsi

popd
