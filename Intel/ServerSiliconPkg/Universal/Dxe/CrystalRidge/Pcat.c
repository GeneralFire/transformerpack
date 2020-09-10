/** @file
  Pcat.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#include "Pcat.h"
#include "CrystalRidge.h"
#include "Nfit.h"
#include <Protocol/PcatProtocol.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <UncoreCommonIncludes.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/KtiApi.h>
#include <MemDecodeCommonIncludes.h>


/*****************************************************************************
 * Definitions
 *****************************************************************************/
// APTIOV_SERVER_OVERRIDE_RC_START : Suppressed PCAT debug messages in Release mode
#ifdef EFI_DEBUG
#define PCATDEBUG_ENABLED 1
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Suppressed PCAT debug messages in Release mode

#if PCATDEBUG_ENABLED
#define PCATDEBUG(Expr) CRDEBUG(Expr)
#else
#define PCATDEBUG(Expr)
#endif

/// @brief Calculate a new address, basing on some address and a byte offset
#define PTR_OFFSET(PTR, OFFSET) ((VOID *) ((UINT8 *) PTR + OFFSET))

/// @brief GUID for SSKU Attribute Extension Table
static const EFI_GUID SSKU_ATTRIBUTE_EXTENSION_TABLE = {
  0xf93032e5,
  0xb045,
  0x40ef,
  { 0x91, 0xc8, 0xf0, 0x2b, 0x6, 0xad, 0x94, 0x8d }
};

/// @brief Address of memory allocated for PCAT table
PNVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE mPcatTablePtr = NULL;
/// @brief Max size of PCAT table (size of allocated memory)
UINT64 mPcatTableSize = 0;

/// @brief List of protocol functions
EFI_ACPI_PCAT_PROTOCOL mAcpiPcat;


/**
  Check if PCAT is already initialized

  PCAT is initialized when:
  -memory buffer for PCAT is allocated
  -allocated memory buffer size is non-zero
  -table header contains valid PCAT signature

  @param        None

  @retval TRUE  PCAT already initialized
  @retval FALSE PCAT not initialized
*/
BOOLEAN
EFIAPI
IsInitialized (
  VOID
  )
{
  if (NULL == mPcatTablePtr) {
    return FALSE;
  }

  if (0 == mPcatTableSize) {
    return FALSE;
  }

  if (mPcatTablePtr->Signature != NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_SIGNATURE) {
    return FALSE;
  }

  return TRUE;
}

/**

  Get the requested DDR Cache Size in units of 1 GiB

  The requested DDR Cache Size is always a power of 2 and is calculated using the formula:
  2^(33 + DdrCacheSize). This is then converted from Byte to GiB Granularity by right shifting
  by 30.

  @param[in] CacheSizeCode  Encoded setup value

  @return The requested DDR Cache Size in units of 1 GiB

**/
UINT32
PcatDecodeDdrCacheSize (
  UINT8 CacheSizeCode
  )
{
  switch (CacheSizeCode) {
    case DDR_ENCODED_CACHE_SIZE_8GB:   return PCAT_DDR_CACHE_SIZE_8GB;
    case DDR_ENCODED_CACHE_SIZE_16GB:  return PCAT_DDR_CACHE_SIZE_16GB;
    case DDR_ENCODED_CACHE_SIZE_32GB:  return PCAT_DDR_CACHE_SIZE_32GB;
    case DDR_ENCODED_CACHE_SIZE_64GB:  return PCAT_DDR_CACHE_SIZE_64GB;
    case DDR_ENCODED_CACHE_SIZE_128GB: return PCAT_DDR_CACHE_SIZE_128GB;
    case DDR_ENCODED_CACHE_SIZE_256GB: return PCAT_DDR_CACHE_SIZE_256GB;

    default: return 0;
  }
}

