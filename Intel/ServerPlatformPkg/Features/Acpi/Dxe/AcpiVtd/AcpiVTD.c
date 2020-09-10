/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

//
// Statements that include other files
//
#include <Acpi/AcpiVTD.h>
#include <Library/SetupLib.h>
#include <IioRegs.h>
#include <IioSetupDefinitions.h>

EFI_VTD_SUPPORT_INSTANCE mPrivateData;
#define MAX_BUS_ADDR_WIDTH 45
/**

  GC_TODO: Add function description

  @param This        - GC_TODO: add argument description
  @param RemapType   - GC_TODO: add argument description
  @param RemapEntry  - GC_TODO: add argument description

  @retval EFI_INVALID_PARAMETER - GC_TODO: Add description for return value
  @retval EFI_SUCCESS           - GC_TODO: Add description for return value

**/
EFI_STATUS
EFIAPI
InsertDmaRemap (
  IN  EFI_DMA_REMAP_PROTOCOL            *This,
  IN  REMAP_TYPE                        RemapType,
  IN  VOID                              *RemapEntry
  )
{
  UINTN                                                       DevIndex;
  EFI_ACPI_DMAR_DESCRIPTION_TABLE                             *Dmar;
  EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE            *Drhd;
  EFI_ACPI_DMAR_RESERVED_MEMORY_REGION_REPORTING_STRUCTURE    *Rmrr;
  EFI_ACPI_DMAR_ROOT_PORT_ATS_CAPABILITY_REPORTING_STRUCTURE  *Atsr;
  EFI_ACPI_DMAR_REMAPPING_HARDWARE_STATIC_AFFINITY_STRUCTURE  *Rhsa;
  EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE                  *DevScope;
  DMAR_DRHD                                                   *DmaRemap;
  DMAR_RMRR                                                   *RevMemRegion;
  DMAR_ATSR                                                   *AtsrRegion;
  DMAR_RHSA                                                   *RhsaRegion;
  EFI_ACPI_DMAR_PCI_PATH                                      *PciPath;
  EFI_ACPI_DMAR_PCI_PATH                                      *PciInputPath;

  //ASSERT(mPrivateData.Dmar != NULL);
  if (mPrivateData.Dmar == NULL) {

      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
  }
  Dmar = mPrivateData.Dmar;
  //ASSERT(((UINT8 *) Dmar + Dmar->Header.Length) != NULL);
  if (((UINT8 *) Dmar + Dmar->Header.Length) == NULL) {

      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
  }
  if (RemapType == DrhdType) {
    DmaRemap = (DMAR_DRHD *) RemapEntry;
    ASSERT (DmaRemap->Signature == DRHD_SIGNATURE);
    Drhd = (EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE *) ((UINT8 *) Dmar + Dmar->Header.Length);
    if (Drhd == NULL) {

      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
    Drhd->Type                = EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_DRHD;
    Drhd->Length              = sizeof (EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE);
    Drhd->Flags               = DmaRemap->Flags;
    Drhd->SegmentNumber       = DmaRemap->SegmentNumber;
    Drhd->RegisterBaseAddress = DmaRemap->RegisterBase;
    DevScope                  = NULL;

    for (DevIndex = 0; DevIndex < DmaRemap->DeviceScopeNumber; DevIndex++) {
      //ASSERT(((UINT8 *) Drhd + Drhd->Length) != NULL);
      if (((UINT8 *) Drhd + Drhd->Length) == NULL) {

          ASSERT (FALSE);
          return EFI_INVALID_PARAMETER;
      }
      DevScope = (EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE *) ((UINT8 *) Drhd + Drhd->Length);
      if (DevScope != NULL) {
        DevScope->DeviceScopeEntryType = DmaRemap->DeviceScope[DevIndex].DeviceType;
        DevScope->Length = sizeof(EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE);
        DevScope->EnumerationID = DmaRemap->DeviceScope[DevIndex].EnumerationID;
        DevScope->StartingBusNumber = DmaRemap->DeviceScope[DevIndex].StartBusNumber;
        if (((UINT8 *)DevScope + DevScope->Length) == NULL) {

          ASSERT(FALSE);
          return EFI_INVALID_PARAMETER;
        }
        PciPath = (EFI_ACPI_DMAR_PCI_PATH *)((UINT8 *)DevScope + DevScope->Length);
        PciInputPath = (EFI_ACPI_DMAR_PCI_PATH *)DmaRemap->DeviceScope[DevIndex].PciNode;
        while (*(UINT8 *)PciInputPath != (UINT8)-1) {
          CopyMem(PciPath, PciInputPath, sizeof(EFI_ACPI_DMAR_PCI_PATH));
          DevScope->Length += sizeof(EFI_ACPI_DMAR_PCI_PATH);
          PciInputPath++;
          PciPath++;
        }
        Drhd->Length = Drhd->Length + (UINT16)DevScope->Length;
      } else {
        DEBUG((DEBUG_ERROR, "DevScope Error. Invalid pointer.\n"));
      }
    }

    Dmar->Header.Length += Drhd->Length;

  } else if (RemapType == RmrrType) {
    RevMemRegion = (DMAR_RMRR *) RemapEntry;
    ASSERT (RevMemRegion->Signature == RMRR_SIGNATURE);
    Rmrr = (EFI_ACPI_DMAR_RESERVED_MEMORY_REGION_REPORTING_STRUCTURE *) ((UINT8 *) Dmar + Dmar->Header.Length);
    if (Rmrr == NULL) {

      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
    Rmrr->Type                              = EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_RMRR;
    Rmrr->Length                            = sizeof (EFI_ACPI_DMAR_RESERVED_MEMORY_REGION_REPORTING_STRUCTURE);
    Rmrr->SegmentNumber                     = RevMemRegion->SegmentNumber;
    Rmrr->ReservedMemoryRegionBaseAddress   = RevMemRegion->RsvdMemBase;
    Rmrr->ReservedMemoryRegionLimitAddress  = RevMemRegion->RsvdMemLimit;
    // Rmrr->ReservedMemoryLimit -= 0x1;
    DevScope = NULL;
    for (DevIndex = 0; DevIndex < RevMemRegion->DeviceScopeNumber; DevIndex++) {
      //ASSERT((UINT8 *) Rmrr + Rmrr->Length != NULL);
      if (((UINT8 *) Rmrr + Rmrr->Length) == NULL) {

        ASSERT (FALSE);
        return EFI_INVALID_PARAMETER;
      }
      DevScope                        = (EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE *) ((UINT8 *) Rmrr + Rmrr->Length);
      if (DevScope != NULL) {
        DevScope->DeviceScopeEntryType  = RevMemRegion->DeviceScope[DevIndex].DeviceType;
        DevScope->StartingBusNumber     = RevMemRegion->DeviceScope[DevIndex].StartBusNumber;
        DevScope->Length                = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE);
        if (((UINT8 *) DevScope + DevScope->Length) == NULL) {

          ASSERT (FALSE);
          return EFI_INVALID_PARAMETER;
        }
        PciPath                         = (EFI_ACPI_DMAR_PCI_PATH *) ((UINT8 *) DevScope + DevScope->Length);
        PciInputPath                    = (EFI_ACPI_DMAR_PCI_PATH *) RevMemRegion->DeviceScope[DevIndex].PciNode;
        while (*(UINT8 *) PciInputPath != (UINT8) -1) {
          CopyMem (PciPath, PciInputPath, sizeof (EFI_ACPI_DMAR_PCI_PATH));
          DevScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
          PciInputPath++;
          PciPath++;
        }
        Rmrr->Length = Rmrr->Length + (UINT16) DevScope->Length;
      }
      else {
        DEBUG((DEBUG_ERROR, "DevScope Error. Invalid pointer.\n"));
      }
    }

    Dmar->Header.Length += Rmrr->Length;

  } else if (RemapType == AtsrType) {
    AtsrRegion = (DMAR_ATSR *) RemapEntry;
    ASSERT (AtsrRegion->Signature == ATSR_SIGNATURE);
    Atsr                  = (EFI_ACPI_DMAR_ROOT_PORT_ATS_CAPABILITY_REPORTING_STRUCTURE *) ((UINT8 *) Dmar + Dmar->Header.Length);
    if (Atsr == NULL) {

      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
    Atsr->Type = EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_ATSR;
    Atsr->Flags = AtsrRegion->Flags;
    Atsr->SegmentNumber = AtsrRegion->SegmentNumber;
    Atsr->Length = sizeof(EFI_ACPI_DMAR_ROOT_PORT_ATS_CAPABILITY_REPORTING_STRUCTURE);
    DevScope = NULL;
    for (DevIndex = 0; DevIndex < AtsrRegion->DeviceScopeNumber; DevIndex++) {
      if ((AtsrRegion->ATSRPresentBit & (01 << DevIndex)) == 00) {
        continue;
      }
      //ASSERT((UINT8 *) Atsr + Atsr->Length != NULL);
      if (((UINT8 *)Atsr + Atsr->Length) == NULL) {

        ASSERT(FALSE);
        return EFI_INVALID_PARAMETER;
      }
      DevScope = (EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE *)((UINT8 *)Atsr + Atsr->Length);
      if (DevScope != NULL) {
        DevScope->DeviceScopeEntryType = AtsrRegion->DeviceScope[DevIndex].DeviceType;
        DevScope->StartingBusNumber = AtsrRegion->DeviceScope[DevIndex].StartBusNumber;
        DevScope->Length = sizeof(EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE);
        if (((UINT8 *)DevScope + DevScope->Length) == NULL) {

          ASSERT(FALSE);
          return EFI_INVALID_PARAMETER;
        }
        PciPath = (EFI_ACPI_DMAR_PCI_PATH *)((UINT8 *)DevScope + DevScope->Length);
        PciInputPath = (EFI_ACPI_DMAR_PCI_PATH *)AtsrRegion->DeviceScope[DevIndex].PciNode;
        while (*(UINT8 *)PciInputPath != (UINT8)-1) {
          CopyMem(PciPath, PciInputPath, sizeof(EFI_ACPI_DMAR_PCI_PATH));
          DevScope->Length += sizeof(EFI_ACPI_DMAR_PCI_PATH);
          PciInputPath++;
          PciPath++;
        }
        Atsr->Length = Atsr->Length + (UINT16)DevScope->Length;
      } else {
        DEBUG((DEBUG_ERROR, "DevScope Error. Invalid pointer.\n"));
      }
    }

    Dmar->Header.Length += Atsr->Length;

  } else if (RemapType == RhsaType) {
    RhsaRegion = (DMAR_RHSA *) RemapEntry;
    ASSERT (RhsaRegion->Signature == RHSA_SIGNATURE);
    Rhsa                  = (EFI_ACPI_DMAR_REMAPPING_HARDWARE_STATIC_AFFINITY_STRUCTURE *) ((UINT8 *) Dmar + Dmar->Header.Length);
    Rhsa->Type            = EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_RHSA;
    Rhsa->ProximityDomain = RhsaRegion->Domian;
    Rhsa->RegisterBaseAddress   = RhsaRegion->RegisterBase;
    Rhsa->Length          = sizeof (EFI_ACPI_DMAR_REMAPPING_HARDWARE_STATIC_AFFINITY_STRUCTURE);
    Dmar->Header.Length += Rhsa->Length;
  } else {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Dmar->Header.Length < TABLE_SIZE);
  return EFI_SUCCESS;
}

/**

    GC_TODO: add routine description

    @param Entry      - GC_TODO: add arg description
    @param Type       - GC_TODO: add arg description
    @param IncludeAll - GC_TODO: add arg description
    @param Length     - GC_TODO: add arg description

    @retval EFI_SUCCESS - GC_TODO: add retval description

**/
EFI_STATUS
GetTablesInfo (
  UINT8       *Entry,
  BOOLEAN     *Type,
  BOOLEAN     *IncludeAll,
  UINTN       *Length
  )
{
  EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE  *Comm;

  Comm    = (EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE *) Entry;
  *Length = Comm->Length;

  if (Comm->Type == EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_RMRR) {
    *Type = RmrrType;
  } else if (Comm->Type == EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_DRHD) {
    *Type = DrhdType;
  } else if (Comm->Type == EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_ATSR) {
    *Type = AtsrType;
  } else if (Comm->Type == EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_RHSA) {
    *Type = RhsaType;
  } else {
    *Type = 0xFF;
  }

  if ((Comm->Flags == EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_ALL_SET) || (Comm->Flags == EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS_SET)) {
    *IncludeAll = TRUE;
  } else {
    *IncludeAll = FALSE;
  }

  return EFI_SUCCESS;
}

/**

    GC_TODO: add routine description

    @param None

    @retval EFI_SUCCESS - GC_TODO: add retval description

**/
EFI_STATUS
ReorderTables (
  VOID
  )
{
  BOOLEAN   Type;
  BOOLEAN   IncludeAll;
  UINTN     Length;
  UINTN     CurrLength;
  UINTN     TableLength;
  UINT8     *Ptr;
  UINT8     *PtrOrder;

  Ptr       = (UINT8 *) mPrivateData.Dmar;
  PtrOrder  = (UINT8 *) mPrivateData.DmarOrder;

  CopyMem (PtrOrder, Ptr, sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE));
  PtrOrder += sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);

  TableLength = mPrivateData.Dmar->Header.Length;

  CurrLength  = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == DrhdType && !IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == DrhdType && IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == RmrrType && !IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == AtsrType && !IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == RhsaType) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  return EFI_SUCCESS;
}

