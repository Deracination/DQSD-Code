@echo off

REM For this build to work, both the VC tools and
REM the NSIS tools need to be on your path.

REM call vcvars32.bat

pushd %~dp0

pushd DQSDSearchWiz
nmake -f "DQSDSearchWiz.mak" CFG="DQSDSearchWiz - Win32 Release MinDependency"

copy /y ReleaseMinDependency\DQSDSearchWiz.dll ..\..\DQSDSearchWiz.dll
popd

makensis searchwiz.nsi

popd
