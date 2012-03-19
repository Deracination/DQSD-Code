@ECHO OFF

SETLOCAL

PUSHD %~dp0

REM Get rid of any remnant build artefacts
CALL scrub.cmd

REM Build 32-bit first and 64-bit second, because the 64-bit 
REM build process is smart enough to exclude all 32-bit artefacts
CALL build32.cmd
IF "%ERRORLEVEL%" NEQ "0" GOTO :FAIL

CALL build64.cmd
IF "%ERRORLEVEL%" NEQ "0" GOTO :FAIL

POPD

ENDLOCAL
GOTO :EOF

:FAIL
@ECHO *** Build failed
POPD