/**

  This routine adds Platform Capability Information Table to PCAT.

  @param None

  @retval None

**/
VOID
AddPlatCapInfoTableToPCAT (
  VOID
  )
{
  UINT64                     PcatAppendPtr;
  PPLAT_CAPABILITIES_INFO    pPlatCapInfo;
  MAX_INT_SET                InterleaveInfo;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Building structure of type %d (Platform Capability Information)\n",
    TYPE_PLAT_CAP_INFO_TABLE));

  // Get the current end of PCAT so we can append the Platform Capabilities Info Table
  PcatAppendPtr = (UINT64) mPcatTablePtr + mPcatTablePtr->Length;
  pPlatCapInfo = (PPLAT_CAPABILITIES_INFO) PcatAppendPtr;
  pPlatCapInfo->Type = TYPE_PLAT_CAP_INFO_TABLE;
  pPlatCapInfo->Length = sizeof (PLAT_CAPABILITIES_INFO);

  // BIOS doesn't support changing config thru Mgmt S/W and doesn't support RT Mgmt change request
  pPlatCapInfo->DcpmmMgmtSwConfigInput = (mSystemMemoryMap->DfxMemVar.dfxDimmManagement == DIMM_MGMT_CR_MGMT_DRIVER);

  //
  // BIOS supports the following memory modes
  //
  if ((mMemMapHost->PcatCapability.MemModeCapabilities & MODE_1LM) != 0) {
    pPlatCapInfo->MemModeCap = PLAT_CAP_MEM_MODE_CAP_1LM;
  }

  if ((mMemMapHost->PcatCapability.MemModeCapabilities & MODE_2LM) != 0) {
    pPlatCapInfo->MemModeCap |= PLAT_CAP_MEM_MODE_CAP_2LM;
  }

  if ((mMemMapHost->PcatCapability.MemModeCapabilities & MODE_AD) != 0) {
    pPlatCapInfo->MemModeCap |= PLAT_CAP_MEM_MODE_CAP_AD;
  }

  if ((mMemMapHost->PcatCapability.MemModeCapabilities & MODE_NATIVE_ADWB) != 0) {
    pPlatCapInfo->MemModeCap |= PLAT_CAP_MEM_MODE_CAP_ADWB;
  }

  switch (mSystemMemoryMap->volMemMode) {
  case VOL_MEM_MODE_1LM:
    pPlatCapInfo->CurrentMemMode = PLAT_CAP_CURRENT_VOL_MEM_MODE_1LM;
    break;

  case VOL_MEM_MODE_2LM:
    if (mSystemMemoryMap->CacheMemType == CACHE_TYPE_DDR_CACHE_PMEM) {
      pPlatCapInfo->CurrentMemMode = PLAT_CAP_CURRENT_VOL_MEM_MODE_2LM;
    } else {
      pPlatCapInfo->CurrentMemMode = PLAT_CAP_CURRENT_VOL_MEM_MODE_1LM;
    }
    break;
  case VOL_MEM_MODE_MIX_1LM2LM:
    pPlatCapInfo->CurrentMemMode = PLAT_CAP_CURRENT_VOL_MEM_MODE_MIX_1LM2LM;
    break;
  }

  if ((mSystemMemoryMap->DfxMemVar.dfxPerMemMode != PER_MEM_MODE) ||
      (mSystemMemoryMap->DfxMemVar.dfxDimmManagement != DIMM_MGMT_CR_MGMT_DRIVER)) {
    pPlatCapInfo->CurrentMemMode |= (PLAT_CAP_CURRENT_PER_MEM_MODE_NONE << 2);
  } else {
    if ((mMemMapHost->PcatCapability.MemModeCapabilities & (MODE_AD | MODE_NATIVE_ADWB)) == (MODE_AD | MODE_NATIVE_ADWB)) {
      pPlatCapInfo->CurrentMemMode |= (PLAT_CAP_CURRENT_PER_MEM_MODE_AD_OR_ADWB << 2);
    } else if ((mMemMapHost->PcatCapability.MemModeCapabilities & MODE_NATIVE_ADWB) != 0) {
      pPlatCapInfo->CurrentMemMode |= (PLAT_CAP_CURRENT_PER_MEM_MODE_ADWB << 2);
    } else {
      pPlatCapInfo->CurrentMemMode |= (PLAT_CAP_CURRENT_PER_MEM_MODE_AD << 2);
    }
  }

  //
  // Allowed volatile memory mode.
  //
  if ((mSystemMemoryMap->MemSetup.volMemMode == VOL_MEM_MODE_2LM) && (mSystemMemoryMap->CacheMemType == CACHE_TYPE_DDR_CACHE_PMEM)) {
    if (mSystemMemoryMap->MemSetup.DdrCacheSize == DDR_ENCODED_CACHE_SIZE_ALL) {
      pPlatCapInfo->CurrentMemMode |= (PLAT_CAP_CURRENT_ALLOW_VOL_MEM_MODE_1LM_OR_2LM << 4);
    } else {
      pPlatCapInfo->CurrentMemMode |= (PLAT_CAP_CURRENT_ALLOW_VOL_MEM_MODE_MIX_1LM2LM << 4);
    }
  } else {
    pPlatCapInfo->CurrentMemMode |= (PLAT_CAP_CURRENT_ALLOW_VOL_MEM_MODE_1LM << 4);
  }

  //
  // Maximum number of interleave sets of any memory type:
  //
  InterleaveInfo = GetMaxInterleaveSet ();
  pPlatCapInfo->MaxPmemIntSet.Data = InterleaveInfo.Data;

  //
  // DdrCacheSize: Capacity in GiB per channel for use as near memory cache in 1LM+2LM
  //
  if (mSystemMemoryMap->volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
    pPlatCapInfo->DdrCacheSize = PcatDecodeDdrCacheSize (mSystemMemoryMap->MemSetup.DdrCacheSize);
  } else {
    pPlatCapInfo->DdrCacheSize = 0;
  }

  //
  // Cache Capabilities
  //
  if (mPcatTablePtr->Revision >= NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_3_00_HDR) {
    pPlatCapInfo->Data.CacheCapabilities = (UINT8) (mMemMapHost->PcatCapability.MmCacheCapabilities | (mMemMapHost->PcatCapability.AdWbCacheCapabilities << 4));
  }

  //
  // Update the size of the PCAT to include this table
  //
  mPcatTablePtr->Length += sizeof (PLAT_CAPABILITIES_INFO);

  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Type:                               %d\n", pPlatCapInfo->Type));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Length:                             %d\n", pPlatCapInfo->Length));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Management SW Config Input Support: 0x%02X\n", pPlatCapInfo->DcpmmMgmtSwConfigInput));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Memory Mode Capabilities:           0x%02X\n", pPlatCapInfo->MemModeCap));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Current Memory Mode:                0x%02X\n", pPlatCapInfo->CurrentMemMode));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Maximum PM Interleave Sets:         0x%02X\n", pPlatCapInfo->MaxPmemIntSet.Data));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "  Per CPU Die:                      0x%d\n", pPlatCapInfo->MaxPmemIntSet.Bits.PerCpuDie));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "  Per PMem:                         0x%d\n", pPlatCapInfo->MaxPmemIntSet.Bits.PerPmem));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "DDR Cache Size:                     0x%02X\n", pPlatCapInfo->DdrCacheSize));
  if (mPcatTablePtr->Revision < NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_3_00_HDR) {
    PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Reserved:                           %02X%02X%02X\n\n",
      pPlatCapInfo->Data.Reserved[0], pPlatCapInfo->Data.Reserved[1], pPlatCapInfo->Data.Reserved[2]));
  } else {
    PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Cache Capabilities:                 0x%02X\n", pPlatCapInfo->Data.CacheCapabilities));
    PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Reserved:                           %02X%02X\n\n", pPlatCapInfo->Data.Reserved[1], pPlatCapInfo->Data.Reserved[2]));
  }
} // AddPlatCapInfoTableToPCAT()


