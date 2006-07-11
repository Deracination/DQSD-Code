@echo off

REM This script scrubs the sources clean
REM Please don't check into cvs any files that are deleted by this script.

pushd %~dp0

pushd DQSDTools
if exist DQSDTools.h del /q DQSDTools.h
if exist DQSDTools.ncb del /q DQSDTools.ncb
if exist DQSDTools.opt del /q DQSDTools.opt
if exist DQSDTools.plg del /q DQSDTools.plg
if exist DQSDTools.tlb del /q DQSDTools.tlb
if exist DQSDTools_i.c del /q DQSDTools_i.c
if exist DQSDTools_p.c del /q DQSDTools_p.c
if exist dlldata.c del /q dlldata.c
if exist Debug rmdir /q /s Debug
if exist ReleaseMinDependency rmdir /q /s ReleaseMinDependency
popd

pushd DQSDHost
if exist DQSDHost.h del /q DQSDHost.h
if exist DQSDHost.vcproj.vspscc del /q DQSDHost.vcproj.vspscc
if exist DQSDHost.suo del /q /ah DQSDHost.suo
if exist DQSDHost.ncb del /q DQSDHost.ncb
if exist DQSDHost.opt del /q DQSDHost.opt
if exist DQSDHost.plg del /q DQSDHost.plg
if exist DQSDHost.tlb del /q DQSDHost.tlb
if exist DQSDHost_i.c del /q DQSDHost_i.c
if exist DQSDHost_p.c del /q DQSDHost_p.c
if exist dlldata.c del /q dlldata.c
if exist Debug rmdir /q /s Debug
if exist Release rmdir /q /s Release
if exist ReleaseMinDependency rmdir /q /s ReleaseMinDependency
popd

popd
