/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <PiPei.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PsmiConfig.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/UsraCsrLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/PeiServicesTablePointerLib.h>

#include <Ppi/PlatformMemoryRange.h>
#include <Ppi/BaseMemoryTest.h>
#include <Ppi/PlatformMemorySize.h>

#include <Guid/FspInfo.h>
#include <Guid/MemoryTypeInformation.h>
#include <Guid/SmramMemoryReserve.h>
#include <Guid/AcpiS3Context.h>
#include <Guid/MemoryMapData.h>
#include <Guid/ReservedMemoryData.h>
#include <Guid/PlatformInfo.h>
#include <Include/Guid/VtdPmrInfoHob.h>

#include <RcRegs.h>
#include <SysHost.h>
#include <Platform.h>
#include <CpuDataStruct.h>

#include <Register/Cpuid.h>

#include <Library/SecurityPolicyLib.h>
#include <Library/MemDecodeLib.h>
#if FixedPcdGetBool(PcdFspBinaryEnable)  == 1
#include <Library/FspCommonLib.h>
#endif
#include <Library/PcdLib.h>

//
// GNRSRF_TODO BIOS HSD-ES 22010930974:
//   Temporarily using ChaRegs.h header file references to workaround register
//   name differences in GNR/SRF until the CHA registers access APIs are
//   implemented in CHA IP library.
//
#include <Library/ChaIpLib/Include/ChaRegs.h>

//
// ------------------------ TSEG Base
// ...............
// ------------------------ RESERVED_CPU_S3_SAVE_OFFSET
// CPU S3 data
// ------------------------ RESERVED_ACPI_S3_RANGE_OFFSET
// S3 Memory base structure
// ------------------------ TSEG + 1 page

#define RESERVED_CPU_S3_SAVE_OFFSET (RESERVED_ACPI_S3_RANGE_OFFSET - sizeof (SMM_S3_RESUME_STATE))

//
// Maximum number of memory ranges supported
//
#define MAX_RANGES        (4 + 8)

#define MEM_EQU_4GB                     0x100000000

//
// SMRAM range definitions
//
#define MC_ABSEG_HSEG_PHYSICAL_START        0x000A0000
#define MC_ABSEG_HSEG_LENGTH                0x00020000
#define MC_ABSEG_CPU_START                  0x000A0000
#define MC_HSEG_CPU_START                   0xFEDA0000

//
// Memory range types
//
typedef enum {
  DualChannelDdrMainMemory,
  DualChannelDdrSmramCacheable,
  DualChannelDdrSmramNonCacheable,
  DualChannelDdrGraphicsMemoryCacheable,
  DualChannelDdrGraphicsMemoryNonCacheable,
  DualChannelDdrReservedMemory,
  DualChannelDdrADRMemory,
  DualChannelDdrMaxMemoryRangeType
} PEI_DUAL_CHANNEL_DDR_MEMORY_RANGE_TYPE;

#pragma pack(1)

//
// Memory map range information
//
typedef struct {
  UINT8                                         RowNumber;
  EFI_PHYSICAL_ADDRESS                          PhysicalAddress;
  EFI_PHYSICAL_ADDRESS                          CpuAddress;
  EFI_PHYSICAL_ADDRESS                          RangeLength;
  PEI_DUAL_CHANNEL_DDR_MEMORY_RANGE_TYPE        Type;
} PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE;

#pragma pack()


VOID
UpdatePsmiTraceInfo (
  EFI_PLATFORM_INFO  *PlatformInfo
  )
{
  EFI_HOB_GUID_TYPE             *GuidHob = NULL;
  PSMI_TRACE_INFO_HOB           *DataHob;
  UINT64                         Size = 0;
  UINT64                         BaseAddress = 0;
  //
  // Locate the Data hob, if not found , return
  //
  GuidHob = GetFirstGuidHob (&gPsmiInitDataGuid);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_INFO, "UpdatePsmiTraceInfo () - GuidHob is NULL for gPsmiInitDataGuid\n"));
    return;
  }

  DataHob = (PSMI_TRACE_INFO_HOB *) GET_GUID_HOB_DATA (GuidHob);
  if (DataHob == NULL) {
    DEBUG ((DEBUG_INFO, "UpdatePsmiTraceInfo () - DataHob is NULL for gPsmiInitDataGuid\n"));
    return;
  }

  if (PlatformInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "UpdatePsmiTraceInfo () - PlatformInfo is NULL\n"));
    return;
  }

  Size = DataHob->SocketBuffer[0].Trace[1].Size;
  BaseAddress = (UINT64) (DataHob->SocketBuffer[0].Trace[1].BaseAddress);

  if ((BaseAddress != 0) && (Size > 0)) {
    PlatformInfo->MemData.PsmiUcTraceRegionBase = BaseAddress;
    PlatformInfo->MemData.PsmiUcTraceRegionSize = Size;
  } else{
    DEBUG ((DEBUG_ERROR, "UpdatePsmiTraceInfo () - BaseAddress and size are not programmed for MTRRs. Base Address: %lx Size: %lx\n", BaseAddress, Size));
  }
}

/**

  This function returns the memory ranges to be enabled, along with information
  describing how the range should be used.

  @param MemoryMap: Buffer to record details of the memory ranges to be enabled.

  NumRanges:        On input, this contains the maximum number of memory ranges that
                    can be described in the MemoryMap buffer.

  @param[in] Host  - Pointer to SysHost


  @retval The MemoryMap buffer will be filled in and NumRanges will contain the actual
  @retval number of memory ranges that are to be anabled.

  0 - completed successfully.

**/
EFI_STATUS
GetMemoryMap (
  IN PSYSHOST Host,
  IN OUT PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE  *MemoryMap,
  IN OUT UINT8                                  *NumRanges
  )
{
  BOOLEAN                           EnableSmram;
  EFI_PHYSICAL_ADDRESS              MemorySize;
  EFI_PHYSICAL_ADDRESS              RowLength;
  EFI_PHYSICAL_ADDRESS              AlignedTsegBase = 0;
  EFI_PHYSICAL_ADDRESS              AlignedIedBase = 0;
  PEI_MEMORY_RANGE_SMRAM            SmramMask;
  PEI_MEMORY_RANGE_SMRAM            TsegMask;
  UINT32                            BlockNum;
  UINT8                             ExtendedMemoryIndex;
  UINT8                             Index;
  UINT8                             IioIndex;
  UINT8                             Socket;
  TSEG_N0_IIO_VTD_STRUCT            TsegBase;
  TSEG_N1_IIO_VTD_STRUCT            TsegLimit;
  EFI_PLATFORM_INFO                 *PlatformInfo;
  EFI_HOB_GUID_TYPE                 *GuidHob;
  UINT32                            BiosGuardMemSizeMB;
  EFI_PHYSICAL_ADDRESS              Imr2BaseAddress;
  VTD_PMR_INFO_HOB                  *VtdPmrHob;
  EFI_CPUID_REGISTER                CpuidRegisters;
#if defined(ME_SUPPORT_FLAG) || defined (IE_SUPPORT)
  UINT32                            SMRRSize;
  UINT32                            GapForSMRRAlign;
  EFI_PHYSICAL_ADDRESS              MemorySize_gap;
  EFI_PHYSICAL_ADDRESS              AlignedTsegBase_gap;
  UINT8                             MesegIndex = 0;
#if ME_SUPPORT_FLAG
  UINT32                            MeAlignment = 1;
#endif // ME_SUPPORT_FLAG
#if IE_SUPPORT
  UINT32                            IeAlignment = 1;
#endif
#if !defined (SKX_HOST)  || defined (IE_SUPPORT)
  UINT8                             IesegIndex = 0;
#endif
  EFI_PHYSICAL_ADDRESS              AlignedMesegBase;
  EFI_PHYSICAL_ADDRESS              AlignedMesegMask;
  MESEG_BASE_N0_CHABC_SAD_STRUCT    MeMemLowBaseAddr;
  MESEG_BASE_N1_CHABC_SAD_STRUCT    MeMemHighBaseAddr;
  MESEG_LIMIT_N0_CHABC_SAD_STRUCT   MeMemLowLimit;
  MESEG_LIMIT_N1_CHABC_SAD_STRUCT   MeMemHighLimit;
#ifdef SKX_HOST
  MENCMEM_BASE_0_IIO_VTD_STRUCT     MeNcMemLowBaseAddr;
  MENCMEM_BASE_1_IIO_VTD_STRUCT     MeNcMemHighBaseAddr;
  MENCMEM_LIMIT_0_IIO_VTD_STRUCT    MeNcMemLowLimit;
  MENCMEM_LIMIT_1_IIO_VTD_STRUCT    MeNcMemHighLimit;
#endif // SKX_HOST
#if !defined(SKX_HOST)
  IMR1BASE_N0_IIO_VTD_STRUCT        Imr1LowBaseAddr;
  IMR1BASE_N1_IIO_VTD_STRUCT        Imr1HighBaseAddr;
  IMR1MASK_N0_IIO_VTD_STRUCT        Imr1LowLimit;
  IMR1MASK_N1_IIO_VTD_STRUCT        Imr1HighLimit;
  IMR1RAC_N0_IIO_VTD_STRUCT         Imr1LowRac;
  IMR1WAC_N0_IIO_VTD_STRUCT         Imr1LowWac;
  IMR0BASE_N0_IIO_VTD_STRUCT        Imr0LowBaseAddr;
  IMR0BASE_N1_IIO_VTD_STRUCT        Imr0HighBaseAddr;
  IMR0MASK_N0_IIO_VTD_STRUCT        Imr0LowLimit;
  IMR0MASK_N1_IIO_VTD_STRUCT        Imr0HighLimit;
  IMR0RAC_N1_IIO_VTD_STRUCT         Imr0HighRac;
  IMR0WAC_N1_IIO_VTD_STRUCT         Imr0HighWac;
#endif // #if !defined(SKX_HOST)
#endif  // ME_SUPPORT_FLAG

  KTI_HOST_OUT                      *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
#if defined(ME_SUPPORT_FLAG) || defined (IE_SUPPORT)

#ifdef SKX_HOST
  ZeroMem (&MeNcMemLowBaseAddr, sizeof (MeNcMemLowBaseAddr));
  ZeroMem (&MeNcMemHighBaseAddr, sizeof (MeNcMemHighBaseAddr));
  ZeroMem (&MeNcMemLowLimit, sizeof (MeNcMemLowLimit));
  ZeroMem (&MeNcMemHighLimit, sizeof (MeNcMemHighLimit));
#endif // #ifdef SKX_HOST

#if !defined(SKX_HOST)
  ZeroMem (&Imr1LowBaseAddr, sizeof (Imr1LowBaseAddr));
  ZeroMem (&Imr1HighBaseAddr, sizeof (Imr1HighBaseAddr));
  ZeroMem (&Imr1LowLimit, sizeof (Imr1LowLimit));
  ZeroMem (&Imr1HighLimit, sizeof (Imr1HighLimit));
  ZeroMem (&Imr0LowBaseAddr, sizeof (Imr0LowBaseAddr));
  ZeroMem (&Imr0HighBaseAddr, sizeof (Imr0HighBaseAddr));
  ZeroMem (&Imr0LowLimit, sizeof (Imr0LowLimit));
  ZeroMem (&Imr0HighLimit, sizeof (Imr0HighLimit));
#endif //#if !defined(SKX_HOST)

#endif // ME_SUPPORT_FLAG || IE_SUPPORT

  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if(GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  PlatformInfo  = GET_GUID_HOB_DATA(GuidHob);

  if ((*NumRanges) < MAX_RANGES) {
    return EFI_BUFFER_TOO_SMALL;
  }

  *NumRanges = 0;

  //
  // Find out which memory ranges to reserve on this platform
  //
  SmramMask = (PlatformInfo->MemData.MemTsegSize >> 17) | PEI_MR_SMRAM_CACHEABLE_MASK | PEI_MR_SMRAM_TSEG_MASK;

  //
  //  DEBUG ((EFI_D_ERROR, " SmramMask          = 0x%X\n", SmramMask));
  //
  // Generate memory ranges for the memory map.
  //
  EnableSmram = FALSE;
  MemorySize  = 0;
  Index       = 0;

  if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    UpdatePsmiTraceInfo (PlatformInfo);
  }
  //
  // Get the current "max usable memory" address from TOLUD because we will not
  // support any memory above 4Gig.  Will ignore the memory between 4G and TOUUD.
  //
  RowLength = MultU64x32 ((Host->var.mem.lowMemBase + Host->var.mem.lowMemSize), CONVERT_64MB_TO_BYTE);
  //
  // Debug code because the MRC isnt filling this data in just yet!
  //
  if (!RowLength) {
    RowLength = 512 * 1024 * 1024;
    //
    // hardcode the $%^&**$%^ to 512MB!
    //
  }
  //
  // System is very unlikely to work with less than 32MB
  //
  ASSERT (RowLength >= (32 * 1024 * 1024));

  DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "MRC: lowMemBase:%x lowMemSize:%x\n", Host->var.mem.lowMemBase, Host->var.mem.lowMemSize));
  DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "MRC: highMemBase:%x highMemSize:%x\n", Host->var.mem.highMemBase, Host->var.mem.highMemSize));


  //
  // Add memory below 640KB to the memory map. Make sure memory between
  // 640KB and 1MB are reserved, even if not used for SMRAM
  //
  MemoryMap[*NumRanges].RowNumber       = Index;
  MemoryMap[*NumRanges].PhysicalAddress = MemorySize;
  MemoryMap[*NumRanges].CpuAddress      = MemorySize;
  MemoryMap[*NumRanges].RangeLength     = 0xA0000;
  MemoryMap[*NumRanges].Type            = DualChannelDdrMainMemory;
  (*NumRanges)++;

  //
  // Reserve ABSEG or HSEG SMRAM if needed
  //
  MemoryMap[*NumRanges].PhysicalAddress = MC_ABSEG_HSEG_PHYSICAL_START;
  MemoryMap[*NumRanges].RowNumber = Index;
  if (SmramMask & (PEI_MR_SMRAM_ABSEG_MASK | PEI_MR_SMRAM_HSEG_MASK)) {
    EnableSmram = TRUE;
    MemoryMap[*NumRanges].RangeLength     = MC_ABSEG_HSEG_LENGTH;

    if (SmramMask & PEI_MR_SMRAM_ABSEG_MASK) {
      MemoryMap[*NumRanges].CpuAddress = MC_ABSEG_CPU_START;
    } else {
      MemoryMap[*NumRanges].CpuAddress = MC_HSEG_CPU_START;
    }
    //
    // Chipset only supports cacheable SMRAM
    //
    MemoryMap[*NumRanges].Type = DualChannelDdrSmramCacheable;

  } else {
    //
    // Just mark this range reserved
    //
    MemoryMap[*NumRanges].CpuAddress      = MC_ABSEG_CPU_START;
    MemoryMap[*NumRanges].RangeLength     = 0x60000;

    MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;
  }
  (*NumRanges)++;

  RowLength -= 0x100000;
  MemorySize = 0x100000;

  //
  // Add remaining memory to the memory map
  //
  if (RowLength > 0) {
    MemoryMap[*NumRanges].RowNumber       = Index;
    MemoryMap[*NumRanges].PhysicalAddress = MemorySize;
    MemoryMap[*NumRanges].CpuAddress      = MemorySize;
    MemoryMap[*NumRanges].RangeLength     = RowLength;
    MemoryMap[*NumRanges].Type            = DualChannelDdrMainMemory;
    (*NumRanges)++;
    MemorySize += RowLength;
  }

  ExtendedMemoryIndex = (UINT8) (*NumRanges - 1);

