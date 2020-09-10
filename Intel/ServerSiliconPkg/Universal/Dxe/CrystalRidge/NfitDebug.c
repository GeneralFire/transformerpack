/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AcpiPlatformProtocol.h>
#include <SysHost.h>
#include "CrystalRidge.h"
#include "Nfit.h"
#include "NfitDebug.h"


extern EFI_GUID AcpiBlkRegionGuid;
extern EFI_GUID AcpiCtrlRegionGuid;
extern EFI_GUID AcpiPmemRegionGuid;
extern EFI_GUID IntelCtrlRegionGuid;

/*****************************************************************************
 * Definitions
 *****************************************************************************/


/*****************************************************************************
 * Variables
 *****************************************************************************/

/*****************************************************************************
 * Functions
 *****************************************************************************/
/**
  @brief Return text name of NFIT stucture type.

  @param[in] NfitType - ACPI NFIT structure type

  @return String describing NFIT type is returned.
**/
CONST CHAR8*
NfitDebugStrcType2Name (
  IN UINT16 Type
  )
{
  static CHAR8 *StcType2Name[] = {"SPA Range Structure",
                                  "NVDIMM Region Mapping Structure",
                                  "Interleave Structure",
                                  "SMBIOS Management Information Structure",
                                  "NVDIMM Control Region Structure",
                                  "NVDIMM Block Data Window Region Structure",
                                  "Flush Hint Address Structure",
                                  "Platform Capabilities Structure",
                                  "Reserved"};

  if (Type >= NELEMENTS (StcType2Name)) {

    Type = NELEMENTS (StcType2Name) - 1;
  }
  return StcType2Name[Type];
}


/**
  @brief Print proximity domains table.

  @param *MemAffDataPtr     - ACPI SRAT Memory Affinity structures
  @param MemAffDataLength   - ACPI SRAT Memory Affinity structures length

  @return Void.
**/
VOID
NfitDumpMemoryAffinityData (
  IN ACPI_MEMORY_AFFINITY_DATA *MemAffDataPtr,
  IN UINTN                      MemAffDataLength
  )
{
  UINT64  DomainBase;
  UINT64  DomainLen;
  UINT16  DomainCount = 0;
  UINT16  Idx;

  if (!mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitDebugLogsEnable) {
    return;
  }
  if (MemAffDataPtr != NULL) {

    for (Idx = 0; Idx < MemAffDataLength; Idx++) {

      DomainLen = ((UINT64) MemAffDataPtr[Idx].LengthHigh << 32) + MemAffDataPtr[Idx].LengthLow;
      if (DomainLen > 0) {

        DomainBase = ((UINT64) MemAffDataPtr[Idx].AddressBaseHigh << 32) + MemAffDataPtr[Idx].AddressBaseLow;
        CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Memory proximity domain 0x%X base 0x%016llX len 0x%016llX flags 0x%X\n",
                  MemAffDataPtr[Idx].ProximityDomain, DomainBase, DomainLen, MemAffDataPtr[Idx].Flags));
        DomainCount++;
      }
    }
  }
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Memory proximity domains found: %d\n", DomainCount));
  CRDEBUG ((DEBUG_ERROR, "\n"));
  return;
}


/**
  @brief Print content of System Physical Address Range Structure.

  @param[in] SpaRangeStrcPtr - Pointer to the structure to dump.

  @return Void.
 **/
