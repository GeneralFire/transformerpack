/** @file
  This file is SampleCode for Intel PEI Platform Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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


#include <KtiSetupDefinitions.h>
#include <Ppi/UpiPolicyPpi.h>
#include <Library/GpioLib.h>
#include <Library/SetupLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Guid/SetupVariable.h>
#include <SiliconSetting.h>
#include <Library/HobLib.h>
#include <Guid/PlatformInfo.h>
#include <Library/DebugLib.h>
#include <UbaKti.h>
#include <Guid/SocketPciResourceData.h>
#include <Library/PcdLib.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/CpuAndRevisionLib.h>
//APTIOV_SERVER_OVERRIDE_RC_START: 
#include <AmiMrcHooksWrapperLib\AmiMrcHooksWrapperLib.h>
#include <Library/SysHostPointerLib.h>
//APTIOV_SERVER_OVERRIDE_RC_END:
#include <Library/SiliconWorkaroundLib.h>
#include <Upi/KtiSi.h>
#include <UncoreCommonIncludes.h>
#include <Ppi/UbaCfgDb.h>
#include <Library/EmulationConfigurationLib.h>
#include <UbaKti.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiUpiPolicyUpdateLib.h>
#include <PlatformInfoTypes.h>
#include <Ppi/FirmwareVolume.h>
// APTIOV_SERVER_OVERRIDE_RC_START : Improvements made to discard the OEM MMIOL/MMIOH resource map if MMCFG base value is changed after 
// SOCKET_PCI_RESOURCE_CONFIGURATION_DATA_NAME NVRAM variable created in previous boots
#include <Library/KtiApi.h>
// APTIOV_SERVER_OVERRIDE_RC_END : Improvements made to discard the OEM MMIOL/MMIOH resource map if MMCFG base value is changed after 
// SOCKET_PCI_RESOURCE_CONFIGURATION_DATA_NAME NVRAM variable created in previous boots
#define WAIT_TIME_FOR_PSBP 30000 // Wait for a maximum of 30 sec

/**
  Get Platform Type

  @retval UINT8 platformtype.

**/
UINT8
GetPlatformType (
  VOID
  )

//
{
  EFI_PLATFORM_INFO                     *PlatformInfo;
  EFI_HOB_GUID_TYPE                     *GuidHob;

  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);

  return PlatformInfo->BoardId;
}

/**

  Read KTI Nvram from variable.

    @param None

    @retval TRUE - KTI nvram data hob is restored from variable, FALSE - KTI nvram data hob is not initialized.

**/
BOOLEAN
ReadKtiNvram (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINTN                           DataSize;
  VOID                            *HobData;
  VOID                            *GuidHob;
  EFI_PEI_SERVICES                **PeiServices;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiVariable;
  CHAR16                          EfiKtiHostNvramVariableName[] = L"KtiNvramData";

  GuidHob = GetFirstGuidHob (&gEfiKtiHostNvramDataHobGuid);
  if (GuidHob == NULL) {
    DEBUG((EFI_D_ERROR, "\n KtiHostNvram hob is not found! \n"));
    return FALSE;
  }
  HobData = GET_GUID_HOB_DATA(GuidHob);
  DataSize = GET_GUID_HOB_DATA_SIZE (GuidHob);

  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();

  //
  // Locate Variable PPI
  //
  Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gEfiPeiReadOnlyVariable2PpiGuid,
                            0,
                            NULL,
                            &PeiVariable
                            );

  if (!EFI_ERROR(Status)) {
    Status = PeiVariable->GetVariable (
                            PeiVariable,
                            EfiKtiHostNvramVariableName,
                            &gEfiKtiHostNvramDataHobGuid,
                            NULL,
                            &DataSize,
                            HobData
                            );
    if (!EFI_ERROR(Status)) {
      return TRUE;
    }
  }
  DEBUG((DEBUG_INFO, "\n Status of getting Variable KtiNvramData: %r \n", Status));
  return FALSE;
}

/**

  OEM hook at the end of topology discovery in KTIRC to be used for error reporting.

  @param[in] None

  @retval None

**/
VOID
OemHookPostTopologyDiscovery (
  VOID
  )
{
  return;
}


/**
   First part of OemGetResourceMapUpdate(), handles PCI bus rebalance request.

   @param[in]     VarPpiPtr    - Variables access protocol
   @param[in,out] CpuRes       - Ptr to array of CPU resources
   @param[in]     SystemParams - structure w/ system parameters needed
**/
VOID
OemGetPciBusResourceUpdate (
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VarPpiPtr,
  KTI_CPU_RESOURCE                  CpuResTable[MAX_SOCKET],
  KTI_OEMCALLOUT                    SystemParams
  )
{
  SYSTEM_PCI_BUS_CONFIGURATION      SystemPciBusConfig;
  UINTN                             VarSize;
  EFI_STATUS                        Status;
  UINT16                            SocketBit;
  UINT8                             SegmentBusBase[MAX_SOCKET] = {0}; // Start of unused bus numbers in a segment
  UINT8                             Segment;
  UINT8                             Socket;
  UINT8                             Stack;

  VarSize = sizeof(SystemPciBusConfig);
  Status = VarPpiPtr->GetVariable (VarPpiPtr, SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME,
                                   &gEfiSocketPciResourceDataGuid, NULL, &VarSize, &SystemPciBusConfig);
  if (EFI_ERROR (Status)) {

    if (Status != EFI_NOT_FOUND) {

      DEBUG((DEBUG_ERROR, "[KTI] ERROR: Reading variable '%s' failed (%r)\n", SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME, Status));
    }
    return;
  }
  //
  // First validate the variable contents. Ignore if something is wrong.
  //
  for (SocketBit = 1, Socket = 0; Socket < MAX_SOCKET; SocketBit <<= 1, Socket++) {

    if ((SystemParams.CpuList & SocketBit) && SystemPciBusConfig.Socket[Socket].Segment != CpuResTable[Socket].PciSegment) {

      DEBUG((DEBUG_WARN, "[KTI] WARNING: PCI segment %d not matching %d for socket %d in PCI bus rebalance request",
             SystemPciBusConfig.Socket[Socket].Segment, CpuResTable[Socket].PciSegment, Socket));
      DEBUG((DEBUG_WARN, " - ignore '%s'\n", SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME));
      return;
    }
    for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {

      if (SystemPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack] > 0xFF - CpuResTable[Socket].TotEnabledStacks) {

        DEBUG((DEBUG_WARN, "[KTI] WARNING: PCI bus pool size %d requested for stack %d in socket %d",
               SystemPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack], Stack, Socket));
        DEBUG((DEBUG_WARN, " - ignore '%s'\n", SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME));
        return;
      }
      if ((SystemParams.CpuList & SocketBit) &&
          CpuResTable[Socket].StackRes[Stack].BusLimit >= CpuResTable[Socket].StackRes[Stack].BusBase) {

        if (SystemPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack] == 0) {

          DEBUG((DEBUG_WARN, "[KTI] WARNING: PCI bus pool size 0 requested for valid stack %d in socket %d",
                Stack, Socket));
          DEBUG((DEBUG_WARN, " - ignore '%s'\n", SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME));
          return;
        }
      } else {

        if (SystemPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack] != 0) {

          DEBUG((DEBUG_WARN, "[KTI] WARNING: PCI bus pool size %d requested for invalid stack %d in socket %d",
                 SystemPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack], Stack, Socket));
          DEBUG((DEBUG_WARN, " - ignore '%s'\n", SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME));
          return;
        }
      }
    } // for (Stack...)
  } // for (Socket...)
  //
  // Rebalance variable request validated, apply it.
  //
  for (SocketBit = 1, Socket = 0; Socket < MAX_SOCKET; SocketBit <<= 1, Socket++) {

    if (!(SystemParams.CpuList & SocketBit)) {
      continue;
    }
    Segment = CpuResTable[Socket].PciSegment;
    CpuResTable[Socket].BusBase = SegmentBusBase[Segment];
    for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {

      if (CpuResTable[Socket].StackRes[Stack].BusLimit >= CpuResTable[Socket].StackRes[Stack].BusBase) {

        CpuResTable[Socket].StackRes[Stack].BusBase = SegmentBusBase[Segment];
        SegmentBusBase[Segment] += SystemPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack];
        CpuResTable[Socket].StackRes[Stack].BusLimit = SegmentBusBase[Segment] - 1;
      }
    } // for (Stack...)
    CpuResTable[Socket].BusLimit = SegmentBusBase[Segment] - 1;
  } // for (Socket...)
  return;
}