#if ME_SUPPORT_FLAG
  //
  // See if we need to trim MESEG out of the highest memory range below 4GB
  //
  PlatformInfo->MemData.MemMESEGSize = 0;
  PlatformInfo->MemData.MemMESEGBase = 0;

  if (GetMeRequestedSize () != 0) {
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "ME FW: meRequestedSize in MB: %d\n", GetMeRequestedSize ()));

    //
    // Remove MESEG DRAM range from the available memory map
    //
    MemoryMap[*NumRanges].RangeLength = (GetMeRequestedSize () * 1024 * 1024);
    MemoryMap[*NumRanges].RowNumber   = MemoryMap[ExtendedMemoryIndex].RowNumber;
    MemorySize -= MemoryMap[*NumRanges].RangeLength;
    //
    // MESEG base requires 1MB alignment.
    // Need to adjust memory map accordingly.
    //
    MeAlignment = GetMeRequestedAlignment ();
    if (MeAlignment == 0) {
      MeAlignment = 1;
    }
    AlignedMesegMask = LShiftU64 ((UINT64)MeAlignment, 20)  - 1;
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "ME FW: MeAlignment in MB: %d\n", MeAlignment));
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "ME FW: AlignedMesegMask: 0x%x\n", AlignedMesegMask));
    AlignedMesegBase = MemorySize &~(AlignedMesegMask);
    MemoryMap[*NumRanges].RangeLength += (MemorySize - AlignedMesegBase);

    MemoryMap[*NumRanges].PhysicalAddress = AlignedMesegBase;
    MemoryMap[*NumRanges].CpuAddress      = AlignedMesegBase;
    MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
    MemoryMap[*NumRanges].Type = DualChannelDdrReservedMemory;

    MesegIndex = (UINT8) (*NumRanges);
    PlatformInfo->MemData.MemMESEGBase = (UINT64)MemoryMap[MesegIndex].PhysicalAddress;
    PlatformInfo->MemData.MemMESEGSize = (UINT64)MemoryMap[MesegIndex].RangeLength;
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "MRC: PhysicalAddress:%x; RangeLength:%x\n",(UINT32)MemoryMap[MesegIndex].PhysicalAddress,MemoryMap[MesegIndex].RangeLength));

    (*NumRanges)++;
  }
#endif // ME_SUPPORT_FLAG

#if IE_SUPPORT
  //
  // See if we need to trim IESEG out of the highest memory range below 4GB
  //
  PlatformInfo->MemData.MemIESEGSize = 0;
  PlatformInfo->MemData.MemIESEGBase = 0;

  if (GetIeRequestedSize () != 0) {
    DEBUG((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "IE FW: ieRequestedSize in MB: %d\n", GetIeRequestedSize ()));

    //
    // Remove IESEG DRAM range from the available memory map
    //
    MemoryMap[*NumRanges].RangeLength = (GetIeRequestedSize () * 1024 * 1024);
    MemoryMap[*NumRanges].RowNumber   = MemoryMap[ExtendedMemoryIndex].RowNumber;
    MemorySize -= MemoryMap[*NumRanges].RangeLength;
    //
    // IESEG base requires 1MB alignment.
    // Need to adjust memory map accordingly.
    //
    IeAlignment = GetIeRequestedAlignment ();
    if (IeAlignment == 0) {
      IeAlignment = 1;
    }
    AlignedMesegMask = LShiftU64((UINT64)IeAlignment, 20)  - 1;
    AlignedMesegBase = MemorySize &~(AlignedMesegMask);
    MemoryMap[*NumRanges].RangeLength += (MemorySize - AlignedMesegBase);

    MemoryMap[*NumRanges].PhysicalAddress = AlignedMesegBase;
    MemoryMap[*NumRanges].CpuAddress      = AlignedMesegBase;
    MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
    MemoryMap[*NumRanges].Type = DualChannelDdrReservedMemory;

    IesegIndex = (UINT8) (*NumRanges);
    PlatformInfo->MemData.MemIESEGBase = (UINT64)MemoryMap[IesegIndex].PhysicalAddress;
    PlatformInfo->MemData.MemIESEGSize = (UINT64)MemoryMap[IesegIndex].RangeLength;
    DEBUG((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "MRC: PhysicalAddress:%x; RangeLength:%x\n",(UINT32)MemoryMap[IesegIndex].PhysicalAddress,MemoryMap[IesegIndex].RangeLength));

    (*NumRanges)++;
  }
#endif // IE_SUPPORT

  //
  // Reserve Memory to set it as the IMR2
  //
  if (PcdGetBool (PcdImr2Enable) == TRUE) {
    if (PcdGet64 (PcdImr2Size) != 0) {
      MemoryMap[*NumRanges].RangeLength           = (UINT64)PcdGet64 (PcdImr2Size);
      MemoryMap[*NumRanges].RowNumber             = MemoryMap[ExtendedMemoryIndex].RowNumber;
      MemorySize                                 -= MemoryMap[*NumRanges].RangeLength;
      MemoryMap[*NumRanges].PhysicalAddress       = MemorySize;
      MemoryMap[*NumRanges].CpuAddress            = MemorySize;
      MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
      MemoryMap[*NumRanges].Type                  = DualChannelDdrReservedMemory;

      Imr2BaseAddress = (UINT64)MemorySize;
      BuildGuidDataHob (
        &gImr2BaseAddressHobGuid,
        &Imr2BaseAddress,
        sizeof (EFI_PHYSICAL_ADDRESS)
        );
      DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "Reserve IMR2[0x%016lx] Size = 0x%016lx\n", MemoryMap[*NumRanges].PhysicalAddress, MemoryMap[*NumRanges].RangeLength));
      (*NumRanges)++;
    }
  }

  //
  // See if we need to trim TSEG out of the highest memory range
  //
  if (SmramMask & PEI_MR_SMRAM_TSEG_MASK) {
    //
    // Create the new range for TSEG and remove that range from the previous SdrDdrMainMemory range
    //
    TsegMask  = (SmramMask & PEI_MR_SMRAM_SIZE_MASK);

    BlockNum  = 1;
    while (TsegMask) {
      TsegMask >>= 1;
      BlockNum <<= 1;
    }

    BlockNum >>= 1;

    switch (BlockNum) {
    case PEI_MR_SMRAM_SIZE_1024K_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_2048K_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_4096K_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_8192K_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_16M_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_32M_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_64M_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_128M_MASK:
      break;

    default:
      //
      // Non supported size. Set to 0.
      //
      BlockNum = 0;
      break;
    }
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "MRC: TSEG BlockNum:%d\n",BlockNum));

    if (BlockNum) {
      EnableSmram                       = TRUE;

#if defined (ME_SUPPORT_FLAG) || defined (IE_SUPPORT)
      //
      // See if we need to build a hob for memory gap caused by SMRR alignment
      //
      SMRRSize = (BlockNum * 128 * 1024);
      DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "MRC: SMRR Size: %x\n", SMRRSize));
      MemorySize_gap = MemorySize - SMRRSize;
      AlignedTsegBase_gap = MemorySize_gap &~(SMRRSize-1);
      GapForSMRRAlign = (UINT32)(MemorySize_gap - AlignedTsegBase_gap);         // If MESeg is at top, then we possibly have SMRR alignment issue
      if( GapForSMRRAlign )
      {
        //
        // Remove SMRR alignment gap reserved range from the available memory map
        //
        MemoryMap[*NumRanges].RangeLength = GapForSMRRAlign;
        MemoryMap[*NumRanges].RowNumber   = MemoryMap[ExtendedMemoryIndex].RowNumber;
        MemorySize -= MemoryMap[*NumRanges].RangeLength;
        MemoryMap[*NumRanges].PhysicalAddress = AlignedTsegBase_gap + SMRRSize;
        MemoryMap[*NumRanges].CpuAddress      = AlignedTsegBase_gap + SMRRSize;
        MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
        MemoryMap[*NumRanges].Type = DualChannelDdrReservedMemory;

        DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "GetMemoryMap: SMRR gap reserved base  : %08X\n", (UINT32)MemoryMap[*NumRanges].PhysicalAddress));
        DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "GetMemoryMap: SMRR gap reserved length: %08X\n", (UINT32)MemoryMap[*NumRanges].RangeLength));
        (*NumRanges)++;
      } //if( GapForSMRRAlign )
#endif // ME_SUPPORT_FLAG || IE_SUPPORT

      MemoryMap[*NumRanges].RangeLength = (BlockNum * 128 * 1024);
      MemoryMap[*NumRanges].RowNumber   = MemoryMap[ExtendedMemoryIndex].RowNumber;
      MemorySize -= MemoryMap[*NumRanges].RangeLength;

      //
      // TSEG base requires TSEG-Size alignment, done at last step.
      //
//    AlignedTsegBase = MemorySize &~(0xFFFFF);
      AlignedTsegBase = MemorySize &~(MemoryMap[*NumRanges].RangeLength-1);
      MemoryMap[*NumRanges].RangeLength += (MemorySize - AlignedTsegBase);
      MemoryMap[*NumRanges].PhysicalAddress = AlignedTsegBase;
      MemoryMap[*NumRanges].CpuAddress      = AlignedTsegBase;
      //
      // Need to reserve the IED from TSEG
      //
      MemoryMap[*NumRanges].RangeLength -= PlatformInfo->MemData.MemIedSize;
      MemorySize += PlatformInfo->MemData.MemIedSize;
      MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
      AlignedIedBase = AlignedTsegBase + MemoryMap[*NumRanges].RangeLength;

    } // if (BlockNum)

    //
    // Chipset only supports cacheable SMRAM
    //
    MemoryMap[*NumRanges].Type = DualChannelDdrSmramCacheable;

    DEBUG ((EFI_D_ERROR, "GetMemoryMap: TsegBase: %08X\n", (UINT32)AlignedTsegBase));
    DEBUG ((EFI_D_ERROR, "GetMemoryMap: TsegRange: %08X\n", (UINT32)MemoryMap[*NumRanges].RangeLength));

    //
    // Program TSEG Base/Limit  in IIO
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      //
      // Continue if this socket is not present
      //
      if (!SocketPresent (Socket)) {
        continue;
      }

      for (IioIndex = 0; IioIndex < MAX_IIO_STACK; IioIndex++) {
        if (KtiHostOutPtr->stackPresentBitmap[Socket] & (1 << IioIndex)) {
          //
          // Program TSEG Base
          //
          TsegBase.Data = UsraCsrRead (Socket, IioIndex, TSEG_N0_IIO_VTD_REG);
          TsegBase.Bits.base = (UINT32) RShiftU64 (MemoryMap[*NumRanges].PhysicalAddress, 20); // In MegaBytes
          UsraCsrWrite (Socket, IioIndex, TSEG_N0_IIO_VTD_REG, TsegBase.Data);
          //
          // Program TSEG Limit
          //
          TsegLimit.Data = UsraCsrRead (Socket, IioIndex, TSEG_N1_IIO_VTD_REG);
          TsegLimit.Bits.limit = (UINT32) RShiftU64 ((MemoryMap[*NumRanges].PhysicalAddress + MemoryMap[*NumRanges].RangeLength + PlatformInfo->MemData.MemIedSize - 1), 20);
          UsraCsrWrite (Socket, IioIndex, TSEG_N1_IIO_VTD_REG, TsegLimit.Data);
        }
      }
    }
    Host->nvram.common.TsegBase.Data = UsraCsrRead (0, 0, TSEG_N0_IIO_VTD_REG);
    Host->nvram.common.TsegLimit.Data = UsraCsrRead (0, 0, TSEG_N1_IIO_VTD_REG);

    (*NumRanges)++;

    //
    // And make an entry just for IED RAM, if used
    //
    if (PlatformInfo->MemData.MemIedSize) {
      MemoryMap[*NumRanges].RangeLength = PlatformInfo->MemData.MemIedSize;
      MemoryMap[*NumRanges].RowNumber   = MemoryMap[ExtendedMemoryIndex].RowNumber;
      MemorySize -= MemoryMap[*NumRanges].RangeLength;

      MemoryMap[*NumRanges].PhysicalAddress = AlignedIedBase;
      MemoryMap[*NumRanges].CpuAddress      = AlignedIedBase;
      MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;

      MemoryMap[*NumRanges].Type = DualChannelDdrReservedMemory;

      DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "GetMemoryMap: IedRamBase: %08X\n", (UINT32)AlignedIedBase));
      DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "GetMemoryMap: IedRamRange: %08X\n", (UINT32)MemoryMap[*NumRanges].RangeLength));

      (*NumRanges)++;
    }
  }


