/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <RegPath.h>
#include REG_PATH(PSEUDO_OFFSET.h)
#include <CsrPseudoOffsetDefine.h>
#include <UncoreCommonIncludes.h>


EFI_PEI_PPI_DESCRIPTOR     mPpiListCsrPseudoOffset = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gCsrPseudoOffsetInitPpiGuid,
  NULL
};

/**

  @retval EFI_SUCCESS       Return the index to use for the pseudo offset table depending on CPU type
                            and Revision.

**/
UINT8
CsrPseudoOffsetGetCpuIdx (
VOID
)
{
  UINT8     CpuIdIndex;

  CpuIdIndex = 0;

  if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    if (IsCpuAndRevision (CPU_SNR, REV_AX)) {
      CpuIdIndex = 0;
    } else if (IsCpuAndRevision (CPU_SNR, REV_BX)) {
      CpuIdIndex = 1;
    } else {
      CpuIdIndex = 2;
    }
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    if (IsCpuAndRevision (CPU_ICXSP, REV_RX)) {
      CpuIdIndex = 0;
    } else {
      CpuIdIndex = 1;
    }
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    if (IsCpuAndRevision (CPU_SPRSP, REV_A0)) {
      CpuIdIndex = 0;
    } else if (IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
      CpuIdIndex = 1;
    } else {
      CpuIdIndex = 2;
    }
  }

  return CpuIdIndex;
}

/**
  PEIM entry point

  @param FileHandle         Pointer to the PEIM FFS file header.
  @param PeiServices        General purpose services available to every PEIM.

  @retval EFI_SUCCESS       Operation completed successfully.

**/
EFI_STATUS
EFIAPI
CsrPseudoOffsetInitPei (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                    Status;

#if (TOTAL_PSEUDO_OFFSET_TABLES == 0)
  Status = PeiServicesInstallPpi (&mPpiListCsrPseudoOffset);
  ASSERT_EFI_ERROR (Status);
  return EFI_SUCCESS;
#else
  UINT16                        TotalSize;
  UINT16                        DataBlockSize;
  UINT8                         Index;
  UINT32                         *Buffer;
  UINT8                         CpuIdIndex;
  UINT32                        *DataArray;
  UINT8                         RowIndex;
  UINT8                         RowIndexMax;

  PSEUDO_OFFSET_TABLE_HEADER    *TableHeader;

  Buffer        = NULL;
  DataBlockSize = 0;

  CpuIdIndex = CsrPseudoOffsetGetCpuIdx();

  if (CpuIdIndex >= CSR_PSEUDO_MAX_CPU_SUPPORT) {
    DEBUG ((DEBUG_ERROR, "CpuIdIndex is out of bounds of Pseudo offset table!\n"));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < TOTAL_PSEUDO_OFFSET_TABLES; Index ++) {
    DataBlockSize += (SubTableInfo[Index].SubTableSize) / CSR_PSEUDO_MAX_CPU_SUPPORT;
  }
  TotalSize = sizeof(PSEUDO_OFFSET_TABLE_HEADER) + DataBlockSize;
  TableHeader = BuildGuidHob (
                   &gCsrPseudoOffsetTableGuid,
                   TotalSize
                   );
  if (TableHeader == NULL) {
    DEBUG ((DEBUG_ERROR, "BuildGuidHob Returned NULL pointer\n"));
    ASSERT (TableHeader != NULL);
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (TableHeader, TotalSize);
  TableHeader->TableSize = TotalSize;
  Buffer = (UINT32*)((UINT8*) TableHeader + sizeof(PSEUDO_OFFSET_TABLE_HEADER));

  for (Index = 0; Index < TOTAL_PSEUDO_OFFSET_TABLES; Index ++) {
    //Save Buffer address into the table header in HOB.
    TableHeader->SubTableLocation[(SubTableInfo[Index].BoxType)][(SubTableInfo[Index].FuncBlk)] = (UINT16)((UINT8*) Buffer - (UINT8*) TableHeader);
    //GetSubTableAddress
    DataArray = (UINT32*)(SubTableInfo[Index].SubTableAddress);
    RowIndexMax = (UINT8)(((SubTableInfo[Index].SubTableSize) / CSR_PSEUDO_MAX_CPU_SUPPORT) / sizeof(UINT32));
    for (RowIndex = 0; RowIndex < RowIndexMax; RowIndex++) {
      *(Buffer) = *(UINT32*) (DataArray + RowIndex*CSR_PSEUDO_MAX_CPU_SUPPORT + CpuIdIndex);
      Buffer++;
    }
  }

  Status = PeiServicesInstallPpi (&mPpiListCsrPseudoOffset);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
#endif // #if (TOTAL_PSEUDO_OFFSET_TABLES == 0)
}