/**

   Callout to allow OEM to adjust the resource map.
   Used to allow adjustment of IO ranges, bus numbers, mmioh and mmiol resource assignments
   Calling code assumes the data structure is returned in a good format.
   SBSP resources must be assigned first, with rest of sockets assigned in ascending order of NID
   Resources must be assigned in ascending consecutive order with no gaps.
   Notes: This does not allow for changing the mmcfg size of mmioh granularity
          However you can allocate multiple consecutive blocks of mmioh to increase size in a particular stack
          and adjust the mmiohbase


   @param CpuRes       - Ptr to array of Cpu Resources
   @param SystemParams - structure w/ system parameters needed

**/
VOID
OemGetResourceMapUpdate (
  KTI_CPU_RESOURCE  CpuRes[MAX_SOCKET],
  KTI_OEMCALLOUT    SystemParams
  )
{
  EFI_STATUS                             Status;
  UINT8                                  Socket;
  UINT8                                  Stack;
  UINTN                                  VariableSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI        *PeiVariable;
  SYSTEM_PCI_BASE_LIMITS                 SystemPciResourceConfig;
  EFI_PEI_SERVICES                       **PeiServices;
  SOCKET_PCI_BASE_LIMITS                 *CurSocket;
  PCI_BASE_LIMITS                        *CurStackLimits;
  PCI_BASE_LIMITS                        *CurSocketLimits;
  // APTIOV_SERVER_OVERRIDE_RC_START : Improvements made to discard the OEM MMIOL/MMIOH resource map if MMCFG base value is changed after 
  // SOCKET_PCI_RESOURCE_CONFIGURATION_DATA_NAME NVRAM variable created in previous boots
  UINT32                                 CurrentBootMmiolBase;
  UINT32                                 LastBootMmiolBase;
  BOOLEAN                                FirstValidSocketFound = FALSE;
  BOOLEAN                                DiscardSocketPciResourceConfigurationMmiolInfo = FALSE;
  // APTIOV_SERVER_OVERRIDE_RC_END  : Improvements made to discard the OEM MMIOL/MMIOH resource map if MMCFG base value is changed after 
  // SOCKET_PCI_RESOURCE_CONFIGURATION_DATA_NAME NVRAM variable created in previous boots

  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();
  Status = (*PeiServices)->LocatePpi (
    PeiServices,
    &gEfiPeiReadOnlyVariable2PpiGuid,
    0,
    NULL,
    &PeiVariable);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "[KTI] ERROR: Locate gEfiPeiReadOnlyVariable2PpiGuid failed (%r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return;
  }
  //
  // First apply PCI bus resource update request.
  //
  OemGetPciBusResourceUpdate (PeiVariable, CpuRes, SystemParams);
  //
  // Now apply address space resource update request.
  //
  VariableSize = sizeof(SYSTEM_PCI_BASE_LIMITS);
  Status = PeiVariable->GetVariable (
    PeiVariable,
    SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME,
    &gEfiSocketPciResourceDataGuid,
    NULL,
    &VariableSize,
    &SystemPciResourceConfig);
  if (EFI_ERROR (Status)) {
// APTIOV_SERVER_OVERRIDE_RC_START : Oem Hook to Override Resource Map
	OemGetResourceMapUpdateOverride (Status, GetSysHostPointer (), PeiVariable, CpuRes, SystemParams);
// APTIOV_SERVER_OVERRIDE_RC_END : Oem Hook to Override Resource Map 
    if (Status != EFI_NOT_FOUND) {
      DEBUG((DEBUG_ERROR, "[KTI] ERROR: Reading variable '%s' failed (%r)\n", SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME, Status));
      ASSERT_EFI_ERROR (Status);
    }
    return;
  }
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    CurSocket = &SystemPciResourceConfig.Socket[Socket];
    CurSocketLimits = &CurSocket->SocketLimits;

    for (Stack = 0; Stack <  MAX_LOGIC_IIO_STACK; Stack++) {
      CurStackLimits = &CurSocket->StackLimits[Stack];
      // APTIOV_SERVER_OVERRIDE_RC_START : Improvements made to discard the OEM MMIOL/MMIOH resource map if MMCFG base value is changed after 
      // SOCKET_PCI_RESOURCE_CONFIGURATION_DATA_NAME NVRAM variable created in previous boots
      if (FirstValidSocketFound == FALSE) {
	      CurrentBootMmiolBase = GetmmCfgBase () + SystemParams.mmcfgSize;
          LastBootMmiolBase = (UINT32) CurSocketLimits->LowMmio.Base;
          FirstValidSocketFound = TRUE;
          if (CurrentBootMmiolBase != LastBootMmiolBase)
              DiscardSocketPciResourceConfigurationMmiolInfo = TRUE;
      }
      // APTIOV_SERVER_OVERRIDE_RC_END : Improvements made to discard the OEM MMIOL/MMIOH resource map if MMCFG base value is changed after 
      // SOCKET_PCI_RESOURCE_CONFIGURATION_DATA_NAME NVRAM variable created in previous boots
      if (CurStackLimits->Io.Base != CurStackLimits->Io.Limit) {
        CpuRes[Socket].StackRes[Stack].IoBase = (UINT16)CurStackLimits->Io.Base;
        CpuRes[Socket].StackRes[Stack].IoLimit = (UINT16)CurStackLimits->Io.Limit;

        if (CurSocketLimits->Io.Base != CurSocketLimits->Io.Limit) {
          CpuRes[Socket].IoBase = (UINT16)CurSocketLimits->Io.Base;
          CpuRes[Socket].IoLimit = (UINT16)CurSocketLimits->Io.Limit;
        }
      }
      // APTIOV_SERVER_OVERRIDE_RC_START : Improvements made to discard the OEM MMIOL/MMIOH resource map if MMCFG base value is changed after 
      // SOCKET_PCI_RESOURCE_CONFIGURATION_DATA_NAME NVRAM variable created in previous boots
      if (!DiscardSocketPciResourceConfigurationMmiolInfo) {
          if ((CurStackLimits->LowMmio.Base != 0 && CurStackLimits->LowMmio.Limit != 0) ||
              (CurStackLimits->LowMmio.Base == (UINT32)-1 && CurStackLimits->LowMmio.Limit == 0)) {
    
            CpuRes[Socket].StackRes[Stack].MmiolBase = (UINT32)CurStackLimits->LowMmio.Base;
            CpuRes[Socket].StackRes[Stack].MmiolLimit = (UINT32)CurStackLimits->LowMmio.Limit;
    
            if (CurSocketLimits->LowMmio.Base != CurSocketLimits->LowMmio.Limit) {
              CpuRes[Socket].MmiolBase = (UINT32)CurSocketLimits->LowMmio.Base;
              CpuRes[Socket].MmiolLimit = (UINT32)CurSocketLimits->LowMmio.Limit;
            }
          }
      }
      // APTIOV_SERVER_OVERRIDE_RC_END : Improvements made to discard the OEM MMIOL/MMIOH resource map if MMCFG base value is changed after 
      // SOCKET_PCI_RESOURCE_CONFIGURATION_DATA_NAME NVRAM variable created in previous boots
      if ((CurStackLimits->HighMmio.Base != 0 && CurStackLimits->HighMmio.Limit != 0) ||
          (CurStackLimits->HighMmio.Base == (UINT64)-1 && CurStackLimits->HighMmio.Limit == 0)) {

        CpuRes[Socket].StackRes[Stack].MmiohBase.lo = (UINT32)CurStackLimits->HighMmio.Base;
        CpuRes[Socket].StackRes[Stack].MmiohBase.hi = (UINT32)(RShiftU64 (CurStackLimits->HighMmio.Base, 32));

        CpuRes[Socket].StackRes[Stack].MmiohLimit.lo = (UINT32)CurStackLimits->HighMmio.Limit;
        CpuRes[Socket].StackRes[Stack].MmiohLimit.hi = (UINT32)(RShiftU64 (CurStackLimits->HighMmio.Limit, 32));

        if (CurSocketLimits->HighMmio.Base != CurSocketLimits->HighMmio.Limit) {
          CpuRes[Socket].MmiohBase.lo = (UINT32)CurSocketLimits->HighMmio.Base;
          CpuRes[Socket].MmiohBase.hi = (UINT32)(RShiftU64 (CurSocketLimits->HighMmio.Base, 32));

          CpuRes[Socket].MmiohLimit.lo = (UINT32)CurSocketLimits->HighMmio.Limit;
          CpuRes[Socket].MmiohLimit.hi = (UINT32)(RShiftU64 (CurSocketLimits->HighMmio.Limit, 32));
        }
      }
    }
    // APTIOV_SERVER_OVERRIDE_RC_START : Improvements made to discard the OEM MMIOL/MMIOH resource map if MMCFG base value is changed after 
    // SOCKET_PCI_RESOURCE_CONFIGURATION_DATA_NAME NVRAM variable created in previous boots
    if (!DiscardSocketPciResourceConfigurationMmiolInfo) {
        if (CurSocket->StackLimits[UBOX_STACK].LowMmio.Base != CurSocket->StackLimits[UBOX_STACK].LowMmio.Limit) {
          CpuRes[Socket].StackRes[UBOX_STACK].MmiolBase = (UINT32)CurSocket->StackLimits[UBOX_STACK].LowMmio.Base;
          CpuRes[Socket].StackRes[UBOX_STACK].MmiolLimit = (UINT32)CurSocket->StackLimits[UBOX_STACK].LowMmio.Limit;
        }
    }
    // APTIOV_SERVER_OVERRIDE_RC_END : Improvements made to discard the OEM MMIOL/MMIOH resource map if MMCFG base value is changed after 
    // SOCKET_PCI_RESOURCE_CONFIGURATION_DATA_NAME NVRAM variable created in previous boots
  }

  return;
}


