@echo off
@REM @file
@REM
@REM @copyright
@REM INTEL CONFIDENTIAL
@REM Copyright 2010 - 2018 Intel Corporation. <BR>
@REM
@REM The source code contained or described herein and all documents related to the
@REM source code ("Material") are owned by Intel Corporation or its suppliers or
@REM licensors. Title to the Material remains with Intel Corporation or its suppliers
@REM and licensors. The Material may contain trade secrets and proprietary    and
@REM confidential information of Intel Corporation and its suppliers and licensors,
@REM and is protected by worldwide copyright and trade secret laws and treaty
@REM provisions. No part of the Material may be used, copied, reproduced, modified,
@REM published, uploaded, posted, transmitted, distributed, or disclosed in any way
@REM without Intel's prior express written permission.
@REM
@REM No license under any patent, copyright, trade secret or other intellectual
@REM property right is granted to or conferred upon you by disclosure or delivery
@REM of the Materials, either expressly, by implication, inducement, estoppel or
@REM otherwise. Any license under such intellectual property rights must be
@REM express and approved by Intel in writing.
@REM
@REM Unless otherwise agreed by Intel in writing, you may not remove or alter
@REM this notice or any other notice embedded in Materials by Intel or
@REM Intel's suppliers or licensors in any way.
@REM

@set SCRIPT_ERROR=0


@set BIOS_INFO_GUID=4A4CA1C6-871C-45bb-8801-6910A7AA5807
@set STARTUP_AC_PARA=


:NoStartupAcm

@REM %EDK_TOOLS_BIN%\FitGen.exe -D %OUTPUT_DIR%\FV\Platform.fd %OUTPUT_DIR%\FV\Platform_TXTSX.fd -F 0x40 -S %FLASH_REGION_BIOS_ACM_BASE% %FLASH_REGION_BIOS_ACM_SIZE% -B %FLASH_REGION_FV_SECPEI_BASE% %FLASH_REGION_FV_SECPEI_SIZE% -U %OUTPUT_DIR%\FV\MICROCODE_FV.Fv %FLASH_REGION_MICROCODE_FV_BASE% -O 0x0C RESERVE 0x500 -O 0x0B RESERVE 0x500 -O 0x0D RESERVE 0x40
@REM FDBin\Tools\FitGen\FitGen.exe -D %OUTPUT_DIR%\FV\Platform.fd %OUTPUT_DIR%\FV\Platform_TXTSX.fd -F 0x40 -NA -S %FLASH_REGION_BIOS_ACM_BASE% %FLASH_REGION_BIOS_ACM_SIZE% -B %FLASH_REGION_FLASHFV_MRCNORMAL_BASE% %FLASH_REGION_FLASHFV_MRCNORMAL_SIZE% -B %FLASH_REGION_FLASHFV_SECPEI_BASE% %FLASH_REGION_FLASHFV_SECPEI_SIZE% -U %OUTPUT_DIR%\FV\MICROCODE_FV.Fv %FLASH_REGION_MICROCODE_FV_BASE% -O 0x9 %FLASH_REGION_BIOS_POLICY_BASE% %FLASH_REGION_BIOS_POLICY_SIZE_FITGEN_INPUT% -O 0x0C RESERVE 0x500 -O 0x0B RESERVE 0x500 -O 0x0D RESERVE 0x40 -P 0xA 0x70 0x71 0x1 0x4 0x2a >> %BUILD_LOG%
FDBin\Tools\FitGen\FitGen.exe -D %OUTPUT_DIR%\FV\Platform.fd %OUTPUT_DIR%\FV\Platform_TXTSX.fd -F 0x40 -NA -S %FLASH_REGION_BIOS_ACM_BASE% %FLASH_REGION_BIOS_ACM_SIZE% -B %FLASH_REGION_FLASHFV_MRCNORMAL_BASE% %FLASH_REGION_FLASHFV_MRCNORMAL_SIZE% -B %FLASH_REGION_FLASHFV_SECPEI_BASE% %FLASH_REGION_FLASHFV_SECPEI_SIZE% -U %OUTPUT_DIR%\FV\MICROCODE_FV.Fv %FLASH_REGION_MICROCODE_FV_BASE% -O 0x0C RESERVE 0x500 -O 0x0B RESERVE 0x500 -O 0x0D RESERVE 0x40 -P 0xA 0x70 0x71 0x1 0x4 0x2a >> %BUILD_LOG%

@if %ERRORLEVEL% NEQ 0 (
  set SCRIPT_ERROR=1
  goto :EOF
)

@set BIOS_INFO_GUID=
EXIT /B %ERRORLEVEL%
