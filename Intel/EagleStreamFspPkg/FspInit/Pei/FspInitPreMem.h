/** @file
  Header file for FSP Init Pre-Memory PEI module

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2019 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification
**/

#ifndef _PLATFORM_EARLY_INIT_PEI_H_
#define _PLATFORM_EARLY_INIT_PEI_H_

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/PciCf8Lib.h>
#include <Library/PciLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/CacheLib.h>
#include <Library/FspCommonLib.h>
#include <Library/FspPlatformLib.h>
#include <Library/FspSwitchStackLib.h>
#include <Library/PreSiliconEnvDetectLib.h>

#include <Library/PchPolicyLib.h>
#include <Library/PeiUpiPolicyLib.h>
#include <Library/RasRcPolicyPpiLib.h>
#include <Library/PeiMemoryPolicyLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/PeiIioSiPolicyLib.h>
#include <Library/PeiIioSiPolicyLib.h>

#include <Ppi/Stall.h>
#include <Ppi/Reset.h>
#include <Ppi/MemoryDiscovered.h>
#include <Guid/MemoryConfigData.h>
#include <Guid/GuidHobFspEas.h>

#include <CpuRegs.h>

#include <FspmUpd.h>
#include <FspStatusCode.h>

#include <Ppi/PlatformMemorySize.h>
#include <Guid/MemoryTypeInformation.h>
#include <Ppi/BaseMemoryTest.h>
#include <Ppi/IioSiPolicyPpi.h>

#define PEI_STALL_RESOLUTION   1

//
// Used in PEI memory test routines
//
#define MEMORY_TEST_COVER_SPAN  0x40000
#define MEMORY_TEST_PATTERN     0x5A5A5A5A


/**
  This function performs IIO PEI Policy update in pre-memory.

  @param[in, out] IioSiPolicyPpi  The IIO PreMem Policy PPI instance
  @param[in]      FspmUpd         The pointer of MemoryInitUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiIioPolicyPreMem (
  IN OUT IIO_SI_POLICY_PPI        *IioSiPolicyPpi,
  IN     FSPM_UPD                 *FspmUpd
  );

/**
  This function performs UPI PEI Policy update in pre-memory.

  @param[in, out] UpiPolicyPpi    The UPI PreMem Policy PPI instance
  @param[in]      FspmUpd         The pointer of MemoryInitUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiUpiPolicyPreMem (
  IN OUT UPI_POLICY_PPI           *UpiPolicyPpi,
  IN     FSPM_UPD                 *FspmUpd
  );

/**
  This function performs RAS PEI Policy update in pre-memory.

  @param[in, out] RasRcPolicyPpi  The RAS PreMem Policy PPI instance
  @param[in]      FspmUpd         The pointer of MemoryInitUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiRasRcPolicyPreMem (
  IN OUT RAS_RC_POLICY_PPI        *RasRcPolicyPpi,
  IN     FSPM_UPD                 *FspmUpd
  );

/**
  This function performs Memory PEI Policy update in pre-memory.

  @param[in, out] MemoryPolicyPpi The Memory PreMem Policy PPI instance
  @param[in]      FspmUpd         The pointer of MemoryInitUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiMemoryPolicyPreMem (
  IN OUT MEMORY_POLICY_PPI        *MemoryPolicyPpi,
  IN     FSPM_UPD                 *FspmUpd
  );

/**
  This function performs CPU PEI Policy update in pre-memory.

  @param[in]      FspmUpd         The pointer of MemoryInitUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiCpuPolicyPreMem (
  IN     FSPM_UPD                 *FspmUpd
  );

/**
  This function will be called when gReferenceCodePolicyPpiGuid is installed or reinstalled.

  @param  PeiServices General purpose services available to every PEIM.

  @param  NotifyDescriptor Information about the notify event..

  @param  Ppi The notify context.

  @retval EFI_SUCCESS If the function completed successfully.
**/
EFI_STATUS
EFIAPI
ReferenceCodePolicyNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**

  GC_TODO: Add function description

  @param PeiServices - GC_TODO: add argument description
  @param This        - GC_TODO: add argument description
  @param MemorySize  - GC_TODO: add argument description

  @retval GC_TODO: add return values

**/
EFI_STATUS
EFIAPI
GetPlatformMemorySize (
  IN      EFI_PEI_SERVICES                       **PeiServices,
  IN      PEI_PLATFORM_MEMORY_SIZE_PPI           *This,
  IN OUT  UINT64                                 *MemorySize
  )
;

EFI_STATUS
EFIAPI
BaseMemoryTest (
  IN  EFI_PEI_SERVICES          **PeiServices,
  IN  PEI_BASE_MEMORY_TEST_PPI  *This,
  IN  EFI_PHYSICAL_ADDRESS      BeginAddress,
  IN  UINT64                    MemoryLength,
  IN  PEI_MEMORY_TEST_OP        Operation,
  OUT EFI_PHYSICAL_ADDRESS      *ErrorAddress
  )
;


/**
  This function will be called when MRC is done.

  @param  PeiServices General purpose services available to every PEIM.

  @param  NotifyDescriptor Information about the notify event..

  @param  Ppi The notify context.

  @retval EFI_SUCCESS If the function completed successfully.
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**

  Checks the setup option for breakpoint type and starts
  a deadloop if After KTI RC was selected

  @param **PeiServices - Pointer to the list of PEI services
  @param *NotifyDescriptor - The notification structure this PEIM registered on install
  @param *Ppi - Pointer to the PPI

  @retval EFI_SUCCESS  -  Function has completed successfully.

**/
EFI_STATUS
EFIAPI
AfterKtiRcBreakpoint (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

/**
  This function provides a blocking stall for reset at least the given number of microseconds
  stipulated in the final argument.

  @param  PeiServices General purpose services available to every PEIM.

  @param  this Pointer to the local data for the interface.

  @param  Microseconds number of microseconds for which to stall.

  @retval EFI_SUCCESS the function provided at least the required stall.
**/
EFI_STATUS
EFIAPI
Stall (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN CONST EFI_PEI_STALL_PPI  *This,
  IN UINTN                    Microseconds
  );

/**
  This function reset the entire platform, including all processor and devices, and
  reboots the system.

  @param  PeiServices General purpose services available to every PEIM.

  @retval EFI_SUCCESS if it completed successfully.
**/
EFI_STATUS
EFIAPI
ResetSystem (
  IN CONST EFI_PEI_SERVICES          **PeiServices
  );

/**
  This function reports and installs new FV

  @retval     EFI_SUCCESS          The function completes successfully
**/
EFI_STATUS
ReportAndInstallNewFv (
  VOID
  );

#endif