/**
  Get the board type bitmask.
    Bits[3:0]   - Socket0
    Bits[7:4]   - Socket1
    Bits[11:8]  - Socket2
    Bits[15:12] - Socket3
    Bits[19:16] - Socket4
    Bits[23:20] - Socket5
    Bits[27:24] - Socket6
    Bits[31:28] - Socket7

  Within each Socket-specific field, bits mean:
    Bit0 = CPU_TYPE_STD support; always 1 on Socket0
    Bit1 = CPU_TYPE_F support
    Bit2 = CPU_TYPE_P support
    Bit3 = reserved
  @retval board type bitmask
**/
UINT32
GetBoardTypeBitmask (
  VOID
  )
{

//
// Allow standard CPUs on all possible 8 slots in any board.
//
UINT32 BoardTypeBitmask = CPU_TYPE_STD_MASK | (CPU_TYPE_STD_MASK << 4) | (CPU_TYPE_STD_MASK << 8) |
                      (CPU_TYPE_STD_MASK << 12) | (CPU_TYPE_STD_MASK << 16) | (CPU_TYPE_STD_MASK << 20) |
                      (CPU_TYPE_STD_MASK << 24) | (CPU_TYPE_STD_MASK << 28);

  BoardTypeBitmask |= PcdGet32(PcdBoardTypeBitmask);

  if (UbiosGenerationOrHsleEnabled ()) {
    //
    // For uBIOS and HSLE  allow all types of parts in all sockets
    //
    BoardTypeBitmask = 0xffffffff;
  }

  return BoardTypeBitmask;
}