VOID
NfitDumpSpaRangeStruct (
  IN NFIT_SPA_RANGE_STRUCTURE *SpaRangeStrcPtr
  )
{
  if (!mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitDebugLogsEnable) {
    return;
  }
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Type:                      %d (%a)\n", SpaRangeStrcPtr->Type, NfitDebugStrcType2Name (SpaRangeStrcPtr->Type)));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Length:                    %d\n", SpaRangeStrcPtr->Length));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "SPA Range Structure Index: %d\n", SpaRangeStrcPtr->SPARangeStructureIndex));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Flags:                     0x%04X\n", SpaRangeStrcPtr->Flags));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Reserved:                  0x%04X\n", SpaRangeStrcPtr->Reserved_8));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Proximity Domain:          %d\n", SpaRangeStrcPtr->ProximityDomain));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Address Range Type GUID:   %g (%a)\n", SpaRangeStrcPtr->AddressRangeTypeGUID,
             CompareGuid (&SpaRangeStrcPtr->AddressRangeTypeGUID, &AcpiPmemRegionGuid) ? "PMEM" :
               CompareGuid (&SpaRangeStrcPtr->AddressRangeTypeGUID, &AcpiCtrlRegionGuid) ? "CTRL" :
                 CompareGuid (&SpaRangeStrcPtr->AddressRangeTypeGUID, &AcpiBlkRegionGuid) ? "BLK" :
                   CompareGuid (&SpaRangeStrcPtr->AddressRangeTypeGUID, &IntelCtrlRegionGuid) ? "Intel Prop" : "other"));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "SPA Range Base:            0x%016llX\n", SpaRangeStrcPtr->SystemPhysicalAddressRangeBase));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "SPA Range Length:          0x%016llX\n", SpaRangeStrcPtr->SystemPhysicalAddressRangeLength));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Memory Mapping Attribute:  0x%016llX\n", SpaRangeStrcPtr->AddressRangeMemoryMappingAttribute));
  CRDEBUG ((DEBUG_ERROR, "\n"));
}


/**
  @brief Print content of NVDIMM Region Mapping Structure.

  @param[in] NvdimmRgnMapPtr - Pointer to the structure to dump.

  @return Void.
**/
VOID
NfitDumpNvdimmRgnMappingStruct (
  IN NFIT_NVDIMM_REGION_MAPPING_STRUCTURE *NvdimmRgnMapPtr
  )
{
  if (!mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitDebugLogsEnable) {
    return;
  }
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Type:                                  %d (%a)\n", NvdimmRgnMapPtr->Type, NfitDebugStrcType2Name (NvdimmRgnMapPtr->Type)));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Length:                                %d\n", NvdimmRgnMapPtr->Length));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "NFIT Device Handle:                    0x%08X\n", *(UINT32*)&NvdimmRgnMapPtr->NFITDeviceHandle));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Memory Device Physical ID:             0x%04X\n", NvdimmRgnMapPtr->NVDIMMPhysicalID));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Memory Device Region ID:               0x%04X\n", NvdimmRgnMapPtr->NVDIMMRegionID));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "SPA Range Structure Index:             %d\n", NvdimmRgnMapPtr->SPARangeStructureIndex));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "NVDIMM Control Region Structure Index: %d\n", NvdimmRgnMapPtr->NVDIMMControlRegionStructureIndex));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "NVDIMM Region Size:                    0x%016llX\n", NvdimmRgnMapPtr->NVDIMMRegionSize));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Region Offset:                         0x%016llX\n", NvdimmRgnMapPtr->RegionOffset));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "NVDIMM Physical Address Region Base:   0x%016llX\n", NvdimmRgnMapPtr->NVDIMMPhysicalAddressRegionBase));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Interleave Structure Index:            %d\n", NvdimmRgnMapPtr->InterleaveStructureIndex));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Interleave Ways:                       %d\n", NvdimmRgnMapPtr->InterleaveWays));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "NVDIMM State Flags:                    0x%04X\n", NvdimmRgnMapPtr->NVDIMMStateFlags));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Reserved:                              0x%04X\n", NvdimmRgnMapPtr->Reserved_46));
  CRDEBUG ((DEBUG_ERROR, "\n"));
}


