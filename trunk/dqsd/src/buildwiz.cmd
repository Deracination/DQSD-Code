@echo off

REM For this build to work, both the VC tools and
REM the NSIS tools need to be on your path.

REM This may not be needed for your environment
call vcvars32.bat

REM This is the path to NSIS, may be different or unnecessary for your environment
SET PATH=%PATH%;"C:\Program Files\NSIS"

pushd %~dp0

pushd DQSDSearchWiz

REM This seems to work sooo much better than NMAKE, and we don't need a makefile
msdev DQSDSearchWiz.dsp /MAKE "DQSDSearchWiz - Win32 Release MinDependency" /REBUILD

copy /y ReleaseMinDependency\DQSDSearchWiz.dll ..\..\DQSDSearchWiz.dll
popd

REM The LZMA compressor should yield the smallest installer with NSIS 2.0
makensis /X"SetCompressor /FINAL lzma" searchwiz.nsi

popd