#if defined(ME_SUPPORT_FLAG) || defined(IE_SUPPORT)
  //
  // MESEG size requested and MESEG enabled in KTI before programming ME registers
  if (GetMeRequestedSize () + GetIeRequestedSize () != 0) {
    //
    // Setup IE/ME SEG
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      //
      // Continue if this socket is not present
      //
      if (!SocketPresent (Socket)) {
        continue;
      }

      MeMemLowBaseAddr.Data = UsraCsrRead (Socket, 0, MESEG_BASE_N0_CHABC_SAD_REG);
      MeMemHighBaseAddr.Data = UsraCsrRead (Socket, 0, MESEG_BASE_N1_CHABC_SAD_REG);
      MeMemLowLimit.Data = UsraCsrRead (Socket, 0, MESEG_LIMIT_N0_CHABC_SAD_REG);
      MeMemHighLimit.Data = UsraCsrRead (Socket, 0, MESEG_LIMIT_N1_CHABC_SAD_REG);

      if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {

#ifdef SKX_HOST
        if (GetMeRequestedSize () != 0) {
          MeNcMemLowBaseAddr.Data = UsraCsrRead (Socket, 0, MENCMEM_BASE_0_IIO_VTD_REG);
          MeNcMemHighBaseAddr.Data = UsraCsrRead (Socket, 0, MENCMEM_BASE_1_IIO_VTD_REG);
          MeNcMemLowLimit.Data = UsraCsrRead (Socket, 0, MENCMEM_LIMIT_0_IIO_VTD_REG);
          MeNcMemHighLimit.Data = UsraCsrRead (Socket, 0, MENCMEM_LIMIT_1_IIO_VTD_REG);

          MeNcMemHighBaseAddr.Bits.addr = 0;
          MeNcMemHighLimit.Bits.addr = 0;
          MeNcMemLowBaseAddr.Bits.addr = (UINT32) RShiftU64 (MemoryMap[MesegIndex].PhysicalAddress, 19);
          MeNcMemLowLimit.Bits.addr = (UINT32) RShiftU64 ((MemoryMap[MesegIndex].PhysicalAddress + MemoryMap[MesegIndex].RangeLength - 1), 19);
        }
#endif // SKX_HOST

      } else {

#if !defined (SKX_HOST)
        // IMR for ME
        if (GetMeRequestedSize () != 0) {
          UINT64 imr1_base;
          UINT64 imr1_mask;

          imr1_base = MemoryMap[MesegIndex].PhysicalAddress;
          imr1_base = RShiftU64 (imr1_base, 12);
          imr1_base &= 0xFFFFFFFFFF; // Get 40 bits only
          imr1_mask = MemoryMap[MesegIndex].RangeLength - 1;
          imr1_mask = ~imr1_mask;
          imr1_mask = RShiftU64 (imr1_mask, 12);
          imr1_mask &= 0xFFFFFFFFFF; // Get 40 bits only

          Imr1LowBaseAddr.Data = UsraCsrRead (Socket, 0, IMR1BASE_N0_IIO_VTD_REG);
          Imr1HighBaseAddr.Data = UsraCsrRead (Socket, 0, IMR1BASE_N1_IIO_VTD_REG);
          Imr1LowLimit.Data = UsraCsrRead (Socket, 0, IMR1MASK_N0_IIO_VTD_REG);
          Imr1HighLimit.Data = UsraCsrRead (Socket, 0, IMR1MASK_N1_IIO_VTD_REG);

          Imr1HighBaseAddr.Bits.imr_base = (UINT16)RShiftU64 (imr1_base, 32);
          Imr1LowBaseAddr.Bits.imr_base = (UINT32)imr1_base;
          Imr1HighLimit.Bits.imr_mask = (UINT16)RShiftU64 (imr1_mask, 32);
          Imr1LowLimit.Bits.imr_mask = (UINT32)imr1_mask;

          Imr1HighBaseAddr.Bits.imr_en = 1;
        }

        // IMR for IE
        if (GetIeRequestedSize () != 0) {
          UINT64 imr0_base;
          UINT64 imr0_mask;

          imr0_base = MemoryMap[IesegIndex].PhysicalAddress;
          imr0_base = RShiftU64 (imr0_base, 12);
          imr0_base &= 0xFFFFFFFFFF; // Get 40 bits only
          imr0_mask = MemoryMap[IesegIndex].RangeLength - 1;
          imr0_mask = ~imr0_mask;
          imr0_mask = RShiftU64 (imr0_mask, 12);
          imr0_mask &= 0xFFFFFFFFFF; // Get 40 bits only

          Imr0LowBaseAddr.Data = UsraCsrRead (Socket, 0, IMR0BASE_N0_IIO_VTD_REG);
          Imr0HighBaseAddr.Data = UsraCsrRead (Socket, 0, IMR0BASE_N1_IIO_VTD_REG);
          Imr0LowLimit.Data = UsraCsrRead (Socket, 0, IMR0MASK_N0_IIO_VTD_REG);
          Imr0HighLimit.Data = UsraCsrRead (Socket, 0, IMR0MASK_N1_IIO_VTD_REG);

          Imr0HighBaseAddr.Bits.imr_base = (UINT16)RShiftU64 (imr0_base, 32);
          Imr0LowBaseAddr.Bits.imr_base = (UINT32)imr0_base;
          Imr0HighLimit.Bits.imr_mask = (UINT16)RShiftU64 (imr0_mask, 32);
          Imr0LowLimit.Bits.imr_mask = (UINT32)imr0_mask;

          Imr0HighBaseAddr.Bits.imr_en = 1;
        }
#endif // #if !defined (SKX_HOST)
      }

      MeMemHighBaseAddr.Bits.mebase = 0;
      MeMemHighLimit.Bits.melimit = 0;

      //
      // get ME base address from previous calculations
      // set base to first available address (IE or IE)
      //
#if !defined (SKX_HOST)
      if (GetIeRequestedSize () > 0) {
        MeMemLowBaseAddr.Bits.mebase = (UINT32) RShiftU64 (MemoryMap[IesegIndex].PhysicalAddress, 19);
      } else {
#endif // SKX_HOST
        if (GetMeRequestedSize () > 0) {
          MeMemLowBaseAddr.Bits.mebase = (UINT32) RShiftU64 (MemoryMap[MesegIndex].PhysicalAddress, 19);
        }
#if !defined (SKX_HOST)
      }
#endif // SKX_HOST
      //
      // get ME and IE length from previous calculations
      // omit possible gap between ME and IE area
      //
      if (GetMeRequestedSize () > 0) {
        MeMemLowLimit.Bits.melimit = (UINT32) RShiftU64 ((MemoryMap[MesegIndex].PhysicalAddress + MemoryMap[MesegIndex].RangeLength - 1), 19);
#if !defined (SKX_HOST)
      } else if (GetIeRequestedSize () > 0) {
        MeMemLowLimit.Bits.melimit = (UINT32) RShiftU64 ((MemoryMap[IesegIndex].PhysicalAddress + MemoryMap[IesegIndex].RangeLength - 1), 19);
#endif // SKX_HOST
      }
      MeMemLowLimit.Bits.en = 1;

      //
      // Program ME SEG Base/Limit in CHA
      //
      UsraCsrWrite (Socket, 0, MESEG_BASE_N0_CHABC_SAD_REG, MeMemLowBaseAddr.Data);
      UsraCsrWrite (Socket, 0, MESEG_BASE_N1_CHABC_SAD_REG, MeMemHighBaseAddr.Data);
      UsraCsrWrite (Socket, 0, MESEG_LIMIT_N0_CHABC_SAD_REG, MeMemLowLimit.Data);
      UsraCsrWrite (Socket, 0, MESEG_LIMIT_N1_CHABC_SAD_REG, MeMemHighLimit.Data);

      //
      // Program ME SEG Base/Limit in IIO
      //
      for (IioIndex = 0; IioIndex < MAX_IIO_STACK; IioIndex++) {
        if (KtiHostOutPtr->stackPresentBitmap[Socket] & (1 << IioIndex)) {
          if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
#ifdef SKX_HOST
            if (GetMeRequestedSize () != 0) {
              UsraCsrWrite (Socket, IioIndex, MENCMEM_BASE_0_IIO_VTD_REG, MeNcMemLowBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, MENCMEM_BASE_1_IIO_VTD_REG, MeNcMemHighBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, MENCMEM_LIMIT_0_IIO_VTD_REG, MeNcMemLowLimit.Data);
              UsraCsrWrite (Socket, IioIndex, MENCMEM_LIMIT_1_IIO_VTD_REG, MeNcMemHighLimit.Data);
            }
#endif // SKX_HOST
          } else {
#if !defined (SKX_HOST)
            // IMR for ME
            if (GetMeRequestedSize () != 0) {

              UsraCsrWrite (Socket, IioIndex, IMR1BASE_N0_IIO_VTD_REG, Imr1LowBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, IMR1BASE_N1_IIO_VTD_REG, Imr1HighBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, IMR1MASK_N0_IIO_VTD_REG, Imr1LowLimit.Data);
              UsraCsrWrite (Socket, IioIndex, IMR1MASK_N1_IIO_VTD_REG, Imr1HighLimit.Data);

              Imr1LowRac.Data = UsraCsrRead (Socket, IioIndex, IMR1RAC_N0_IIO_VTD_REG);
              Imr1LowWac.Data = UsraCsrRead (Socket, IioIndex, IMR1WAC_N0_IIO_VTD_REG);
              Imr1LowRac.Bits.imr1_sai_pol |= BIT16;
              Imr1LowWac.Bits.imr1_sai_pol |= BIT16;
              UsraCsrWrite (Socket, IioIndex, IMR1RAC_N0_IIO_VTD_REG, Imr1LowRac.Data);
              UsraCsrWrite (Socket, IioIndex, IMR1WAC_N0_IIO_VTD_REG, Imr1LowWac.Data);
            }

            // IMR for IE
            if (GetIeRequestedSize () != 0) {
              UsraCsrWrite (Socket, IioIndex, IMR0BASE_N0_IIO_VTD_REG, Imr0LowBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, IMR0BASE_N1_IIO_VTD_REG, Imr0HighBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, IMR0MASK_N0_IIO_VTD_REG, Imr0LowLimit.Data);
              UsraCsrWrite (Socket, IioIndex, IMR0MASK_N1_IIO_VTD_REG, Imr0HighLimit.Data);

              Imr0HighRac.Data = UsraCsrRead (Socket, IioIndex, IMR0RAC_N1_IIO_VTD_REG);
              Imr0HighWac.Data = UsraCsrRead (Socket, IioIndex, IMR0WAC_N1_IIO_VTD_REG);
              Imr0HighRac.Bits.imr0_sai_pol |= BIT25; //  BIT57
              Imr0HighWac.Bits.imr0_sai_pol |= BIT25; //  BIT57
              UsraCsrWrite (Socket, IioIndex, IMR0RAC_N1_IIO_VTD_REG, Imr0HighRac.Data);
              UsraCsrWrite (Socket, IioIndex, IMR0WAC_N1_IIO_VTD_REG, Imr0HighWac.Data);
            }
#endif // #if !defined (SKX_HOST)
          }
        }
      }
    }
    //
    //  Save CSR register values in Host->nvram struc for use on S3 resume
    //
    Host->nvram.common.MeMemLowBaseAddr.Data = UsraCsrRead (0, 0, MESEG_BASE_N0_CHABC_SAD_REG);
    Host->nvram.common.MeMemHighBaseAddr.Data = UsraCsrRead (0, 0, MESEG_BASE_N1_CHABC_SAD_REG);
    Host->nvram.common.MeMemLowLimit.Data = UsraCsrRead (0, 0, MESEG_LIMIT_N0_CHABC_SAD_REG);
    Host->nvram.common.MeMemHighLimit.Data = UsraCsrRead (0, 0, MESEG_LIMIT_N1_CHABC_SAD_REG);
    if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
#ifdef SKX_HOST
      if (GetMeRequestedSize () != 0) {
        Host->nvram.common.MeNcMemLowBaseAddr.Data = UsraCsrRead (0, 0, MENCMEM_BASE_0_IIO_VTD_REG);
        Host->nvram.common.MeNcMemHighBaseAddr.Data = UsraCsrRead (0, 0, MENCMEM_BASE_1_IIO_VTD_REG);
        Host->nvram.common.MeNcMemLowLimit.Data = UsraCsrRead (0, 0, MENCMEM_LIMIT_0_IIO_VTD_REG);
        Host->nvram.common.MeNcMemHighLimit.Data = UsraCsrRead (0, 0, MENCMEM_LIMIT_1_IIO_VTD_REG);
      }
#endif // SKX_HOST
    } else {
#if !defined (SKX_HOST)
      if (GetMeRequestedSize () != 0) {
        Host->nvram.common.MeNcMemLowBaseAddr.Data = UsraCsrRead (0, 0, IMR1BASE_N0_IIO_VTD_REG);
        Host->nvram.common.MeNcMemHighBaseAddr.Data = UsraCsrRead (0, 0, IMR1BASE_N1_IIO_VTD_REG);
        Host->nvram.common.MeNcMemLowLimit.Data = UsraCsrRead (0, 0, IMR1MASK_N0_IIO_VTD_REG);
        Host->nvram.common.MeNcMemHighLimit.Data = UsraCsrRead (0, 0, IMR1MASK_N1_IIO_VTD_REG);
        Host->nvram.common.MeNcMemLowRac.Data = UsraCsrRead (0, 0, IMR1RAC_N0_IIO_VTD_REG);
        Host->nvram.common.MeNcMemLowWac.Data = UsraCsrRead (0, 0, IMR1WAC_N0_IIO_VTD_REG);
      }
      if (GetIeRequestedSize () != 0) {
        Host->nvram.common.IeNcMemLowBaseAddr.Data = UsraCsrRead (0, 0, IMR0BASE_N0_IIO_VTD_REG);
        Host->nvram.common.IeNcMemHighBaseAddr.Data = UsraCsrRead (0, 0, IMR0BASE_N1_IIO_VTD_REG);
        Host->nvram.common.IeNcMemLowLimit.Data = UsraCsrRead (0, 0, IMR0MASK_N0_IIO_VTD_REG);
        Host->nvram.common.IeNcMemHighLimit.Data = UsraCsrRead (0, 0, IMR0MASK_N1_IIO_VTD_REG);
        Host->nvram.common.IeNcMemHighRac.Data = UsraCsrRead (0, 0, IMR0RAC_N1_IIO_VTD_REG);
        Host->nvram.common.IeNcMemHighWac.Data = UsraCsrRead (0, 0, IMR0WAC_N1_IIO_VTD_REG);
      }
#endif // #if !defined (SKX_HOST)
    }
  }
#endif // ME_SUPPORT_FLAG || IE_SUPPORT

#ifdef LT_FLAG
  if (PcdGetBool(PcdDprSizeFeatureSupport)) {
    MemorySize -= PcdGet32 (PcdLtMemSize);
  } else {
    MemorySize -= PlatformInfo->MemData.MemLtMemSize;
  }
  MemoryMap[*NumRanges].RowNumber       = MemoryMap[ExtendedMemoryIndex].RowNumber;
  MemoryMap[*NumRanges].PhysicalAddress = MemorySize &~(0xFFFFF);
  MemoryMap[*NumRanges].CpuAddress      = MemorySize &~(0xFFFFF);
  MemoryMap[*NumRanges].RangeLength     = PlatformInfo->MemData.MemLtMemSize;
  if (PcdGetBool(PcdDprSizeFeatureSupport)) {
    MemoryMap[*NumRanges].RangeLength   = PcdGet32 (PcdLtMemSize);
  }
  MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;
  MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
  PlatformInfo->MemData.MemLtMemAddress = (UINT32)(MemoryMap[*NumRanges].PhysicalAddress);
  DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "Reserve LT Mem[0x%016lx] Size = 0x%016lx\n", MemoryMap[*NumRanges].PhysicalAddress, MemoryMap[*NumRanges].RangeLength));
  (*NumRanges)++;