/**
  @brief This function prints Interleave Structure

  @param[in] IntStrcPtr - Pointer to the structure to dump

  @return Void.
**/
VOID
NfitDumpInterleaveStruct (
  IN NFIT_INTERLEAVE_STRUCTURE *IntStrcPtr
  )
{
  UINT32 i;
  UINT32 *LineOffset;

  if (!mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitDebugLogsEnable) {
    return;
  }
  LineOffset = (UINT32*)&IntStrcPtr[1];
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Type:                       %d (%a)\n", IntStrcPtr->Type, NfitDebugStrcType2Name (IntStrcPtr->Type)));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Length:                     %d\n", IntStrcPtr->Length));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Interleave Structure Index: %d\n", IntStrcPtr->InterleaveStructureIndex));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Reserved:                   0x%04X\n", IntStrcPtr->Reserved_6));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Number of Lines Described:  %d\n", IntStrcPtr->NumberOfLines));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Line Size:                  %d\n", IntStrcPtr->LineSize));
  for (i = 0; i < IntStrcPtr->NumberOfLines; i++) {
    CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Line %2d Offset:             %d\n", i, LineOffset[i]));
  }
  CRDEBUG ((DEBUG_ERROR, "\n"));
}


/**
  @brief Print content of SMBIOS Management Information Structure.

  @param[in] SmbiosInfoPtr - Pointer to the structure to dump.

  @return Void.
**/
VOID
NfitDumpSmbiosInfoStruct (
  IN NFIT_SMBIOS_INFORMATION_STRUCTURE *SmbiosInfoPtr
  )
{
  if (!mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitDebugLogsEnable) {
    return;
  }
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Type:      %d (%a)\n", SmbiosInfoPtr->Type, NfitDebugStrcType2Name (SmbiosInfoPtr->Type)));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Type:      %d\n", SmbiosInfoPtr->Type));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Length:    %d\n", SmbiosInfoPtr->Length));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Reserved:  0x%08X\n", SmbiosInfoPtr->Reserved_4));
  CRDEBUG ((DEBUG_ERROR, "\n"));
}


/**
  @brief Print content of NVDIMM Control Region Structure.

  @param CtrlRgnStrcPtr - Pointer to the structure to dump.

  @return Void.
**/
VOID
NfitDumpCtrlRgnStruct (
  IN NFIT_NVDIMM_CONTROL_REGION_STRUCTURE *CtrlRgnStrcPtr
  )
{
  if (!mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitDebugLogsEnable) {
    return;
  }
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Type:                                  %d (%a)\n", CtrlRgnStrcPtr->Type, NfitDebugStrcType2Name (CtrlRgnStrcPtr->Type)));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Length:                                %d\n", CtrlRgnStrcPtr->Length));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "NVDIMM Control Region Structure Index: %d\n", CtrlRgnStrcPtr->NVDIMMControlRegionStructureIndex));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Vendor ID:                             0x%04X\n", CtrlRgnStrcPtr->VendorID));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Device ID:                             0x%04X\n",  CtrlRgnStrcPtr->DeviceID));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Revision ID:                           0x%04X\n", CtrlRgnStrcPtr->RevisionID));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Subsystem Vendor ID:                   0x%04X\n", CtrlRgnStrcPtr->SubsystemVendorID));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Subsystem Device ID:                   0x%04X\n", CtrlRgnStrcPtr->SubsystemDeviceID));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Subsystem Revision ID:                 0x%04X\n", CtrlRgnStrcPtr->SubsystemRevisionID));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Valid Fields:                          0x%02X\n", CtrlRgnStrcPtr->ValidFields));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Manufacturing Location:                0x%02X\n", CtrlRgnStrcPtr->ManufacturingLocation));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Manufacturing Date:                    0x%04X\n", CtrlRgnStrcPtr->ManufacturingDate));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Reserved:                              0x%02X%02X\n",
            CtrlRgnStrcPtr->Reserved_22[1], CtrlRgnStrcPtr->Reserved_22[0]));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Serial Number:                         0x%08X\n", CtrlRgnStrcPtr->SerialNumber));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Region Format Interface Code:          0x%04X\n", CtrlRgnStrcPtr->RegionFormatInterfaceCode));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Number Of Block Control Windows:       %d\n", CtrlRgnStrcPtr->NumberOfBlockControlWindows));

  if (CtrlRgnStrcPtr->Length > 32) {

    CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Size Of Block Control Window:                     0x%016llX\n",
              CtrlRgnStrcPtr->SizeOfBlockControlWindow));
    CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Command Register Offset In Block Control Window:  0x%016llX\n",
              CtrlRgnStrcPtr->CommandRegisterOffsetInBlockControlWindow));
    CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Size Of Command Register In Block Control Windows:0x%016llX\n",
              CtrlRgnStrcPtr->SizeOfCommandRegisterInBlockControlWindows));
    CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Status Register Offset In Block Control Window:   0x%016llX\n",
              CtrlRgnStrcPtr->StatusRegisterOffsetInBlockControlWindow));
    CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Size Of Status Register In Block Control Windows: 0x%016llX\n",
              CtrlRgnStrcPtr->SizeOfStatusRegisterInBlockControlWindows));
    CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "NVDIMM Control Region Flag:                       0x%04X\n",
              CtrlRgnStrcPtr->NVDIMMControlRegionFlag));
    CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Reserved:                                         0x%02X%02X%02X%02X%02X%02X\n",
              CtrlRgnStrcPtr->Reserved_74[5], CtrlRgnStrcPtr->Reserved_74[4], CtrlRgnStrcPtr->Reserved_74[3],
              CtrlRgnStrcPtr->Reserved_74[2], CtrlRgnStrcPtr->Reserved_74[1], CtrlRgnStrcPtr->Reserved_74[0]));
  }
  CRDEBUG ((DEBUG_ERROR, "\n"));
}

