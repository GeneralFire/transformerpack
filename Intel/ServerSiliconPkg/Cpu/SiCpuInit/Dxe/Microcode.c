/** @file
  Code for microcode update

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "SiCpuInitDxe.h"

VOID      *mMicrocodePatchAddress;
UINT64    mMicrocodePatchSize;

/**
  Get the cached microcode patch base address and size from the HOB.

  @retval  EFI_SUCCESS     HOB is found and the microcode base address and size
                           are updated.
  @retval  EFI_NOT_FOUND   HOB is not found.

**/
EFI_STATUS
GetMicrocodePatchInfoFromHob (
  VOID
  )
{
  EFI_HOB_GUID_TYPE            *GuidHob;
  EDKII_MICROCODE_PATCH_HOB    *MicrocodePathHob;

  GuidHob = GetFirstGuidHob (&gEdkiiMicrocodePatchHobGuid);
  if (GuidHob == NULL) {
    DEBUG((DEBUG_ERROR, "%a - ERROR: Fail to locate Microcode cache HOB!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  MicrocodePathHob = GET_GUID_HOB_DATA (GuidHob);

  mMicrocodePatchAddress = (VOID *)(UINTN)MicrocodePathHob->MicrocodePatchAddress;
  mMicrocodePatchSize    = MicrocodePathHob->MicrocodePatchRegionSize;

  DEBUG((
    DEBUG_INFO, "%a: MicrocodeBase = 0x%p, MicrocodeSize = 0x%lx\n",
    __FUNCTION__, mMicrocodePatchAddress, mMicrocodePatchSize
    ));

  return EFI_SUCCESS;
}

/**
  Get the cached microcode patch base address and size.

  @retval  EFI_SUCCESS             The microcode base address and size are
                                   updated.
  @retval  EFI_OUT_OF_RESOURCES    Not enough memory to complete the request.

**/
EFI_STATUS
GetMicrocodePatchInfo (
  VOID
  )
{
  EFI_STATUS    Status;

  Status = GetMicrocodePatchInfoFromHob ();
  if (EFI_ERROR (Status)) {
    //
    // If the loaded microcode patches data information cannot be fetched from
    // HOB, copy the microcode patches data to reserved memory
    //
    mMicrocodePatchSize    = PcdGet64 (PcdCpuMicrocodePatchRegionSize);
    mMicrocodePatchAddress = AllocateReservedPool ((UINTN) mMicrocodePatchSize);
    if (mMicrocodePatchAddress == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    CopyMem(
      mMicrocodePatchAddress,
      (VOID*)(UINTN)PcdGet64 (PcdCpuMicrocodePatchAddress),
      mMicrocodePatchSize
      );
  }

  return EFI_SUCCESS;
}

/**
  Detect whether specified processor can find matching mirocode patch.

  This function detects whether specified processor can find matching mirocode patch.

  @param  ProcessorNumber   The handle number of specified processor.

**/
VOID
MicrocodeDetect (
  IN UINTN   ProcessorNumber
  )
{
  UINT32                                  ExtendedTableLength;
  UINT32                                  ExtendedTableCount;
  CPU_MICROCODE_EXTENDED_TABLE            *ExtendedTable;
  CPU_MICROCODE_EXTENDED_TABLE_HEADER     *ExtendedTableHeader;
  CPU_MICROCODE_HEADER                    *MicrocodeEntryPoint;
  UINTN                                   MicrocodeEnd;
  UINTN                                   Index;
  UINT8                                   PlatformId;
  UINT32                                  RegEax;
  UINT32                                  LatestRevision;
  UINTN                                   TotalSize;
  UINT32                                  CheckSum32;
  BOOLEAN                                 CorrectMicrocode;
  MICROCODE_INFO                          *MicrocodeInfo;

  MicrocodeInfo = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData.MicrocodeInfo;

  ExtendedTableLength = 0;
  //
  // Here data of CPUID leafs have not been collected into context buffer, so
  // GetProcessorCpuid() cannot be used here to retrieve CPUID data.
  //
  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);

  //
  // The index of platform information resides in bits 50:52 of MSR IA32_PLATFORM_ID
  //
  PlatformId = (UINT8) (RShiftU64 (AsmReadMsr64 (MSR_IA32_PLATFORM_ID), 50) & 0x07);

  LatestRevision = 0;

  MicrocodeEntryPoint = mMicrocodePatchAddress;
  MicrocodeEnd = (UINTN) ((UINT64) MicrocodeEntryPoint + mMicrocodePatchSize);

  do {
    //
    // Check if the microcode is for the Cpu and the version is newer
    // and the update can be processed on the platform
    //
    CorrectMicrocode = FALSE;

    if (MicrocodeEntryPoint->HeaderVersion == 0x1) {
      //
      // It is the microcode header. It is not the padding data between microcode patches
      // because the padding data should not include 0x00000001 and it should be the repeated
      // byte format (like 0xXYXYXYXY....).
      //
      if (MicrocodeEntryPoint->ProcessorSignature.Uint32 == RegEax &&
          MicrocodeEntryPoint->UpdateRevision > LatestRevision &&
          (MicrocodeEntryPoint->ProcessorFlags & (1 << PlatformId))
          ) {
        if (MicrocodeEntryPoint->DataSize == 0) {
          CheckSum32 = CalculateSum32 ((UINT32 *)MicrocodeEntryPoint, 2048);
        } else {
          CheckSum32 = CalculateSum32 ((UINT32 *)MicrocodeEntryPoint, (MicrocodeEntryPoint->DataSize + sizeof(CPU_MICROCODE_HEADER)));
        }
        if (CheckSum32 == 0) {
          CorrectMicrocode = TRUE;
        }
      } else if ((MicrocodeEntryPoint->DataSize != 0) &&
                 (MicrocodeEntryPoint->UpdateRevision > LatestRevision)) {

        ExtendedTableLength = MicrocodeEntryPoint->TotalSize - (MicrocodeEntryPoint->DataSize + sizeof (CPU_MICROCODE_HEADER));
        if (ExtendedTableLength != 0) {
          //
          // Extended Table exist, check if the CPU in support list
          //
          ExtendedTableHeader = (CPU_MICROCODE_EXTENDED_TABLE_HEADER *)((UINT8 *)(MicrocodeEntryPoint) + MicrocodeEntryPoint->DataSize + sizeof (CPU_MICROCODE_HEADER));
          //
          // Calulate Extended Checksum
          //
          if ((ExtendedTableLength % 4) == 0) {
            CheckSum32 = CalculateSum32 ((UINT32 *)ExtendedTableHeader, ExtendedTableLength);
            if (CheckSum32 == 0) {
              //
              // Checksum correct
              //
              ExtendedTableCount = ExtendedTableHeader->ExtendedSignatureCount;
              ExtendedTable      = (CPU_MICROCODE_EXTENDED_TABLE *)(ExtendedTableHeader + 1);
              for (Index = 0; Index < ExtendedTableCount; Index ++) {
                CheckSum32 = CalculateSum32 ((UINT32 *)ExtendedTable, sizeof(CPU_MICROCODE_EXTENDED_TABLE));
                if (CheckSum32 == 0) {
                  //
                  // Verify Header
                  //
                  if ((ExtendedTable->ProcessorSignature.Uint32 == RegEax) &&
                      (ExtendedTable->ProcessorFlag & (1 << PlatformId)) ) {
                    //
                    // Find one
                    //
                    CorrectMicrocode = TRUE;
                    break;
                  }
                }
                ExtendedTable ++;
              }
            }
          }
        }

      }
    } else {
      //
      // It is the padding data between the microcode patches for microcode patches alignment.
      // Because the microcode patch is the multiple of 1-KByte, the padding data should not
      // exist if the microcode patch alignment value is not larger than 1-KByte. So, the microcode
      // alignment value should be larger than 1-KByte. We could skip SIZE_1KB padding data to
      // find the next possible microcode patch header.
      //
      MicrocodeEntryPoint = (CPU_MICROCODE_HEADER *) (((UINTN) MicrocodeEntryPoint) + SIZE_1KB);
      continue;
    }

    //
    // Get the next patch.
    //
    if (MicrocodeEntryPoint->DataSize == 0) {
      TotalSize = 2048;
    } else {
      TotalSize = MicrocodeEntryPoint->TotalSize;
    }

     if (CorrectMicrocode) {
       LatestRevision = MicrocodeEntryPoint->UpdateRevision;
       MicrocodeInfo->MicrocodeData = (VOID *)((UINTN)MicrocodeEntryPoint + sizeof (CPU_MICROCODE_HEADER));
       MicrocodeInfo->MicrocodeSize = TotalSize;
       MicrocodeInfo->ProcessorId = RegEax;
     }

    MicrocodeEntryPoint = (CPU_MICROCODE_HEADER *) (((UINTN) MicrocodeEntryPoint) + TotalSize);
  } while (((UINTN) MicrocodeEntryPoint < MicrocodeEnd));
}
