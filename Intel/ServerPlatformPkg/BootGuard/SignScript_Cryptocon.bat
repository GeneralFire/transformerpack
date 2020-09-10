@ECHO OFF

REM ===============================================================================
REM Script input parameters
REM ===============================================================================
SET INPUT_UNSIGN_FILE=%~1
SET OUTPUT_SIGN_FILE=%~2
SET CRYPTCON=Cryptocon.exe
IF "%1" == "BpmWithoutSigElement.bin" SET KEY=BpmPEM.key
IF "%1" == "KmWithoutSigElement.bin" SET KEY=KmPEM.key
IF "%1" == "FvWithoutSigElement.bin" SET KEY=BpmPEM.key
IF "%3" == "2" SET KEY=Km2.key
IF "%3" == "3" SET KEY=Km3.key

if %BOOTGUARD_BUILD_TIME_SUPPORT_USING_BATCH_FILE%==0 (
ServerPlatformPkg\BootGuard\%CRYPTCON% -s -k ServerPlatformPkg\BootGuard\%KEY% -f %INPUT_UNSIGN_FILE% -o %OUTPUT_SIGN_FILE%
) else (
%CRYPTCON% -s -k ..\..\ServerPlatformPkg\BootGuard\%KEY% -f %INPUT_UNSIGN_FILE% -o %OUTPUT_SIGN_FILE%
)