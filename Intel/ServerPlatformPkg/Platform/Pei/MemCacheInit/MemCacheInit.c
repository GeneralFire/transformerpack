/** @file
  EFI 2.0 PEIM to set up memory type range registers.

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

#include <Library/MemTypeLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/KtiApi.h>
#include "MemCacheInit.h"

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  MemCacheEndOfPeiCallback
};

/**

  This function finds the start address and size of control or Block window region in the system.

  @param Host - pointer to sysHost structure on stack
  @param Memtype - Type of the memory range
  @param RangeSize - pointer to the variable to store the StartAddress
  @param RangeSize - pointer to the variable to store RangeSize

  @retval EFI_SUCCESS - success
          EFI_NOT_FOUND - Region not found.

**/
EFI_STATUS
GetMemoryRegionRange(
  struct SystemMemoryMapHob  *systemMemoryMap,
  UINT16                     Memtype,
  EFI_PHYSICAL_ADDRESS       *StartAddress,
  EFI_PHYSICAL_ADDRESS       *RangeSize)
{
  UINT8         Index;
  EFI_STATUS    Status = EFI_NOT_FOUND;
  UINT8         Socket = 0;
  EFI_PHYSICAL_ADDRESS Limit = 0;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (systemMemoryMap->Socket[Socket].SAD[0].Enable == 0) {
      continue;
    }

    for (Index = 0; Index < MAX_SAD_RULES; Index++) {
      if (systemMemoryMap->Socket[Socket].SAD[Index].Enable == 0) {
        continue;
      }
      if (systemMemoryMap->Socket[Socket].SAD[Index].type == Memtype) {
        //
        // Assign the new address values only when the found range limit is larger than the input limit
        //
        if (MultU64x32((UINT64)systemMemoryMap->Socket[Socket].SAD[Index].Limit, CONVERT_64MB_TO_BYTE) > (*StartAddress + *RangeSize)) {
          //
          // Store/Update the start and end addresses for the specified range in bytes if greater than previous limit
          //
          *StartAddress = (EFI_PHYSICAL_ADDRESS)MultU64x32 ((UINT64)systemMemoryMap->Socket[Socket].SAD[Index].Base, CONVERT_64MB_TO_BYTE);
          Limit = (EFI_PHYSICAL_ADDRESS)MultU64x32 ((UINT64)systemMemoryMap->Socket[Socket].SAD[Index].Limit, CONVERT_64MB_TO_BYTE);
          break;
        }
      }
    }
    if (Limit != 0) {
      break;
    }
  }

  if (Limit != 0) {
    *RangeSize = Limit - *StartAddress;
    Status = EFI_SUCCESS;
  }

  return Status;
}

/**

  Funtion to set MSR to enable MCA on cacheable MMIO access. It
  is executed on all sockets.

  @param MemoryLength -  Memory length of entire memory region

  @retval EFI_SUCCESS -  Writes BIT0 of McaOnCacheableMmio

**/
EFI_STATUS
EnableMcaOnCacheableMmio(
    VOID
)
{
  MSR_MCA_ON_NONNEM_CACHABLEMMIO_EN_REGISTER MsrReg;

  MsrReg.Uint64 = 0;
  MsrReg.Bits.McaNonnemCacheableMmioEn = 1;
  AsmWriteMsr64 (MSR_MCA_ON_NONNEM_CACHABLEMMIO_EN, MsrReg.Uint64);
  return EFI_SUCCESS;
}