#endif

  //
  // Reserve Memory for DMA accesses
  //
  DEBUG ((DEBUG_INFO, "\tPcdVTdPolicyPropertyMask = 0x%08x\n", (UINT8)PcdGet8 (PcdVTdPolicyPropertyMask)));
  if (PcdGet8 (PcdVTdPolicyPropertyMask) & BIT3) {
    if (PcdGet32 (PcdVTdPeiDmaBufferSize) != 0) {
      MemoryMap[*NumRanges].RangeLength           = (UINT32)PcdGet32 (PcdVTdPeiDmaBufferSize);
      MemoryMap[*NumRanges].RowNumber             = MemoryMap[ExtendedMemoryIndex].RowNumber;
      MemorySize                                 -= MemoryMap[*NumRanges].RangeLength;
      MemoryMap[*NumRanges].PhysicalAddress       = MemorySize;
      MemoryMap[*NumRanges].CpuAddress            = MemorySize;
      MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
      MemoryMap[*NumRanges].Type                  = DualChannelDdrMainMemory;

      VtdPmrHob = BuildGuidHob (&gVtdPmrInfoDataHobGuid, sizeof(VTD_PMR_INFO_HOB));
      if (VtdPmrHob) {
        VtdPmrHob->ProtectedLowBase   = 0;
        VtdPmrHob->ProtectedLowLimit  = (UINT32)MemoryMap[*NumRanges].PhysicalAddress;
        VtdPmrHob->ProtectedHighBase  = (UINT32)(MemoryMap[*NumRanges].PhysicalAddress + MemoryMap[*NumRanges].RangeLength);
        //
        // Get the supported Physical Address Size (EAX register. EAX[7:0])
        //
        AsmCpuid (CPUID_VIR_PHY_ADDRESS_SIZE, &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
        VtdPmrHob->ProtectedHighLimit = LShiftU64 (1, (UINT8)(CpuidRegisters.RegEax & 0xFF));

        //DEBUG_CODE_BEGIN
        DEBUG ((DEBUG_INFO, "\tVtdPmrInfoDataHob HOB was created (gVtdPmrInfoDataHobGuid = %g)\n", &gVtdPmrInfoDataHobGuid));
        DEBUG ((DEBUG_INFO, "\tVtdPmrInfoDataHob->ProtectedLowBase   = 0x%08x\n",    (UINT32)VtdPmrHob->ProtectedLowBase));
        DEBUG ((DEBUG_INFO, "\tVtdPmrInfoDataHob->ProtectedLowLimit  = 0x%08x\n",    (UINT32)VtdPmrHob->ProtectedLowLimit));
        DEBUG ((DEBUG_INFO, "\tVtdPmrInfoDataHob->ProtectedHighBase  = 0x%016lx\n",  (UINT64)VtdPmrHob->ProtectedHighBase));
        DEBUG ((DEBUG_INFO, "\tVtdPmrInfoDataHob->ProtectedHighLimit = 0x%016lx\n",  (UINT64)VtdPmrHob->ProtectedHighLimit));
        //DEBUG_CODE_END
      } else {
        DEBUG ((DEBUG_ERROR, "\tVtdPmrInfoDataHob HOB was NOT created (gVtdPmrInfoDataHobGuid = %g)\n", gVtdPmrInfoDataHobGuid));
      }
      DEBUG ((EFI_D_INFO, "Reserve DMA Enabled Region[0x%016lx] Size = 0x%016lx\n", MemoryMap[*NumRanges].PhysicalAddress, MemoryMap[*NumRanges].RangeLength));
      (*NumRanges)++;
    }
  }

  //
  // Reserve BIOS Guard Memory based on the size requested if enabled
  //
  BiosGuardMemSizeMB = PlatformInfo->MemData.BiosGuardMemSize;

  if (BiosGuardMemSizeMB != 0) {

    MemoryMap[*NumRanges].RangeLength           = (UINT64) LShiftU64 (BiosGuardMemSizeMB, 20);
    MemoryMap[*NumRanges].RowNumber             = MemoryMap[ExtendedMemoryIndex].RowNumber;
    MemorySize                                 -= MemoryMap[*NumRanges].RangeLength;
    MemoryMap[*NumRanges].PhysicalAddress       = MemorySize;
    MemoryMap[*NumRanges].CpuAddress            = MemorySize;
    MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
    MemoryMap[*NumRanges].Type                  = DualChannelDdrReservedMemory;

    PlatformInfo->MemData.BiosGuardPhysBase = (UINT32)MemorySize;
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "Reserve BIOS Guard Mem[0x%08x] Size = 0x%08x\n", MemoryMap[*NumRanges].PhysicalAddress, MemoryMap[*NumRanges].RangeLength));
    (*NumRanges)++;
  }
  ASSERT((*NumRanges) <= MAX_RANGES);
  return EFI_SUCCESS;
}

/**

  Determine the memory size desired.  Based on HOB information.

  @param RequiredMemSize - pointer for reporting FSP memory size required
  @param BootLoaderTolumSize - pointer for reporting Bootloader memory size required

  @retval 0 if completed successfully

**/
EFI_STATUS
RetrieveFspRequiredMemorySize (
  OUT UINTN    *RequiredMemSize,
  OUT UINT32   *BootLoaderTolumSize
  )
{
  EFI_FSP_INFO         *FspInfo;
  EFI_HOB_GUID_TYPE    *GuidHob;

  GuidHob       = GetFirstGuidHob (&gFspInfoHobGuid);
  ASSERT (GuidHob != NULL);
  if(GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  FspInfo = GET_GUID_HOB_DATA(GuidHob);
  *RequiredMemSize = FspInfo->FspReservedMemoryLength;
  *BootLoaderTolumSize = FspInfo->FspBootLoaderTolumSize;

  return EFI_SUCCESS;
}

/**

  Determine the memory size desired.  Based on HOB memory information.

  @param size - pointer for reporting memory size required

  @retval None

**/
VOID
RetrieveRequiredMemorySize (
  UINTN    *Size
  )
{
  EFI_MEMORY_TYPE_INFORMATION *MemoryData;
  UINT8                       Index;
  UINTN                       TempPageNum;
  EFI_HOB_GUID_TYPE           *GuidHob;

  *Size         = 0;
  MemoryData    = NULL;

  GuidHob     = GetFirstGuidHob (&gEfiMemoryTypeInformationGuid);
  ASSERT (GuidHob != NULL);
  if(GuidHob == NULL) {
    return;
  }
  MemoryData  = GET_GUID_HOB_DATA(GuidHob);

  //
  // Platform PEIM should supply such a information. Generic PEIM doesn't assume any default value
  //
  if (MemoryData == NULL) {
    return;
  }

  TempPageNum = 0;
  for (Index = 0; MemoryData[Index].Type != EfiMaxMemoryType; Index++) {
    //
    // Accumulate default memory size requirements
    //
    TempPageNum += MemoryData[Index].NumberOfPages;
  }

  if (TempPageNum == 0) {
    return;
  }
  //
  // Two more pages are used by DXE memory manager
  //
  *Size = (TempPageNum + 4) * EFI_PAGE_SIZE;

  return;
}

/**

  This function Builds Memory Descriptor Hob for a
  given Range of Memory and its size.
  @param StartAddr     - Starting Address of the Memory Range
  @param MemSize       - Size of the Memory that needs to be
                         added in the Hob
  @param AttrType      - Resource type
  @param Attributes    - Additional EFI Resource Attributes
  @retval None         - VOID

**/
VOID
AddADescriptor(
   UINT64           StartAddr,
   UINT64           BlockSize,
   UINT8            AttrType,
   UINT32           Attributes
   )
{
  switch (AttrType) {
    default:
    case MEMORY_REGION_DESC:
      BuildResourceDescriptorHob(
        EFI_RESOURCE_SYSTEM_MEMORY,
        (
         EFI_RESOURCE_ATTRIBUTE_PRESENT |
         EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
         EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
         EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
         EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
         EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
         Attributes
         ),
        StartAddr,
        BlockSize
        );
      break;
    case RESERVED_REGION_DESC:
      BuildResourceDescriptorHob (
        EFI_RESOURCE_MEMORY_RESERVED,
        0,
        StartAddr,
        BlockSize
        );
      break;
    case RESERVED_SYSTEM_MEMORY_REGION_DESC:
      BuildResourceDescriptorHob (
        EFI_RESOURCE_MEMORY_RESERVED,
        (
         EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
         EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
         EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
         EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
         Attributes
         ),
        StartAddr,
        BlockSize
        );
      break;
    case MMIO_REGION_DESC:
      BuildResourceDescriptorHob (
        EFI_RESOURCE_MEMORY_MAPPED_IO,
        (
         EFI_RESOURCE_ATTRIBUTE_PRESENT |
         EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
         EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
         Attributes
         ),
        StartAddr,
        BlockSize
        );
      BuildMemoryAllocationHob (
        StartAddr,
        BlockSize,
        EfiMemoryMappedIO
        );
      break;
  }
}

/**

Routine Description: Check if a given memory range is persistent,
reserved, or volatile in the SAD. This is bit complicated as ONLY
part of the given range could be persistent and this routine need
to handle all cases and return the actual result of this check.
  @param  StartAddr    - Start Address of the Range.
  @param  *Size        - Contains the size of the Range, also
          will carry the size of the result returned.

  @retval Result      - Will contain the result of this check
          details as follows:
          MEM_RANGE_RESERVED when the given range is reserved.

          MEM_RANGE_VOLATILE_RESERVED when the given range is usable volatile
          system memory that is an part of a reserved address range.

          MEM_RANGE_PERSISTENT when the given range is persistent.

          MEM_RANGE_VOLATILE when the given range is non-mirrored volatile.

          MEM_RANGE_CTRL_REGION when the given range is a control
          region

          MEM_RANGE_MIRRORED when the given range is mirrored.

          MEM_RANGE_PART_RESERVED when only part of the Range is
          in a SAD that is reserved.

          MEM_RANGE_PART_PERSISTENT when only part of the Range is
          in a SAD that is PM.

          MEM_RANGE_PART_VOLATILE when only part of the range
          is in a SAD that is non-mirrored volatile.

          MEM_RANGE_PART_CTRL_REGION when only part of the given range
          is a control region

          MEM_RANGE_PART_BLK_WINDOW when only part of the given range
          is a block window

          MEM_RANGE_PART_MIRRORED when only part of the Range is
          in a SAD that is mirrored.

          In the cases of Result being part, new Size of this found
          will be in Size.
**/
UINT32
GetMemRangeType(
  IN     PSYSHOST          Host,
  IN     UINT64            StartAddr,
  IN OUT UINT64            *Size
  )
{
  UINT8                    Socket;
  UINT8                    SadIndex;
  UINT64                   SadStartAddr;
  UINT64                   SadLimit;
  UINT64                   BlkSize;
  UINT64                   NewSize;
  SYSTEM_MEMORY_MAP_HOB    *SystemMemoryMap;
  EFI_HOB_GUID_TYPE        *GuidHob;
  RESERVED_MEMORY_DATA_HOB *ReservedHighMemory;
  UINT64                   ReservedMemStart;
  UINT64                   ReservedMemEnd;
  UINT64                   ReservedMemSize;
  SECURITY_POLICY          *SecurityPolicy;
  UINT64                   PrmrrStart;
  UINT64                   PrmrrEnd;
  UINT64                   PrmrrSize;
  UINT8                    Count;
  SGX_PRMRR_DATA           SgxMemMapData;
  EFI_STATUS               Status;
  UINT8                    ClusterId;
  UINT8                    ClusterSadBegin;
  UINT8                    ClusterSadEnd;
  MEMORY_MAP_HOST          *MemMapHost;
  TDX_SEAMRR_DATA          TdxMemMapData;
  UINT64                   SeamrrBase;
  UINT64                   SeamrrEnd;

  BlkSize = *Size;
  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  MemMapHost = GetMemMapHostPointer ();
  if (MemMapHost == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Check the Reserved Memory Data GUID HOB. If it's present.
  // check if this range is reserved.
  //
  GuidHob = GetFirstGuidHob (&gEfiReservedMemoryDataGuid);
  if (GuidHob != NULL) {
    ReservedHighMemory = GET_GUID_HOB_DATA (GuidHob);
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (ReservedHighMemory->IsMemoryReserved[Socket]) {
        ReservedMemStart = ReservedHighMemory->ReservedMemoryBaseAddress[Socket];
        ReservedMemEnd = ReservedMemStart + ReservedHighMemory->ReservedMemorySize[Socket];
        ReservedMemSize = ReservedHighMemory->ReservedMemorySize[Socket];

        if ((StartAddr >= ReservedMemStart) && (StartAddr < ReservedMemEnd)) {
          //
          // If this range begins in a reserved memory region, mark it as reserved.
          //
          if ((StartAddr + *Size) > ReservedMemEnd) {
            //
            // The range extends beyond the end of the reserved region. Adjust the *Size
            // to notify the caller that only part of the memory range has been identified.
            //
            *Size -= ((StartAddr + *Size) - ReservedMemEnd);
          }
          return MEM_RANGE_VOLATILE_RESERVED;
        } else if ((ReservedMemStart >= StartAddr) && (ReservedMemStart < (StartAddr + *Size))) {
          //
          // If any part of the range is includes reserved memory, remove it from the range
          // and adjusts the *Size to notify the caller that the only part of the memory range
          // has been indentified
          //
          *Size -= ((StartAddr + *Size) - ReservedMemStart);
          BlkSize = *Size;
          //
          // Continue checking the rest of the region
          //
          break;
        }
      } // if (ReservedHighMemory->IsMemoryReserved[Socket])
    } // (Socket = 0; Socket < MAX_SOCKET; Socket++)
  } // GuidHob != NULL

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    SecurityPolicy = GetSecurityPolicy ();
    //
    // Check the SGX_PRMRR_DATA. If it's present.
    // check if this range is PRMRR range, if yes return MEM_RANGE_VOLATILE_RESERVED.
    //
    if (SecurityPolicy != NULL) {
      PrmrrSize = SecurityPolicy->PrmrrSize;
      if (SecurityPolicy->EnableSgx == SECURITY_POLICY_ENABLE) {
        Status = GetSgxPrmrrData (&SgxMemMapData);
        if (Status == EFI_SUCCESS) {
          for (Count = 0; Count < SgxMemMapData.PrmrrCount; Count++) {
            PrmrrStart = SgxMemMapData.PrmrrBase[Count];
            PrmrrEnd = PrmrrStart + PrmrrSize;
            if ((StartAddr >= PrmrrStart) && (StartAddr < PrmrrEnd)) {
              //
              // If this range begins in a PRMRR memory region, mark it as reserved.
              //
              if ((StartAddr + *Size) > PrmrrEnd) {
                *Size -= ((StartAddr + *Size) - PrmrrEnd);
              }
              return MEM_RANGE_VOLATILE_RESERVED;
            } else if ((PrmrrStart >= StartAddr) && (PrmrrStart < (StartAddr + *Size))) {
              //
              // If any part of the range is includes PRMRR memory, remove it from the range
              // and adjusts the *Size to notify the caller that the only part of the memory range
              // has been indentified
              //
              *Size -= ((StartAddr + *Size) - PrmrrStart);
              BlkSize = *Size;
              //
              // Continue checking the rest of the region
              //
              break;
            }
          }
        } // Status == EFI_SUCCESS
      } // SecurityPolicy->EnableSgx == SECURITY_POLICY_ENABLE

      //
      // Check the TDX_SEAMRR_DATA. If it's present.
      // check if this range is in SEAMRR range, if yes return MEM_RANGE_VOLATILE_RESERVED.
      // SEAMR size is fixed as 32M, so we only need to get the SEAMRR base from mem map
      if (SecurityPolicy->EnableTdx == SECURITY_POLICY_ENABLE) {
        Status = GetTdxSeamrrData (&TdxMemMapData);
        if (Status == EFI_SUCCESS) {
          SeamrrBase = TdxMemMapData.SeamrrBase;
          SeamrrEnd = TdxMemMapData.SeamrrBase + 0x2000000;
          if ((StartAddr >= SeamrrBase) && (StartAddr < SeamrrEnd)) {
            if ((StartAddr + *Size) > SeamrrEnd) {
              *Size -= ((StartAddr + *Size) - SeamrrEnd);
            }
            return MEM_RANGE_VOLATILE_RESERVED;
          } else if ((SeamrrBase >= StartAddr) && (SeamrrBase < (StartAddr + *Size))) {
            *Size -= ((StartAddr + *Size) - SeamrrBase);
            BlkSize = *Size;
          }
        } // Status == EFI_SUCCESS
      } // SecurityPolicy->EnableTdx == SECURITY_POLICY_ENABLE
    } // SecurityPolicy != NULL
  } // (IsCpuAndRevision (CPU_SPRSP, REV_ALL))

  //
  // Check thru all of the SADs of all sockets and see if this range exists,
  // and if so, check and see if this range is persistent
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SystemMemoryMap->Socket[Socket].SAD[0].Enable == 0) {
      continue;
    }
    //
    // Traverse thru all SAD entries to build resource HOBs
    //
    for (ClusterId = 0; ClusterId < MemMapHost->NumberofChaDramClusters; ClusterId++) {
      ClusterSadBegin = ClusterId * MAX_SAD_RULES;
      ClusterSadEnd = ClusterSadBegin + MAX_SAD_RULES;

      for (SadIndex = ClusterSadBegin; SadIndex < ClusterSadEnd; SadIndex++) {
        if (SystemMemoryMap->Socket[Socket].SAD[SadIndex].Enable == 0) {
          continue;
        }
        if (SystemMemoryMap->Socket[Socket].SAD[SadIndex].local == 0 &&
          SystemMemoryMap->Socket[Socket].SAD[SadIndex].type != MemTypeNxm) {
          continue;
        }

        SadStartAddr = LShiftU64 (SystemMemoryMap->Socket[Socket].SAD[SadIndex].Base, 26);
        SadLimit = LShiftU64 (SystemMemoryMap->Socket[Socket].SAD[SadIndex].Limit, 26);

        if ((StartAddr >= SadStartAddr) && (StartAddr < SadLimit)) {
          if (((IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) && (SystemMemoryMap->Socket[Socket].SAD[SadIndex].Attr == SAD_NXM_ATTR)) ||
              ((!(IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL))) && (SystemMemoryMap->Socket[Socket].SAD[SadIndex].type == MemTypeNxm)) ||
              IsMemTypeCxl (SystemMemoryMap->Socket[Socket].SAD[SadIndex].type) ||
              IsMemTypeReserved (SystemMemoryMap->Socket[Socket].SAD[SadIndex].type)) {

            // Current SAD type is Reserved
            if ((StartAddr + BlkSize) <= SadLimit) {
              return MEM_RANGE_RESERVED;
            } else {
              // Possibly spawns over multiple SADS
              // Continue scan recursively
              BlkSize -= SadLimit - StartAddr;
              NewSize = BlkSize;
              if (GetMemRangeType (Host, SadLimit, &NewSize) == MEM_RANGE_RESERVED) {
                return MEM_RANGE_RESERVED;
              } else {
                // Adjust region size
                *Size -= BlkSize;

                return MEM_RANGE_PART_RESERVED;
              }
            }
          } else if (SystemMemoryMap->Socket[Socket].SAD[SadIndex].type == MemType1lmCtrl) {
            // Current SAD type is CTRL REGION
            if ((StartAddr + BlkSize) <= SadLimit) {
              return MEM_RANGE_CTRL_REGION;
            } else {
              // Possibly spawns over multiple SADS
              // Continue scan recursively
              BlkSize -= SadLimit - StartAddr;
              NewSize = BlkSize;
              if (GetMemRangeType (Host, SadLimit, &NewSize) == MEM_RANGE_CTRL_REGION) {
                return MEM_RANGE_CTRL_REGION;
              } else {
                // Adjust region size
                *Size -= BlkSize;

                return MEM_RANGE_PART_CTRL_REGION;
              }
            }
          } else if (IsMemTypeAppDirect (SystemMemoryMap->Socket[Socket].SAD[SadIndex].type)) {
            // Current SAD type is Persistent
            if ((StartAddr + BlkSize) <= SadLimit) {
              return MEM_RANGE_PERSISTENT;
            } else {
              // Possibly spawns over multiple SADS
              // Continue scan recursively
              BlkSize -= SadLimit - StartAddr;
              NewSize = BlkSize;
              if (GetMemRangeType (Host, SadLimit, &NewSize) == MEM_RANGE_PERSISTENT) {
                return MEM_RANGE_PERSISTENT;
              } else {
                // Adjust region size
                *Size -= BlkSize;

                return MEM_RANGE_PART_PERSISTENT;
              }
            }
          } else if (SystemMemoryMap->Socket[Socket].SAD[SadIndex].mirrored == 1) {
            // Current SAD type is mirrored volatile
            if ((StartAddr + BlkSize) <= SadLimit) {
              return MEM_RANGE_MIRRORED;
            } else {
              // Possibly spawns over multiple SADS
              // Continue scan recursively
              BlkSize -= SadLimit - StartAddr;
              NewSize = BlkSize;
              if (GetMemRangeType (Host, SadLimit, &NewSize) == MEM_RANGE_MIRRORED) {
                return MEM_RANGE_MIRRORED;
              } else {
                // Adjust region size
                *Size -= BlkSize;

                return MEM_RANGE_PART_MIRRORED;
              }
            }
          } else {
            // Current SAD Type is non-mirrored volatile
            if ((StartAddr + BlkSize) <= SadLimit) {
              return MEM_RANGE_VOLATILE;
            } else {
              // Possibly spawns over mulltiple SADs
              // Continue scan recursively
              BlkSize -= SadLimit - StartAddr;
              NewSize = BlkSize;
              if (GetMemRangeType (Host, SadLimit, &NewSize) == MEM_RANGE_VOLATILE) {
                return MEM_RANGE_VOLATILE;
              } else {
                // Adjust region size
                *Size -= BlkSize;

                return  MEM_RANGE_PART_VOLATILE;
              }
            }
          }
        } // if ((StartAddr >= SadStartAddr) && (StartAddr < SadLimit)) {
      } // for (SadIndex = ClusterSadBegin; SadIndex < ClusterSadEnd; SadIndex++) {
    } // for (ClusterId = 0; ClusterId < MemMapHost->NumberofChaDramClusters; ClusterId++) {
  } // for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
  //
  // If this gets here, then the range is not found in the SAD.
  return MEM_RANGE_INVALID;
}

