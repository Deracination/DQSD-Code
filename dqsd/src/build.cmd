@echo off

REM For this build to work, both the VC tools and
REM the NSIS tools need to be on your path.

call vcvars32.bat
SET PATH=%PATH%;"C:\Program Files\NSIS"

pushd %~dp0

pushd DQSDTools
nmake -f "DQSDTools.mak" CFG="DQSDTools - Win32 Release MinDependency"

copy /y ReleaseMinDependency\DQSDTools.dll ..\..\DQSDTools.dll
popd

REM NSIS-compile with bz2 compression - saves 120K off installer size
makensis.exe /X"SetCompressor bzip2" search.nsi

popd
