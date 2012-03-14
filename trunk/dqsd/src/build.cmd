@echo off

REM Customize these paths to your local install
set NSISINSTALLDIR=C:\Program Files\NSIS

REM **** Build DQSD DLLs ****
call "%PROGRAMFILES%\Microsoft Visual Studio 9.0\Common7\Tools\vsvars32.bat"

pushd %~dp0

pushd DQSDTools
DEVENV DQSDTools.sln /rebuild "Release|Win32"
copy /y Win32\Release\DQSDTools.dll ..\..\DQSDTools.dll
popd

REM **** Build installer ****
REM The SOLID LZMA compressor should yield the smallest installer with NSIS 2.0
"%NSISINSTALLDIR%\makensis" /X"SetCompressor /SOLID lzma" search.nsi

popd