/**

  This function Build Persistent Memory Descriptor Hobs for a
  given Range of Memory and its size. Also this function handles
  the cases for both Persistent Memory Types, Reserved Memory,
  and Non-Persistent Memory types

  @param[in] Host        - Pointer to SysHost
  @param StartAddr       - Starting Address of the Memory Range
  @param MemSize         - Size of the Memory that needs to be
                           added in the Hob
  @param Attributes      - Additional EFI Resource Attributes
  @retval               None
  @retval

**/
VOID
BuildResourceDescriptorHobs (
  IN PSYSHOST            Host,
  IN UINT64              StartAddr,
  IN UINT64              MemSize,
  IN UINT32              Attributes
  )
{
  UINT64            BlockSize;
  UINT32            Result;

  BlockSize = MemSize;
  Result = GetMemRangeType (Host, StartAddr, &BlockSize);

  // Create descriptor for initial range
  if (Result == MEM_RANGE_VOLATILE_RESERVED) {
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "Creating Reserved System Memory Resource Descriptor: StartAddr: 0x%016lx  Size: 0x%016lx\n", StartAddr, BlockSize));
    AddADescriptor(StartAddr, BlockSize, RESERVED_SYSTEM_MEMORY_REGION_DESC, Attributes);
  } else if ((Result == MEM_RANGE_RESERVED) || (Result == MEM_RANGE_PART_RESERVED)) {
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "Creating Reserved Resource Descriptor:               StartAddr: 0x%016lx  Size: 0x%016lx\n", StartAddr, BlockSize));
    AddADescriptor(StartAddr, BlockSize, RESERVED_REGION_DESC, Attributes);
  } else if ((Result == MEM_RANGE_MIRRORED) || (Result == MEM_RANGE_PART_MIRRORED)) {
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "Creating Mirrored System Memory Resource Descriptor: StartAddr: 0x%016lx  Size: 0x%016lx\n", StartAddr, BlockSize));
    AddADescriptor(StartAddr, BlockSize, MEMORY_REGION_DESC, Attributes | EFI_RESOURCE_ATTRIBUTE_MORE_RELIABLE);
  } else if ((Result == MEM_RANGE_PERSISTENT) || (Result == MEM_RANGE_PART_PERSISTENT)) {
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "Creating PMEM Resource Descriptor:                   StartAddr: 0x%016lx  Size: 0x%016lx\n", StartAddr, BlockSize));
    AddADescriptor(StartAddr, BlockSize, MEMORY_REGION_DESC, Attributes | PMEM_DESCRIPTOR_ATTRIBUTES);
  } else if ((Result == MEM_RANGE_CTRL_REGION) || (Result == MEM_RANGE_PART_CTRL_REGION)) {
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "Creating UC MMIO Resource Descriptor:                StartAddr: 0x%016lx  Size: 0x%016lx\n", StartAddr, BlockSize));
    AddADescriptor(StartAddr, BlockSize, MMIO_REGION_DESC, Attributes);
  } else {
    DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "Creating System Memory Resource Descriptor:          StartAddr: 0x%016lx  Size: 0x%016lx\n", StartAddr, BlockSize));
    AddADescriptor(StartAddr, BlockSize, MEMORY_REGION_DESC, Attributes);
  }

  if (BlockSize < MemSize) {
    // Handle the remainder of the memory range
    BuildResourceDescriptorHobs (Host, StartAddr + BlockSize, MemSize - BlockSize, Attributes);
  }
}

/**

  This function Build Persistent Reserved Memory Descriptor Hobs
  with owner GUIDs for a given Range of Memory and its size.

  @param PhysicalStart  - Starting Address of the Memory Range
  @param NumberOfBytes  - Size of the Memory that needs to be
                          added in the Hob
  @param OwnerGuid      - The owning GUID for the Hob
  @param SetAttribute   - Boolean to determine value of Attribute
  @retval               None
  @retval

**/
VOID
BuildFspResourceDescriptorHob(
  IN EFI_PHYSICAL_ADDRESS    PhysicalStart,
  IN UINT64                  NumberOfBytes,
  IN EFI_GUID                *OwnerGuid
  )
{
  EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttribute;

  ResourceAttribute = (
      EFI_RESOURCE_ATTRIBUTE_PRESENT |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_TESTED |
      EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
      );

  DEBUG ((EFI_D_ERROR, " Creating Reserved System Memory Resource Descriptor: StartAddr: 0x%016lx  Size: 0x%016lx\n", PhysicalStart, NumberOfBytes));
  BuildResourceDescriptorWithOwnerHob (
            EFI_RESOURCE_MEMORY_RESERVED,             // MemoryType,
            ResourceAttribute,
            PhysicalStart,
            NumberOfBytes,
            OwnerGuid
            );
}

/**
  This function builds block decoder resource descriptor HOBs for the whole system
**/
VOID
BuildBlockDecoderResourceDescriptorHobs (
  VOID
  )
{
  SYSTEM_MEMORY_MAP_HOB           *SystemMemoryMap;
  MEMORY_MAP_BLOCK_DECODER_DATA   *BlockDecoder;
  EFI_PHYSICAL_ADDRESS            StartAddr;
  UINT64                          Length;

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    ASSERT (FALSE);
    return;
  }
  BlockDecoder = &SystemMemoryMap->BlockDecoderData;
  if (BlockDecoder->BlockSocketEnable != 0) {
    StartAddr = LShiftU64 (BlockDecoder->BlockDecoderBase, 26); // Convert 64MB to byte
    Length = LShiftU64 (BlockDecoder->BlockDecoderLimit, 26) - StartAddr; // Convert 64MB to byte
    DEBUG ((EFI_D_ERROR, "Creating Block Decoder Resource Descriptor:          StartAddr: 0x%016lx  Size: 0x%016lx\n", StartAddr, Length));
    BuildResourceDescriptorHob (
      EFI_RESOURCE_MEMORY_MAPPED_IO,
      EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE,
      StartAddr,
      Length
    );
    BuildMemoryAllocationHob (
      StartAddr,
      Length,
      EfiMemoryMappedIO
    );
  }
}

/**

  Find memory that is reserved so PEI has some to use.

  @param[in] Host  - Pointer to SysHost

  @retval 0 - Success
  @retval Other - failure

**/

