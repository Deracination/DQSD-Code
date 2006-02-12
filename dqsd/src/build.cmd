@echo off

REM For this build to work, both the VC tools and
REM the NSIS tools need to be on your path.

set SDKINCLUDE=C:\Program Files\Microsoft Visual Studio .NET 2003\VC7\PlatformSDK\include

REM **** Build DQSDTools ****

call vcvars32.bat
pushd %~dp0
pushd DQSDTools

REM We don't need a makefile if we run msdev.exe directly
REM msdev DQSDTools.dsp /MAKE "DQSDTools - Win32 Release MinDependency" /REBUILD

REM Use this instead if you only have VS.NET 2003 - make sure DQSDTools.mak is up to date
nmake -a -f "DQSDTools.mak" CFG="DQSDTools - Win32 Release MinDependency" SDKINCLUDE="%SDKINCLUDE%"

REM **** Build installer ****

copy /y ReleaseMinDependency\DQSDTools.dll ..\..\DQSDTools.dll
popd

REM The LZMA compressor should yield the smallest installer with NSIS 2.0
rem makensis /X"SetCompressor /FINAL lzma" search.nsi

popd