/**
  @brief Print content of Flush Hint Address Structure.

  @param FlushHintStrcPtr - Pointer to the structure to dump.

  @return Void.
**/
VOID
NfitDumpFlushHintAddrStruct (
  IN NFIT_FLUSH_HINT_ADDRESS_STRUCTURE *FlushHintStrcPtr
  )
{
  UINT32 i;
  UINT64 *AddrTablePtr;

  if (!mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitDebugLogsEnable) {
    return;
  }
  AddrTablePtr = (UINT64*)&FlushHintStrcPtr[1];

  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Type:                           %d (%a)\n", FlushHintStrcPtr->Type, NfitDebugStrcType2Name (FlushHintStrcPtr->Type)));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Length:                         %d\n", FlushHintStrcPtr->Length));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "NFIT Device Handle:             0x%04X\n", *(UINT32*)&FlushHintStrcPtr->NFITDeviceHandle));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Number Of Flush Hint Addresses: %d\n", FlushHintStrcPtr->NumberOfFlushHintAddresses));
  for (i = 0; i < FlushHintStrcPtr->NumberOfFlushHintAddresses; i++) {
    CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Flush Hint Address [%d]:         0x%llX\n", i, AddrTablePtr[i]));
  }
  CRDEBUG ((DEBUG_ERROR, "\n"));
}


/**
  @brief Print content of Platform Capabilities Structure.

  @param[in] PlatCapPtr - Pointer to the structure to dump.

  @return Void.
**/
VOID
NfitDumpPlatCapStruct (
  IN NFIT_PLATFORM_CAPABILITIES_STRUCTURE *PlatCapPtr
  )
{
  if (!mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitDebugLogsEnable) {
    return;
  }
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Type:                     %d (%a)\n", PlatCapPtr->Type, NfitDebugStrcType2Name (PlatCapPtr->Type)));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Length:                   %d\n", PlatCapPtr->Length));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Highest Valid Capability: 0x%X\n", PlatCapPtr->HighestValidCapability));
  CRDEBUG ((DEBUG_ERROR, CR_INFO_STR ("NFIT") "Capabilities:             0x%X\n", PlatCapPtr->Capabilities));
  CRDEBUG ((DEBUG_ERROR, "\n"));
}

