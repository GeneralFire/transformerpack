/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/SystemInfoLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <IndustryStandard/Pci.h>
#include <IndustryStandard/Acpi.h>
#include <MemHostChipCommon.h>
#include <Upi/KtiHost.h>
#include <Guid/CxlNodeHob.h>

#ifndef _CXL_DXE_H_
#define _CXL_DEX_H_

typedef struct {
  UINT8                Segment;
  UINT8                Bus;
} CXL_NODE_HANDLE;

typedef struct {
  UINT32               Signature;
  UINT32               ProximityDomain;
  LIST_ENTRY           ListEntry;
  CXL_NODE_INFO        Info;
  UINT8                SocketId;
  UINT8                HandleCount;
  CXL_NODE_HANDLE      Handles[MAX_CXL_PER_SOCKET];
} CXL_NODE_DATA;

typedef struct {
  UINT32               Signature;
  UINT32               NodeCount;
  UINT32               MemoryNodeCount;
  LIST_ENTRY           List;
} CXL_DATA;

#define CXL_DATA_SIGNATURE        SIGNATURE_32 ('C', 'X', 'L', 'D')
#define CXL_NODE_SIGNATURE        SIGNATURE_32 ('C', 'X', 'L', 'N')
#define CXL_NODE_FROM_LIST(a)     CR (a, CXL_NODE_DATA, ListEntry, CXL_NODE_SIGNATURE)

extern CXL_DATA    gCxlData;

/**
  Handler to update CXL ACPI table.

  @param[in, out] TablePtr       Pointer to ACPI buffer pointer.

  @retval EFI_SUCCESS            The ACPI table has been updated and saved in TablePtr.
  @retval Others                 Some error happens when update the ACPI table.

**/
typedef
EFI_STATUS
(EFIAPI *CXL_ACPI_TABLE_HANDLER)(
  IN OUT EFI_ACPI_SDT_HEADER          **TablePtr
  );



/**

  This function constructs CXL UEFI memory map by promoting normal memory to CXL memory

  @param[in] CxlNodeHob               Pointer to CXL node HOB.

**/
VOID
ConstructCxlUefiMemoryMap (
  IN CXL_NODE_SOCKET                  *CxlNodeHob
  );


/**
  Calculates CXL PCI handles.

  This function is invoked after PCI enumeration to get the exact PCI handle for all PCI devices

**/
VOID
CalculateCxlPciHandles (
  VOID
  );


/**
  Update CXL ACPI SRAT table.

  @param[in, out] TablePtr       Pointer to ACPI buffer pointer.

  @retval EFI_SUCCESS            The ACPI table has been updated and saved in TablePtr.
  @retval Others                 Some error happens when update the ACPI table.

**/
EFI_STATUS
UpdateSratTable (
  IN OUT EFI_ACPI_SDT_HEADER          **TablePtr
  );


#endif