/**

  MP programming MSR_MCA_ON_NONNEM_CACHABLEMMIO_EN at socket level.

  @param PeiServices -      Ptr of EFI_PEI_SERVICES ptr
  @param mPeiMpServices -   Ptr of EFI_PEI_MP_SERVICES_PPI

  @retval EFI_SUCCESS     -  Programming done
  @retval EFI_UNSUPPORTED -  Not support this platform

**/
EFI_STATUS
PkgMpEnableMcaOnCacheableMmio(
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_MP_SERVICES_PPI    *mPeiMpServices
)
{
  UINT16                    PackageDoneBitmap;
  UINTN                     NumberOfProcessors;
  UINTN                     NumberEnabledProcessors;
  UINTN                     Index;
  UINT8                     TotalSockets = 0;
  EFI_PROCESSOR_INFORMATION ProcInfo;
  EFI_STATUS                Status;

  if (IsSimicsEnvironment ()) {
    return EFI_UNSUPPORTED;
  }

  if (!IsCpuAndRevision (CPU_SKX, REV_ALL) && !IsCpuAndRevision (CPU_CLX, REV_ALL) && !IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    return EFI_UNSUPPORTED;
  }

  PackageDoneBitmap = 0;
  mPeiMpServices->GetNumberOfProcessors(
                    PeiServices,
                    mPeiMpServices,
                    &NumberOfProcessors,
                    &NumberEnabledProcessors
                    );

  //
  // Find total number of sockets present on the board
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (SocketPresent (Index)) {
      TotalSockets++;
    }
  }

  //
  // Loop through all the enabled processors and find one thread per socket present
  // to write the MSR.  Remote sockets need to use StartupThisAP.
  //
  for (Index = 0; Index < NumberOfProcessors; Index++) {
    mPeiMpServices->GetProcessorInfo(
                      PeiServices,
                      mPeiMpServices,
                      Index,
                      &ProcInfo
                      );

    if (!(PackageDoneBitmap & (1 << ProcInfo.Location.Package)) && (ProcInfo.StatusFlag & PROCESSOR_ENABLED_BIT)) {

      PackageDoneBitmap |= (1 << ProcInfo.Location.Package);
      TotalSockets--;

      if (ProcInfo.StatusFlag & PROCESSOR_AS_BSP_BIT) {
        EnableMcaOnCacheableMmio();
      } else {
        Status = mPeiMpServices->StartupThisAP (
                      PeiServices,
                      mPeiMpServices,
                      (EFI_AP_PROCEDURE)EnableMcaOnCacheableMmio,
                      Index,
                      0,
                      NULL
                      );
      }

      //
      // All sockets are programmed, skip checking rest of threads
      //
      if (TotalSockets == 0) {
        break;
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  Function to set all MTRRs on the current thread to the MTRR set passed in.

  @param MtrrSettings -  Pointer to MTRR Settings to use

  @retval None

**/
VOID
SetAllMtrrs(
    VOID    *MtrrSettings
)
{
  MtrrSetAllMtrrs((MTRR_SETTINGS*)MtrrSettings);
}

/**

  Function that analyzes memory length passed in to see if there is an
  appropriate address to perform a more efficient top down coverage of
  the memory range.  At this address bit, it is more efficient to overshoot
  memory range with WB cache type and cover the gap with UC cache type.

  @param MemoryLength -  Memory length of entire memory region

  @retval Zero            - No efficient top down approaches found.  More efficient
                            to cover memory range conventional way (bottom up).
          PwrOfTwo Value  - PowerOfTwo bit where the top down approach is most efficient

**/
UINT64
FindTopDownPowerOfTwo (
  IN UINT64               MemoryLength
  )
{
  UINT8   Index;
  BOOLEAN FoundOne = FALSE;
  UINT8   ZeroCount = 0;
  UINT8   OnesCount = 0;
  UINT64  TopDownBit = 0;
  UINT8   MtrrSavings = 0;

  for (Index = 0; Index < 64; Index++){
    if (MemoryLength & LShiftU64(1, Index)) {
      OnesCount++;
      FoundOne = TRUE;
    } else {
      //
      // If there are more 1's than 0's plus 2 between least significant bit set
      // and current bit under test, then top down approach is more efficient.
      // Continue to loop through memory length to look for more efficiencies
      // and compare them against previous efficiencies found to pick best power of two.
      //
      if (((ZeroCount + 2) < OnesCount) && ((OnesCount - (ZeroCount + 2)) > MtrrSavings)) {
        TopDownBit = LShiftU64(1, (Index - 1));
        MtrrSavings = OnesCount - (ZeroCount + 2);
      }
      if (FoundOne) {
        ZeroCount++;
      }
    }
  }

  //
  // MtrrLib can handle this case efficiently
  //
  if (TopDownBit == GetPowerOfTwo64(MemoryLength)) {
    TopDownBit = 0;
  }

  return TopDownBit;
}

/**

  Optimize the memory length such that it can locate in NXM region to mitigate
  the exhaustion of MTRRs

  @param MemoryLength      -  current memory length and would be assigned to new
                              value if the NXM region is valid for optimization

  @retval EFI_SUCCESS - success, memory length has been optimized with NXM region
          EFI_UNSUPPORTED - NXM region is not valid for optimization.

**/
EFI_STATUS
MemLengthOptimizeForNxm (
  IN OUT UINT64              *MemoryLength
  )
{
  UINT8  BitIndex;
  UINT8  Index;
  UINT8  HighBit = 0;
  UINT8  LowBit = 0;
  UINT64 TempMemoryLength;
  struct SystemMemoryMapHob  *mSystemMemoryMap;
  EFI_PHYSICAL_ADDRESS  NxmBase = 0;
  EFI_PHYSICAL_ADDRESS  NxmRange = 0;
  EFI_STATUS Status = EFI_UNSUPPORTED;

  //
  // Get NXM region information for optimizing the new memory length
  //
  mSystemMemoryMap = GetSystemMemoryMapData ();
  if (mSystemMemoryMap == NULL) {
    //
    // Return since the system memory map data is not available
    //
    return EFI_UNSUPPORTED;
  }

  do {
    Status = GetMemoryRegionRange(mSystemMemoryMap, MemTypeNxm, &NxmBase, &NxmRange);

    //
    // Check if the found NXM range is appended to the DRAM high memory
    //
    if (Status == EFI_SUCCESS && *MemoryLength == NxmBase) {
      break;
    }
  } while (Status == EFI_SUCCESS);

  if (EFI_ERROR (Status) ) {
    //
    // Return without optimizing memory length since there is no valid NXM region
    //
    return EFI_UNSUPPORTED;
  }

  //
  // Find the highest and the lowest set bits(denote as HighBit and LowBit)
  // for optimization process
  //
  for (BitIndex = 1; BitIndex <= 64 ;BitIndex++) {
    if ((RShiftU64(*MemoryLength, 64 - BitIndex) & 1) == 1) {
      if (HighBit == 0) {
        HighBit = 64 - BitIndex;
      }
      LowBit = 64 - BitIndex;
    }
  }

  //
  // Check the most optimal address by shifting left the highest order bit
  // and check if this optimal address fits within the NXM region.
  //
  if (LShiftU64(1, HighBit+1) < NxmBase+NxmRange && (HighBit+1) < 64 ) {
    *MemoryLength = LShiftU64(1, HighBit+1);
    Status = EFI_SUCCESS;
  } else {
    //
    // Find next optimal address by setting next highest power of 2 zero bit
    // position to one and clearing the rest of the address, then check if
    // this optimal address locates within NXM range.
    //
    for (BitIndex = HighBit;BitIndex > LowBit; BitIndex--) {
      if ((RShiftU64(*MemoryLength, BitIndex) & 1) == 0) {
        TempMemoryLength = *MemoryLength;

        TempMemoryLength |= LShiftU64(1, BitIndex);
        for (Index = LowBit;Index < BitIndex ;Index++) {
          TempMemoryLength &= ~LShiftU64(1, Index);
        }

        if (TempMemoryLength <= NxmBase+NxmRange) {
          *MemoryLength = TempMemoryLength;
          Status = EFI_SUCCESS;
          break;
        }
      }
    }
  }
  return Status;
}

/**

  Recalculate the memory length to prevent MTRR out of resource error.

  @param MemoryLength      -  Memory Length that we want to truncate

  @retval UINT64   - New truncated memory length

**/
UINT64
MemLengthRecalculation (
  IN UINT64                     MemoryLength
  )
{
  UINT8  BitIndex;

  for (BitIndex = 0;BitIndex < 64; BitIndex++) {
    if ((RShiftU64(MemoryLength, BitIndex) & 1) == 1) {
      //
      // Clear lowest power of two bit found
      //
      MemoryLength &= ~LShiftU64(1, BitIndex);
      break;
    }
  }
  return MemoryLength;
}

/**

  Sets the uncached part of upper memory as reserved to prevent OS from using.
  The uncached region will always be at the top of high memory.

  @param OriginalMemoryLength   -  Original top of memory value
  @param NewMemoryLength        -  New memory range used for successful programming

  @retval None

**/
VOID
ReserveUncachedMemory (
  IN UINT64           OriginalMemoryLength,
  IN UINT64           NewMemoryLength
  )
{
  EFI_PEI_HOB_POINTERS          Hob;
  EFI_HOB_RESOURCE_DESCRIPTOR   *ResourceHob;
  VOID                          *HobStart;
  UINT64                        TempLength;

  HobStart = GetFirstHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR);
  //
  // Scan resource descriptor hobs to set our required range attribute as tested
  //
  for (Hob.Raw = HobStart; !END_OF_HOB_LIST(Hob); Hob.Raw = GET_NEXT_HOB(Hob)) {
    if (GET_HOB_TYPE (Hob) == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
      ResourceHob = Hob.ResourceDescriptor;
      if ((ResourceHob->PhysicalStart >= NewMemoryLength) &&
        ((ResourceHob->PhysicalStart + ResourceHob->ResourceLength) <= OriginalMemoryLength)) {
        //
        // Range is completely included in the uncached area, mark as tested
        //
        ResourceHob->ResourceAttribute |= EFI_RESOURCE_ATTRIBUTE_TESTED;
      } else if ((ResourceHob->PhysicalStart < NewMemoryLength) &&
        (ResourceHob->PhysicalStart + ResourceHob->ResourceLength > NewMemoryLength)) {
        //
        // Shrink previous HOB to base of uncached region, create new hob to cover uncached space
        //
        TempLength = ResourceHob->ResourceLength;
        ResourceHob->ResourceLength = NewMemoryLength - ResourceHob->PhysicalStart;
        BuildResourceDescriptorHob(
          EFI_RESOURCE_SYSTEM_MEMORY,
          (
            EFI_RESOURCE_ATTRIBUTE_PRESENT |
            EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
            EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
            EFI_RESOURCE_ATTRIBUTE_TESTED
            ),
          ResourceHob->PhysicalStart + ResourceHob->ResourceLength,
          TempLength - ResourceHob->ResourceLength
          );
        if ((ResourceHob->PhysicalStart + ResourceHob->ResourceLength +
          (TempLength - ResourceHob->ResourceLength)) == OriginalMemoryLength) {
            break;
        }
      }
      if (ResourceHob->PhysicalStart + ResourceHob->ResourceLength == OriginalMemoryLength) {
        break;
      }
    }
  }

  HobStart = GetFirstHob (EFI_HOB_TYPE_MEMORY_ALLOCATION);
  //
  // Scan memory allocation hobs to make sure this range is free
  //
  for (Hob.Raw = HobStart; !END_OF_HOB_LIST(Hob); Hob.Raw = GET_NEXT_HOB(Hob)) {
    if (GET_HOB_TYPE (Hob) == EFI_HOB_TYPE_MEMORY_ALLOCATION) {
      if ((Hob.MemoryAllocation->AllocDescriptor.MemoryBaseAddress > NewMemoryLength) &&
        (OriginalMemoryLength > Hob.MemoryAllocation->AllocDescriptor.MemoryBaseAddress)) {
        //
        // If hob range is above the range we want to reserve, set limit to that base.
        //
        OriginalMemoryLength = Hob.MemoryAllocation->AllocDescriptor.MemoryBaseAddress;
      } else if ((Hob.MemoryAllocation->AllocDescriptor.MemoryBaseAddress +
        Hob.MemoryAllocation->AllocDescriptor.MemoryLength - 1) > NewMemoryLength) {
        //
        // If there is a range allocated that starts below but comes into the reserved range,
        // adjust the base so it starts above previous allocation
        //
        NewMemoryLength = Hob.MemoryAllocation->AllocDescriptor.MemoryBaseAddress +
          Hob.MemoryAllocation->AllocDescriptor.MemoryLength;
      }
    }
  }

  //
  // Create hob to reserve the memory
  //
  if (NewMemoryLength < OriginalMemoryLength) {
    BuildMemoryAllocationHob (
      (EFI_PHYSICAL_ADDRESS)NewMemoryLength,
      OriginalMemoryLength - NewMemoryLength,
      EfiReservedMemoryType
    );
  }
}

/**

  End of PEI Callback to set up the MTRRs for system boot.

  @param PeiServices       -  General purpose services available to every PEIM.
  @param NotifyDescriptor  -  Not used.
  @param Ppi               -  Not used.

  @retval EFI_SUCCESS   - MTRRs successfully programmed
          EFI_NOT_FOUND - Required HOB not found

**/
EFI_STATUS
MemCacheEndOfPeiCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS            Status;
  UINT64                LowUncableBase;
  UINT64                MemoryBase;
  UINT64                TempQword;
  UINT64                MemoryLength;
  UINT64                TempMemoryLength;
  UINT64                OriginalMemoryLength;
  UINT64                TopDownBit;
  EFI_PEI_HOB_POINTERS  Hob;
  EFI_PLATFORM_INFO     *PlatformInfo;
  EFI_HOB_GUID_TYPE     *GuidHob;
  IIO_UDS               *IioUds;
  MTRR_SETTINGS         MtrrSettings;
  EFI_GUID              UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_PEI_MP_SERVICES_PPI    *mPeiMpServices = NULL;
  EFI_BOOT_MODE              BootMode;

  BootMode = GetBootModeHob();
  if (BootMode != BOOT_ON_S3_RESUME) {
    //
    // Get required HOBs to be used to generate MTRR programming
    //
    GuidHob = GetFirstGuidHob (&UniversalDataGuid);
    if (GuidHob == NULL) {
      ASSERT(GuidHob != NULL);
      return EFI_NOT_FOUND;
    }
    IioUds = GET_GUID_HOB_DATA (GuidHob);

    GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
    if (GuidHob == NULL) {
      ASSERT(GuidHob != NULL);
      return EFI_NOT_FOUND;
    }
    PlatformInfo = GET_GUID_HOB_DATA(GuidHob);
    //
    // Calculate the low uncacheable base address
    //
    if (IioUds->PlatformData.PciExpressBase < IioUds->PlatformData.PlatGlobalMmio32Base) {
      LowUncableBase = IioUds->PlatformData.PciExpressBase;
    } else {
      LowUncableBase = IioUds->PlatformData.PlatGlobalMmio32Base;
    }

    LowUncableBase &= (0x0FFF00000);

    //
    // Reset all Mtrrs to 0 including fixed MTRR and variable MTRR
    //
    ZeroMem(&MtrrSettings, sizeof(MTRR_SETTINGS));

    //
    // Set fixed cache for memory range below 1MB
    //
    Status = MtrrSetMemoryAttributeInMtrrSettings(
      &MtrrSettings,
      0,
      0xA0000,
      EFI_CACHE_WRITEBACK
      );
    ASSERT_EFI_ERROR(Status);

    Status = MtrrSetMemoryAttributeInMtrrSettings(
      &MtrrSettings,
      0xA0000,
      0x60000,
      EFI_CACHE_UNCACHEABLE
      );
    ASSERT_EFI_ERROR(Status);

    //
    // Base set to 1mb due to MtrrLib programming method
    //
    MemoryBase = BASE_1MB;
    MemoryLength = LowUncableBase;

    Status = (*PeiServices)->GetHobList (PeiServices, &Hob.Raw);
    while (!END_OF_HOB_LIST (Hob)) {
      if (Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
        if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) ||
            (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_MAPPED_IO &&
            (Hob.ResourceDescriptor->ResourceAttribute & EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE)) ||
            (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED &&
             (Hob.ResourceDescriptor->ResourceAttribute & EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE))) {
          if (Hob.ResourceDescriptor->PhysicalStart > EFI_MAX_ADDRESS) {
            TempQword = Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength;
            if (TempQword > MemoryLength) {
              MemoryLength = TempQword;
            }
          }
        }
      }
      Hob.Raw = GET_NEXT_HOB (Hob);
    }

    DEBUG((DEBUG_INFO, "Total Memory size: 0x%lx\n", MemoryLength));

    //
    // Optimize the memory length if NXM region is valid to use
    //
    Status = MemLengthOptimizeForNxm(&MemoryLength);

    if (!EFI_ERROR (Status) ) {
      DEBUG((DEBUG_INFO, "NXM optimized memory size: 0x%lx\n", MemoryLength));
    }

    TempMemoryLength = MemoryLength;
    OriginalMemoryLength = MemoryLength;

    Status = EFI_SUCCESS;
    //
    // Loop will continue until MTRR programming is successfully done.
    // All regions reserved in this loop are considered essential regions
    // If any of them fail to fit, memory is truncated by lowest power of
    // two until all regions fit into the programming
    //
    do {
      if (Status == RETURN_OUT_OF_RESOURCES){
        //
        // Ran out of MTRRs: lower top of high memory by lowest power of two bit and retry
        //
        MemoryLength = MemLengthRecalculation (TempMemoryLength);
        DEBUG((DEBUG_ERROR, "MTRR: %r, attempting: 0x%lx\n", Status, MemoryLength));
        //
        // Restore the MemoryBase to its original value, save MemoryLength
        //
        MemoryBase = BASE_1MB;
        TempMemoryLength = MemoryLength;
        ZeroMem(&(MtrrSettings.Variables), sizeof(MTRR_VARIABLE_SETTINGS));
      }

      TopDownBit = FindTopDownPowerOfTwo(MemoryLength);

      //
      // If TopDownBit has a value, then we found a more efficient address length
      // to use a top down approach.  We will walk through the full address length to
      // program MTRRs individually.  BASE_1MB fixups are due to MtrrLib program method.
      //
      if(TopDownBit){
        DEBUG((DEBUG_INFO, "Efficient Top Down Power of Two = %lx\n\n", TopDownBit));
        while (MemoryLength != 0) {
          if (GetPowerOfTwo64(MemoryLength) == TopDownBit) {
            //
            // Overshoot address with WB and cover remaining gap with UC
            //
            TempQword = MemoryLength;
            MemoryLength = LShiftU64(GetPowerOfTwo64 (MemoryLength), 1);

            if(MemoryBase == BASE_1MB) {
              MemoryLength -= BASE_1MB;
            }

            Status = MtrrSetMemoryAttributeInMtrrSettings(
                &MtrrSettings,
                MemoryBase,
                MemoryLength,
                EFI_CACHE_WRITEBACK
                );

            if (Status == RETURN_OUT_OF_RESOURCES) {
              break;
            }

            if(MemoryBase == BASE_1MB) {
              MemoryBase = 0;
              MemoryLength += BASE_1MB;
            }

            MemoryBase += TempQword;
            MemoryLength -= TempQword;

            //
            // Program UC region gap between top of memory and WB MTRR
            //
            Status = MtrrSetMemoryAttributeInMtrrSettings(
                &MtrrSettings,
                MemoryBase,
                MemoryLength,
                EFI_CACHE_UNCACHEABLE
                );

            if (Status == RETURN_OUT_OF_RESOURCES) {
              break;
            }

            MemoryLength = 0;
          } else {
            //
            // Grow next power of two upwards and adjust base and length
            //
            TempQword = GetPowerOfTwo64(MemoryLength);
            MemoryLength -= TempQword;

            if(MemoryBase == BASE_1MB) {
              TempQword -= BASE_1MB;
            }

            Status = MtrrSetMemoryAttributeInMtrrSettings(
              &MtrrSettings,
              MemoryBase,
              TempQword,
              EFI_CACHE_WRITEBACK
              );

            if (Status == RETURN_OUT_OF_RESOURCES) {
              break;
            }

            MemoryBase += TempQword;
          }
        }
        if (Status == RETURN_OUT_OF_RESOURCES) {
          continue;
        }
      } else {
        //
        // Create a WB region for the entire memory region
        //
        Status = MtrrSetMemoryAttributeInMtrrSettings(
          &MtrrSettings,
          MemoryBase,
          MemoryLength - BASE_1MB,
          EFI_CACHE_WRITEBACK
          );

        if (Status == RETURN_OUT_OF_RESOURCES) {
          continue;
        }
      }

      //
      // Punch UC hole for lower MMIO region
      //
      Status = MtrrSetMemoryAttributeInMtrrSettings(
        &MtrrSettings,
        LowUncableBase,
        EFI_MAX_ADDRESS - LowUncableBase + 1,
        EFI_CACHE_UNCACHEABLE
        );

      if (Status == RETURN_OUT_OF_RESOURCES) {
        continue;
      }
#if ME_SUPPORT_FLAG
      //
      // Set MESEG memory range to be un-cachable if MESEG enabled
      //
      if( PlatformInfo->MemData.MemMESEGSize != 0 ) {
        Status = MtrrSetMemoryAttributeInMtrrSettings(
          &MtrrSettings,
          (EFI_PHYSICAL_ADDRESS)PlatformInfo->MemData.MemMESEGBase,
          (UINT64)PlatformInfo->MemData.MemMESEGSize,
          EFI_CACHE_UNCACHEABLE
          );

        if (Status == RETURN_OUT_OF_RESOURCES) {
          continue;
        }
      }
#endif //ME_SUPPORT_FLAG

#if IE_SUPPORT
      //
      // Set IESEG memory range to be un-cachable if MESEG enabled
      //
      if( PlatformInfo->MemData.MemIESEGSize != 0 ) {
        Status = MtrrSetMemoryAttributeInMtrrSettings (
          &MtrrSettings,
          (EFI_PHYSICAL_ADDRESS)PlatformInfo->MemData.MemIESEGBase,
          (UINT64)PlatformInfo->MemData.MemIESEGSize,
          EFI_CACHE_UNCACHEABLE
          );

        if (Status == RETURN_OUT_OF_RESOURCES) {
          continue;
        }
      }
#endif // IE_SUPPORT
    } while (Status == RETURN_OUT_OF_RESOURCES);
    //
    // Assert if there was an error other than resource issue
    //
    ASSERT_EFI_ERROR (Status);

    //
    // Set PSMI Trace Region to uncached
    //
    if (PlatformInfo->MemData.PsmiUcTraceRegionSize != 0) {
      Status = MtrrSetMemoryAttributeInMtrrSettings(
          &MtrrSettings,
          PlatformInfo->MemData.PsmiUcTraceRegionBase,
          PlatformInfo->MemData.PsmiUcTraceRegionSize,
          EFI_CACHE_UNCACHEABLE
          );
      if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "PSMI- Could not set Trace Region MemType to UC\n"));
      }
    }

    //
    // Set the calculated MTRR settings for the BSP
    //
    if (MsrTraceEnabled ()) {
      SetMsrTraceOutputMode (MSR_OUTPUT_ENABLE);
      DEBUG((DEBUG_ERROR, "MTRR Trace Start\n"));
      UbiosCall("DisableCache", SCOPE_THREAD);
    }
    MtrrSetAllMtrrs (&MtrrSettings);
    if (MsrTraceEnabled ()) {
      UbiosCall("EnableCache", SCOPE_THREAD);
      DEBUG((DEBUG_ERROR, "MTRR Trace End\n"));
      SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
    }

    //
    // Need to mark the uncached memory as reserved
    //
    if (OriginalMemoryLength > TempMemoryLength) {
      DEBUG((DEBUG_ERROR, "New TOHM: 0x%lx, Previous: 0x%lx\n", TempMemoryLength, OriginalMemoryLength));
      ReserveUncachedMemory (OriginalMemoryLength, TempMemoryLength);
    }
  }

  Status = (*PeiServices)->LocatePpi (
                    PeiServices,
                    &gEfiPeiMpServicesPpiGuid,
                    0,
                    NULL,
                    &mPeiMpServices
                    );

  if (BootMode != BOOT_ON_S3_RESUME) {
    if(!EFI_ERROR(Status)){
      //
      // Sync all AP MTRRs with BSP
      //
      Status = mPeiMpServices->StartupAllAPs (
                    PeiServices,
                    mPeiMpServices,
                    (EFI_AP_PROCEDURE)SetAllMtrrs,
                    FALSE,
                    0,
                    (VOID*)&MtrrSettings
                    );
    }
  }

  PkgMpEnableMcaOnCacheableMmio (PeiServices, mPeiMpServices);

  //
  // Report Status Code to indicate Cache initialization is complete.
  //
  REPORT_STATUS_CODE (
    EFI_PROGRESS_CODE,
    (EFI_COMPUTING_UNIT_HOST_PROCESSOR  | EFI_CU_HP_PC_CACHE_INIT)
    );

  return EFI_SUCCESS;
}


