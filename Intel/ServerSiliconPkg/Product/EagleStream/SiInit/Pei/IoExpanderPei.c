/** @file
  This driver manages the initial phase of I/O Expander

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchInfoLib.h>
#include <Library/HobLib.h>
#include <IioPlatformData.h>
#include "PchIoExpander.h"

/**
  Add MMIO base, detect I/O Expander and install I/O Expander Info PPI

  @param[in] IoExpanderInfo          I/O Expander Info PPI

  @retval None
**/
VOID
EFIAPI
IoExpanderDiscover (
  IO_EXPANDER_INFO    *IoExpanderInfo
  )
{
  UINT32             IoExpanderCount = 0;
  UINT32             IoExpanderIndex;
  UINT8              Segment;
  UINT8              Bus;
  UINT64             LpcBaseAddress;
  UINT16             DeviceId;
  EFI_HOB_GUID_TYPE  *GuidHob;
  IIO_GLOBALS        *IioGlobalData;
  UINT64             PchMmioBase;
  UINT8              SocketIndex;

  DEBUG ((DEBUG_ERROR, "[I/O Expander] IoExpanderDiscover - Start\n"));

  GuidHob = GetFirstGuidHob (&gIioSiPolicyHobGuid);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_ERROR, "[I/O Expander] ERROR: Failed to get gIioSiPolicyHobGuid Hob!\n"));
    ASSERT (FALSE);
    return;
  }
  IioGlobalData = (IIO_GLOBALS *) GET_GUID_HOB_DATA (GuidHob);

  for (SocketIndex = 1; (SocketIndex < MAX_SOCKET) && (IoExpanderCount < IO_EXPANDER_MAX); SocketIndex++) {
    if (!IioGlobalData->IioVar.IioVData.SocketPresent[SocketIndex]) {
      continue;
    }
    Segment = IioGlobalData->IioVar.IioVData.SegmentSocket[SocketIndex];
    Bus = IioGlobalData->IioVar.IioVData.SocketBaseBusNumber[SocketIndex];
    if (Segment != 0) {
      DEBUG ((DEBUG_ERROR, "[I/O Expander] Non Zero Segment is currently not supported: 0x%02x, skipping...\n", Segment));
      continue;
    }
    DEBUG ((DEBUG_ERROR, "[I/O Expander] Segment: 0x%02x, Bus: 0x%02x\n", Segment, Bus));
    LpcBaseAddress = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, LpcDevNumber (), LpcFuncNumber (), 0);
    DeviceId = PciSegmentRead16 (LpcBaseAddress + PCI_DEVICE_ID_OFFSET);
    if (DeviceId == 0xffff) {
      DEBUG ((DEBUG_ERROR, "[I/O Expander] Not found LPC device on Bus: 0x%02x, skipping...\n", Bus));
    } else if (IsPchSupported ()) {
      DEBUG ((DEBUG_ERROR, "[I/O Expander] Found PCH on Bus: 0x%02x, LPC DeviceId: 0x%04x\n", Bus, DeviceId));
      PchMmioBase = IioGlobalData->IioVar.IioVData.PchSegRegBaseAddress[SocketIndex];
      if (PchMmioBase == 0) {
        DEBUG ((DEBUG_ERROR, "[I/O Expander] MMIO base on Socket %d is 0, skipping!\n", SocketIndex));
        continue;
      }
      DEBUG ((DEBUG_ERROR, "[I/O Expander] MMIO base on Socket %d is 0x%04x\n", SocketIndex, PchMmioBase));
      for (IoExpanderIndex = 0; IoExpanderIndex < IO_EXPANDER_MAX; IoExpanderIndex++) {
        if (IoExpanderInfo->Valid[IoExpanderIndex]) {
          continue;
        }
        PchFillIoExpanderInfo (
            IoExpanderInfo,
            IoExpanderIndex,
            SocketIndex,
            Segment,
            Bus,
            PchMmioBase
            );
        IoExpanderCount++;
        break;
      }
    } else {
      DEBUG ((DEBUG_ERROR, "[I/O Expander] Unsupported PCH SKU, Bus: 0x%02x, LPC DeviceId: 0x%04x!\n", Bus, DeviceId));
      ASSERT (FALSE);
    }
  }

  DEBUG ((DEBUG_ERROR, "[I/O Expander] IoExpanderDiscover - End\n"));
}

/**
 @brief
   I/O Expander PCH entry point.

 @param[in] FileHandle  PEIM file handle
 @param[in] PeiServices General purpose services available to every PEIM

 @retval EFI_SUCCESS    The function completed successfully.
**/
EFI_STATUS
IoExpanderPeiEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                    Status;
  IO_EXPANDER_INFO              *IoExpanderInfo;
  EFI_PEI_PPI_DESCRIPTOR        *IoExpanderInfoPpiDesc;

  DEBUG ((DEBUG_ERROR, "[I/O Expander] IoExpanderPeiEntryPoint called.\n"));

  //
  // Create IoExpanderInfo
  //
  IoExpanderInfo = (IO_EXPANDER_INFO *) AllocateZeroPool (sizeof (IO_EXPANDER_INFO));
  if (IoExpanderInfo == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  IoExpanderInfo->Revision = IO_EXPANDER_INFO_REVISION;

  IoExpanderDiscover (IoExpanderInfo);

  //
  // Install IoExpanderInfoPpi
  //
  IoExpanderInfoPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (IoExpanderInfoPpiDesc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  IoExpanderInfoPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  IoExpanderInfoPpiDesc->Guid  = &gIoExpanderInfoPpiGuid;
  IoExpanderInfoPpiDesc->Ppi   = IoExpanderInfo;

  Status = PeiServicesInstallPpi (IoExpanderInfoPpiDesc);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}