UINT8
GetNumSockets (
  VOID
  )
{
  UINT8            NumSocket = 0;
  UINT8            SktIndex;

  for (SktIndex = 0; SktIndex < MAX_SOCKET; SktIndex++) {
    if (mSystemMemoryMap->Socket[SktIndex].SAD[0].Enable) {
      NumSocket++;
    }
  }

  return (NumSocket);
}

/**

  This routine adds Memory Interleave Capability Information Table to PCAT for App Direct WB memory mode.

  @param None

  @retval None

**/
VOID
AddAppDirectWbInterleaveCapabilitiesInfoTable (
  VOID
  )
{
  UINT64                     PcatAppendPtr;
  PINTERLEAVE_CAPABILITIES   pIntCap;
  UINT8                      PlatformMaxSocket, PlatformMaxChannelsPerSocket, PlatformMaxDimm;
  UINT8                      IntFormat;
  MAX_INT_SET                InterleaveInfo;
  UINT16                     NumIntFormats;
  UINT8                      DdrPmemCachingMode;

  if (mPcatTablePtr->Revision < NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_3_00_HDR) {
    return;
  }

  if ((mMemMapHost->PcatCapability.MemModeCapabilities & MODE_NATIVE_ADWB) == 0) {
    return;
  }

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Building structure of type %d (Memory Interleave Capability Information) for AD WB Memory\n",
    TYPE_INTERLEAVE_CAPABILITY_DESCRIPTOR));

  //
  // Make sure that the structure size matches the expected INTERLEAVE_CAP_HEADER_SIZE value
  //
  ASSERT (sizeof(INTERLEAVE_CAPABILITIES) == INTERLEAVE_CAP_HEADER_SIZE);

  NumIntFormats = GetAppDirectWbNumIntFormats (
                    mSystemMemoryMap->sncEnabled,
                    mSystemMemoryMap->NumOfCluster
                  );

  //
  // If the new PCAT Length is larger than MAX_PCAT_SIZE, then this indicates that
  // the IntFormat List will cause a buffer overflow. This is a fatal error.
  //
  if (mPcatTablePtr->Length + INTERLEAVE_CAP_HEADER_SIZE + (sizeof (PM_INTERLEAVE) * NumIntFormats) > MAX_PCAT_SIZE) {
    return;
  }

  // Get the current end of PCAT so we can append the Interleave Capabilities Info Table
  PcatAppendPtr = (UINT64) mPcatTablePtr + mPcatTablePtr->Length;
  pIntCap = (PINTERLEAVE_CAPABILITIES) PcatAppendPtr;

  // Interleave Capabilities for PMEM as it is the only mode that BIOS supports configuration requests
  pIntCap->Type = TYPE_INTERLEAVE_CAPABILITY_DESCRIPTOR;
  pIntCap->MemMode = ADWB_MODE;
  pIntCap->IntAlignSize = INTERLEAVE_ALIGNMENT_SIZE;

  // get max channel per socket info via PCD
  OemGetMemTopologyInfo (&PlatformMaxSocket, &PlatformMaxChannelsPerSocket, &PlatformMaxDimm);

  pIntCap->InterleaveSize = CH_INT_SIZE_256B | IMC_INT_SIZE_256B;

  DdrPmemCachingMode = mSystemMemoryMap->volMemMode;
  if ((DdrPmemCachingMode == VOL_MEM_MODE_2LM) && (mSystemMemoryMap->CacheMemType != CACHE_TYPE_DDR_CACHE_PMEM)) {
    DdrPmemCachingMode = VOL_MEM_MODE_1LM;
  }

  InterleaveInfo = GetAppDirectWbMaxInterleaveSet (DdrPmemCachingMode);
  pIntCap->MaxInterleaveSet.Data = InterleaveInfo.Data;

  pIntCap->NumIntFormats = NumIntFormats;

  // Update the Length field of Memory Interleave Capability Info Table to include the format lists of variable length (4 x m)
  pIntCap->Length = (sizeof (UINT32) * (pIntCap->NumIntFormats)) + INTERLEAVE_CAP_HEADER_SIZE;

  // Update the Length field of PCAT table accordingly
  mPcatTablePtr->Length += pIntCap->Length;

  //
  // Fetch the Interleave Format List directly into the PCAT table
  //
  GetAppDirectWbIntFormatList (
    mSystemMemoryMap->sncEnabled,
    mSystemMemoryMap->NumOfCluster,
    pIntCap->IntFormatList
  );

  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Type:                               %d\n", pIntCap->Type));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Length:                             %d\n", pIntCap->Length));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Memory Mode:                        %d\n", pIntCap->MemMode));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Reserved:                           %02X%02X%02X\n",
    pIntCap->Rsvrd[0], pIntCap->Rsvrd[1], pIntCap->Rsvrd[2]));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Interleave Alignment Size:          %d\n", pIntCap->IntAlignSize));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Interleave Size:                    0x%x\n", pIntCap->InterleaveSize));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Max Interleave Sets For Mem Type:\n"));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "  Per CPU Die:                      %d\n", pIntCap->MaxInterleaveSet.Bits.PerCpuDie));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "  Per PMem:                         %d\n", pIntCap->MaxInterleaveSet.Bits.PerPmem));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Number of Interleave Formats:       %d\n", pIntCap->NumIntFormats));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Interleave Format List:             \n"));
  for (IntFormat = 0; IntFormat < pIntCap->NumIntFormats; IntFormat++) {
    PCATDEBUG ((DEBUG_INFO, "  InterleaveBitmap: 0x%04X; Recommended: %d; Reserved: 0x%x\n",
      pIntCap->IntFormatList[IntFormat].InterleaveBitmap,
      pIntCap->IntFormatList[IntFormat].Recommended & BIT0,
      pIntCap->IntFormatList[IntFormat].Reserved));
  }
  PCATDEBUG ((DEBUG_INFO, "\n\n"));
}

