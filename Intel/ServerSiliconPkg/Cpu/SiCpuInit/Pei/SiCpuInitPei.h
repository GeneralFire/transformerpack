/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _SI_CPU_INIT_PEI_H_
#define _SI_CPU_INIT_PEI_H_

#include <PiPei.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/PostBootScriptTable.h>
#include <Ppi/CpuLatePolicyPpi.h>
#include <Ppi/MpServices.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RegisterCpuFeaturesLib.h>
#include <Library/PcdLib.h>
#include <Library/CpuPreMemPolicyLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/SystemInfoLib.h>
#include <Register/Cpuid.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>
#include <CpuDataStruct.h>
#include <IioUniversalData.h>
#include <UncoreCommonIncludes.h>

/**
  A callback function to build CPUs information Guided HOB.

  This function is a callback function to build one Guided HOB to pass
  CPUs' information to DXE phase.

  @param PeiServices        Pointer to PEI Services Table
  @param NotifyDescriptor   Address if the notification descriptor data structure
  @param Ppi                Address of the PPI that was installed

**/
EFI_STATUS
EFIAPI
BuildCpuInfoHob(
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
);

/**
  Call into Cpu library to restore the necessary MSR settings late in S3 resume.

  @param PeiServices    Pointer to PEI Services Table.
  @param NotifyDesc     Pointer to the descriptor for the Notification
                        event that caused this function to execute.
  @param Ppi            Pointer the the Ppi data associated with this function.

  @retval EFI_SUCCESS   S3 Cpu MSR restore call completed successfully.

**/
EFI_STATUS
EFIAPI
S3RestoreMsrCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
);

/**
  PEI CPU late policy PPI callback.

  @param PeiServices    Pointer to PEI Services Table.
  @param NotifyDesc     Pointer to the descriptor for the Notification
                        event that caused this function to execute.
  @param Ppi            Pointer to the Ppi data associated with this function.

  @retval EFI_SUCCESS   PEI CPU late policy PPI callback completed successfully.

**/
EFI_STATUS
EFIAPI
PeiCpuLatePolicyReadyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
  );

/**
  Call into Cpu library to perform any PEI CPU Initialization needed
  in PEI phase that requires multi-threaded execution. This is executed
  on normal boots and S3 resume.

  @param PeiServices    Pointer to PEI Services Table.
  @param NotifyDesc     Pointer to the descriptor for the Notification
                        event that caused this function to execute.
  @param Ppi            Pointer to the Ppi data associated with this function.

  @retval EFI_SUCCESS   PEI MP Services callback completed successfully.

**/
EFI_STATUS
EFIAPI
PeiMpServicesReadyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
);

#endif // _SI_CPU_INIT_PEI_H_