/**
  The Entry point of the Memory Cache PEIM.

  This function set up a callback for end of PEI to program MTRRs for
  the remainder of the boot process.

  @param  FileHandle    Handle of the file being invoked.
  @param  PeiServices   Describes the list of possible PEI Services.

  @retval EFI_SUCCESS   Notify PPI is installed successfully.

**/
EFI_STATUS
EFIAPI
MemCacheInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;
  //
  // Install notify PPIs
  //
// APTIOV_SERVER_OVERRIDE_RC_START : To initialze syshost structure size in PEI Phase 
#if BUILD_FSP_BINARY == 1 || FSP_SUPPORT_IN_BIOS == 1
    Status = PcdSet32S (PcdPeiSyshostMemorySize, sizeof (SYSHOST));
    DEBUG ((DEBUG_ERROR, "PcdPeiSyshostMemorySize Status = %r\n",Status));
  
  if(Status == EFI_SUCCESS) {
  DEBUG ((DEBUG_ERROR, "PcdPeiSyshostMemorySize value = %d\n", PcdGet32(PcdPeiSyshostMemorySize)));  
  }
#endif  
// APTIOV_SERVER_OVERRIDE_RC_END : To initialze syshost structure size in PEI Phase
  
  Status = PeiServicesNotifyPpi (&mNotifyList);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