/**

  This routine adds Memory Interleave Capability Information Table to PCAT for App Direct memory mode.

  @param None

  @retval None

**/
VOID
AddAppDirectInterleaveCapabilitiesInfoTable (
  VOID
  )
{
  UINT64                     PcatAppendPtr;
  PINTERLEAVE_CAPABILITIES   pIntCap;
  UINT8                      PlatformMaxSocket, PlatformMaxChannelsPerSocket, PlatformMaxDimm;
  UINT8                      IntFormat;
  MAX_INT_SET                InterleaveInfo;
  UINT16                     NumIntFormats;

  if ((mMemMapHost->PcatCapability.MemModeCapabilities & MODE_AD) == 0) {
    return;
  }

  DEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Building structure of type %d (Memory Interleave Capability Information) for AD Memory\n",
    TYPE_INTERLEAVE_CAPABILITY_DESCRIPTOR));

  //
  // Make sure that the structure size matches the expected INTERLEAVE_CAP_HEADER_SIZE value
  //
  ASSERT (sizeof(INTERLEAVE_CAPABILITIES) == INTERLEAVE_CAP_HEADER_SIZE);

  NumIntFormats = GetAppDirectNumIntFormats ();

  //
  // If the new PCAT Length is larger than MAX_PCAT_SIZE, then this indicates that
  // the IntFormat List will cause a buffer overflow. This is a fatal error.
  //
  if (mPcatTablePtr->Length + INTERLEAVE_CAP_HEADER_SIZE + (sizeof (PM_INTERLEAVE) * NumIntFormats) > MAX_PCAT_SIZE) {
    return;
  }

  // Get the current end of PCAT so we can append the Interleave Capabilities Info Table
  PcatAppendPtr = (UINT64) mPcatTablePtr + mPcatTablePtr->Length;
  pIntCap = (PINTERLEAVE_CAPABILITIES) PcatAppendPtr;

  // Interleave Capabilities for PMEM as it is the only mode that BIOS supports configuration requests
  pIntCap->Type = TYPE_INTERLEAVE_CAPABILITY_DESCRIPTOR;
  pIntCap->MemMode = AD_MODE;
  pIntCap->IntAlignSize = INTERLEAVE_ALIGNMENT_SIZE;

  // get max channel per socket info via PCD
  OemGetMemTopologyInfo (&PlatformMaxSocket, &PlatformMaxChannelsPerSocket, &PlatformMaxDimm);

  if (mSystemMemoryMap->DdrtIntlvGranularity == MEM_INT_GRAN_CH_256B_TGT_4KB) {
    pIntCap->InterleaveSize = CH_INT_SIZE_256B | IMC_INT_SIZE_256B;
  } else {
    pIntCap->InterleaveSize = CH_INT_SIZE_4KB | IMC_INT_SIZE_4KB;
  }

  InterleaveInfo = GetAppDirectMaxInterleaveSet ();
  pIntCap->MaxInterleaveSet.Data = InterleaveInfo.Data;

  pIntCap->NumIntFormats = NumIntFormats;

  // Update the Length field of Memory Interleave Capability Info Table to include the format lists of variable length (4 x m)
  pIntCap->Length = (sizeof (UINT32) * (pIntCap->NumIntFormats)) + INTERLEAVE_CAP_HEADER_SIZE;

  // Update the Length field of PCAT table accordingly
  mPcatTablePtr->Length += pIntCap->Length;

  //
  // Fetch the Interleave Format List directly into the PCAT table
  //
  GetAppDirectIntFormatList (pIntCap->IntFormatList);

  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Type:                               %d\n", pIntCap->Type));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Length:                             %d\n", pIntCap->Length));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Memory Mode:                        %d\n", pIntCap->MemMode));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Reserved:                           %02X%02X%02X\n",
    pIntCap->Rsvrd[0], pIntCap->Rsvrd[1], pIntCap->Rsvrd[2]));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Interleave Alignment Size:          %d\n", pIntCap->IntAlignSize));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Interleave Size:                    0x%x\n", pIntCap->InterleaveSize));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Max Interleave Sets For Mem Type:\n"));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "  Per CPU Die:                      %d\n", pIntCap->MaxInterleaveSet.Bits.PerCpuDie));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "  Per PMem:                         %d\n", pIntCap->MaxInterleaveSet.Bits.PerPmem));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Number of Interleave Formats:       %d\n", pIntCap->NumIntFormats));
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Interleave Format List:             \n"));
  for (IntFormat = 0; IntFormat < pIntCap->NumIntFormats; IntFormat++) {
    PCATDEBUG ((DEBUG_INFO, "  InterleaveBitmap: 0x%04X; Recommended: %d; Reserved: 0x%x\n",
      pIntCap->IntFormatList[IntFormat].InterleaveBitmap,
      pIntCap->IntFormatList[IntFormat].Recommended & BIT0,
      pIntCap->IntFormatList[IntFormat].Reserved));
  }
  PCATDEBUG ((DEBUG_INFO, "\n\n"));
}