EFI_STATUS
EFIAPI
InstallS3Memory (
  IN PSYSHOST Host
  )
{
  EFI_STATUS                  Status;
  UINTN                       BufferSize;
  UINT8                       Index;
  UINT8                       IioIndex;
  UINT8                       NumRanges;
  UINT8                       SmramIndex;
  UINT8                       SmramRanges;
  EFI_PHYSICAL_ADDRESS        TopUseableMemStart;
  EFI_PHYSICAL_ADDRESS        TopUseableMemSize;
  EFI_PHYSICAL_ADDRESS        LowMemSize;
  UINTN                       S3MemoryBase;
  UINTN                       S3MemorySize;
  EFI_PEI_HOB_POINTERS        Hob;
  RESERVED_ACPI_S3_RANGE      *S3MemoryRangeData;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK        *SmramHobDescriptorBlock;
  PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE MemoryMap[MAX_RANGES];
  EFI_SMRAM_DESCRIPTOR        DescriptorAcpiVariable;
  UINT8                       Socket;
  KTI_HOST_OUT *KtiHostOutPtr;
  EFI_PHYSICAL_ADDRESS        RelocateAddr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  DEBUG ((EFI_D_ERROR, "InstallS3Memory()\n"));

  //
  // Get the Memory Map and calculate TSEG place
  //
  NumRanges = MAX_RANGES;
  ZeroMem (MemoryMap, sizeof (PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE) * NumRanges);
  Status = GetMemoryMap (Host, (PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE *) MemoryMap, &NumRanges);
  ASSERT_EFI_ERROR (Status);

  //
  // Get how manage SMRAM desc
  //
  SmramRanges = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    if  ((MemoryMap[Index].Type == DualChannelDdrSmramCacheable) ||
          (MemoryMap[Index].Type == DualChannelDdrSmramNonCacheable)
          ) {
      SmramRanges++;
    }
  }
  BufferSize = sizeof (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK);

  if (SmramRanges > 0) {
    BufferSize += ((SmramRanges - 1) * sizeof (EFI_SMRAM_DESCRIPTOR));
  }

  Hob.Raw = BuildGuidHob (
              &gEfiSmmPeiSmramMemoryReserveGuid,
              BufferSize
                              );
  ASSERT (Hob.Raw);

  SmramHobDescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *) (Hob.Raw);
  SmramHobDescriptorBlock->NumberOfSmmReservedRegions = SmramRanges;

  SmramIndex = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    if ((MemoryMap[Index].Type == DualChannelDdrSmramCacheable) ||
        (MemoryMap[Index].Type == DualChannelDdrSmramNonCacheable)
        ) {
      //
      // This is an SMRAM range, create an SMRAM descriptor
      //
      SmramHobDescriptorBlock->Descriptor[SmramIndex].PhysicalStart = MemoryMap[Index].PhysicalAddress;
      SmramHobDescriptorBlock->Descriptor[SmramIndex].CpuStart      = MemoryMap[Index].CpuAddress;
      SmramHobDescriptorBlock->Descriptor[SmramIndex].PhysicalSize  = MemoryMap[Index].RangeLength;
      if (MemoryMap[Index].Type == DualChannelDdrSmramCacheable) {
        SmramHobDescriptorBlock->Descriptor[SmramIndex].RegionState = EFI_SMRAM_CLOSED | EFI_CACHEABLE;
      } else {
        SmramHobDescriptorBlock->Descriptor[SmramIndex].RegionState = EFI_SMRAM_CLOSED;
      }

      SmramIndex++;
    }
  }

  //
  // Build a HOB with the location of the reserved memory range.
  //
  CopyMem (&DescriptorAcpiVariable, &SmramHobDescriptorBlock->Descriptor[SmramRanges-1], sizeof(EFI_SMRAM_DESCRIPTOR));
  DescriptorAcpiVariable.CpuStart += RESERVED_CPU_S3_SAVE_OFFSET;

  BuildGuidDataHob (
            &gEfiAcpiVariableGuid,
            &DescriptorAcpiVariable,
            sizeof (EFI_SMRAM_DESCRIPTOR)
            );

  DEBUG ((DEBUG_INFO, "Installs3Memory: After Hob build.\n"));

  //
  // Get the location and size of the S3 memory range in the reserved page and
  // install it as PEI Memory.
  //

  DEBUG ((EFI_D_ERROR, "TSEG Base = 0x%08x\n", SmramHobDescriptorBlock->Descriptor[SmramRanges-1].PhysicalStart));
  S3MemoryRangeData = (RESERVED_ACPI_S3_RANGE*)(UINTN)
    (SmramHobDescriptorBlock->Descriptor[SmramRanges-1].PhysicalStart + RESERVED_ACPI_S3_RANGE_OFFSET);

  S3MemoryBase  = (UINTN) (S3MemoryRangeData->AcpiReservedMemoryBase);
  DEBUG ((EFI_D_ERROR, "S3MemoryBase = 0x%08x\n", S3MemoryBase));
  S3MemorySize  = (UINTN) (S3MemoryRangeData->AcpiReservedMemorySize);
  DEBUG ((EFI_D_ERROR, "S3MemorySise = 0x%08x\n", S3MemorySize));

  DEBUG ((DEBUG_INFO, "Installs3Memory: S3MemoryBase: %x, S3MemorySize: %x.\n", S3MemoryBase, S3MemorySize));

  //
  // Program TSEG Base/Limit in IIO
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (Socket)) {
      continue;
    }

    for (IioIndex = 0; IioIndex < MAX_IIO_STACK; IioIndex++) {
      if (KtiHostOutPtr->stackPresentBitmap[Socket] & (1 << IioIndex)) {
          //
          // Program TSEG Base/Limit
          //
          UsraCsrWrite (Socket, IioIndex, TSEG_N0_IIO_VTD_REG, Host->nvram.common.TsegBase.Data);
          UsraCsrWrite (Socket, IioIndex, TSEG_N1_IIO_VTD_REG, Host->nvram.common.TsegLimit.Data);
      }
    }
  }

#if defined(ME_SUPPORT_FLAG) || defined(IE_SUPPORT)
  //
  // MESEG size requested and MESEG enabled in KTI before programming ME registers
  if (GetMeRequestedSize () + GetIeRequestedSize () != 0) {
    //
    // Setup ME SEG
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      //
      // Continue if this socket is not present
      //
      if (!SocketPresent (Socket)) {
        continue;
      }

      //
      // Program IE/ME SEG Base/Limit in CHA
      //
      UsraCsrWrite (Socket, 0, MESEG_BASE_N0_CHABC_SAD_REG, Host->nvram.common.MeMemLowBaseAddr.Data);
      UsraCsrWrite (Socket, 0, MESEG_BASE_N1_CHABC_SAD_REG, Host->nvram.common.MeMemHighBaseAddr.Data);
      UsraCsrWrite (Socket, 0, MESEG_LIMIT_N0_CHABC_SAD_REG, Host->nvram.common.MeMemLowLimit.Data);
      UsraCsrWrite (Socket, 0, MESEG_LIMIT_N1_CHABC_SAD_REG, Host->nvram.common.MeMemHighLimit.Data);

      //
      // Program ME SEG Base/Limit in IIO
      //
      for (IioIndex = 0; IioIndex < MAX_IIO_STACK; IioIndex++) {
        if (KtiHostOutPtr->stackPresentBitmap[Socket] & (1 << IioIndex)) {
          if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
#ifdef SKX_HOST
            if (GetMeRequestedSize () != 0) {
              UsraCsrWrite (Socket, IioIndex, MENCMEM_BASE_0_IIO_VTD_REG, Host->nvram.common.MeNcMemLowBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, MENCMEM_BASE_1_IIO_VTD_REG, Host->nvram.common.MeNcMemHighBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, MENCMEM_LIMIT_0_IIO_VTD_REG, Host->nvram.common.MeNcMemLowLimit.Data);
              UsraCsrWrite (Socket, IioIndex, MENCMEM_LIMIT_1_IIO_VTD_REG, Host->nvram.common.MeNcMemHighLimit.Data);
            }
#endif // SKX_HOST
          } else {
#if !defined(SKX_HOST)
            if (GetMeRequestedSize () != 0) {

              UsraCsrWrite (Socket, IioIndex, IMR1BASE_N0_IIO_VTD_REG, Host->nvram.common.MeNcMemLowBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, IMR1BASE_N1_IIO_VTD_REG, Host->nvram.common.MeNcMemHighBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, IMR1MASK_N0_IIO_VTD_REG, Host->nvram.common.MeNcMemLowLimit.Data);
              UsraCsrWrite (Socket, IioIndex, IMR1MASK_N1_IIO_VTD_REG, Host->nvram.common.MeNcMemHighLimit.Data);
              // Restore RAC/WAC registers
              UsraCsrWrite (Socket, IioIndex, IMR1RAC_N0_IIO_VTD_REG, Host->nvram.common.MeNcMemLowRac.Data);
              UsraCsrWrite (Socket, IioIndex, IMR1WAC_N0_IIO_VTD_REG, Host->nvram.common.MeNcMemLowWac.Data);
            }
            if (GetIeRequestedSize () != 0) {
              UsraCsrWrite (Socket, IioIndex, IMR0BASE_N0_IIO_VTD_REG, Host->nvram.common.IeNcMemLowBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, IMR0BASE_N1_IIO_VTD_REG, Host->nvram.common.IeNcMemHighBaseAddr.Data);
              UsraCsrWrite (Socket, IioIndex, IMR0MASK_N0_IIO_VTD_REG, Host->nvram.common.IeNcMemLowLimit.Data);
              UsraCsrWrite (Socket, IioIndex, IMR0MASK_N1_IIO_VTD_REG, Host->nvram.common.IeNcMemHighLimit.Data);
              // Restore RAC/WAC registers
              UsraCsrWrite (Socket, IioIndex, IMR0RAC_N1_IIO_VTD_REG, Host->nvram.common.IeNcMemHighRac.Data);
              UsraCsrWrite (Socket, IioIndex, IMR0WAC_N1_IIO_VTD_REG, Host->nvram.common.IeNcMemHighWac.Data);
            }
#endif // #if !defined(SKX_HOST)
          }
        }
      }
    }
  }
#endif // ME_SUPPORT_FLAG

  S3MemorySize  = S3MemorySize - EFI_PAGE_SIZE;
  Status        = PeiServicesInstallPeiMemory (S3MemoryBase, S3MemorySize);

  DEBUG ((DEBUG_INFO, "Installs3Memory: After InstallPeiMemory. \n"));

  ASSERT_EFI_ERROR (Status);

  //
  // Build Hob for PBSB NEM Tear Down
  //
  RelocateAddr = S3MemoryBase + S3MemorySize;
  BuildGuidDataHob (&gPbspTearDownNemAddressHobGuid, &RelocateAddr, sizeof(EFI_PHYSICAL_ADDRESS));

  //
  // Retrieve the system memory length and build memory hob for the system
  // memory above 1MB. So Memory Callback can set cache for the system memory
  // correctly on S3 boot path, just like it does on Normal boot path.
  //
  LowMemSize = (EFI_PHYSICAL_ADDRESS)MultU64x32 (Host->var.mem.lowMemSize, CONVERT_64MB_TO_BYTE);

  ASSERT ((S3MemoryRangeData->SystemMemoryLength - 0x100000) > 0);
  BuildResourceDescriptorHob (
            EFI_RESOURCE_SYSTEM_MEMORY,
            (
            EFI_RESOURCE_ATTRIBUTE_PRESENT |
            EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
            EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
            EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
            EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
            EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
            ),
            0x100000,
            (S3MemoryRangeData->SystemMemoryLength - LowMemSize) - 0x100000
            );

  for (Index = 0; Index < NumRanges; Index++) {
    if ((MemoryMap[Index].Type == DualChannelDdrMainMemory) &&
        (MemoryMap[Index].PhysicalAddress + MemoryMap[Index].RangeLength < 0x100000)) {
      BuildResourceDescriptorHob (
                EFI_RESOURCE_SYSTEM_MEMORY,
                (
                EFI_RESOURCE_ATTRIBUTE_PRESENT |
                EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
                ),
                MemoryMap[Index].PhysicalAddress,
                MemoryMap[Index].RangeLength
                );
      DEBUG ((EFI_D_INFO, "Build resource HOB for Legacy Region on S3 patch :"));
      DEBUG ((EFI_D_INFO, " Memory Base:0x%lX Length:0x%lX\n", MemoryMap[Index].PhysicalAddress, MemoryMap[Index].RangeLength));
    }
  }

  //
  // Get the current "Top of Upper Usable Memory" address from TOUUD.
  // If TOUUD > 4G, it means memory re-mapped.
  //
  TopUseableMemSize = (EFI_PHYSICAL_ADDRESS) MultU64x32 (Host->var.mem.highMemBase, CONVERT_64MB_TO_BYTE) + (EFI_PHYSICAL_ADDRESS) MultU64x32 (Host->var.mem.highMemSize, CONVERT_64MB_TO_BYTE) - MEM_EQU_4GB;
  TopUseableMemStart = MEM_EQU_4GB;

  //Add descriptor hob for memory above 4GB
  if (TopUseableMemSize > 0) {
    //
    // Create hob for memory above 4G memory.
    //
    BuildResourceDescriptorHob (
              EFI_RESOURCE_SYSTEM_MEMORY,
              (
              EFI_RESOURCE_ATTRIBUTE_PRESENT |
              EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
              EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
              EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
              EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
              EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
              ),
              TopUseableMemStart,
              TopUseableMemSize
              );
  }

  return EFI_SUCCESS;
}

