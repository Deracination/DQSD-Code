@echo off

REM This script scrubs the sources clean
REM Please don't check into cvs any files that are deleted by this script.

pushd %~dp0

pushd ..
if exist dqsd.exe del dqsd.exe
if exist DQSDTools.dll del DQSDTools.dll
popd

call .\clean.cmd

popd
