@echo off

REM For this build to work, both the VC tools and
REM the NSIS tools need to be on your path.

call vcvars32.bat
SET PATH=%PATH%;"C:\Program Files\NSIS"

pushd %~dp0

pushd DQSDTools

REM This seems to work sooo much better than NMAKE, and we don't need a makefile
msdev DQSDTools.dsp /MAKE "DQSDTools - Win32 Release MinDependency" /REBUILD

copy /y ReleaseMinDependency\DQSDTools.dll ..\..\DQSDTools.dll
popd

REM The LZMA compressor should yield the smallest installer with NSIS 2.0
makensis /X"SetCompressor /FINAL lzma" search.nsi

popd