/**

  This function installs memory.

  @param[in] Host  - Pointer to SysHost

  @retval 0 - success
  @retval Other -  failure

**/
EFI_STATUS
EFIAPI
InstallEfiMemory (
  IN PSYSHOST Host
  )
{
  EFI_STATUS                            Status;
  UINT8                                 Index;
  UINT8                                 NumRanges;
  UINT8                                 SmramIndex;
  UINT8                                 SmramRanges;
  UINT64                                PeiMemoryLength;
  UINTN                                 BufferSize;
  UINTN                                 PeiMemoryIndex;
  UINTN                                 RequiredMemSize;
  EFI_PEI_HOB_POINTERS                  Hob;
  EFI_PHYSICAL_ADDRESS                  PeiMemoryBaseAddress;
  EFI_PHYSICAL_ADDRESS                  TopUseableMemStart;
  EFI_PHYSICAL_ADDRESS                  TopUseableMemSize;
  EFI_PHYSICAL_ADDRESS                  FpgaMemBase;
  EFI_PHYSICAL_ADDRESS                  FpgaMemSize;
  PEI_MEMORY_TEST_OP                    MemoryTestOp;
  PEI_BASE_MEMORY_TEST_PPI              *BaseMemoryTestPpi;
  PEI_PLATFORM_MEMORY_SIZE_PPI          *MemSize;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK        *SmramHobDescriptorBlock;
  PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE MemoryMap[MAX_RANGES];
  EFI_PHYSICAL_ADDRESS                  BadMemoryAddress;
  EFI_BOOT_MODE                         BootMode;
  EFI_PEI_SERVICES                      **gPeiServices;
  EFI_SMRAM_DESCRIPTOR                  DescriptorAcpiVariable;
  UINT32                                NormalMemSize64 = 0;
  UINT32                                NVMemSize64 = 0;
  EFI_PHYSICAL_ADDRESS                  NormalMemSize=0;
  EFI_PHYSICAL_ADDRESS                  NVMemSize = 0;
  EFI_PHYSICAL_ADDRESS                  TotIntMemSize = 0;
  EFI_PHYSICAL_ADDRESS                  TotalMemSizeAbove4G = 0;
  UINT8                                 sckt = 0;
  UINT8                                 NumSockets;
  SYS_SETUP                             *Setup;
  EFI_PHYSICAL_ADDRESS                  RelocateAddr;
#if FixedPcdGetBool(PcdFspBinaryEnable) == 1
  UINT32                                BootLoaderTolumSize;
#endif
  MEMORY_MAP_HOST                       *MemMapHost;

// APTIOV_SERVER_OVERRIDE_RC_START : To support Capsule update in EGS
#if Capsule_SUPPORT
  PEI_CAPSULE_PPI                       *Capsule;
  VOID                                  *CapsuleBuffer;
  UINTN                                 CapsuleBufferLength;
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : To support Capsule update in EGS
  Setup = GetSysSetupPointer ();
  MemMapHost = GetMemMapHostPointer ();

  gPeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();

  DEBUG ((EFI_D_ERROR, "InstallEfiMemory()\n"));

  if (Host->var.mem.subBootMode == AdrResume) {
    // Change Boot Mode to skip the memory init.
    BootMode = BOOT_WITH_MINIMAL_CONFIGURATION;
  } else {
    //
    // Determine boot mode
    //
    Status = PeiServicesGetBootMode (&BootMode);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Get the Memory Map
  //
  NumRanges = MAX_RANGES;
  ZeroMem ((UINT8 *) MemoryMap, sizeof (PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE) * NumRanges);
  Status = GetMemoryMap (Host, (PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE *) MemoryMap, &NumRanges);
  ASSERT_EFI_ERROR (Status);

  //
  // Find the highest memory range in processor native address space to give to
  // PEI. Then take the top.
  // If this algorithm changes, then we need to fix the capsule memory
  // selection algorithm below.
  //
  PeiMemoryBaseAddress = 0;

  //
  // Query the platform for the minimum memory size
  //
  Status = PeiServicesLocatePpi (&gPeiPlatformMemorySizePpiGuid, 0, NULL, &MemSize);
  ASSERT_EFI_ERROR (Status);

  Status = MemSize->GetPlatformMemorySize (gPeiServices, MemSize, &PeiMemoryLength);
  ASSERT_EFI_ERROR (Status);

  //
  // Get required memory size for ACPI use. This helps to put ACPI memory on the top
  //
  RequiredMemSize = 0;
  // APTIOV_SERVER_OVERRIDE_RC_START : WA.to disallow platform code to subtract the BS memory from platform memory.
  //RetrieveRequiredMemorySize (host, &RequiredMemSize);
  // APTIOV_SERVER_OVERRIDE_RC_END : WA.to disallow platform code to subtract the BS memory from platform memory.
  DEBUG ((EFI_D_ERROR, " RequiredMemSize for ACPI = 0x%X bytes\n", RequiredMemSize));

#if FixedPcdGetBool(PcdFspBinaryEnable)  == 1
  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) {
    Status = RetrieveFspRequiredMemorySize (&RequiredMemSize, &BootLoaderTolumSize);
    ASSERT_EFI_ERROR (Status);
    DEBUG ((EFI_D_ERROR, "FSP RequiredMemSize        = 0x%X bytes\n", RequiredMemSize));
    DEBUG ((EFI_D_ERROR, "Bootloader RequiredMemSize = 0x%X bytes\n", BootLoaderTolumSize));

    if ((RequiredMemSize + BootLoaderTolumSize) > PeiMemoryLength) {
      DEBUG ((DEBUG_ERROR, "Error: BootLoaderTolumSize Greater than available memory.\n"));
      DEBUG ((DEBUG_ERROR, "Force BootLoaderTolumSize to zero.\n"));
      BootLoaderTolumSize = 0;
    }
  } // PcdFspBinaryEnable
#endif

  PeiMemoryIndex = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    DEBUG ((EFI_D_ERROR, "Found 0x%016lX bytes at ", MemoryMap[Index].RangeLength));
    DEBUG ((EFI_D_ERROR, "0x%016lX\n", MemoryMap[Index].PhysicalAddress));
    if ((MemoryMap[Index].Type == DualChannelDdrMainMemory) &&
        (MemoryMap[Index].PhysicalAddress + MemoryMap[Index].RangeLength < MAX_ADDRESS) &&
        (MemoryMap[Index].PhysicalAddress >= PeiMemoryBaseAddress) &&
        (MemoryMap[Index].RangeLength >= PeiMemoryLength)
        ) {
      if (PcdGetBool (PcdPeiMemoryBaseAddressDefault)){
        PeiMemoryBaseAddress  = MemoryMap[Index].PhysicalAddress + MemoryMap[Index].RangeLength - PeiMemoryLength;
      } else {
        PeiMemoryBaseAddress  = MemoryMap[Index].PhysicalAddress;
      }
      PeiMemoryIndex        = Index;
    }
  }
  //
  // Test only the PEI memory if necessary. Some platforms do not require the
  // Base Memory PEIM to be present
  //
  BaseMemoryTestPpi = NULL;
  Status = PeiServicesLocatePpi (&gPeiBaseMemoryTestPpiGuid, 0, NULL, &BaseMemoryTestPpi);

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR,  "Base Memory Test PPI is not available\n"));
  }

  switch (BootMode) {

  case BOOT_WITH_FULL_CONFIGURATION:
    MemoryTestOp = Quick;
    break;

  case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
    MemoryTestOp = Extensive;
    break;

  default:
    MemoryTestOp = Ignore;
    break;
  }

  if (BaseMemoryTestPpi != NULL) {
    Status = BaseMemoryTestPpi->BaseMemoryTest (
                                  gPeiServices,
                                  BaseMemoryTestPpi,
                                  PeiMemoryBaseAddress,
                                  PeiMemoryLength,
                                  MemoryTestOp,
                                  &BadMemoryAddress
                                  );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "Memory test failure at 0x%016lX.\n", BadMemoryAddress));
    }

    ASSERT_EFI_ERROR (Status);
  }

// APTIOV_SERVER_OVERRIDE_RC_START : To support Capsule update in EGS
#if Capsule_SUPPORT
    Capsule             = NULL;
    CapsuleBuffer       = NULL;
    CapsuleBufferLength = 0;

    if (BootMode == BOOT_ON_FLASH_UPDATE) {
      Status = PeiServicesLocatePpi (
                      &gEfiPeiCapsulePpiGuid,
                      0,
                      NULL,
                      (VOID **) &Capsule
                      );
      ASSERT_EFI_ERROR (Status);

      if (Status == EFI_SUCCESS) {
        //
        // Find the largest memory range excluding that given to PEI.
        //
        for (Index = 0; Index < NumRanges; Index++) {
          if ((MemoryMap[Index].Type == DualChannelDdrMainMemory) &&
            (MemoryMap[Index].PhysicalAddress + MemoryMap[Index].RangeLength < MAX_ADDRESS)) {
            if (Index != PeiMemoryIndex) {
              if (MemoryMap[Index].RangeLength > CapsuleBufferLength) {
                CapsuleBuffer       = (VOID *) ((UINTN) MemoryMap[Index].PhysicalAddress);
                CapsuleBufferLength = (UINTN) MemoryMap[Index].RangeLength;
              }
            } else {
              if ((MemoryMap[Index].RangeLength - PeiMemoryLength) >= CapsuleBufferLength) {
                CapsuleBuffer       = (VOID *) ((UINTN) MemoryMap[Index].PhysicalAddress);
                CapsuleBufferLength = (UINTN) (MemoryMap[Index].RangeLength - PeiMemoryLength);
              }
            }
          }
        }
        //
        // Call the Capsule PPI Coalesce function to coalesce the capsule data.
        //
        Status = Capsule->Coalesce ((EFI_PEI_SERVICES **)gPeiServices, &CapsuleBuffer, &CapsuleBufferLength);
      }
      //
      // If it failed, then NULL out our capsule PPI pointer so that the capsule
      // HOB does not get created below.
      //
      if (Status != EFI_SUCCESS) {
        Capsule = NULL;
      }
    }
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : To support Capsule update in EGS

  //
  // Carve out the top memory reserved for ACPI
  //
#if FixedPcdGetBool(PcdFspBinaryEnable) == 1
  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) {
    // Carve out the top 1 PAGE from Pei Memory for PBSP NEM tear down
    RequiredMemSize = RequiredMemSize - EFI_PAGE_SIZE;
    RelocateAddr = PeiMemoryBaseAddress + PeiMemoryLength - BootLoaderTolumSize;
    Status = PeiServicesInstallPeiMemory (PeiMemoryBaseAddress + PeiMemoryLength - BootLoaderTolumSize - RequiredMemSize, RequiredMemSize);
  } else {
#endif
    // Carve out the top 1 PAGE from Pei Memory for PBSP NEM tear down
    RequiredMemSize = RequiredMemSize + EFI_PAGE_SIZE;
    RelocateAddr = PeiMemoryBaseAddress + PeiMemoryLength - RequiredMemSize;
    Status = PeiServicesInstallPeiMemory (PeiMemoryBaseAddress, PeiMemoryLength - RequiredMemSize);
#if FixedPcdGetBool(PcdFspBinaryEnable) == 1
  }
#endif
  ASSERT_EFI_ERROR (Status);

  //
  // Build Hob for PBSB NEM Tear Down
  //
  BuildGuidDataHob (&gPbspTearDownNemAddressHobGuid, &RelocateAddr, sizeof(EFI_PHYSICAL_ADDRESS));

  //
  // Save MemoryMap Data into Hob
  //
  DEBUG ((EFI_D_ERROR, "Save MemoryMap data into Hob\n"));
  SaveSystemMemoryMapData ();

  DEBUG ((EFI_D_ERROR, "Building RESOURCE_SYSTEM_MEMORY Hob:\n"));
  DEBUG ((
    EFI_D_ERROR,
    " PeiMemoryBaseAddress = 0x%lX, PeiMemoryLength = 0x%lX\n",
    PeiMemoryBaseAddress,
    PeiMemoryLength)
    );

#if FixedPcdGetBool(PcdFspBinaryEnable) == 1
  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) {
    // Build HOB that describes at BootLoader TOLUM Resource reserved at top of main memory below 4GB.
// APTIOV_SERVER_OVERRIDE_RC_START: Guids not available in source declaration files
      /*
    DEBUG ((DEBUG_INFO, "FSP_BOOT_LOADER_TOLUM_HOB\n"));
    BuildFspResourceDescriptorHob (
                PeiMemoryBaseAddress + PeiMemoryLength - BootLoaderTolumSize,
                BootLoaderTolumSize,
                &gFspBootLoaderTolumHobGuid
              );

    // HOB that describes FSP reserved memory below BootLoader TOLUM.
    DEBUG ((DEBUG_INFO, "FSP_RESERVED_MEMORY_RESOURCE_HOB\n"));
    BuildFspResourceDescriptorHob (
              PeiMemoryBaseAddress + PeiMemoryLength - BootLoaderTolumSize - RequiredMemSize,
              RequiredMemSize,
              &gFspReservedMemoryResourceHobGuid
              );
              */
// APTIOV_SERVER_OVERRIDE_RC_START: Guids not available in source declaration files
  } else {
#endif
    if (PeiMemoryBaseAddress != 0) {
      BuildResourceDescriptorHobs (Host, PeiMemoryBaseAddress, PeiMemoryLength, EFI_RESOURCE_ATTRIBUTE_TESTED);
    }
#if FixedPcdGetBool(PcdFspBinaryEnable) == 1
  } // PcdFspBinaryEnable