/**
    To get KTI Alllanes and PerLane Eparams
    @param Pointer to AllLanes Eparam table
           Size of AllLanes Eparam table
           Pointer to PerLane Eparam table
           Size of PerLane Eparam table

    @retval VOID
**/
VOID
GetKtiLaneEparam (
  IN OUT ALL_LANES_EPARAM_LINK_INFO          **PtrAllLanes,
  IN OUT UINT32                               *SizeOfAallLanesTable,
  IN OUT PER_LANE_EPARAM_LINK_INFO           **PtrPerLane,
  IN OUT UINT32                               *SizeOfPerLaneTable
  )
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_KTI_EPARAM_UPDATE_TABLE  KtiEparamTable;
  UINTN                             TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR,"[UBA KTI] Locate UbaConfigPPi failure - %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  TableSize = sizeof(KtiEparamTable);
  Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformKtiEparamUpdateDataGuid,
                                 &KtiEparamTable,
                                 &TableSize
                                 );

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR,"[UBA KTI] Get platform kti lane eparam data failure - %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  ASSERT (KtiEparamTable.Signature == PLATFORM_KTIEP_UPDATE_SIGNATURE);
  ASSERT (KtiEparamTable.Version == PLATFORM_KTIEP_UPDATE_VERSION);

  *PtrAllLanes = KtiEparamTable.AllLanesEparamTablePtr;
  *PtrPerLane = KtiEparamTable.PerLaneEparamTablePtr;

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    *SizeOfAallLanesTable = (UINT32) (KtiEparamTable.SizeOfAllLanesEparamTable / sizeof(ALL_LANES_PRESET_COEFF_LINK_INFO));
    *SizeOfPerLaneTable = (UINT32)(KtiEparamTable.SizeOfPerLaneEparamTable/sizeof(PER_LANE_PRESET_COEFF_LINK_INFO));
  } else {
    *SizeOfAallLanesTable = (UINT32) (KtiEparamTable.SizeOfAllLanesEparamTable / sizeof(ALL_LANES_EPARAM_LINK_INFO));
    *SizeOfPerLaneTable = (UINT32)(KtiEparamTable.SizeOfPerLaneEparamTable/sizeof(PER_LANE_EPARAM_LINK_INFO));
  }

  return;
}

/**
  Platform hook to get TXEQ adaptation parameters

  @param[out] AdaptTbl - The adapation table of the speed

  @retval TRUE / FALSE

**/
BOOLEAN
OemGetAdaptedEqSettings (
  KTI_ADAPTATION_TABLE     *AdaptTbl
  )
{
  EFI_STATUS                               Status;
  UINTN                                    AdaptationTableSize;
  EFI_PEI_SERVICES                         **PeiServices;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI          *PeiVariable;

  if (AdaptTbl == NULL) {
    return FALSE;
  }

  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();
  Status = (*PeiServices)->LocatePpi (
                                  PeiServices,
                                  &gEfiPeiReadOnlyVariable2PpiGuid,
                                  0,
                                  NULL,
                                  &PeiVariable);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return FALSE;
  }

  AdaptationTableSize = sizeof(KTI_ADAPTATION_TABLE) * KTI_SPEEDS_SUPPORTED;
  Status = PeiVariable->GetVariable (
                                PeiVariable,
                                KTI_ADAPTATION_TABLE_DATA_NAME,
                                &gKtiAdaptationTableGuid,
                                NULL,
                                &AdaptationTableSize,
                                AdaptTbl);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
}

/**

  Platform hook to check if the CPU parts are changed or swapped

  @retval TRUE    - CPU parts are changed or swapped
          FALSE   - No change on CPU parts

**/
BOOLEAN
OemCheckCpuPartsChangeSwap(
  VOID
  )
{
  return FALSE;
}

/**

Routine Description:

  This function Create Cfr ImageList.
  It extract the Cfr images from BIOS region and Generation an image list.
  The Image list end with NULL.

  @retval :
       0      There is no Cfr image in the BIOS. CFR feature disable.
       not 0  The Point to the CFR image List.
**/
UINT32
CreateImageList (
  VOID
  )
{
  EFI_PEI_FIRMWARE_VOLUME_PPI           *FvPpi;
  EFI_STATUS                            Status;
  EFI_PEI_FV_HANDLE                     FvHandle;
  EFI_PEI_FILE_HANDLE                   FileHandle;
  UINT8                                 *Buffer;
  UINT32                                *ImageList;

  ImageList = NULL;
  //
  // Locate the corresponding FV_PPI according to the format GUID of the FV found
  //
  Status = PeiServicesLocatePpi (
             &gEfiFirmwareFileSystem2Guid,
             0,
             NULL,
             (VOID**)&FvPpi
             );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Can't Locate Firmware Volume Ppi to process CFR Images.\n"));
    goto ErrorExit;
  }

  //
  // Process CFR FV and get FV handle.
  //
  Status = FvPpi->ProcessVolume (
                    FvPpi,
                    (VOID*)(UINTN)PcdGet32(PcdFlashCfrRegionBase),
                    PcdGet32(PcdFlashCfrRegionSize),
                    &FvHandle
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Fail to process CFR FV, FV may be corrupted!\n"));
    goto ErrorExit;
  }


  ImageList = AllocateZeroPool (sizeof(UINT32) * 6);
  if (ImageList == NULL) {
    goto ErrorExit;
  }
  //
  // Find the first CFR image in the FV.
  //
  FileHandle = NULL;
  Status = FvPpi->FindFileByType(
                    FvPpi,
                    EFI_FV_FILETYPE_RAW,
                    FvHandle,
                    &FileHandle
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Can't finde CFR image in CFR FV - %r!\n", Status));
    goto ErrorExit;
  }

  //
  // Get the Image address;
  //
  Buffer = (UINT8 *) FileHandle + sizeof (EFI_FFS_FILE_HEADER);

  if (ImageList != NULL) {
    ImageList[0] = (UINT32)(UINTN)Buffer;
    ImageList[1] = FFS_FILE_SIZE (FileHandle) - sizeof (EFI_FFS_FILE_HEADER);
  }

  DEBUG((EFI_D_ERROR, "Find CFR Image Base %x Size %x\n", ImageList[0], ImageList[1]));

  Status = FvPpi->FindFileByType(
                    FvPpi,
                    EFI_FV_FILETYPE_RAW,
                    FvHandle,
                    &FileHandle
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Can't finde CFR image in CFR FV - %r!\n", Status));
    goto ErrorExit;
  }

  //
  // Get the Image address;
  //
  Buffer = (UINT8 *) FileHandle + sizeof (EFI_FFS_FILE_HEADER);

  ImageList[2] = (UINT32)(UINTN)Buffer;
  ImageList[3] = FFS_FILE_SIZE (FileHandle) - sizeof (EFI_FFS_FILE_HEADER);
  DEBUG ((EFI_D_ERROR, "Find CFR Image Base %x Size %x\n", ImageList[2], ImageList[3]));

