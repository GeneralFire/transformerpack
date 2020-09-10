/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2016 Intel Corporation. <BR>

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

#ifndef _LT_DXE_LIB_H_
#define _LT_DXE_LIB_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Guid/PlatformTxt.h>

UINTN
GetSinitAddress (
  VOID
  );


EFI_STATUS
GetBiosAcmErrorReset (
  UINT8 *BiosAcmErrorResetBit
  );


EFI_STATUS
GetAcmType (
  UINT8 *AcmTypeValue
  );

EFI_STATUS
SetAcmType (
  IN UINT8 AcmTypeValue
  );


EFI_STATUS
GetLockChipset (
  UINT8 *LockChipsetBit
  );


EFI_STATUS
GetEVMode (
  UINT8 *EVModeBit
  );


EFI_STATUS
SetLockChipset (
  IN UINT8 LockChipsetValue
  );


EFI_STATUS
SetEVMode (
  IN UINT8 EVModeValue
  );

EFI_STATUS
GetProcessorLtsxEnable (
  UINT8 *ProcessorLtsxEnableBit
  );


EFI_STATUS
SetDisableLtsx (
  VOID
  );


VOID
SetDisableSoftwareSmi (
  VOID
  );

EFI_STATUS
SetupLtDeviceMemory (
  IN LT_DXE_LIB_CONTEXT *LtDxeCtx
  );

// APTIOV_SERVER_OVERRIDE_RC_START : Moved later to get ACM type to get BiosAcmAddress from PlatformTxtPolicyData
EFI_STATUS
FindAcmBinaryType(
  IN LT_DXE_LIB_CONTEXT *LtDxeCtx
  );
// APTIOV_SERVER_OVERRIDE_RC_END : Moved later to get ACM type to get BiosAcmAddress from PlatformTxtPolicyData


#endif //_LT_DXE_LIB_H_