#endif

  //
  // Install physical memory descriptor hobs for each memory range.
  //
  SmramRanges = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    if (MemoryMap[Index].Type == DualChannelDdrMainMemory) {

      if (Index == PeiMemoryIndex) {
        //
        // This is a partially tested Main Memory range, give it to EFI
        //
#if FixedPcdGetBool(PcdFspBinaryEnable) == 1
        if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) {
          BuildResourceDescriptorHobs (Host,
                                      MemoryMap[Index].PhysicalAddress,
                                      MemoryMap[Index].RangeLength - BootLoaderTolumSize - RequiredMemSize,
                                      EFI_RESOURCE_ATTRIBUTE_TESTED);
          DEBUG ((EFI_D_ERROR, "PEI- Partially tested Main Memory Base:0x%016lX Length:0x%016lX End::0x%016lX \n",MemoryMap[Index].PhysicalAddress,MemoryMap[Index].RangeLength - BootLoaderTolumSize - RequiredMemSize, MemoryMap[Index].PhysicalAddress+MemoryMap[Index].RangeLength - BootLoaderTolumSize - RequiredMemSize));

          //
          // Test this memory range
          //
          if (BaseMemoryTestPpi != NULL) {
            Status = BaseMemoryTestPpi->BaseMemoryTest (
                                          gPeiServices,
                                          BaseMemoryTestPpi,
                                          MemoryMap[Index].PhysicalAddress,
                                          MemoryMap[Index].RangeLength - BootLoaderTolumSize - RequiredMemSize,
                                          MemoryTestOp,
                                          &BadMemoryAddress
                                          );
          }
        } else {
#endif
          BuildResourceDescriptorHobs (Host,
                                      PcdGetBool (PcdPeiMemoryBaseAddressDefault) ? MemoryMap[Index].PhysicalAddress : MemoryMap[Index].PhysicalAddress + PeiMemoryLength,
                                      MemoryMap[Index].RangeLength - PeiMemoryLength,
                                      EFI_RESOURCE_ATTRIBUTE_TESTED);
          DEBUG ((EFI_D_ERROR, "PEI- Partially tested Main Memory Base:0x%016lX Length:0x%016lX End::0x%016lX \n",MemoryMap[Index].PhysicalAddress,MemoryMap[Index].RangeLength - PeiMemoryLength, MemoryMap[Index].PhysicalAddress+MemoryMap[Index].RangeLength - PeiMemoryLength));

          //
          // Test this memory range
          //
          if (BaseMemoryTestPpi != NULL) {
            Status = BaseMemoryTestPpi->BaseMemoryTest (
                                          gPeiServices,
                                          BaseMemoryTestPpi,
                                          MemoryMap[Index].PhysicalAddress,
                                          MemoryMap[Index].RangeLength - PeiMemoryLength,
                                          MemoryTestOp,
                                          &BadMemoryAddress
                                          );
          }
#if FixedPcdGetBool(PcdFspBinaryEnable) == 1
        } // PcdFspBinaryEnable
#endif
        ASSERT_EFI_ERROR (Status);
      } else {
		// APTIOV_SERVER_OVERRIDE_RC_START : memory range 0-9FFFF is not available in the GetMemoryMap() service
	     EFI_RESOURCE_ATTRIBUTE_TYPE ResourceAttribute;
		 if((MemoryMap[Index].PhysicalAddress==0)&&(MemoryMap[Index].RangeLength==0xa0000)) {
		     ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_TESTED;
		 } else {
		     ResourceAttribute = 0x00;
		 }
        // APTIOV_SERVER_OVERRIDE_RC_END : memory range 0-9FFFF is not available in the GetMemoryMap() service 
        //
        // This is an untested Main Memory range, give it to EFI
        //
		// APTIOV_SERVER_OVERRIDE_RC_START : memory range 0-9FFFF is not available in the GetMemoryMap() service
        BuildResourceDescriptorHobs (Host, MemoryMap[Index].PhysicalAddress, MemoryMap[Index].RangeLength, 0|ResourceAttribute);
        // APTIOV_SERVER_OVERRIDE_RC_END : memory range 0-9FFFF is not available in the GetMemoryMap() service
        DEBUG ((EFI_D_ERROR, "Untested Main Memory range Base:0x%016lX Length:0x%016lX End:0x%016lX  \n",MemoryMap[Index].PhysicalAddress,MemoryMap[Index].RangeLength, MemoryMap[Index].PhysicalAddress+MemoryMap[Index].RangeLength));

        //
        // Test this memory range
        //
        if (BaseMemoryTestPpi != NULL) {
          Status = BaseMemoryTestPpi->BaseMemoryTest (
                                        gPeiServices,
                                        BaseMemoryTestPpi,
                                        MemoryMap[Index].PhysicalAddress,
                                        MemoryMap[Index].RangeLength,
                                        MemoryTestOp,
                                        &BadMemoryAddress
                                        );
          ASSERT_EFI_ERROR (Status);
        }
      }
    } else {
      if ((MemoryMap[Index].Type == DualChannelDdrSmramCacheable) ||
          (MemoryMap[Index].Type == DualChannelDdrSmramNonCacheable)
          ) {
        SmramRanges++;
      }
      //
      // Make sure non-system memory is marked as reserved
      //
      BuildResourceDescriptorHob (
                EFI_RESOURCE_MEMORY_RESERVED,     // MemoryType,
                0,                                // MemoryAttribute
                MemoryMap[Index].PhysicalAddress, // MemoryBegin
                MemoryMap[Index].RangeLength      // MemoryLength
                );
    }
  }

  BufferSize = sizeof (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK);
  if (SmramRanges > 0) {
    BufferSize += ((SmramRanges - 1) * sizeof (EFI_SMRAM_DESCRIPTOR));
  }

  Hob.Raw = BuildGuidHob (
              &gEfiSmmPeiSmramMemoryReserveGuid,
              BufferSize
              );
  ASSERT (Hob.Raw);

  SmramHobDescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *) (Hob.Raw);
  SmramHobDescriptorBlock->NumberOfSmmReservedRegions = SmramRanges;

  SmramIndex = 0;

  for (Index = 0; Index < NumRanges; Index++) {

    if ((MemoryMap[Index].Type == DualChannelDdrSmramCacheable) ||
        (MemoryMap[Index].Type == DualChannelDdrSmramNonCacheable)
        ) {
      //
      // This is an SMRAM range, create an SMRAM descriptor
      //
      SmramHobDescriptorBlock->Descriptor[SmramIndex].PhysicalStart = MemoryMap[Index].PhysicalAddress;
      SmramHobDescriptorBlock->Descriptor[SmramIndex].CpuStart      = MemoryMap[Index].CpuAddress;
      SmramHobDescriptorBlock->Descriptor[SmramIndex].PhysicalSize  = MemoryMap[Index].RangeLength;

      if (MemoryMap[Index].Type == DualChannelDdrSmramCacheable) {
        SmramHobDescriptorBlock->Descriptor[SmramIndex].RegionState = EFI_SMRAM_CLOSED | EFI_CACHEABLE;
      } else {
        SmramHobDescriptorBlock->Descriptor[SmramIndex].RegionState = EFI_SMRAM_CLOSED;
      }
      SmramIndex++;
    }
  }

  //
  // Build a HOB with the location of the reserved memory range.
  //
  CopyMem (&DescriptorAcpiVariable, &SmramHobDescriptorBlock->Descriptor[SmramRanges-1], sizeof(EFI_SMRAM_DESCRIPTOR));
  DescriptorAcpiVariable.CpuStart += RESERVED_CPU_S3_SAVE_OFFSET;
  BuildGuidDataHob (
    &gEfiAcpiVariableGuid,
    &DescriptorAcpiVariable,
    sizeof (EFI_SMRAM_DESCRIPTOR)
    );

  //
  // Get the current "Top of Upper Usable Memory" address from TOUUD.
  // If TOUUD > 4G, it means memory re-mapped.
  //
  TopUseableMemSize = (EFI_PHYSICAL_ADDRESS) MultU64x32 (Host->var.mem.highMemBase, CONVERT_64MB_TO_BYTE) + (EFI_PHYSICAL_ADDRESS) MultU64x32 (Host->var.mem.highMemSize, CONVERT_64MB_TO_BYTE) - MEM_EQU_4GB;
  TopUseableMemStart = MEM_EQU_4GB;

  DEBUG ((EFI_D_ERROR, "TOHM:0x%016lX\n", TopUseableMemStart + TopUseableMemSize));

  // Check for FPGA memory and remove from TOHM
  if (MemMapHost->FpgaMemCnt) {
    UINT8 Rindex;
    DEBUG ((EFI_D_ERROR, "Found FPGA Memory\n"));
    for (Rindex = 0; Rindex < MemMapHost->FpgaMemCnt; Rindex++) {
      if (MemMapHost->FpgaRsadRules[Rindex].Attr != SAD_FPGA_ATTR) {
        continue;
      }
      FpgaMemBase = (EFI_PHYSICAL_ADDRESS) MultU64x32 (MemMapHost->FpgaRsadRules[Rindex].Base, CONVERT_64MB_TO_BYTE);
      FpgaMemSize = ((EFI_PHYSICAL_ADDRESS) MultU64x32 (MemMapHost->FpgaRsadRules[Rindex].Limit, CONVERT_64MB_TO_BYTE)) - (EFI_PHYSICAL_ADDRESS)FpgaMemBase;
      DEBUG ((EFI_D_ERROR, "Removing Skt-%d FPGA Memory from TOHM of size=0x%lx\n", MemMapHost->FpgaRsadRules[Rindex].NodeId, FpgaMemSize));

      TopUseableMemSize = (EFI_PHYSICAL_ADDRESS)TopUseableMemSize - (EFI_PHYSICAL_ADDRESS)FpgaMemSize;
    }
    DEBUG ((EFI_D_ERROR, "TOHM Without FPGA:0x%016lX\n", TopUseableMemSize));
  }

  NumSockets = 0;
  for (sckt = 0; sckt < MAX_SOCKET; sckt++) {
    if (Host->nvram.mem.socket[sckt].enabled && Host->var.mem.socket[sckt].memSize) {
      NumSockets += 1;
    }
  }

  // Calculate the Non volatile memory behind each socket
  if ((PcdGetBool (PcdNvDimmEn)) && (Host->var.mem.NVmemSize)) {
    DEBUG ((EFI_D_ERROR, "Calculating ADR Memory regions\n"));
    TotalMemSizeAbove4G = TopUseableMemSize;
    for (sckt = 0; sckt < MAX_SOCKET; sckt++ ) {
      if (!(Setup->mem.options & NUMA_AWARE) && (NumSockets > 1)) {
        NormalMemSize64 = Host->var.mem.TotalInterleavedMemSize;
        NVMemSize64 = Host->var.mem.TotalInterleavedNVMemSize;
        DEBUG ((EFI_D_ERROR, "NormalMemSize64(Host->var.mem.socket[%d].TotalInterleavedMemSize) = 0x%x\n", sckt, NormalMemSize64));
        DEBUG ((EFI_D_ERROR, "NVMemSize64(Host->var.mem.socket[%d].TotalInterleavedNVMemSize) = 0x%x\n", sckt, NVMemSize64));
      } else if (Host->nvram.mem.socket[sckt].enabled == 0 || Host->var.mem.socket[sckt].memSize == 0) {
        continue;
      } else {
        NormalMemSize64 = Host->var.mem.socket[sckt].TotalInterleavedMemSize;
        NVMemSize64 = Host->var.mem.socket[sckt].TotalInterleavedNVMemSize;
        DEBUG ((EFI_D_ERROR, "NormalMemSize64(Host->var.mem.socket[%d].TotalInterleavedMemSize) = 0x%x\n", sckt, NormalMemSize64));
        DEBUG ((EFI_D_ERROR, "NVMemSize64(Host->var.mem.socket[%d].TotalInterleavedNVMemSize) = 0x%x\n", sckt, NVMemSize64));
      }

      NormalMemSize = (EFI_PHYSICAL_ADDRESS)MultU64x32 (NormalMemSize64, CONVERT_64MB_TO_BYTE);
      NVMemSize = (EFI_PHYSICAL_ADDRESS)MultU64x32 (NVMemSize64, CONVERT_64MB_TO_BYTE);
      TotIntMemSize += (NormalMemSize + NVMemSize);

      //
      // Adjust NormalMemSize if necessary
      //
      if ((sckt == 0) && (Host->var.mem.TotalInterleavedMemSize == 0) && (Setup->mem.MinNormalMemSize > 2)) {
        NormalMemSize = (EFI_PHYSICAL_ADDRESS)MultU64x32 (Setup->mem.MinNormalMemSize - 2, 1024 * 1024 * 1024);
        NVMemSize -= NormalMemSize;
      }

      DEBUG ((EFI_D_ERROR, "Before TOHM Adjust: NormalMemSize = 0x%016lX, NVMemSize = 0x%016lX, TotIntMemSize = 0x%016lx\n", NormalMemSize, NVMemSize, TotIntMemSize));

      // Cap the maximum memory to TopUseableMem
      if (TotIntMemSize > TotalMemSizeAbove4G) {
        if (NVMemSize) {
          // NVMem present in the system(or this socket in case of NUMA)
          NVMemSize -= (TotIntMemSize - TotalMemSizeAbove4G);
        } else {//  This socket does not have NVMEM so reduce normal mem.
          NormalMemSize -= (TotIntMemSize - TotalMemSizeAbove4G);
        }
      }
      DEBUG ((EFI_D_ERROR, "After TOHM Adjust: NormalMemSize = 0x%016lX, NVMemSize = 0x%016lX\n", NormalMemSize, NVMemSize));

      //
      // Create hob for the normal memory above 4G memory.
      //
      if (NormalMemSize) {
        BuildResourceDescriptorHob (
          EFI_RESOURCE_SYSTEM_MEMORY,
          (
          EFI_RESOURCE_ATTRIBUTE_PRESENT |
          EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
          EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
          ),
          TopUseableMemStart,
          NormalMemSize
          );
        DEBUG ((EFI_D_ERROR, "Just added 0x%lx of System memory at =0x%lx\n", NormalMemSize, TopUseableMemStart));
        TopUseableMemStart += NormalMemSize;
      }

      //
      // Create hob for NV Memory
      //
      if (NVMemSize) {
        BuildResourceDescriptorHob (
          EFI_RESOURCE_SYSTEM_MEMORY,
          EFI_RESOURCE_ATTRIBUTE_PERSISTENT | EFI_RESOURCE_ATTRIBUTE_PERSISTABLE,
          TopUseableMemStart,
          NVMemSize
          );
        DEBUG ((EFI_D_ERROR, "Just added 0x%lx of Persistent (ADR) memory at =0x%lx\n", NVMemSize, TopUseableMemStart));
        TopUseableMemStart += NVMemSize;
      }
      if ( !(Setup->mem.options & NUMA_AWARE)) {
        break;
      }
    }
  } else {
    //Add descriptor hob for memory above 4GB
    if (TopUseableMemSize > 0) {
      //
      // Create hob for memory above 4G memory.
      //
      BuildResourceDescriptorHobs (Host, TopUseableMemStart, TopUseableMemSize, 0);
    }
  }

  BuildBlockDecoderResourceDescriptorHobs ();
// APTIOV_SERVER_OVERRIDE_RC_START : To support Capsule update in EGS
#if Capsule_SUPPORT
  //
  // If we found the capsule PPI (and we didn't have errors), then
  // call the capsule PEIM to allocate memory for the capsule.
  //
  if (Capsule != NULL) {
    Status = Capsule->CreateState ((EFI_PEI_SERVICES **)gPeiServices, CapsuleBuffer, CapsuleBufferLength);
  }
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : To support Capsule update in EGS

  // Create memory descriptors for FPGA memory
  if (MemMapHost->FpgaMemCnt) {
    UINT8 Rindex;
    for (Rindex = 0; Rindex < MemMapHost->FpgaMemCnt; Rindex++) {
      if (MemMapHost->FpgaRsadRules[Rindex].Attr != SAD_FPGA_ATTR) {
        continue;
      }
      FpgaMemBase = (EFI_PHYSICAL_ADDRESS) MultU64x32 (MemMapHost->FpgaRsadRules[Rindex].Base, CONVERT_64MB_TO_BYTE);
      FpgaMemSize = ((EFI_PHYSICAL_ADDRESS) MultU64x32 (MemMapHost->FpgaRsadRules[Rindex].Limit, CONVERT_64MB_TO_BYTE)) - (EFI_PHYSICAL_ADDRESS)FpgaMemBase;

      DEBUG ((DEBUG_GCD|DEBUG_CACHE|DEBUG_INFO, "Creating System Memory Resource Descriptor(FPGA):          StartAddr: 0x%016lx  Size: 0x%016lx\n", FpgaMemBase, FpgaMemSize));
      AddADescriptor(FpgaMemBase, FpgaMemSize, MEMORY_REGION_DESC, 0);
    }
  }
  return EFI_SUCCESS;
}