/**

  This routine adds Memory Interleave Capability Information Table to PCAT for supported persistent memory modes.

  @param None

  @retval None

**/
VOID
AddInterleaveCapabilitiesInfoTable (
  VOID
  )
{
  if ((mMemMapHost->PcatCapability.MemModeCapabilities & MODE_AD) != 0) {
    AddAppDirectInterleaveCapabilitiesInfoTable ();
  }

  if ((mMemMapHost->PcatCapability.MemModeCapabilities & MODE_NATIVE_ADWB) != 0) {
    AddAppDirectWbInterleaveCapabilitiesInfoTable ();
  }

} // AddInterleaveCapabilitiesInfoTable()


/**
  This routine adds Socket SKU Information Table to PCAT.

  @param void

  @retval EFI_BAD_BUFFER_SIZE    Memory buffer was not the correct size
 **/
EFI_STATUS
EFIAPI
AddSocketSkuInfoTable (
  VOID
  )
{
  UINT64                     PcatAppendPtr;
  SOCKET_SKU_INFO_TABLE      *SocSkuInfo;
  UINT8                      PlatformMaxSocket;
  UINT8                      PlatformMaxChannelsPerSocket;
  UINT8                      PlatformMaxDimm;
  UINT8                      SktIndex;
  UINT8                      SocketDieCount;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Building structure of type %d (Socket SKU Information Table)\n",
    TYPE_SOCKET_SKU_INFORMATION_TABLE));
  //
  // Get the current end of PCAT so we can append the Socket SKU Info Table
  //
  PcatAppendPtr = (UINTN)mPcatTablePtr + mPcatTablePtr->Length;
  SocSkuInfo = (PSOCKET_SKU_INFO_TABLE) PcatAppendPtr;
  //
  // get max socket info via PCD
  //
  OemGetMemTopologyInfo (&PlatformMaxSocket, &PlatformMaxChannelsPerSocket, &PlatformMaxDimm);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "PCAT Table Dump SKU, number of sockets:%d\n", PlatformMaxSocket));
  //
  // fill the table with necessary info as per Intel NVDIMM S-F Interface Spec 1.01
  //
  for (SktIndex = 0; SktIndex < PlatformMaxSocket; SktIndex++) {

    if (mSystemMemoryMap->Socket[SktIndex].SktSkuValid) {

      if (mPcatTablePtr->Length > MAX_PCAT_SIZE - sizeof (SOCKET_SKU_INFO_TABLE)) {

        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCAT") "Socket SKU Information Table exceeds max PCAT size (%d > %d)\n",
                mPcatTablePtr->Length + sizeof (SOCKET_SKU_INFO_TABLE), MAX_PCAT_SIZE));
        return EFI_BAD_BUFFER_SIZE;
      }
      SocSkuInfo->Type = TYPE_SOCKET_SKU_INFORMATION_TABLE;
      SocSkuInfo->Length = sizeof (SOCKET_SKU_INFO_TABLE);
      SocketDieCount = GetAcpiDieCount (SktIndex);
      SocSkuInfo->SocketId = (SktIndex / SocketDieCount);
      SocSkuInfo->DieId = (SktIndex % SocketDieCount);
      SocSkuInfo->MemMapSizeLimit = ((UINT64) mSystemMemoryMap->Socket[SktIndex].SktSkuLimit << CONVERT_64MB_TO_B);
      SocSkuInfo->TotMemSizeMapSPA = ((UINT64) mSystemMemoryMap->Socket[SktIndex].SktTotMemMapSPA << CONVERT_64MB_TO_B);
      SocSkuInfo->CacheMemSize2LM = ((UINT64) mSystemMemoryMap->Socket[SktIndex].SktMemSize2LM << CONVERT_64MB_TO_B);

      PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Type:                                  %d\n", SocSkuInfo->Type));
      PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Length:                                %d\n", SocSkuInfo->Length));
      PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Socket ID:                             %d\n", SocSkuInfo->SocketId));
      PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Die ID:                                %d\n", SocSkuInfo->DieId));
      PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Mapped Memory Size Limit:              0x%016llX\n", SocSkuInfo->MemMapSizeLimit));
      PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Total Memory Size Mapped into SPA:     0x%016llX\n", SocSkuInfo->TotMemSizeMapSPA));
      PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Memory Size Excluded in Limit for 2LM: 0x%016llX\n", SocSkuInfo->CacheMemSize2LM));
      //
      // Update the size of the PCAT to include this table
      //
      PcatAppendPtr = (UINT64) PcatAppendPtr + SocSkuInfo->Length;
      mPcatTablePtr->Length += SocSkuInfo->Length;
      SocSkuInfo = (SOCKET_SKU_INFO_TABLE*)PcatAppendPtr;
    }
  } // for (SktIndex...)
  return EFI_SUCCESS;
} // AddSocketSkuInfoTable()


