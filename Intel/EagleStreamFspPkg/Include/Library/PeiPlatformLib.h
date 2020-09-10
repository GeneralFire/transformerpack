/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2014 Intel Corporation. <BR>

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

#ifndef _PEI_PLATFORM_LIB_H_
#define _PEI_PLATFORM_LIB_H_

#define PEI_DEVICE_DISABLED 0
#define PEI_DEVICE_ENABLED  1

typedef struct {
  UINT8   Register;
  UINT32  Value;
} PCH_GPIO_DEV;

//
// GPIO Initialization Data Structure
//
typedef struct{
  PCH_GPIO_DEV Use_Sel;
  PCH_GPIO_DEV Use_Sel2;
  PCH_GPIO_DEV Use_Sel3;
  PCH_GPIO_DEV Io_Sel;
  PCH_GPIO_DEV Io_Sel2;
  PCH_GPIO_DEV Io_Sel3;
  PCH_GPIO_DEV Lvl;
  PCH_GPIO_DEV Lvl2;
  PCH_GPIO_DEV Lvl3;
  PCH_GPIO_DEV Inv;
  PCH_GPIO_DEV Blink;
  PCH_GPIO_DEV Rst_Sel;
  PCH_GPIO_DEV Rst_Sel2;
} GPIO_INIT_STRUCT;

EFI_STATUS
GetPlatformInfo (
  OUT PLATFORM_INFO_HOB                 **PlatformInfoHob
  );



EFI_STATUS
PlatformInitStage1 (
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
/*++
  Description :
    Program ICH registers for platform specific requirement. For example, decode region
    program, GPIO program, KSC init

  Parameters:
    PeiSercies  - Point to Pei Services structure

  Returns:

--*/
;

EFI_STATUS
IsRecoveryMode (
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
/*++

Routine Description:

  Detect Recovery Mode.

Arguments:

  PeiServices - General purpose services available to every PEIM.

Returns:

  EFI_SUCCESS     - System in Recovery Mode
  EFI_UNSUPPORTED - System doesn't support Recovery Mode
  EFI_NOT_FOUND   - System is not in Recovery Mode

--*/
;

EFI_STATUS
PlatformInitStage2 (
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
/*++
  Description :
    Do platform specific programming. For example, KSC init, Chipset programming

  Parameters:
    PeiSercies  - Point to Pei Services structure

  Returns:

--*/
;

#endif
