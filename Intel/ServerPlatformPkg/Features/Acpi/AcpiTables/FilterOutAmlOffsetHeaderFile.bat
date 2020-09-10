@echo off

if "%1" == "DSDT" goto :Generate_DSDT_OFFSET
if "%1" == "SSDT" goto :Generate_SSDT_OFFSET
 
:Generate_DSDT_OFFSET
echo Filtering DSDT %2.offset.h file
REM Create tmp file from %2.offset.h
move /y %WORKSPACE%\%BUILD_DIR%\%2.offset.h %WORKSPACE%\%BUILD_DIR%\%2.offset.h.tmp

REM Filter out everything except tool info, typedef, and start of structure definition.
REM When search string has " in it, must use ^" to indicate string quote for command processor, and \" as quote character in string.
findstr /v ^"{\" {NULL, };" %WORKSPACE%\%BUILD_DIR%\%2.offset.h.tmp > %WORKSPACE%\%BUILD_DIR%\%2.offset.h

REM Filter out everything except the AML operators that need fixup; this should match AcpiPlatformHooks.c BiosFixupAmlOffsetTable[] entries.
REM When search string has " in it, must use ^" to indicate string quote for command processor, and \" as quote character in string.
REM When search string has spaces in it, must use a literal search separate from the regex expressions.

REM Filter out everything except structure terminator and end of structure definition.
REM Copying Patch details to AMI File
findstr "{NULL, };" %WORKSPACE%\%BUILD_DIR%\%2.offset.h.tmp >> %WORKSPACE%\%BUILD_DIR%\%2.offset.h

REM Create new offset table for AMI to handle build time change in OEM table Id.
@echo AML_OFFSET_TABLE_ENTRY DSDTPatchOffsetTable[] = { >> %WORKSPACE%\%BUILD_DIR%\%2.offset.h
findstr /r ^"\"PSYS\" \.FIX[0-9,A-Z]\" BBI[0] BBU[0] CRCM BAR0 \.CCT[0-9,A-Z]\" \.CFH[0-9,A-Z]\" \.FXCD\" \.FXST\" \.FXIN\" \.FXOU\" \.FXBS\" \.FXFH\" \.CENA\" DRVT CFIS\"" %WORKSPACE%\%BUILD_DIR%\%2.offset.h.tmp >> %WORKSPACE%\%BUILD_DIR%\%2.offset.h
REM Copying Patch details to AMI File
findstr "{NULL, };" %WORKSPACE%\%BUILD_DIR%\%2.offset.h.tmp >> %WORKSPACE%\%BUILD_DIR%\%2.offset.h

goto :Remove_Intermediate_Files

:Generate_SSDT_OFFSET
echo Filtering SSDT %2.offset.h file
REM Create tmp file from %2.offset.h
move /y %WORKSPACE%\%BUILD_DIR%\%2.offset.h %WORKSPACE%\%BUILD_DIR%\%2.offset.h.tmp

REM Filter out everything except tool info, typedef, and start of structure definition.
REM When search string has " in it, must use ^" to indicate string quote for command processor, and \" as quote character in string.
findstr /v ^"{\" {NULL, };" %WORKSPACE%\%BUILD_DIR%\%2.offset.h.tmp > %WORKSPACE%\%BUILD_DIR%\%2.offset.h
REM Copying Patch details to AMI File
REM Filter out everything except structure terminator and end of structure definition.
findstr "{NULL, };" %WORKSPACE%\%BUILD_DIR%\%2.offset.h.tmp >> %WORKSPACE%\%BUILD_DIR%\%2.offset.h

REM Create new offset table for AMI to handle build time change in OEM table Id.
@echo AML_OFFSET_TABLE_ENTRY SSDTPatchOffsetTable[] = { >> %WORKSPACE%\%BUILD_DIR%\%2.offset.h
REM Copying Patch details to AMI File
findstr /r "DOMN NCPU PSDC PSDD PSDE NPSS SPSS" %WORKSPACE%\%BUILD_DIR%\%2.offset.h.tmp >> %WORKSPACE%\%BUILD_DIR%\%2.offset.h
findstr "{NULL, };" %WORKSPACE%\%BUILD_DIR%\%2.offset.h.tmp >> %WORKSPACE%\%BUILD_DIR%\%2.offset.h

goto :Remove_Intermediate_Files

:Remove_Intermediate_Files
REM Delete intermediate files
REM del %BUILD_DIR%\%2.offset.h.tmp
REM del %BUILD_DIR%\%2.asl
REM del %BUILD_DIR%\%2.aml
REM del %BUILD_DIR%\%2.iii
REM del %BUILD_DIR%\%2.iiii
goto :EOF
