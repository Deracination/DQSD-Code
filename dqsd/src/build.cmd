@echo off

REM For this build to work, both the VC tools and
REM the NSIS tools need to be on your path.

call vcvars32.bat
SET PATH=%PATH%;"C:\Program Files\NSIS"

pushd %~dp0

pushd DQSDTools

REM We don't need a makefile if we run msdev.exe directly
msdev DQSDTools.dsp /MAKE "DQSDTools - Win32 Release MinDependency" /REBUILD

REM Use this instead if you only have VS.NET 2003 - make sure DQSDTools.mak is up to date
REM nmake -f "DQSDTools.mak" CFG="DQSDTools - Win32 Release MinDependency"

copy /y ReleaseMinDependency\DQSDTools.dll ..\..\DQSDTools.dll
popd

REM The LZMA compressor should yield the smallest installer with NSIS 2.0
makensis /X"SetCompressor /FINAL lzma" search.nsi

popd