/**
  @brief Initialize ACPI Platform Configuration Attribute Table

  Prepare header of ACPI Platform Configuration Attribute Table
  Memory for the table must be already allocated by the caller. This function
  only configures PCAT header fields.
  Whole unused memory buffer (outside of PCAT header) is cleared.
  Address and size of the buffer are stored in global variables (mPcatTablePtr + mPcatTableSize)

  @param[out]     PcatTablePtr    Address of allocated memory buffer, where PCAT header will be prepared
  @param[in]      PcatTableSize   Size of allocated memory buffer (bytes)

  @retval EFI_SUCCESS             PCAT header initialized
  @retval EFI_ALREADY_STARTED     PCAT already initialized, nothing done this time
  @retval EFI_INVALID_PARAMETER   NULL pointer passed as a parameter
  @retval EFI_BUFFER_TOO_SMALL    Memory buffer too small to fit PCAT header
  @retval EFI_BAD_BUFFER_SIZE     Memory buffer was not the correct size
**/
EFI_STATUS
EFIAPI
InitializeAcpiPcat (
  OUT UINT64 *PcatTablePtr,
  IN  UINT64 PcatTableSize
  )
{
  UINT64 TempOemTableId;
  EFI_STATUS  Status;

  ASSERT (PcatTablePtr != NULL);
  if (PcatTablePtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Don't initialize the table twice
  //
  if (IsInitialized ()) {
    CRDEBUG ((DEBUG_ERROR,  CR_ERROR_STR ("PCAT") "PCAT table already initialized\n"));
    return EFI_ALREADY_STARTED;
  }

  if (PcatTableSize < sizeof (NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE)) {
    CRDEBUG ((DEBUG_ERROR,  CR_ERROR_STR ("PCAT") "Buffer too small to fit PCAT header\n"));
    return EFI_BUFFER_TOO_SMALL;
  }

  // Call OEM-hook to create PCAT table in buffer. If success, then skip code below and return
  if (OemCreatePcatTable (PcatTablePtr, PcatTableSize) == EFI_SUCCESS) {
    CRDEBUG ((DEBUG_ERROR,  CR_INFO_STR ("PCAT") "PCAT created by OEM-hook\n"));
    return EFI_SUCCESS;
  }
  // Save address and size of PCAT for future purposes
  mPcatTablePtr = (PNVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE) PcatTablePtr;
  mPcatTableSize = PcatTableSize;

  // Clear whole buffer
  ZeroMem (mPcatTablePtr, mPcatTableSize);

  TempOemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);

  mPcatTablePtr->Signature   = NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_SIGNATURE;
  // Length = only header right now
  mPcatTablePtr->Length      = sizeof (NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE);
  mPcatTablePtr->Revision    = GetPcatHeaderRevision ();
  PCATDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCAT") "Creating PCAT table for header revision 0x%02x\n", mPcatTablePtr->Revision));
  CopyMem (mPcatTablePtr->OemID,     PcdGetPtr (PcdAcpiDefaultOemId), sizeof (mPcatTablePtr->OemID));
  CopyMem (mPcatTablePtr->OemTblID,  &TempOemTableId, sizeof (mPcatTablePtr->OemTblID));
  mPcatTablePtr->OemRevision = EFI_ACPI_OEM_REVISION;
  mPcatTablePtr->CreatorID   = EFI_ACPI_CREATOR_ID;
  mPcatTablePtr->CreatorRev  = EFI_ACPI_CREATOR_REVISION;
  //
  // Add both the Platform Capability Table, Interleave Capabilities and SocketSKUinfo table to the PCAT
  // These two table when they added, they update PCAT Length.
  //
  AddPlatCapInfoTableToPCAT ();
  AddInterleaveCapabilitiesInfoTable ();
  Status = AddSocketSkuInfoTable ();
  if (!EFI_ERROR (Status)) {

    CRDEBUG ((DEBUG_INFO,  CR_INFO_STR ("PCAT") "Table length: %d\n", mPcatTablePtr->Length));
    //
    // Call OEM-hook to update PCAT table in buffer
    //
    OemUpdatePcatTable (PcatTablePtr);
    //
    // Calculate the checksum for the table
    //
    ComputeChecksum ((UINT8*)PcatTablePtr, mPcatTablePtr->Length, &(mPcatTablePtr->Checksum));
  }
  return Status;
}


