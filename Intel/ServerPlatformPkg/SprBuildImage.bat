@REM #  Module Name:
@REM #
@REM #    SprBuildImage.bat
@REM #
@REM #  Abstract:
@REM #
@REM #    Platform full 8M flash image build script.
@REM #
@REM #--*/

if not exist %OUTPUT_DIR%\ROM mkdir %OUTPUT_DIR%\ROM

@REM ---run FTool and create ROM images---
  cd %WORKSPACE%\%OUTPUT_DIR%\ROM
if "%3" == "ME" goto BUILD_ME

if "%~5"=="FPGA" (
  set IFWI_FLASH_SIZE=0x4000000
) else (
  set IFWI_FLASH_SIZE=0x2000000
)

set IFWI_FLASH_SIZE=0x4000000

@REM OEM RELEASE NOTE START: INTEGRATE RESTRICTED ITEM
%WORKSPACE%\%1\Tools\FlashImageTool\FITm_GUI_and_CLI_Version\FITm_cmd.exe -b -i %WORKSPACE%\%2\%3.xml --params descriptor:NumberOfSpiComponents=0 descriptor:FlashComponent1Size=%IFWI_FLASH_SIZE% me:input_file=%WORKSPACE%\%1\MeRegion.bin me:OemExtInputFile=%WORKSPACE%\%1\oemsptkeymn2.bin pmc:PMCBinary=%WORKSPACE%\%1\pmcp.bin layout:output_file=%WORKSPACE%\%OUTPUT_DIR%\ROM\OutImage.bin --layout bios=%WORKSPACE%\%OUTPUT_DIR%\FV\AMIROM.fd gbe=%WORKSPACE%\%OUTPUT_DIR%\ROM\GBE_NVM.bin --skip_access_check
if NOT "%~4" == "NULL" %WORKSPACE%\%1\Tools\FlashImageTool\FITm_GUI_and_CLI_Version\FITm_cmd.exe -b -i %WORKSPACE%\%2\%4.xml --params descriptor:NumberOfSpiComponents=0 descriptor:FlashComponent1Size=%IFWI_FLASH_SIZE% me:input_file=%WORKSPACE%\%1\MeRegion.bin me:OemExtInputFile=%WORKSPACE%\%1\oemsptkeymn2.bin pmc:PMCBinary=%WORKSPACE%\%1\pmcp.bin layout:output_file=%WORKSPACE%\%OUTPUT_DIR%\ROM\OutImage_NM.bin --layout bios=%WORKSPACE%\%OUTPUT_DIR%\FV\AMIROM.fd gbe=%WORKSPACE%\%OUTPUT_DIR%\ROM\GBE_NVM.bin --skip_access_check

@REM OEM RELEASE NOTE END
@REM cd %WORKSPACE%
@REM cd %1\Tools\FlashImageTool
@REM  @del spsfitc.ini /f /q
goto END_BUILD

:BUILD_ME
%WORKSPACE%\%1\Tools\FlashImageTool\fit.exe -b -f %WORKSPACE%\%2\%3.xml -bios %WORKSPACE%\%OUTPUT_DIR%\FV\AMIROM.fd -me %WORKSPACE%\%OUTPUT_DIR%\ROM\ME_11.bin -gbe %WORKSPACE%\%OUTPUT_DIR%\ROM\GBE_NVM.bin -o %WORKSPACE%\%OUTPUT_DIR%\ROM\OutImage.bin

:END_BUILD
@echo off
