@echo off

REM For this build to work, both the VC tools and
REM the NSIS tools need to be on your path.

pushd %~dp0

pushd DQSDTools
nmake -f "DQSDTools.mak" CFG="DQSDTools - Win32 Release MinDependency"
copy ReleaseMinDependency\DQSDTools.dll ..\..
popd

pushd ..
makensis search.nsi
popd

popd
