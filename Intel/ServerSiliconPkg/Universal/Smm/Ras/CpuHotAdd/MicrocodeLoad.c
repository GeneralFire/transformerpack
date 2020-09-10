/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2019 Intel Corporation. <BR>

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

#include "CpuHotAdd.h"

VOID
LoadMicrocodeEx (
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
  MicrocodeEnd = (UINTN) (PcdGet64 (PcdCpuMicrocodePatchAddress) + PcdGet64 (PcdCpuMicrocodePatchRegionSize));
  MicrocodeEntryPoint = (CPU_MICROCODE_HEADER *) (UINTN) PcdGet64 (PcdCpuMicrocodePatchAddress);
  do {
    //
    // Check if the microcode is for the Cpu and the version is newer
    // and the update can be processed on the platform
    //
    CorrectMicrocode = FALSE;

    if (MicrocodeEntryPoint->HeaderVersion == 0x1) {
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
    }

     if (CorrectMicrocode) {
        //
        // If the patch is qualified, set corresponding capability, together with the microcode address
        // and the index of the patch.
        //
        LatestRevision = MicrocodeEntryPoint->UpdateRevision;
        //SetProcessorFeatureCapability (ProcessorNumber, Microcode, (VOID *)((UINTN)MicrocodeEntryPoint + sizeof (CPU_MICROCODE_HEADER)));

        AsmWriteMsr64 (
          MSR_BIOS_UPDT_TRIG,
          (UINT64) ((UINTN) MicrocodeEntryPoint + sizeof (CPU_MICROCODE_HEADER))
          );
     }

    //
    // Get the next patch.
    //
    if (MicrocodeEntryPoint->DataSize == 0) {
      TotalSize = 2048;
    } else {
      TotalSize = MicrocodeEntryPoint->TotalSize;
    }
    MicrocodeEntryPoint = (CPU_MICROCODE_HEADER *) (((UINTN) MicrocodeEntryPoint) + TotalSize);
  } while (((UINTN) MicrocodeEntryPoint < MicrocodeEnd) && (MicrocodeEntryPoint->TotalSize != 0xFFFFFFFF));
}


