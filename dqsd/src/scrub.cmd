@echo off

REM This script scrubs the sources clean
REM Please don't check into cvs any files that are deleted by this script.

pushd %~dp0

pushd ..
if exist dqsd.exe del dqsd.exe
if exist DQSDTools.dll del DQSDTools.dll
if exist dqsdt253.dll del dqsdt253.dll
if exist dqsdt254.dll del dqsdt254.dll
if exist dqsdt255.dll del dqsdt255.dll
popd

call .\clean.cmd

popd
