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

makensis search.nsi

popd
