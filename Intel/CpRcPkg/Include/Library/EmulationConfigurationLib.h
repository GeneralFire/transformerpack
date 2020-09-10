/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/

#ifndef _EMULATION_CONFIGURATION_LIB_H_
#define _EMULATION_CONFIGURATION_LIB_H_

#include <Uefi.h>

// Emulation Polling Loops
#define LabelPrint(tempLabelCount){                                           \
if (CheckCurrentReset (POST_RESET_POWERGOOD))                    \
  RcDebugPrint (SDBG_DEFAULT, "\nLABEL_c_mbox%d:\n", tempLabelCount);    \
else                                                                          \
  RcDebugPrint (SDBG_DEFAULT, "\nLABEL_w_mbox%d:\n", tempLabelCount);    \
}

#define MaskPrint(tempLabelCount) {                                           \
RcDebugPrint (SDBG_DEFAULT, "and eax, 0%08xh\n", mask);                                   \
if (CheckCurrentReset (POST_RESET_POWERGOOD))                    \
  RcDebugPrint (SDBG_DEFAULT, "jz LABEL_c_mbox%d\n", tempLabelCount);                 \
else                                                                          \
  RcDebugPrint (SDBG_DEFAULT, "jz LABEL_w_mbox%d\n", tempLabelCount);                 \
}

#define ReadByteAndShift { \
  RcDebugPrint (SDBG_DEFAULT, "mov bl, BYTE PTR ds:[0%08xh]\n", RegAddr); \
  RcDebugPrint (SDBG_DEFAULT, "shl ecx, 8\n"); \
  RcDebugPrint (SDBG_DEFAULT, "or cl, bl\n"); \
}


#pragma pack(1)

typedef struct {
  UINT8   UbiosGenerationSetting;                        // indicate if uBIOS generation is enable or not in setup menu
  UINT8   HybridSystemLevelEmulationSetting;             // indicate if HSLE is enable or not in setup menu
  UINT8   UbiosOutputMode;                               // indicate if we should output register writes to the serial port
  UINT32  LoopBackLabelNumber;                           // used to create ASM reads for emulation
  UINT8   FnvAccessValue;                                // indicate if FNV access enable or not
  UINT8   MsrTraceEnable;                                // indicate if Mrs Trace is enable or not in setup menu
  UINT8   MsrTraceOutputMode;                            // indicate if Mrs Trace Asm Output Mode
} EMULATION_SETTING;

#pragma pack()

#define ASM_OUTPUT_DISABLE          0                    // flag to indicate ASM output is disabled
#define ASM_OUTPUT_ENABLE           BIT0                 // flag to indicate ASM output is enabled
#define ASM_OUTPUT_ENABLE_ON_SMBUS  BIT1                 // flag to indicate ASM outout is enabled on SMBUS activity

#define FNV_ACCESS_DISABLE          0                    // flag to indicate FNV access is disabled
#define FNV_ACCESS_ENABLE           1                    // flag to indicate FNV access is enabled

#define MSR_OUTPUT_DISABLE          0                    // flag to indicate Mrs Trace Asm Output is disabled
#define MSR_OUTPUT_ENABLE           1                    // flag to indicate Mrs Trace Asm Output is enabled

#define SCOPE_BSP    0x0
#define SCOPE_THREAD    0x1
#define SCOPE_CORE  0x2
#define SCOPE_PACKAGE   0x3
#define SCOPE_AUTO 0x4
#define SCOPE_MAX   0x5

/**

   Return if Hybrid System Level Emulation is enabled or not

  @param none

  @retval True     Hybrid System Level Emulation is enabled
  @retval False    Hybrid System Level Emulation is disabled

**/
BOOLEAN
EFIAPI
HybridSystemLevelEmulationEnabled (
  VOID
  );

/**

   Return if Ubios Generation is enabled or not

  @param none

  @retval True     Ubios Generation is enabled
  @retval False    Ubios Generation is disabled

**/
BOOLEAN
EFIAPI
UbiosGenerationEnabled (
  VOID
  );

/**

   Get the uBIOS output mode

  @param    none

  @retval   uBIOS output mode

**/

UINT8
EFIAPI
GetUbiosOutputMode (
  VOID
  );

