@echo off

REM For this build to work, both the VC tools and
REM the NSIS tools need to be on your path.

REM This may not be needed for your environment
call vcvars32.bat

REM This is the path to NSIS, may be different or unnecessary for your environment
SET PATH=%PATH%;"C:\Program Files\NSIS"

pushd %~dp0

pushd DQSDSearchWiz
nmake -f "DQSDSearchWiz.mak" CFG="DQSDSearchWiz - Win32 Release MinDependency"

copy /y ReleaseMinDependency\DQSDSearchWiz.dll ..\..\DQSDSearchWiz.dll
popd

makensis searchwiz.nsi

popd