ErrorExit:
  return (UINT32)ImageList;
}

/**

Routine Description:

  This function according Cfr knobs to update Cfr fields in Upi policy.

  @param[out]      UpiPolicyPpi  - Pointer to the Upi policy structure.
  @param[in]       SetupData - Pointer to the setup knobs data structure.

  @retval VOID

**/
VOID
UpdateCFRConfig (
  UPI_POLICY_PPI     *UpiPolicyPpi,
  // APTIOV_SERVER_OVERRIDE_RC_START
  INTEL_SETUP_DATA         *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END
)
{
  if (SetupData->SocketConfig.SocketProcessorCoreConfiguration.CFRS3mEnable == 0 && SetupData->SocketConfig.SocketProcessorCoreConfiguration.CFRPucodeEnable == 0) {
    UpiPolicyPpi->Upi.CFRImagePtr = 0;
    UpiPolicyPpi->Upi.S3mCFRCommit = 0;
    UpiPolicyPpi->Upi.PucodeCFRCommit = 0;
    DEBUG((EFI_D_INFO, "UpdateCFRConfig Disabled, Exit\n"));
    return;
  }

  //
  //  0 - Disable S3m CFR flow.
  //  1 - Provision S3m CFR but not Commit.
  //  2 - Provsion and Commit S3M CFR.
  //
  if (SetupData->SocketConfig.SocketProcessorCoreConfiguration.CFRS3mEnable == 1) {
    if (SetupData->SocketConfig.SocketProcessorCoreConfiguration.CFRS3mManualCommit == 1) {
      UpiPolicyPpi->Upi.S3mCFRCommit = 2;
    } else {
      UpiPolicyPpi->Upi.S3mCFRCommit = 1;
    }
  } else {
      UpiPolicyPpi->Upi.S3mCFRCommit = 0;
  }

  //
  //  0 - Disable Pcode/Ucode CFR flow.
  //  1 - Provision Pcode/Ucode CFR but not Commit.
  //  2 - Provsion and Commit Pcode/Ucode CFR.
  //
  if (SetupData->SocketConfig.SocketProcessorCoreConfiguration.CFRPucodeEnable == 1) {
    if (SetupData->SocketConfig.SocketProcessorCoreConfiguration.CFRPucodeManualCommit == 1) {
      UpiPolicyPpi->Upi.PucodeCFRCommit = 2;
    } else {
      UpiPolicyPpi->Upi.PucodeCFRCommit = 1;
    }
  } else {
    UpiPolicyPpi->Upi.PucodeCFRCommit = 0;
  }

  UpiPolicyPpi->Upi.CFRImagePtr = CreateImageList();
}

/**

Routine Description:

  This function performs Upi Silicon init policy initialzation

Returns:

**/
EFI_STATUS
EFIAPI
UpdatePeiUpiPolicy (
  IN OUT UPI_POLICY_PPI     *UpiPolicyPpi,
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve redefinition error
  IN OUT INTEL_SETUP_DATA          *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve redefinition error
  )
{
  KTI_DFX_CPU_LINK_SETTING    *pKtiDfxCpuLink;
  KTI_DFX_CPU_PHY_SETTING     *pKtiDfxCpuPhy;
  KTI_CPU_PHY_SETTING         *pKtiCpuPhy;
  KTI_CPU_LINK_SETTING        *pKtiCpuLink;
  UINT8                       *PerPortOption;
  UINT8                       Ctr1, Ctr2;
  UINT8                       Soc;
  UINT8                       PlatformType;
  UINT8                       KtiDfxCpuLink[MAX_SOCKET][MAX_KTI_PORTS][4];
  UINT8                       KtiPortCnt;
  BOOLEAN                     IsSnrOrIcxd;

  //
  // EPSDIP_SETUP_MMIO_START
  //
  //
  // EPSDIP_SETUP_MMIO_END
  //
  pKtiDfxCpuLink = NULL;
  pKtiDfxCpuPhy = NULL;
  pKtiCpuPhy = NULL;
  pKtiCpuLink = NULL;
  PerPortOption = NULL;

  IsSnrOrIcxd = IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL) ||
                IsCpuAndRevision (CPU_TNR, REV_ALL);

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    KtiPortCnt = SI_MAX_KTI_PORTS_SPR;
  } else if (IsCpuAndRevision (CPU_GNRSP, REV_ALL) || IsCpuAndRevision (CPU_SRFSP, REV_ALL)) {
    KtiPortCnt = SI_MAX_KTI_PORTS_GNRSRF;
  } else if (IsSnrOrIcxd) {
    KtiPortCnt = 0;
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL)) {
    KtiPortCnt = SI_MAX_KTI_PORTS_SKX;
  } else if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    KtiPortCnt = SI_MAX_KTI_PORTS_CPX;
  } else {
    KtiPortCnt = SI_MAX_KTI_PORTS_ICX;
  }
  if (KtiPortCnt > MAX_KTI_PORTS){
    return EFI_UNSUPPORTED;
  }

  UpiPolicyPpi->Upi.D2KCreditConfig     = SetupData->SocketConfig.UpiConfig.D2KCreditConfig;
  UpiPolicyPpi->Upi.SnoopThrottleConfig = SetupData->SocketConfig.UpiConfig.SnoopThrottleConfig;

  UpiPolicyPpi->Upi.LegacyVgaStack     = SetupData->SocketConfig.UpiConfig.LegacyVgaStack;
  UpiPolicyPpi->Upi.DebugPrintLevel    = SetupData->SocketConfig.UpiConfig.DebugPrintLevel;
  UpiPolicyPpi->Upi.SplitLock          = SetupData->SocketConfig.UpiConfig.SplitLock;
  UpiPolicyPpi->Upi.P2pRelaxedOrdering = SetupData->SocketConfig.UpiConfig.P2pRelaxedOrdering;