/**
  @brief Create SSKU Attribute Extension Table in ACPI PCAT

  Create a new SSKU Attribute Extension Table at the end of PCAT.
  PCAT must be already initialized and have a free space in the
  allocated memory buffer to fit SAET. PCAT may already contain other
  extension tables.
  Newly created SAET will be empty (only headers are configured).
  Actual size of PCAT is updated after SAET creation.

  @param                          None

  @retval EFI_SUCCESS             Empty SAET added to PCAT
  @retval EFI_NOT_STARTED         PCAT not initialized yet
  @retval EFI_INVALID_PARAMETER   NULL pointer passed as a parameter
  @retval EFI_BUFFER_TOO_SMALL    Left space in PCAT is to small to fit new empty SAET
**/
EFI_STATUS
EFIAPI
CreateAcpiPcatSaet (
  VOID
  )
{
  PNVDIMM_SSKU_ATTR_EXT_TABLE AttrExtTable;

  CRDEBUG ((EFI_D_INFO, "[CreateAcpiPcatSaet] start\n"));

  if (!IsInitialized ()) {
    CRDEBUG ((DEBUG_ERROR,  "[CreateAcpiPcatSaet] ERROR - PCAT table not initialized yet\n"));
    return EFI_NOT_STARTED;
  }

  // Check if buffer is large enough to fit SSKU Attribute Extension Table header
  if (mPcatTableSize - mPcatTablePtr->Length < sizeof (NVDIMM_SSKU_ATTR_EXT_TABLE)) {
    CRDEBUG ((DEBUG_ERROR,  "[CreateAcpiPcatSaet] ERROR - ACPI Table size not big enough to fit SSKU Attribute Extension Table header\n"));
    return EFI_BUFFER_TOO_SMALL;
  }

  // Append SSKU Attribute Extension Table at the actual end of PCAT
  AttrExtTable = (PNVDIMM_SSKU_ATTR_EXT_TABLE) PTR_OFFSET (mPcatTablePtr, mPcatTablePtr->Length);

  // Fill SSKU Attribute Extension Table header
  AttrExtTable->Type = SSKU_ATTRIBUTE_EXTENSION_TABLE_TYPE;
  AttrExtTable->VendorID = SSKU_ATTRIBUTE_EXTENSION_TABLE_VENDOR_ID;
  CopyMem (&AttrExtTable->Guid, &SSKU_ATTRIBUTE_EXTENSION_TABLE, sizeof (AttrExtTable->Guid));
  // Length = only header right now
  AttrExtTable->Length = sizeof (NVDIMM_SSKU_ATTR_EXT_TABLE);

  // Update length of whole PCAT table
  mPcatTablePtr->Length += AttrExtTable->Length;

  return EFI_SUCCESS;
}

