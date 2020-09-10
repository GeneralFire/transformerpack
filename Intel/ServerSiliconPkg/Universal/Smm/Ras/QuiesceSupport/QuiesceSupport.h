/** @file QuiesceSupport.h
  Quiesce driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#ifndef _QUIESCE_SUPPORT_H
#define _QUIESCE_SUPPORT_H

//
// Include files
//
#include "UncoreCommonIncludes.h"
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>

#define QUIESCE_CODE_SIZE (64 * 1024)
#define QUIESCE_DATA_SIZE (64 * 1024)
#define QUIESCE_STACK_SIZE (2 * 1024)


// This switch, SV_QUIESCE_CODE must be defined upon SV's request
//#define SV_QUIESCE_CODE    1

typedef struct {
  EFI_PHYSICAL_ADDRESS  CodeBaseAddress;
  UINTN                 CodeSize;
  EFI_PHYSICAL_ADDRESS  DataBaseAddress;
  UINTN                 DataSize;
  EFI_PHYSICAL_ADDRESS  PageTableBaseAddr;
  UINTN                 PageTableSize;
  EFI_PHYSICAL_ADDRESS  BuildDataBaseAddress;
  UINTN                 BuildDataSize;
  UINT64                MMCfgBase[MAX_SOCKET];
  UINT64                MiscFeatureMsrVal;
  UINT32                AvailableBufferLegth;
  UINT32                MonarchSocketId;
  UINT32                MonarchApicId;
  volatile UINT32       QuiesceStatus4APs;
  BOOLEAN               CodeCoppied;
  BOOLEAN               SMTEnabled;     //HT Enabled
  BOOLEAN               MonarchApCheckInStatus;

} QUIESCE_COMMUNICATION_DATA_STRUCT;



/**

  This routine returns pointer to base address and size of .
  Quiesce Code memory.

  @param QuiesceCodeBaseAddress  - Base address of Quiesce Code memory
  @param QuiesceCodeSize         - Quiesce Code memory size

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
GetQuiesceCodeMemoryInfo (
  IN OUT UINTN *QuiesceCodeBaseAddress,
  IN OUT UINTN *QuiesceCodeSize
)
;


/**

  This routine returns pointer to base address and size of .
  Quiesce Data memory.

  @param QuiesceDataBaseAddress  - Base address of Quiesce Data memory
  @param QuiesceDataSize         - Quiesce Data memory size

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
GetQuiesceDataMemoryInfo (
  IN OUT UINTN *QuiesceDataBaseAddress,
  IN OUT UINTN *QuiesceDataSize
)
;


EFI_STATUS
EFIAPI
QuiesceApProc(
  VOID)
/**

  This routine is API for AP to enter Quiesce AP handler.

  @param None

  @retval EFI_STATUS

**/
;

/**

  This routine returns pointer to next avaialble address in
  Quiesce Data memory.

  @param BufferAddress       -  pointer to next avialble quiesce memory

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
AquireDataBuffer(
  IN OUT EFI_PHYSICAL_ADDRESS   *BufferAddress
  )
;


/**

  This routine release the Quiesce Data memory from the address
  passed in.

  @param BaseAddress       -  Start address to be freed

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
ReleaseDataBuffer(
  IN EFI_PHYSICAL_ADDRESS  BaseAddress
  )
;

/**

  This routine disable MSI/VLW from IOH PCIE/Internal devices,
  and set quiesce command.

  @param None

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
SystemQuiesceUnQuiesce(
  BOOLEAN QuiesceFlag
  )
;

VOID
DisableIioPcieMsg(
    UINT8     ProgAll
);

VOID
RestoreIioPcieMsg(
    UINT8     ProgAll
);

/**

  This routine is main entry point for Monarch to do system Quiesce.
  It contains main flow for Monarch to Quiesce/UnQuiesce system.

  @param None

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
QuiesceMain(
  VOID
)
;

EFI_STATUS
EFIAPI
MonarchAPCheckIn (
  IN BOOLEAN  CheckIn
  );

/**

  This routine is API for AP to enter Quiesce AP handler and wait for BSP completion.

  @param None

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
QuiesceApPollingProc (
  VOID
);


#endif
