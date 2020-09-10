/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2020 Intel Corporation. <BR>

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

#ifndef _HPIOXACCESS_H_
#define _HPIOXACCESS_H_

#include "Platform.h"
#include <Register/PchRegsSmbus.h>
#include <RcRegs.h>
#include "SmbAccess.h"
#include <Protocol/IioUds.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/IioUds.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PciLib.h>
#include <Protocol/CpuCsrAccess.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PciExpressLib.h>

#define  R_PCH_SMBUS_PCICMD             0x04
#define  B_PCH_SMBUS_PCICMD_IOSE        BIT0

//
// Produced protocols
//
#include <Protocol/HpIoxAccess.h>

#define IN_PROGRESS                EFI_ALREADY_STARTED

typedef struct {
  UINT8 Addr;
  UINT8 PortNumber;
  UINT8 Value;
}SMBUS_VALUE;

typedef
EFI_STATUS
(EFIAPI *NEXT_ENTRY_POINTER) (
  VOID
  );

typedef struct _NEXT_STATE_ENTRY {
  NEXT_ENTRY_POINTER     NextEntryPointer;
} NEXT_STATE_ENTRY;

EFI_STATUS
InitializationHPIOX(
  VOID
  );

EFI_STATUS
EFIAPI
ReadIoxAndUpdateHpRequest(
  IN UINT8      *HpRequest,
  IN BOOLEAN    TimeSliced
  );

VOID
UpdateHpRequest(
  IN UINT8*     HpRequest
  );

VOID
DelayFn(
  UINT32 dCount
  );

EFI_STATUS
EFIAPI
IssuePldCmdThroIox (
  IN UINT8           Request,
  IN UINT8           SocketId,
  IN BOOLEAN         TimeSliced
  );

EFI_STATUS
EFIAPI
ClearAttnLatch (
  IN BOOLEAN    TimeSliced
  );

EFI_STATUS
EFIAPI
ClearInterrupt(
  IN  BOOLEAN TimeSliced
  );

EFI_STATUS
EFIAPI
HpSmbusReadWrite (
  IN      UINT8    SlaveAddress,
  IN      UINT8    Operation,
  IN      UINT8    Command,
  IN OUT  VOID     *Buffer,
  IN      BOOLEAN  TimeSliced
  );

BOOLEAN
EFIAPI
CheckOnlineOfflineRequest(
VOID
  );

#endif