#if MAX_SOCKET > 1
  UpiPolicyPpi->Upi.LegacyVgaSoc       = SetupData->SocketConfig.UpiConfig.LegacyVgaSoc;
  UpiPolicyPpi->Upi.KtiLinkSpeedMode   = SetupData->SocketConfig.UpiConfig.QpiLinkSpeedMode;
  UpiPolicyPpi->Upi.DegradePrecedence  = SetupData->SocketConfig.UpiConfig.DegradePrecedence;
  UpiPolicyPpi->Upi.Degrade4SPreference= SetupData->SocketConfig.UpiConfig.Degrade4SPreference;
  UpiPolicyPpi->Upi.DirectoryModeEn    = SetupData->SocketConfig.UpiConfig.DirectoryModeEn;

  UpiPolicyPpi->Upi.KtiLinkSpeed      = SetupData->SocketConfig.UpiConfig.QpiLinkSpeed;
  UpiPolicyPpi->Upi.KtiLinkL0pEn      = SetupData->SocketConfig.UpiConfig.KtiLinkL0pEn;
  UpiPolicyPpi->Upi.KtiLinkL1En       = SetupData->SocketConfig.UpiConfig.KtiLinkL1En;
  UpiPolicyPpi->Upi.KtiFailoverEn     = SetupData->SocketConfig.UpiConfig.KtiFailoverEn;
  UpiPolicyPpi->Upi.KtiLbEn           = SetupData->SocketConfig.UpiConfig.KtiLbEn;
  UpiPolicyPpi->Upi.KtiCrcMode        = SetupData->SocketConfig.UpiConfig.KtiCrcMode;

  UpiPolicyPpi->Upi.KtiPrefetchEn       = SetupData->SocketConfig.UpiConfig.KtiPrefetchEn;
  UpiPolicyPpi->Upi.XptRemotePrefetchEn = SetupData->SocketConfig.UpiConfig.XptRemotePrefetchEn;

  UpiPolicyPpi->Upi.KtiCpuSktHotPlugEn        = SetupData->SocketConfig.UpiConfig.QpiCpuSktHotPlugEn;
  UpiPolicyPpi->Upi.KtiCpuSktHotPlugTopology  = SetupData->SocketConfig.UpiConfig.KtiCpuSktHotPlugTopology;
  UpiPolicyPpi->Upi.KtiSkuMismatchCheck       = SetupData->SocketConfig.UpiConfig.KtiSkuMismatchCheck;
  UpiPolicyPpi->Upi.IrqThreshold              = SetupData->SocketConfig.UpiConfig.IrqThreshold;
  UpiPolicyPpi->Upi.TorThresLoctoremNorm      = SetupData->SocketConfig.UpiConfig.TorThresLoctoremNorm;
  UpiPolicyPpi->Upi.TorThresLoctoremEmpty     = SetupData->SocketConfig.UpiConfig.TorThresLoctoremEmpty;
  UpiPolicyPpi->Upi.IoDcMode                  = SetupData->SocketConfig.UpiConfig.IoDcMode;

  UpiPolicyPpi->Upi.KtiAdaptationEn         = SetupData->SocketConfig.UpiConfig.KtiAdaptationEn;
  UpiPolicyPpi->Upi.KtiAdaptationSpeed      = SetupData->SocketConfig.UpiConfig.KtiAdaptationSpeed;
#endif//if MAX_SOCKET > 1

  //
  //These two knobs are hidden for SNR, but we can use their default values.
  //
  UpiPolicyPpi->Upi.SncEn             = SetupData->SocketConfig.UpiConfig.SncEn;
  UpiPolicyPpi->Upi.XptPrefetchEn     = SetupData->SocketConfig.UpiConfig.XptPrefetchEn;
  if (UpiPolicyPpi->Upi.SncEn == KTI_DISABLE) {
    UpiPolicyPpi->Upi.UmaClustering     = SetupData->SocketConfig.CommonRcConfig.UmaBasedClustering;    //GetUmaBasedClustering ();
  } else {
    UpiPolicyPpi->Upi.UmaClustering   = UMABASEDCLUSTERING_DISABLED;
  }

  UpiPolicyPpi->Upi.StaleAtoSOptEn    = SetupData->SocketConfig.UpiConfig.StaleAtoSOptEn;
  UpiPolicyPpi->Upi.LLCDeadLineAlloc  = SetupData->SocketConfig.UpiConfig.LLCDeadLineAlloc;

  //
  //Set CFR config according CFR knobs.
  //
  UpdateCFRConfig (UpiPolicyPpi, SetupData);

  //
  // Set DDRT QoS Cfg, and it is only for ICX-SP and SPR. ICX-D and SNR don't support DDRT.
  //
  if (IsSnrOrIcxd) {
    UpiPolicyPpi->Upi.DdrtQosMode = KTI_DISABLE;
  } else {
    UpiPolicyPpi->Upi.DdrtQosMode = SetupData->SocketConfig.MemoryConfig.NvmQos;
  }

  UpiPolicyPpi->Upi.BusRatio[0] = SetupData->SocketConfig.UpiConfig.BusRatio[0];
#if (MAX_SOCKET > 1)
  UpiPolicyPpi->Upi.BusRatio[1] = SetupData->SocketConfig.UpiConfig.BusRatio[1];
#endif
#if (MAX_SOCKET > 2)
  UpiPolicyPpi->Upi.BusRatio[2] = SetupData->SocketConfig.UpiConfig.BusRatio[2];
#endif
#if (MAX_SOCKET > 3)
  UpiPolicyPpi->Upi.BusRatio[3] = SetupData->SocketConfig.UpiConfig.BusRatio[3];
#endif
#if (MAX_SOCKET > 4)
  UpiPolicyPpi->Upi.BusRatio[4] = SetupData->SocketConfig.UpiConfig.BusRatio[4];
#endif
#if (MAX_SOCKET > 5)
  UpiPolicyPpi->Upi.BusRatio[5] = SetupData->SocketConfig.UpiConfig.BusRatio[5];
#endif
#if (MAX_SOCKET > 6)
  UpiPolicyPpi->Upi.BusRatio[6] = SetupData->SocketConfig.UpiConfig.BusRatio[6];
#endif
#if (MAX_SOCKET > 7)
  UpiPolicyPpi->Upi.BusRatio[7] = SetupData->SocketConfig.UpiConfig.BusRatio[7];
#endif