/**

   Set the uBIOS output mode

  @param[in] Value         uBIOS output moder

  @retval EFI_SUCCESS      Success to set the uBIOS output mode
  @retval Other            Fail to set the uBIOS output mode

**/

EFI_STATUS
EFIAPI
SetUbiosOutputMode (
  IN UINT8 Value
  );

/**

   Get the Label Number which is used in the ASM output loop back

  @param[in] Value         Loop Back Label Number

  @retval EFI_SUCCESS      Success to get the Label Number
  @retval Other            Fail to get the Label Number

**/

EFI_STATUS
EFIAPI
GetUbiosLoopbackLabelNumber (
  OUT UINT32 *Value
  );

/**

   Set the Label Number which is used in the ASM output loop back

  @param[in] Value         Loop Back Label Number

  @retval EFI_SUCCESS      Success to set the Label Number
  @retval Other            Fail to set the Label Number

**/

EFI_STATUS
EFIAPI
SetUbiosLoopbackLabelNumber (
  IN UINT32 Value
  );

/**

   Get the value of FnvAccess

  @param    none

  @retval   FnvAccess value

**/

UINT8
EFIAPI
GetUbiosFnvAccessValue (
  VOID
  );

/**

   Set the value of FnvAccess

  @param[in] Value         Loop Back Label Number

  @retval EFI_SUCCESS      Success to set the value of FnvAccess
  @retval Other            Fail to set the value of FnvAccess

**/

EFI_STATUS
EFIAPI
SetUbiosFnvAccessValue (
  IN UINT8 Value
  );

/**

   Check the setting of Ubios Generation and Hybrid System Level Emulation.

  @param none

  @retval True     Ubios Generation or Hybrid System Level Emulation is enabled
  @retval False    Ubios Generation or Hybrid System Level Emulation is disabled

**/
BOOLEAN
EFIAPI
UbiosGenerationOrHsleEnabled (
  VOID
  );

/**

   Return if Msr Trace is enabled or not

  @param none

  @retval True     Msr Trace is enabled
  @retval False    Msr Trace is disabled

**/

BOOLEAN
MsrTraceEnabled (
  VOID
  );

/**

   Get the Msr Trace output mode

  @param    none

  @retval   Msr Trace output mode

**/

UINT8
EFIAPI
GetMsrTraceOutputMode (
  VOID
  );

/**

   Set the Msr Trace output mode

  @param[in] Value         Msr Trace output moder

  @retval EFI_SUCCESS      Success to set Msr Trace output moder
  @retval Other            Fail to set Msr Trace output moder

**/

EFI_STATUS
EFIAPI
SetMsrTraceOutputMode (
  IN UINT8 Value
  );

  /**

  Prints address as an ASM DD section

  @param RegionName  - Name for the section

  @retval None

**/
VOID
DataRegionAsmDump (
  IN UINT8 *RegionName,
  IN UINT8 *address,
  UINT64    size
  );

/**

  EmulationPolling is an assembly polling loop for reading CSRs
  EmulationPolling allows you to poll multiple bits

  @param[in] *Address    - Address to read
  @param[in] Mask        - Bit mask for the value read
  @param[in] Logic       - polling for the register bit to become 0 or 1
  @param[in] PowerGood   - Reset Type for choosing emulation loop label.

  @retval N/A

**/
VOID
EFIAPI
EmulationPolling (
  IN UINT8    *Address,
  IN UINT32   Mask,
  IN UINT8    Logic,
  IN BOOLEAN  PowerGood
  );

/**

   Surround ubios call print with scope. This is for the ubios replay table support.

  @param CallName   Str with the name of the function/proc to call
  @param Scope      The call should be made in the defined scope.
                    SCOPE_AUTO - Default/global scope is used so delimiters for scope are skipped.
                    SCOPE_THREAD - use THREAD SCOPE which will cause it to run on all available threads
                    SCOPE_CORE - use CORE SCOPE which will cause it to run on all available cores
                    SCOPE_PACKAGE - use CORE SCOPE which will cause it to run on all available PACKAGES

  @retval EFI_SUCCESS      Success Ubios Call print
**/

EFI_STATUS
UbiosCall (
  IN CHAR8 *CallName,
  IN UINT8 Scope
  );

#endif // _EMULATION_CONFIGURATION_LIB_H_