/**
  @brief Find SSKU Attribute Extension Table in ACPI PCAT

  Look for a SSKU Attribute Extension Table, where new VLR could be appended.
  To append new VLR, SAET must be at the end of PCAT (complicated resize of
  existing tables inside of PCAT).
  If there is a SAET inside of PCAT but not at the end, a failue will be
  returned.

  @param[in]      PcatTablePtr      PCAT buffer (already initialized)
  @param[out]     PcatSeatTablePtr  Found SAET

  @retval EFI_SUCCESS             SAET found at the end of PCAT
  @retval EFI_NOT_STARTED         PCAT not initialized yet
  @retval EFI_NOT_FOUND           SAET not found at the end of PCAT
  @retval EFI_INVALID_PARAMETER   NULL pointer passed as a parameter

**/
EFI_STATUS
FindAcpiPcatSaet (
  OUT PNVDIMM_SSKU_ATTR_EXT_TABLE *PcatSeatTablePtr
  )
{
  PNVDIMM_SSKU_ATTR_EXT_TABLE CurrentPtr;

  if ((NULL == PcatSeatTablePtr)) {
    CRDEBUG ((DEBUG_ERROR,  "[FindAcpiPcatSaet] ERROR - NULL pointer as a parameter\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (!IsInitialized ()) {
    CRDEBUG ((DEBUG_ERROR,  "[FindAcpiPcatSaet] ERROR - PCAT table not initialized yet\n"));
    return EFI_NOT_STARTED;
  }

  // Start with end of PCAT header
  CurrentPtr = (PNVDIMM_SSKU_ATTR_EXT_TABLE) PTR_OFFSET (mPcatTablePtr, sizeof (*mPcatTablePtr));

  // Don't go outside of current PCAT length
  while ((VOID*) CurrentPtr < PTR_OFFSET (mPcatTablePtr, mPcatTablePtr->Length)) {
    if ((CurrentPtr->VendorID == SSKU_ATTRIBUTE_EXTENSION_TABLE_VENDOR_ID) &&
        (CurrentPtr->Type == SSKU_ATTRIBUTE_EXTENSION_TABLE_TYPE) &&
        (!CompareMem (&CurrentPtr->Guid, &SSKU_ATTRIBUTE_EXTENSION_TABLE, sizeof (CurrentPtr->Guid)))) {
      // Found SSKU Attribute Extension Table, check if it is the last table in PCAT
      if (PTR_OFFSET (CurrentPtr, CurrentPtr->Length) == PTR_OFFSET (mPcatTablePtr, mPcatTablePtr->Length)) {
        // There is nothing after SAET in PCAT
        *PcatSeatTablePtr = CurrentPtr;
        return EFI_SUCCESS;
      }
      // Something is after the SAET, won't be able to append to it, so need to look for another one
    }
    // Check the next table in PCAT
    CurrentPtr = (PNVDIMM_SSKU_ATTR_EXT_TABLE) PTR_OFFSET (CurrentPtr, CurrentPtr->Length);
  }

  return EFI_NOT_FOUND;
}

/**
  @brief Add new VLR to SSKU Attribute Extension Table in ACPI PCAT

  Add new VLR (Variable Length Record) to SAET in PCAT. If there is no SAET at
  the actual end of PCAT, a new SAET is created.
  PCAT must be already initialized and have a free space in the allocated
  memory buffer to fit VLR (and eventual SAET).
  Actual sizes of PCAT and SAET are updated after appending VLR.


  @param[out]   PcatTablePtr      PCAT buffer (already initialized)
  @param[in]    PcatTableSize     Allowed total PCAT size- size of allocated memory buffer (bytes)
  @param[in]    Vlr               VLR to be added to SAET
  @param[in]    VlrSize           Size of the VLR to be added to SAET

  @retval EFI_SUCCESS             VLR added to SAET
  @retval EFI_NOT_STARTED         PCAT not initialized yet
  @retval EFI_INVALID_PARAMETER   NULL pointer passed as a parameter
  @retval EFI_NOT_FOUND           Cannot find a SAET and a new one cannot be created
  @retval EFI_BUFFER_TOO_SMALL    Left space in PCAT is to small to fit new VLR

**/
EFI_STATUS
EFIAPI
AddVlr2PcatSaet (
  IN  UINT8   *Vlr,
  IN  UINTN   VlrSize
  )
{
  PNVDIMM_SSKU_ATTR_EXT_TABLE AttrExtTable;
  UINTN SpaceLeft;
  EFI_STATUS Status;

  if ((NULL == Vlr) ||
      (0 == VlrSize)) {
    CRDEBUG ((DEBUG_ERROR,  "[AddVlr2PcatSaet] ERROR - NULL pointer as a parameter\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (!IsInitialized ()) {
    CRDEBUG ((DEBUG_ERROR,  "[AddVlr2PcatSaet] ERROR - PCAT table not initialized yet\n"));
    return EFI_NOT_STARTED;
  }

  ASSERT (VlrSize < MAX_UINT16);

  // Find SSKU Attribute Extension Table to append to
  Status = FindAcpiPcatSaet (&AttrExtTable);
  if (EFI_NOT_FOUND == Status) {
    // Need to create SSKU Attribute Extension Table first
    Status = CreateAcpiPcatSaet ();
  }

  if (Status != EFI_SUCCESS) {
    CRDEBUG ((DEBUG_ERROR,  "[AddVlr2PcatSaet] ERROR - Cannot find/create SSKU Attribute Extension Table in PCAT\n"));
    return EFI_NOT_FOUND;
  }

  // Check if there is enough space left in PCAT buffer to fit another VLR
  SpaceLeft = mPcatTableSize - mPcatTablePtr->Length;
  if (SpaceLeft < VlrSize) {
    CRDEBUG ((DEBUG_ERROR,  "[AddVlr2PcatSaet] ERROR - ACPI Table size not big enough to fit another VLR; have only %d bytes free, next VLR requires %d\n", SpaceLeft, VlrSize));
    return EFI_BUFFER_TOO_SMALL;
  }

  // Copy VLR to the end of SSKU Attribute Extension Table
  CopyMem ((UINT8*) AttrExtTable + AttrExtTable->Length, Vlr, VlrSize);

  // Update current size of ACPI tables
  AttrExtTable->Length += (UINT16) VlrSize;
  mPcatTablePtr->Length += (UINT16) VlrSize;

  CRDEBUG ((DEBUG_INFO,  "[AddVlr2PcatSaet] VLR added to PCAT->SSKU Attribute Extension Table\n"));

  return EFI_SUCCESS;
}

/**
  @brief Install protocol to access ACPI PCAT supporting functions
*/
EFI_STATUS
InstallAcpiPcatProtocol (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;
  if (NULL == SystemTable) {
    ASSERT (0);
    return EFI_INVALID_PARAMETER;
  }

  mAcpiPcat.InitializeAcpiPcat  = InitializeAcpiPcat;
  mAcpiPcat.CreateAcpiPcatSaet  = CreateAcpiPcatSaet;
  mAcpiPcat.AddVlr2PcatSaet     = AddVlr2PcatSaet;
  mAcpiPcat.IsInitialized       = IsInitialized;

  Status = gBS->InstallMultipleProtocolInterfaces (
    &ImageHandle,
    &gAcpiPcatProtocolGuid,
    &mAcpiPcat,
    NULL
    );

  return EFI_SUCCESS;
}