if (FixedPcdGetBool (PcdBiosDfxKnobEnabled) == 1) {

  UpiPolicyPpi->Upi.DfxParm.DfxHaltLinkFailReset          = SetupData->SocketConfig.UpiConfig.DfxHaltLinkFailReset;
  UpiPolicyPpi->Upi.DfxParm.DfxKtiMaxInitAbort            = SetupData->SocketConfig.UpiConfig.DfxKtiMaxInitAbort;
  UpiPolicyPpi->Upi.DfxParm.DfxLlcShareDrdCrd             = SetupData->SocketConfig.UpiConfig.DfxLlcShareDrdCrd;
  UpiPolicyPpi->Upi.DfxParm.DfxBiasFwdMode                = SetupData->SocketConfig.UpiConfig.DfxBiasFwdMode;
  UpiPolicyPpi->Upi.DfxParm.DfxSnoopFanoutEn              = SetupData->SocketConfig.UpiConfig.DfxSnoopFanoutEn;
  UpiPolicyPpi->Upi.DfxParm.DfxHitMeEn                    = SetupData->SocketConfig.UpiConfig.DfxHitMeEn;
  UpiPolicyPpi->Upi.DfxParm.DfxFrcfwdinv                  = SetupData->SocketConfig.UpiConfig.DfxFrcfwdinv;
  UpiPolicyPpi->Upi.DfxParm.DfxDbpEnable                  = SetupData->SocketConfig.UpiConfig.DfxDbpEnable;
  UpiPolicyPpi->Upi.DfxParm.DfxOsbEn                      = SetupData->SocketConfig.UpiConfig.DfxOsbEn;
  UpiPolicyPpi->Upi.DfxParm.DfxHitMeRfoDirsEn             = SetupData->SocketConfig.UpiConfig.DfxHitMeRfoDirsEn;
  UpiPolicyPpi->Upi.DfxParm.DfxGateOsbIodcAllocEn         = SetupData->SocketConfig.UpiConfig.DfxGateOsbIodcAllocEn;
  UpiPolicyPpi->Upi.DfxParm.DfxDualLinksInterleavingMode  = SetupData->SocketConfig.UpiConfig.DfxDualLinksInterleavingMode;
  UpiPolicyPpi->Upi.DfxParm.DfxSystemDegradeMode          = SetupData->SocketConfig.UpiConfig.DfxSystemDegradeMode;
  UpiPolicyPpi->Upi.DfxParm.DfxVn1En                      = SetupData->SocketConfig.UpiConfig.DfxVn1En;
  UpiPolicyPpi->Upi.DfxParm.DfxD2cEn                      = SetupData->SocketConfig.UpiConfig.DfxD2cEn;
  UpiPolicyPpi->Upi.DfxParm.DfxD2kEn                      = SetupData->SocketConfig.UpiConfig.DfxD2kEn;
  UpiPolicyPpi->Upi.DfxParm.DfxLockMaster                 = SetupData->SocketConfig.UpiConfig.DfxLockMaster;
  UpiPolicyPpi->Upi.DfxParm.DfxOsbLocRd                   = SetupData->SocketConfig.UpiConfig.DfxOsbLocRd;
  UpiPolicyPpi->Upi.DfxParm.DfxOsbLocRdCur                = SetupData->SocketConfig.UpiConfig.DfxOsbLocRdCur;
  UpiPolicyPpi->Upi.DfxParm.DfxOsbRmtRd                   = SetupData->SocketConfig.UpiConfig.DfxOsbRmtRd;
  UpiPolicyPpi->Upi.DfxParm.DfxCleanEvictAlwaysLive       = SetupData->SocketConfig.UpiConfig.DfxCleanEvictAlwaysLive;
  UpiPolicyPpi->Upi.DfxParm.DfxModifiedEvictAlwaysLive    = SetupData->SocketConfig.UpiConfig.DfxModifiedEvictAlwaysLive;
  UpiPolicyPpi->Upi.DfxParm.DfxM3KtiCountMismatchEn       = SetupData->SocketConfig.UpiConfig.DfxM3KtiCountMismatchEn;
  UpiPolicyPpi->Upi.DfxParm.DfxSnoopFanoutChaInterleaveEn = SetupData->SocketConfig.UpiConfig.DfxSnoopFanoutChaInterleaveEn;
  UpiPolicyPpi->Upi.DfxParm.DfxXptFifoEnabledCredit       = SetupData->SocketConfig.UpiConfig.DfxXptFifoEnabledCredit;

  UpiPolicyPpi->Upi.DfxParm.DfxClockModulationEn          = SetupData->SocketConfig.UpiConfig.DfxClockModulationEn;
  UpiPolicyPpi->Upi.DfxParm.DfxMdfisAdvancedTrainingEn    = SetupData->SocketConfig.UpiConfig.DfxMdfisAdvancedTrainingEn;
  UpiPolicyPpi->Upi.DfxParm.DfxCxlSecLvl                  = SetupData->SocketConfig.UpiConfig.DfxCxlSecLvl;
  UpiPolicyPpi->Upi.DfxParm.DfxCxlStcge                   = SetupData->SocketConfig.UpiConfig.DfxCxlStcge;
  UpiPolicyPpi->Upi.DfxParm.DfxCxlSdcge                   = SetupData->SocketConfig.UpiConfig.DfxCxlSdcge;
  UpiPolicyPpi->Upi.DfxParm.DfxCxlDlcge                   = SetupData->SocketConfig.UpiConfig.DfxCxlDlcge;
  UpiPolicyPpi->Upi.DfxParm.DfxCxlLtcge                   = SetupData->SocketConfig.UpiConfig.DfxCxlLtcge;
  UpiPolicyPpi->Upi.DfxParm.DfxCxlVid                     = SetupData->SocketConfig.UpiConfig.DfxCxlVid;

  UpiPolicyPpi->Upi.DfxParm.DfxPmonConfig  = SetupData->SocketConfig.UpiConfig.DfxPmonConfig;
  UpiPolicyPpi->Upi.DfxParm.DfxM2IosfPmonAccessControl  = SetupData->SocketConfig.UpiConfig.DfxM2IosfPmonAccessControl;
  UpiPolicyPpi->Upi.DfxParm.DfxIioStackEn  = SetupData->SocketConfig.UpiConfig.DfxIioStackEn;
  UpiPolicyPpi->Upi.DfxParm.DfxHqmEn  = SetupData->SocketConfig.UpiConfig.DfxHqmEn;

  UpiPolicyPpi->Upi.DfxParm.DfxIioDfxEnabled              = SetupData->SocketConfig.IioConfig.DFXEnable;
  //
  // DFX Per port option
  //
  CopyMem (KtiDfxCpuLink, (UINT8 *)(&SetupData->SocketConfig.UpiConfig.DfxKtiCpuPerPortStartTag) + 1, MAX_SOCKET * MAX_KTI_PORTS * 4);
  for (Ctr1 = 0; Ctr1 < MAX_SOCKET; Ctr1++) {
    for (Ctr2 = 0; Ctr2 < KtiPortCnt; Ctr2++) {
      pKtiDfxCpuLink = (KTI_DFX_CPU_LINK_SETTING *) &(UpiPolicyPpi->Upi.DfxCpuCfg[Ctr1].Link[Ctr2]);
      pKtiDfxCpuPhy  = (KTI_DFX_CPU_PHY_SETTING *) &(UpiPolicyPpi->Upi.DfxCpuCfg[Ctr1].Phy[Ctr2]);

      pKtiDfxCpuLink->DfxCrcMode      = KtiDfxCpuLink[Ctr1][Ctr2][0] & 0x3;
      pKtiDfxCpuLink->DfxL0pEnable    = KtiDfxCpuLink[Ctr1][Ctr2][1] & 0x3;
      pKtiDfxCpuLink->DfxL1Enable     = KtiDfxCpuLink[Ctr1][Ctr2][2] & 0x3;
      pKtiDfxCpuPhy->DfxKtiFailoverEn = KtiDfxCpuLink[Ctr1][Ctr2][3] & 0x3;
    }
  }
}
  //
  // Per port option
  //
