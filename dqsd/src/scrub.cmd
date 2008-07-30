@echo off

REM This script scrubs the sources clean
REM Please don't check into cvs any files that are deleted by this script.

pushd %~dp0

pushd ..
if exist dqsd.exe del dqsd.exe
if exist dqsdx64.zip del dqsdx64.zip
if exist DQSDTools.dll del DQSDTools.dll
if exist DQSDTools64.dll del DQSDTools64.dll
popd

REM Clean temporary files from source tree
pushd DQSDTools
if exist DQSDTools.h del /q DQSDTools.h
if exist DQSDTools*.ncb del /q DQSDTools*.ncb
if exist DQSDTools.opt del /q DQSDTools.opt
if exist DQSDTools.plg del /q DQSDTools.plg
if exist DQSDTools.tlb del /q DQSDTools.tlb
if exist DQSDTools_i.c del /q DQSDTools_i.c
if exist DQSDTools_p.c del /q DQSDTools_p.c
if exist dlldata.c del /q dlldata.c
if exist DQSDTools*.suo del /q DQSDTools*.suo
if exist DQSDTools*.user del /q DQSDTools*.user

if exist Debug rmdir /q /s Debug
if exist Release rmdir /q /s Release
if exist Win32 rmdir /q /s Win32
if exist x64 rmdir /q /s x64
popd


popd