/**

  GC_TODO: Add function description

  @param This      - GC_TODO: add argument description
  @param DmarTable - GC_TODO: add argument description

  @retval EFI_INVALID_PARAMETER - GC_TODO: Add description for return value
  @retval EFI_UNSUPPORTED       - GC_TODO: Add description for return value
  @retval EFI_SUCCESS           - GC_TODO: Add description for return value

**/
EFI_STATUS
EFIAPI
GetDmarTable (
  IN  EFI_DMA_REMAP_PROTOCOL            *This,
  IN  VOID                              **DmarTable
  )
{
  if (DmarTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (mPrivateData.Dmar->Header.Length <= sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE)) {
    return EFI_UNSUPPORTED;
  }

  ReorderTables ();
  *DmarTable = mPrivateData.DmarOrder;
  return EFI_SUCCESS;
}

/**

    GC_TODO: add routine description

    @param ImageHandle - GC_TODO: add arg description
    @param SystemTable - GC_TODO: add arg description

    @retval Status - GC_TODO: add retval description

**/
EFI_STATUS
EFIAPI
VtdTableEntryPoint (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;
  EFI_ACPI_DMAR_DESCRIPTION_TABLE       *Dmar;
  UINT64                                TempOemTableId;
  UINT8                                 VTdSupport;
  UINT8                                 InterruptRemap;
  UINT8                                 ATS;
  UINTN                                 Dmarlength;

  //
  // Initialize our protocol
  //
  ZeroMem (&mPrivateData, sizeof (EFI_VTD_SUPPORT_INSTANCE));

  Status = GetOptionData(&gEfiSocketIioVariableGuid, OFFSET_OF (SOCKET_IIO_CONFIGURATION, VTdSupport), &VTdSupport, sizeof(VTdSupport));
  Status |= GetOptionData(&gEfiSocketIioVariableGuid, OFFSET_OF (SOCKET_IIO_CONFIGURATION, InterruptRemap), &InterruptRemap, sizeof(InterruptRemap));
  Status |= GetOptionData(&gEfiSocketIioVariableGuid, OFFSET_OF (SOCKET_IIO_CONFIGURATION, ATS), &ATS, sizeof(ATS));
  if (EFI_ERROR(Status)) {

    mPrivateData.DmaRemapProt.VTdSupport  = FALSE;
    mPrivateData.DmaRemapProt.InterruptRemap  = FALSE;
    mPrivateData.DmaRemapProt.ATS  = FALSE;

  } else {

    mPrivateData.DmaRemapProt.VTdSupport = VTdSupport;
    mPrivateData.DmaRemapProt.InterruptRemap = VTdSupport && (InterruptRemap != IIO_OPTION_DISABLE);
    mPrivateData.DmaRemapProt.ATS = ATS;
  }

  mPrivateData.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;

  Dmarlength = MAX_SOCKET * NUMBER_PORTS_PER_SOCKET * ( sizeof(EFI_ACPI_DMAR_DESCRIPTION_TABLE) + sizeof (EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE) +
                    sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE) + sizeof (EFI_ACPI_DMAR_PCI_PATH));

  mPrivateData.Dmar = (EFI_ACPI_DMAR_DESCRIPTION_TABLE *)AllocateZeroPool (Dmarlength);

  mPrivateData.DmarOrder = (EFI_ACPI_DMAR_DESCRIPTION_TABLE *)AllocateZeroPool (Dmarlength);

  mPrivateData.DmaRemapProt.InsertDmaRemap = InsertDmaRemap;
  mPrivateData.DmaRemapProt.GetDmarTable = GetDmarTable;

  if (mPrivateData.Dmar != NULL) {
    Dmar                          = mPrivateData.Dmar;
    Dmar->Header.Length           = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
    Dmar->Header.Signature        = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
    Dmar->Header.Revision         = EFI_ACPI_DMAR_DESCRIPTION_TABLE_REVISION;
    Dmar->Header.OemRevision      = EFI_ACPI_DMAR_OEM_REVISION;
    Dmar->Header.CreatorId        = EFI_ACPI_DMAR_OEM_CREATOR_ID;
    Dmar->Header.CreatorRevision  = EFI_ACPI_DMAR_OEM_CREATOR_REVISION;
    Dmar->HostAddressWidth        = MAX_BUS_ADDR_WIDTH;

    TempOemTableId = PcdGet64(PcdAcpiDefaultOemTableId);

    CopyMem (Dmar->Header.OemId, PcdGetPtr(PcdAcpiDefaultOemId), sizeof(Dmar->Header.OemId));
    CopyMem (&Dmar->Header.OemTableId, &TempOemTableId, sizeof(Dmar->Header.OemTableId));

    Status = gBS->InstallProtocolInterface (
                    &mPrivateData.Handle,
                    &gEfiDmaRemapProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPrivateData.DmaRemapProt
                    );
  } else {

    ASSERT_EFI_ERROR(EFI_INVALID_PARAMETER);
    Status = EFI_OUT_OF_RESOURCES;
  }

  return Status;
}