#if MAX_SOCKET > 1
  PerPortOption = ((UINT8 *)(&SetupData->SocketConfig.UpiConfig.KtiCpuPerPortStartTag) + 1);
  for (Ctr1 = 0; Ctr1 < MAX_SOCKET; Ctr1++) {
    for (Ctr2 = 0; Ctr2 < KtiPortCnt; Ctr2++) {

      pKtiCpuLink = (KTI_CPU_LINK_SETTING *) &(UpiPolicyPpi->Upi.PhyLinkPerPortSetting[Ctr1].Link[Ctr2]);
      pKtiCpuPhy = (KTI_CPU_PHY_SETTING *) &(UpiPolicyPpi->Upi.PhyLinkPerPortSetting[Ctr1].Phy[Ctr2]);

      pKtiCpuLink->KtiPortDisable = (*(PerPortOption++)) & 0x1;                   // This option is per-port only

      if (SetupData->SocketConfig.UpiConfig.QpiLinkSpeed == FREQ_PER_LINK) {         // Implies "Per Link Setting" 7 = FREQ_PER_LINK
        pKtiCpuPhy->KtiLinkSpeed = (*(PerPortOption++)) & 0x7;                      // Use per port option
      } else {
        pKtiCpuPhy->KtiLinkSpeed =  SetupData->SocketConfig.UpiConfig.QpiLinkSpeed & 0x7;      // Use system wide option
        PerPortOption++;
      }

      if (SetupData->SocketConfig.UpiConfig.KtiLinkVnaOverride == KTI_LL_VNA_SETUP_PER_LINK) { // Implies "Per Link Setting"
        pKtiCpuLink->KtiLinkVnaOverride = (*(PerPortOption++)) & 0x7f;                        // Use per port option
      } else {
        pKtiCpuLink->KtiLinkVnaOverride = SetupData->SocketConfig.UpiConfig.KtiLinkVnaOverride & 0x7f; // Use system wide option
        PerPortOption++;
      }
    }
  }
#endif//if MAX_SOCKET > 1

  UpiPolicyPpi->Upi.mmCfgBase = SetupData->SocketConfig.CommonRcConfig.MmcfgBase;
  UpiPolicyPpi->Upi.mmCfgSize = SetupData->SocketConfig.CommonRcConfig.MmcfgSize;

  switch(SetupData->SocketConfig.CommonRcConfig.MmiohBase){
    case 0:
      UpiPolicyPpi->Upi.mmiohBase = 0x3800;
      break;
    case 1:
      UpiPolicyPpi->Upi.mmiohBase = 0x2800;
      break;
    case 2:
      UpiPolicyPpi->Upi.mmiohBase = 0x2000;        // 32TB
      break;
    case 3:
      UpiPolicyPpi->Upi.mmiohBase = 0x1800;
      break;
    case 4:
      UpiPolicyPpi->Upi.mmiohBase = 0x1000;
      break;
    case 5:
      UpiPolicyPpi->Upi.mmiohBase = 0x400;
      break;
    case 6:
      UpiPolicyPpi->Upi.mmiohBase = 0x200;
      break;
    case 7:
      UpiPolicyPpi->Upi.mmiohBase = 0x100;
      break;
    case 8:
      UpiPolicyPpi->Upi.mmiohBase = 0x80;
      break;
    case 9:
      UpiPolicyPpi->Upi.mmiohBase = MMIOH_BASE_ICX;
      break;
    default:
      UpiPolicyPpi->Upi.mmiohBase = MMIOH_BASE;
      break;
  }
  UpiPolicyPpi->Upi.mmiohSize = SetupData->SocketConfig.CommonRcConfig.MmiohSize;


  UpiPolicyPpi->Upi.highGap = HIGH_GAP;            // High MMIO gap = 256 MB

  if (SetupData->SocketConfig.CommonRcConfig.IsocEn >= 2) {
    UpiPolicyPpi->Upi.isocEn = 0;
  } else {
    UpiPolicyPpi->Upi.isocEn = SetupData->SocketConfig.CommonRcConfig.IsocEn;
  }

  //
  // Initialize DCA Enable to '0' as DCA was replaced by TPH
  //
  UpiPolicyPpi->Upi.dcaEn = 0;

  //
  // Optional: indicate which sockets are expected to be connected to coherent FPGA
  //
  for (Soc = 0; Soc < MAX_SOCKET; Soc ++) {
    UpiPolicyPpi->Upi.KtiFpgaEnable[Soc] = KTI_AUTO;
  }

  UpiPolicyPpi->Upi.TscSyncEn = SetupData->SocketConfig.UpiConfig.TscSyncEn;
  //
  // TSC SYNC knob AUTO sets to Enable for LR board.
  //
  if (UpiPolicyPpi->Upi.TscSyncEn == KTI_AUTO) {
    PlatformType = GetPlatformType ();

    if (PlatformType >= TypeLightningRidgeEXRP) {
      UpiPolicyPpi->Upi.TscSyncEn = KTI_ENABLE;
    } else {
      UpiPolicyPpi->Upi.TscSyncEn = KTI_DISABLE;
    }
  }

  UpiPolicyPpi->Upi.BoardTypeBitmask = GetBoardTypeBitmask();

  GetKtiLaneEparam ((ALL_LANES_EPARAM_LINK_INFO **)&(UpiPolicyPpi->Upi.AllLanesPtr),
                    &(UpiPolicyPpi->Upi.AllLanesSizeOfTable),
                    (PER_LANE_EPARAM_LINK_INFO **)&(UpiPolicyPpi->Upi.PerLanePtr),
                    &(UpiPolicyPpi->Upi.PerLaneSizeOfTable)
                  );

  UpiPolicyPpi->Upi.WaitTimeForPSBP = WAIT_TIME_FOR_PSBP;
  UpiPolicyPpi->Upi.IsKtiNvramDataReady = ReadKtiNvram ();

  UpiPolicyPpi->Upi.OemHookPostTopologyDiscovery = (UINT32)OemHookPostTopologyDiscovery;
  UpiPolicyPpi->Upi.OemGetResourceMapUpdate = (UINT32)OemGetResourceMapUpdate;
  UpiPolicyPpi->Upi.OemGetAdaptedEqSettings = (UINT32)OemGetAdaptedEqSettings;
  UpiPolicyPpi->Upi.OemCheckCpuPartsChangeSwap = (UINT32)OemCheckCpuPartsChangeSwap;

  UpiPolicyPpi->Upi.WaSerializationEn = PcdGetBool(PcdWaSerializationEn);
  UpiPolicyPpi->Upi.KtiInEnableMktme = SetupData->SocketConfig.SocketProcessorCoreConfiguration.EnableMktme;
  return EFI_SUCCESS;
}

