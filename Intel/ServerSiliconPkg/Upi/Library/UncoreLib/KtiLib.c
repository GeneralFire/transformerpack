/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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


#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <KtiMisc.h>
#include <KtiSetupDefinitions.h>
#include <Upi/KtiSi.h>
#include <UncoreCommonIncludes.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/CheckpointLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/IoAccessLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <RcRegs.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>
#include "UncoreLibInternal.h"
#include <Library/ChaLib.h>
#include <Library/PcuIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Guid/MrcPlatformHooksGuid.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/SiliconWorkaroundLib.h>

// APTIOV_SERVER_OVERRIDE_RC_START
#include <AmiMrcHooksWrapperLib/AmiMrcHooksWrapperLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END


/**
  Returns WayNess Type.

  @param SocId         - Soc ID of the CPU who's Ways is requested, 0...7
  @param SocketWays    - Socket ways

  @retval 0 - Successful
  @retval 1 - Failure
 **/
KTI_STATUS
GetSocketWays (
  UINT8            SocId,
  UINT8            *SocketWays
  )
{

  *SocketWays = PcuGetWayness (SocId);

  return KTI_SUCCESS;
}

/**
Update Max Kti Agent for a given CPU socket.

@param KtiInternalGlobal - KTIRC internal variables.
@param SocId             - Soc ID

@retval 0 - Successful
@retval 1 - Failure

**/
KTI_STATUS
UpdateMaxKtiAgentAndM3KtiCount (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     SocId
 )
{
  UINT8  MaxM3Kti;
  UINT8  CpuType;

  CpuType = KtiInternalGlobal->CpuType;

  if (CpuType == CPU_CPX) {     //check if is CPX
    MaxM3Kti = MAX_CPX_M3KTI;
  } else if (IsSkxFamilyCpu (CpuType)) {           //for SKX/CLX
    MaxM3Kti = MAX_SKX_M3KTI;
  } else if (IsSprFamilyCpu (CpuType)) {           //for SPR
    MaxM3Kti = MAX_SPR_M3KTI;
  } else if (IsGnrSrfFamilyCpu (CpuType)) {        //for GNR/SRF
    MaxM3Kti = MAX_SPR_M3KTI;  // This would be changed to GNR/SRF in HSD 14010367046
  } else {                                        //for ICX/SPR
    MaxM3Kti = MAX_ICX_M3KTI;
  }

  KTIVAR->MaxKtiAgent[SocId] = KtiInternalGlobal->KtiPortCnt;
  KTIVAR->MaxM3Kti[SocId] = MaxM3Kti;

  return KTI_SUCCESS;
}

/**
  Returns the total M2Mem enabled for a given CPU socket.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocId             - Soc ID of the CPU who's enabled M2Mem info is requested, 0...3
  @param M2MemCount        - Total M2Mems in given socket

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
GetM2MemCount (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     SocId,
  UINT8                     *M2MemCount
  )
{
  UINT8   Ctr, MaxM2mem;
  UINT8   Count = 0;
  UINT16  Data16;
  UINT8   CpuType;
  UINT8   DieCount;
  UINT32  M2mBitmap;
  UINT32  Capid3Value;
  UINT32  HbmMemInstBitmap;

  CpuType    = KtiInternalGlobal->CpuType;
  MaxM2mem = 0;
  DieCount = 0;
  M2mBitmap = 0;

  if (IsSprFamilyCpu (CpuType)) {
    Capid3Value = PcuGetCapid3RegisterValue (SocId);

    M2mBitmap |= ((Capid3Value >> 23) & 0x0000000F);  //DDR Memory Instance
    for (Ctr = 0; Ctr < 4; Ctr++) {
      if ((M2mBitmap & (1 << Ctr)) != 0) {
        Count++;
      }
    }

    if (KTIVAR->HbmSku) {  //SPR+HBM
      HbmMemInstBitmap = ((Capid3Value >> 27) & 0x0000000F);

      for (Ctr = 0; Ctr < 4; Ctr++) {
        if ((HbmMemInstBitmap & (1 << Ctr)) != 0) {
          M2mBitmap |= (0xF << (4 + Ctr * 4));  //HBM Memory Instance ID starts from 4, up to 19
          Count += 4;
        }
      }
    }
  } else {
    if (IsGnrSrfFamilyCpu (CpuType)) {
      MaxM2mem = MAX_GNRSRF_M2MEM;
    } else if (IsSkxFamilyCpu (CpuType)) {
      MaxM2mem = MAX_SKX_M2MEM;
    } else if (IsIcxFamilyCpu (CpuType)) {
      if (CpuType == CPU_ICXD) {
        MaxM2mem = MAX_ICXD_M2MEM;
      } else if (CpuType == CPU_SNR || CpuType == CPU_TNR) {
        MaxM2mem = MAX_SNR_M2MEM;
      } else {
        MaxM2mem = MAX_ICX_M2MEM; //ICX-SP
      }
    }

    for (Ctr = 0; Ctr < MaxM2mem; Ctr++) {
      Data16 = (UINT16) GetM2mVid (SocId, Ctr);
      if (Data16 != 0xFFFF) {
        M2mBitmap |= (1 << Ctr);
        Count++;
      }
    }
  }

  KTIVAR->CpuInfo[SocId].M2mBitmap = M2mBitmap;

  *M2MemCount = Count;
  return KTI_SUCCESS;
}

/**
  Get system RAS type and save this info into KTI internal global buffer and KTI output buffer.

  @param SocId             - Soc ID of the CPU who's enabled M2Pcie info is requested, 0...3
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
GetRasType (
  UINT8                      SocId,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8   SegType;
  BOOLEAN Scalable, CpuHotAdd;
  UINT32  RasType;

  SegType   = PcuGetSegType (SocId);
  Scalable  = PcuIsScalable (SocId);
  CpuHotAdd = PcuIsCpuHotAddEn (SocId);

  RasType = GetRasTypeByCapRegister (SocId);

  if (!IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    switch ( RasType) {
      case 0:
        KtiInternalGlobal->RasType[SocId] = HEDT_RAS;
        break;
      case 5:
        KtiInternalGlobal->RasType[SocId] = S1WS_RAS;
        break;
      case 6:
        KtiInternalGlobal->RasType[SocId] = CORE_RAS;
        break;
      case 1:
        KtiInternalGlobal->RasType[SocId] = STANDARD_RAS;
        break;
      case 7:
        KtiInternalGlobal->RasType[SocId] = FPGA_RAS;
        break;
      case 3:
        KtiInternalGlobal->RasType[SocId] = ADVANCED_RAS;
        break;
      case 9:
        KtiInternalGlobal->RasType[SocId] = STANDARD_PLUS_RAS;
        break;
      default :
        if (CheckEmulationType (SIMICS_FLAG)) {
          KtiInternalGlobal->RasType[SocId] = STANDARD_RAS;
        } else {
          KtiInternalGlobal->RasType[SocId] = UNDEFINED_RAS;
        }
        break;
    }
  } else {
    /**
    CAPID2_[4:0] indicate RAS levels: 5'b00000,
                                 HEDT; 5'b00001, Std; 5'b00010, HPC; 5'b00011,
                                 Adv; 5'b00101, 1SWS; 5'b00111, FPGA; 5'b01000,
                                 Cirrus 5'b01001, Core;
    **/
    switch ( RasType) {
      case 0:
        KtiInternalGlobal->RasType[SocId] = HEDT_RAS;
        break;
      case 5:
        KtiInternalGlobal->RasType[SocId] = S1WS_RAS;
        break;
      case 9:
        KtiInternalGlobal->RasType[SocId] = CORE_RAS;
        break;
      case 1:
        KtiInternalGlobal->RasType[SocId] = STANDARD_RAS;
        break;
      case 7:
        KtiInternalGlobal->RasType[SocId] = FPGA_RAS;
        break;
      case 3:
        KtiInternalGlobal->RasType[SocId] = ADVANCED_RAS;
        break;
      case 2:
        KtiInternalGlobal->RasType[SocId] = HPC_RAS;
        break;
      case 8:
        KtiInternalGlobal->RasType[SocId] = CIRRUS_RAS;
        break;
      default :
        if (CheckEmulationType (SIMICS_FLAG)) {
          KtiInternalGlobal->RasType[SocId] = STANDARD_RAS;
        } else {
          KtiInternalGlobal->RasType[SocId] = UNDEFINED_RAS;
        }
        break;
    }
  }

  //
  // Set system to lowest common Ras set
  //
  if (KTIVAR->OutSystemRasType > KtiInternalGlobal->RasType[SocId]) {
    KTI_HOST_OUT_PTR->SystemRasType = KTIVAR->OutSystemRasType = KtiInternalGlobal->RasType[SocId];
  }

  return KTI_SUCCESS;
}

/**
  Returns the Legacy Pch Socket Id for this CPU socket.

  @param SocId          - Soc ID of the calling CPU
  @param LegacyPchSktId - Legacy PCH Socket ID

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
GetLegacyPchTargetSktId (
  UINT8            SocId,
  UINT8            *LegacyPchSktId
  )
{
  *LegacyPchSktId = GetSysSbspSocketIdNv ();

  return KTI_SUCCESS;
}

/**
  Assign the real value for MMCFG size and base, MMIOL base and size and validate these value.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param TotalCpuNumber    - Socket number

  @retval KTI_SUCCESS

**/
KTI_STATUS
UpdateMmcfgMmiolBaseSize (
  IN OUT KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  UINT32   Data32;
  UINT8    CpuType;
  UINT8    TotalCpuNumber;
  UINT8    MmcfgBaseOptionValue;
  UINT8    MmcfgSizeOptionValue;

  //
  // Initialize CpuType in KtiInternalGlobal
  //
  KtiGetCpuTypeInfo (KtiInternalGlobal);
  CpuType     = KtiInternalGlobal->CpuType;

  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE) {
    if (KTISETUP->KtiCpuSktHotPlugTopology == 0) {
      TotalCpuNumber =  4;         //online/offline based on 4S topology
    } else {
      TotalCpuNumber =  8;         //online/offline based on 8S topology
    }
  } else {
    TotalCpuNumber =  KtiInternalGlobal->TotCpu;
  }

  //
  // Validate MMCFG base and size value from Setup
  //
  KTI_ASSERT ((KTISETUP->mmCfgBase <= MMCFG_BASE_AUTO), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_67);
  KTI_ASSERT ((KTISETUP->mmCfgSize <= MMCFG_SIZE_AUTO), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_68);

  //
  // Set MMCFG base/size for auto value,
  // if socket number <= 4, base:2G, size:256M;
  // else 8 sockets, base: 2G, size 512M.
  //
  if (KTISETUP->mmCfgBase == MMCFG_BASE_AUTO){
    MmcfgBaseOptionValue = MMCFG_BASE_2G;
  } else {
    MmcfgBaseOptionValue = KTISETUP->mmCfgBase;
  }

  if (KTISETUP->mmCfgSize == MMCFG_SIZE_AUTO){
    if (TotalCpuNumber < 4){
      MmcfgSizeOptionValue = MMCFG_SIZE_256M;
    } else if (TotalCpuNumber == 4){
      if (IsSprFamilyCpu (CpuType) || IsGnrSrfFamilyCpu (CpuType)) {
        MmcfgSizeOptionValue = MMCFG_SIZE_512M;
      } else {
        MmcfgSizeOptionValue = MMCFG_SIZE_256M;
      }
    } else {
      MmcfgSizeOptionValue = MMCFG_SIZE_512M;
    }

    if ((KtiInternalGlobal->TotCpu > 1) && IsSiliconWorkaroundEnabled("S14011307716")){
      switch (KtiInternalGlobal->TotCpu){
        case 2:
          MmcfgSizeOptionValue = MMCFG_SIZE_512M;
          break;
        case 4:
          MmcfgSizeOptionValue = MMCFG_SIZE_1G;
          break;
        case 8:
          MmcfgSizeOptionValue = MMCFG_SIZE_2G;
          break;
        default:
          break;
      }
    }
  } else {
    MmcfgSizeOptionValue = KTISETUP->mmCfgSize;
  }

  //
  // Obtain MMCFG base and size
  //
  switch (MmcfgBaseOptionValue){
    case MMCFG_BASE_1G:
      KTIVAR->mmCfgBase = (UINT32)1*1024*1024*1024;
      break;
    case MMCFG_BASE_1_5G:
      KTIVAR->mmCfgBase = (UINT32)3*512*1024*1024;
      break;
    case MMCFG_BASE_1_75G:
      KTIVAR->mmCfgBase = (UINT32)7*256*1024*1024;
      break;
    case MMCFG_BASE_2G:
      KTIVAR->mmCfgBase = (UINT32)2*1024*1024*1024;
      break;
    case MMCFG_BASE_2_25G:
      KTIVAR->mmCfgBase = (UINT32)9*256*1024*1024;
      break;
    case MMCFG_BASE_3G:
      KTIVAR->mmCfgBase = (UINT32)3*1024*1024*1024;
      break;
    default:
      KTIVAR->mmCfgBase = (UINT32)2*1024*1024*1024;
      break;
  }

  switch(MmcfgSizeOptionValue){
    case MMCFG_SIZE_64M:
      KTIVAR->mmCfgSize = 64*1024*1024;
      break;
    case MMCFG_SIZE_128M:
      KTIVAR->mmCfgSize = 128*1024*1024;
      break;
    case MMCFG_SIZE_256M:
      KTIVAR->mmCfgSize = 256*1024*1024;
      break;
    case MMCFG_SIZE_512M:
      KTIVAR->mmCfgSize = 512*1024*1024;
      break;
    case MMCFG_SIZE_1G:
      KTIVAR->mmCfgSize = (UINT32)1024*1024*1024;
      break;
    case MMCFG_SIZE_2G:
      KTIVAR->mmCfgSize = (UINT32)2048*1024*1024;
      break;
    default:
      KTIVAR->mmCfgSize = 256*1024*1024;
      break;
  }

  //
  // Update MMIOL base, size and lowGap.
  //
  KTIVAR->mmiolBase = KTIVAR->mmCfgBase + KTIVAR->mmCfgSize;
  KTIVAR->mmiolSize = MMIOL_LIMIT - KTIVAR->mmiolBase + 1;
  KTIVAR->lowGap = FOUR_GB_MEM - (KTIVAR->mmCfgBase >> 26);


  Data32 = MMIOL_LIMIT - KTIVAR->mmCfgBase + 1;
  KTI_ASSERT ((KTIVAR->mmCfgSize <= Data32), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_69);

  //
  // Make sure MMCFG & MMIOL regions are not overlapping
  //
  KTI_ASSERT ((KTIVAR->mmCfgBase != KTIVAR->mmiolBase), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_6A);

  if (KTIVAR->mmCfgBase < KTIVAR->mmiolBase) {
    Data32 = KTIVAR->mmiolBase - KTIVAR->mmCfgBase;
    KTI_ASSERT ((Data32 >= KTIVAR->mmCfgSize), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_6B);
  } else {
    Data32 = KTIVAR->mmCfgBase - KTIVAR->mmiolBase;
    KTI_ASSERT ((Data32 >= KTIVAR->mmiolSize), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_6C);
  }

  return KTI_SUCCESS;
}
/**
  Ration the Bus/IO/MMIO/IOAPIC resources using the ratio specified for the CPU
  sockets populated. If equal distribution is not possible, assign the remaining resources
  to the CPUs with lowest NID. Legacy CPU is always allocated space at the base of the
  resource region. Other CPUs are assigned resources in the increasing order of Socket ID.
  Example: CPU2's resource base address will be greater than CPU1's resource base address.

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS

**/
KTI_STATUS
AllocateIioResources (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8          TmpTotCpu;
  UINT8          BusSize[MAX_SOCKET];

  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_PROGRAM_FINAL_IO_SAD, MINOR_STS_ALLOCATE_CPU_RESOURCE);

  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE) {
    if (KTISETUP->KtiCpuSktHotPlugTopology == 0) {
      TmpTotCpu =  4;         //online/offline based on 4S topology
    } else {
      TmpTotCpu =  8;         //online/offline based on 8S topology
    }
  } else {
    TmpTotCpu =  KtiInternalGlobal->TotCpu;
  }

  //
  // Check we don't overrun structures
  //
  KTI_ASSERT ((TmpTotCpu <= MAX_SOCKET), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_8);

  //
  // Fill in stack structure for hotplug systems based on SBSP.  This allows for preallocation of resources
  //
  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE) {
    UpdateStructureForHotPlug (SocketData, KtiInternalGlobal, TmpTotCpu);
  }

  // Calculate the resources for each CPU based on input given to KTIRC
  CalculateBusResourceRatio (SocketData, KtiInternalGlobal, BusSize, TmpTotCpu);

  CalculateBusResources (SocketData, KtiInternalGlobal, BusSize, TmpTotCpu);

  CalculateIoResources (SocketData, KtiInternalGlobal, TmpTotCpu);

  CalculateIoApicResources (SocketData, KtiInternalGlobal);

  CalculateMmiolResources (SocketData, KtiInternalGlobal, TmpTotCpu);
  CalculateMmiohResources (SocketData, KtiInternalGlobal, TmpTotCpu);

  //
  // Check for OEM override
  //
  CheckForOemResourceUpdate (SocketData, KtiInternalGlobal);

  //
  // Get min granularity per stack for later perf programming.
  //
  CalculateMmiolGranularity (SocketData, KtiInternalGlobal);
  CalculateMmiohGranularity (SocketData, KtiInternalGlobal);

  //
  // Make sure at least 32M of MMCFG space is available for each CPU.
  //
  if (KTIVAR->mmCfgSize < (UINT32)(TmpTotCpu * 32 * 1024 * 1024)) {
    KtiCheckPoint (0xFF, 0xFF, 0xFF, ERR_SAD_SETUP, MINOR_ERR_INSUFFICIENT_RESOURCE_SPACE);
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid MMCFG Size Supplied. "));
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Each CPU must be allocated at least 32MB of MMCFG space"));
    KTI_ASSERT (FALSE, ERR_SAD_SETUP, MINOR_ERR_INSUFFICIENT_RESOURCE_SPACE);
  }


  //
  // Export Granularity info in IioUds
  //
  KTIVAR->OutIoGranularity    = SAD_LEG_IO_GRANTY;
  KTIVAR->OutMmiolGranularity = SAD_MMIOL_GRANTY;

  KTIVAR->tolmLimit = ((FOUR_GB_MEM - KTIVAR->lowGap) & ~0x3);

  KTIVAR->OutMmiohGranularity.lo = 1;
  KTIVAR->OutMmiohGranularity.hi = 0;
  KTIVAR->OutMmiohGranularity = LShiftUINT64(KTIVAR->OutMmiohGranularity, KtiInternalGlobal->MmiohGranularity);

  if (IsSkxFamilyCpu (KtiInternalGlobal->CpuType)) {
    KTIVAR->OutUboxMmioSize = 0;
  } else {
    KTIVAR->OutUboxMmioSize = UBOX_MMIO_BAR_SIZE;
  }

  CalculateLatestConsumedPA (SocketData, KtiInternalGlobal);
  //
  // Dump Resource allocation
  //
  DataDumpKtiResourceAllocation (SocketData, KtiInternalGlobal);

  return KTI_SUCCESS;
}


/**
  For Hotplug systems, this routine updates the internal IIO stack structures to allow for preallocation of system resources.

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param TmpTotCpu         - NumberofCpus for purpose of resource allocation

  @retval KTI_SUCCESS

**/
KTI_STATUS
UpdateStructureForHotPlug (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      TmpTotCpu
  )
{
  UINT8               Stack;
  UINT8               Socket;

  //
  // Check input
  //
  KTI_ASSERT ((TmpTotCpu <= MAX_SOCKET), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_9);

  for (Socket = 0; Socket < TmpTotCpu; Socket++) {
    if (SocketData->Cpu[Socket].Valid != TRUE) {
      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality = KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].Personality;
        if ((KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality == TYPE_UBOX_IIO)
          || (KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality == TYPE_MCP)
          || (KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality == TYPE_DINO))
        {
          KtiInternalGlobal->TotSadTargets = KtiInternalGlobal->TotSadTargets + 1;
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  This routine assigns resources for CPUs that are populated. It consults the appropriate inputs given to KTIRC and
  applies the following set of rules:
    1. Resource will be allocated only for the CPUs that are currently populated.
       (Except IOAPIC range which is always reserved for all 4 sockets; this makes ACPI & other IOAPIC related setting easier)
    2. If a CPU is populated, bus resource must be requested by the platform. Otherwise force the resource ratio to 1 for that CPU.

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param BusSize           - Calculated Bus size for CPUs 0..3 (Granularity is 32 buses)
  @param TmpTotCpu         - NumberofCpus for purpose of resource allocation

  @retval KTI_SUCCESS

**/
KTI_STATUS
CalculateBusResourceRatio (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      *BusSize,
  UINT8                      TmpTotCpu
  )
{
  BOOLEAN        WarnFlag;
  UINT8          Idx1, Idx2, AvailBusChunk;
  UINT8          TotBusChunk;
  UINT8          ResiBusChunk;
  UINT8          TmpBusRatio[MAX_SOCKET];
  UINT8          MaxSocket;
  UINT32         SocketMmcfgSize;
  UINT8          TmpBusSize;
  UINT8          FpgaCount = 0;

  //
  // Check input
  //
  KTI_ASSERT ((TmpTotCpu <= MAX_SOCKET), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_A);

  MaxSocket = TmpTotCpu;

  if (KTIVAR->mmCfgSize > 256*1024*1024) {
    //
    // Multiseg mode.  Assign at most a whole segment to each socket
    //

    SocketMmcfgSize = ((KTIVAR->mmCfgSize / MaxSocket) >> 20);
    if (SocketMmcfgSize > 256) {
      SocketMmcfgSize = 256;
    }

    //
    // Assign SBSP first
    //
    KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.PciSegment = 0;
    TmpBusSize = BusSize[KtiInternalGlobal->SbspSoc] = (UINT8)(SocketMmcfgSize/SAD_BUS_GRANTY);

    if (TmpBusSize % SAD_BUS_TARGETS == 0) {
      Idx2 = 1;
    } else {
      Idx2 = 0;
    }

    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if (Idx1 == KtiInternalGlobal->SbspSoc) {
        continue;
      }

      if (SocketData->Cpu[Idx1].Valid == TRUE || KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE ) {
        BusSize[Idx1] = (UINT8)(SocketMmcfgSize/SAD_BUS_GRANTY);
        TmpBusSize += BusSize[Idx1];
        KTIVAR->CpuInfo[Idx1].CpuRes.PciSegment = Idx2;

        if (TmpBusSize % SAD_BUS_TARGETS == 0) {
          Idx2 = Idx2 + 1;
        }
      }
    }
  } else {

    AvailBusChunk = (UINT8)(KTIVAR->mmCfgSize >> 25); // Convert the available MMCFG space into 32M chunck size

    // Sanity check the resource allocation requested by the platform
    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if ( ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU) ) {
        WarnFlag = FALSE;

        //
        // Make sure Bus resource is requested. Otherwise log a warning and force the ratio for this CPU to 1.
        //
        if (KTISETUP->BusRatio[Idx1] == 0) {
          TmpBusRatio[Idx1] = 1;
          WarnFlag = TRUE;
        } else {
          TmpBusRatio[Idx1] = KTISETUP->BusRatio[Idx1];
        }

        if (WarnFlag == TRUE) {
          KtiLogWarning ((UINT8)(1 << Idx1), SOCKET_TYPE_CPU, 0xFF, WARN_RESOURCE_NOT_REQUESTED_FOR_CPU);
          KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  A resource is not requested for CPU%u. Forcing resource allocation. ", Idx1));
        }
      } else {
        //
        // The CPU is not present, so don't allocate resources.
        //
        TmpBusRatio[Idx1] = 0;
      }
    }

    //
    // Calculate total resource ratio requested by the platform
    //
    TotBusChunk = 0;
    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if ( ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU) ) {
        TotBusChunk    = TotBusChunk + TmpBusRatio[Idx1];
      } else if (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_FPGA) {
        FpgaCount += 1;
      }
    }

    // Log a warning if the platform requested resource is more than what is available
    if ((TotBusChunk > AvailBusChunk)) {
      KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_RESOURCE_REQUEST_NOT_MET);
      KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  Requested Resource can not be allocated. Forcing to maximum possible value. "));

      //
      // Force the resource to maximum possible value and force the resource ratio to 1 for each socket
      // for which the resource is requested
      //
      if (TotBusChunk > AvailBusChunk) {
        for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
          if (SocketData->Cpu[Idx1].Valid == TRUE || KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE ) {
            if (TmpBusRatio[Idx1] != 0) {
              TmpBusRatio[Idx1] = 1;
            }
          }
        }
      }

      //
      // Recalculate total resource ratio requested by the platform
      //
      TotBusChunk = 0;
      for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
        if ( ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU) ) {
          TotBusChunk    = TotBusChunk + TmpBusRatio[Idx1];
        }
      }
    }

    if (TotBusChunk == 0){
      KTI_ASSERT (FALSE, STS_COLLECT_EARLY_SYSTEM_INFO, MINOR_STS_COLLECT_INFO);
      return KTI_FAILURE;
    }
    //
    // Calculate the residual resources
    //
    ResiBusChunk    = (AvailBusChunk - FpgaCount) % TotBusChunk;

    //
    // Distribute the "whole" resources
    //
    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if ( ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU) ) {
        BusSize[Idx1]    = TmpBusRatio[Idx1] * ((AvailBusChunk - FpgaCount) / TotBusChunk);
      } else {
        BusSize[Idx1]    = 1;
      }
    }

    //
    // Distribute the "residual" resources
    //
    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if ( ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU) ) {
        if (ResiBusChunk && (TmpBusRatio[Idx1] != 0) ) {
          BusSize[Idx1]++;
          ResiBusChunk--;
        }
      }
    }

    //
    // Anything left, apply to Legacy socket
    //
    BusSize[KtiInternalGlobal->SbspSoc] = BusSize[KtiInternalGlobal->SbspSoc] + ResiBusChunk;

    //
    // Verify the resource allocation
    //
    TotBusChunk = 0;
    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if (SocketData->Cpu[Idx1].Valid == TRUE || KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE ) {
        TotBusChunk    = TotBusChunk + BusSize[Idx1];
      }
    }
    KTI_ASSERT ((TotBusChunk == AvailBusChunk), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_B);
  }
  return KTI_SUCCESS;
}

/**
  This routine assigns IO resources for CPUs/stacks that are populated. It consults the appropriate inputs given to KTIRC and
  applies the following set of rules:
    1. Resource will be allocated only for the CPUs that are currently populated
     1a. If hot pluggable system, will preallocate resources for all sockets
     1b. On EX Hotpluggable systems, assume same number stacks per socket as sbsp

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param TmpTotCpu         - NumberofCpus for purpose of resource allocation

  @retval KTI_SUCCESS

**/
KTI_STATUS
CalculateIoResources (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      TmpTotCpu
  )
{
  UINT8          Stack;
  UINT32         TmpIoSize, IoBase;
  UINT8          Socket;
  UINT8          IoAlloc[MAX_SOCKET][MAX_LOGIC_IIO_STACK];
  UINT8          RemainingIoResources;
  UINT8          StackPersonality;
  UINT8          NumSbspIoStacks;
  UINT8          MaxTargetPerSkt;
  UINT8          TargetInSkt;
  UINT8          SadLegIoSize;

  ZeroMem ((UINT8 *) &IoAlloc, sizeof (IoAlloc));
  IoBase = 0;

  SadLegIoSize = GetLegacyStackIoSize (KtiInternalGlobal);

  //
  // Assign IO to legacy stack
  //
  IoAlloc[KtiInternalGlobal->SbspSoc][IIO_CSTACK] = SadLegIoSize;
  RemainingIoResources = (UINT8)((MAX_SAD_TARGETS - SadLegIoSize) / 2);

  NumSbspIoStacks = 0;
  for (Stack = IIO_CSTACK; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
    StackPersonality = KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].Personality;
    if ((StackPersonality == TYPE_UBOX_IIO) || (StackPersonality == TYPE_HFI) || (StackPersonality == TYPE_NAC)) {
      NumSbspIoStacks++;
    }
  }

  //
  // Check if 1S configuration and only legacy stack exists, then all io resource will be assigned to this legacy stack
  //
  if ((KTIVAR->SysConfig == SYS_CONFIG_1S) && NumSbspIoStacks == 0x1) {
    IoAlloc[KtiInternalGlobal->SbspSoc][IIO_CSTACK] = MAX_SAD_TARGETS;
    RemainingIoResources = 0;
  }

  //
  // Assign remaining IO to each regular IIO.   Not all IIOs will have IO in a fully populated 8S
  //
  MaxTargetPerSkt = SI_MAX_IO_IOAPIC_TARGET_IN_KTITAD;
  while (RemainingIoResources != 0) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SocketData->Cpu[Socket].Valid == TRUE || KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE) {
        //
        // Already allocate resource for SBSP legacy stack, then only allocate resource for other (MaxTargetPerSkt - 1) stacks
        // SBSP's CSTACK is handled before, skip it.
        // PBSP need to look all the MaxTargetPerSkt.
        //
        if (Socket == KtiInternalGlobal->SbspSoc) {
          TargetInSkt = 1;
        } else {
          TargetInSkt = 0;
        }
        for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
          if ((Socket == KtiInternalGlobal->SbspSoc) && (Stack == IIO_CSTACK)) {
            continue;
          }
          StackPersonality = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality;
          if ((TargetInSkt < MaxTargetPerSkt) && ((StackPersonality == TYPE_UBOX_IIO) || (StackPersonality == TYPE_HFI) || (StackPersonality == TYPE_NAC))) {
            if (RemainingIoResources != 0) {
              IoAlloc[Socket][Stack] = IoAlloc[Socket][Stack] + 2;
              RemainingIoResources = RemainingIoResources - 1;
              TargetInSkt++;
            } else {
              Socket = MAX_SOCKET;
              break;
            }
          }
        }
      }
    }
  }

  //
  // Assign IO addresses to SBSP first
  //
  KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.IoBase  = (UINT16)IoBase;

  for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
    if (IoAlloc[KtiInternalGlobal->SbspSoc][Stack] == 0) {
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].IoBase = 0xFFFF;
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].IoLimit = 0;
    } else {
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].IoBase = (UINT16)IoBase;
      TmpIoSize = (IoAlloc[KtiInternalGlobal->SbspSoc][Stack] * SAD_LEG_IO_GRANTY) - 1;
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].IoLimit = (UINT16)(IoBase + TmpIoSize);
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.IoLimit = (UINT16)(IoBase + TmpIoSize);
      IoBase = KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].IoLimit + 1;
    }
  }

  CalculateIoResourcesForPbsp (SocketData, KtiInternalGlobal, IoAlloc, IoBase);

  return KTI_SUCCESS;
}

/**
  This routine assigns IoApic resources for CPUs/stacks that are populated. It consults the appropriate inputs given to KTIRC and
  applies the following set of rules:
    1. IOAPIC range which is always reserved for every possible stack; this makes ACPI & other IOAPIC related setting easier

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS

**/
KTI_STATUS
CalculateIoApicResources (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8          Idx1;
  UINT8          Stack;
  UINT8          SadIndex;
  UINT8          StackPersonality;
  UINT8          TargetInSkt = 0;

  SadIndex = 0;
  KTI_ASSERT (KtiInternalGlobal->TotSadTargets, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_D);
  KTI_ASSERT (KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.TotEnabledStacks <= MAX_LOGIC_IIO_STACK, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_E);

  if (!IsSkxFamilyCpu (KtiInternalGlobal->CpuType)) {
    //
    // Assign the whole Io APIC region (0xFEC00000 - 0xFECFFFFF) to Legacy PCH's stack.
    //
    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if (Idx1 == KtiInternalGlobal->SbspSoc) {
        KTIVAR->CpuInfo[Idx1].CpuRes.IoApicBase = IOAPIC_BASE;
        KTIVAR->CpuInfo[Idx1].CpuRes.IoApicLimit = IOAPIC_LIMIT;
      } else {
        KTIVAR->CpuInfo[Idx1].CpuRes.IoApicBase = 0xFFFFFFFF;
        KTIVAR->CpuInfo[Idx1].CpuRes.IoApicLimit = 0;
      }
      for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        if ((Idx1 == KtiInternalGlobal->SbspSoc) && (Stack == 0)) {
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].IoApicBase = IOAPIC_BASE;
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].IoApicLimit = IOAPIC_LIMIT;
        } else {
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].IoApicBase = 0xFFFFFFFF;
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].IoApicLimit = 0;
        }
      }
    }
    return KTI_SUCCESS;
  }
  //
  // Assign IoApic addresses to SBSP first
  //
  KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.IoApicBase = IOAPIC_BASE + (SadIndex * SAD_IOAPIC_GRANTY);

  for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
    StackPersonality = KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].Personality;
    if ((TargetInSkt < SI_MAX_IO_IOAPIC_TARGET_IN_KTITAD) && ((StackPersonality == TYPE_UBOX_IIO) || (StackPersonality == TYPE_MCP) || (StackPersonality == TYPE_FPGA) || (StackPersonality == TYPE_HFI) || (StackPersonality == TYPE_NAC))) {
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].IoApicBase = IOAPIC_BASE + (SadIndex * SAD_IOAPIC_GRANTY);
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].IoApicLimit = IOAPIC_BASE + (SadIndex * SAD_IOAPIC_GRANTY) + SAD_IOAPIC_GRANTY - 1 ;
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.IoApicLimit = KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].IoApicLimit;
      SadIndex = SadIndex + 1;
      TargetInSkt++;
    } else {
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].IoApicBase = 0xFFFFFFFF;
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].IoApicLimit = 0;
    }
  }

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    TargetInSkt = 0;
    if (Idx1 == KtiInternalGlobal->SbspSoc) {
      continue;
    }
    //
    // if socket exists or Hot Plug enabled (reserved for onlining sockets)
    //
    if ( ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU) ) {
      KTIVAR->CpuInfo[Idx1].CpuRes.IoApicBase = IOAPIC_BASE + (SadIndex * SAD_IOAPIC_GRANTY);
      // Assign to each stacks.
      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        StackPersonality = KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].Personality;
        if ((TargetInSkt < SI_MAX_IO_IOAPIC_TARGET_IN_KTITAD) && ((StackPersonality == TYPE_UBOX_IIO) || (StackPersonality == TYPE_MCP) || (StackPersonality == TYPE_FPGA) || (StackPersonality == TYPE_HFI) || (StackPersonality == TYPE_NAC))) {
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].IoApicBase = IOAPIC_BASE + (SadIndex * SAD_IOAPIC_GRANTY);
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].IoApicLimit = IOAPIC_BASE + (SadIndex * SAD_IOAPIC_GRANTY) + SAD_IOAPIC_GRANTY - 1 ;
          KTIVAR->CpuInfo[Idx1].CpuRes.IoApicLimit = KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].IoApicLimit;
          SadIndex = SadIndex + 1;
          TargetInSkt++;
        } else {
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].IoApicBase = 0xFFFFFFFF;
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].IoApicLimit = 0;
        }
      }
    } else { // Indicate the resources are not valid by setting Base > Limit
      KTIVAR->CpuInfo[Idx1].CpuRes.IoApicBase = 0xFFFFFFFF;
      KTIVAR->CpuInfo[Idx1].CpuRes.IoApicLimit = 0;

      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].IoApicBase = 0xFFFFFFFF;
        KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].IoApicLimit = 0;
      }
    }
  }
  return KTI_SUCCESS;
}

/**
  This routine assigns Mmiol resources for CPUs/stacks that are populated. It consults the appropriate inputs given to KTIRC and
  applies the following set of rules:
  For hot plug systems resources will be preallocated

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param TmpTotCpu         - NumberofCpus for purpose of resource allocation

  @retval KTI_SUCCESS

**/
KTI_STATUS
CalculateMmiolResources (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      TmpTotCpu
  )
{
  UINT8          Idx1;
  UINT16         TmpMmiolRatio, ExtraMmiol;
  UINT8          Stack;
  UINT8          StackPersonality;
  UINT16         AvailMmiolChunk;
  UINT32         TmpMmiolSize, MmiolBase, Data32;
  UINT8          UboxAlignment;

  //
  // MMIOL = MMCFG Base + MMCFG Size. Make sure at least 64M of MMIOL space is available for each CPU.
  //
  Data32 = MMIOL_LIMIT - (KTIVAR->mmCfgBase + KTIVAR->mmCfgSize - 1) ;
  if ((Data32 >> 26)  < TmpTotCpu) {
    KtiCheckPoint (0xFF, 0xFF, 0xFF, ERR_SAD_SETUP, MINOR_ERR_INSUFFICIENT_RESOURCE_SPACE);
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid MMCFG Base Supplied. Insufficient MMIOL space. "));
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Each CPU must be allocated at least 64MB of MMIOL space"));
    KTI_ASSERT (FALSE, ERR_SAD_SETUP, MINOR_ERR_INSUFFICIENT_RESOURCE_SPACE);
  }
  MmiolBase = KTIVAR->mmCfgBase + KTIVAR->mmCfgSize;

  //MmiolLimit = TmpMmiolBase + (MmiolSize * SAD_MMIOL_GRANTY);
  AvailMmiolChunk  = (UINT16)((UINT32)((MMIOL_LIMIT - MmiolBase) + 1) >> 22); // Convert the available MMIOL space into 4M chunck size

  //
  // Evenly split Mmiol across all enabled system stacks
  //
  KTI_ASSERT (KtiInternalGlobal->TotSadTargets, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_F);
  KTI_ASSERT (KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.TotEnabledStacks <= MAX_LOGIC_IIO_STACK, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_10);
//  TmpMmiolRatio = (MAX_SAD_TARGETS / KtiInternalGlobal->TotSadTargets);
//  ExtraMmiol = (MAX_SAD_TARGETS % KtiInternalGlobal->TotSadTargets);
  TmpMmiolRatio = (AvailMmiolChunk / KtiInternalGlobal->TotSadTargets);
  ExtraMmiol = (AvailMmiolChunk % KtiInternalGlobal->TotSadTargets);


  //
  // Assign any extra Mmiol to SBSP
  //
  TmpMmiolSize = (((KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.TotEnabledStacks * TmpMmiolRatio) + ExtraMmiol) * SAD_MMIOL_GRANTY);

  //
  // UBOX SB MMIO need the 8MB alignment, if mmiol limit is not 8MB alignment, add one 4MB to adjust the
  // mmiol limit to 8MB alignment
  //
  UboxAlignment = 0;
  if (((MmiolBase + TmpMmiolSize) & 0x007FFFFF) != 0){
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Adjust SBSP MmiolLimit from %x to ", MmiolBase + TmpMmiolSize));
    TmpMmiolSize = TmpMmiolSize + SAD_MMIOL_GRANTY;
    UboxAlignment++;
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "%x\n", MmiolBase + TmpMmiolSize));

  }

  KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.MmiolBase  = MmiolBase;
  KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.MmiolLimit = MmiolBase + (TmpMmiolSize - 1);

  //
  // Assign extra Mmiol to DMI/Ctack
  //
  KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[IIO_CSTACK].MmiolBase = MmiolBase;
  KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[IIO_CSTACK].MmiolLimit = MmiolBase + (((TmpMmiolRatio + ExtraMmiol + UboxAlignment) * SAD_MMIOL_GRANTY) - 1);
  MmiolBase = KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[IIO_CSTACK].MmiolLimit + 1;
  TmpMmiolSize = ((TmpMmiolRatio * SAD_MMIOL_GRANTY) - 1);

  for (Stack = IIO_PSTACK0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
    StackPersonality = KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].Personality;
    if ((StackPersonality == TYPE_UBOX_IIO)
      || (StackPersonality == TYPE_MCP)
      || (StackPersonality == TYPE_FPGA)
      || (StackPersonality == TYPE_HFI)
      || (StackPersonality == TYPE_NAC)
      || (StackPersonality == TYPE_DINO))
    {
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiolBase = MmiolBase;
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiolLimit = MmiolBase + TmpMmiolSize;
      MmiolBase = KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiolLimit + 1;
    } else {
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiolBase = 0xFFFFFFFF;
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiolLimit = 0;
    }
  }


  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (Idx1 == KtiInternalGlobal->SbspSoc) {
      continue;
    }
    //
    // if socket exists or Hot Plug enabled (reserved for onlining sockets)
    //
    if ( ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU) ) {
      KTIVAR->CpuInfo[Idx1].CpuRes.MmiolBase  = MmiolBase;
      // Evenly split Mmio amongst all possible stacks.
      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        StackPersonality = KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].Personality;
        if ((StackPersonality == TYPE_UBOX_IIO)
          || (StackPersonality == TYPE_MCP)
          || (StackPersonality == TYPE_FPGA)
          || (StackPersonality == TYPE_HFI)
          || (StackPersonality == TYPE_NAC)
          || (StackPersonality == TYPE_DINO))
        {
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolBase = MmiolBase;
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolLimit = MmiolBase + TmpMmiolSize - (UboxAlignment*SAD_MMIOL_GRANTY);
          KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit = MmiolBase + TmpMmiolSize - (UboxAlignment*SAD_MMIOL_GRANTY);
          MmiolBase = KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolLimit + 1;
          if (UboxAlignment != 0) {
            UboxAlignment--;
          }
        } else {
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolBase = 0xFFFFFFFF;
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolLimit = 0;
        }
      }
    } else { // Indicate the resources are not valid by setting Base > Limit
      KTIVAR->CpuInfo[Idx1].CpuRes.MmiolBase = 0xFFFFFFFF;
      KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit = 0;

      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolBase = 0xFFFFFFFF;
        KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolLimit = 0;
      }
    }

    //
    // Adjust the Socket MmiolLimit to 8MB alignment for UBOX MMIO requirement
    //
    if (KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit != 0){
      if (((KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit + 1) & 0x007FFFFF) != 0){
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "Adjust the MmiolLimit from %x  to ", KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit));
        //
        // Find the last valid stack and increase a SAD_MMIOL_GRANTY
        //
        for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
          if (KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit == KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolLimit){
            KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit = KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit + SAD_MMIOL_GRANTY;
            KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolLimit = KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit;
            MmiolBase = KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolLimit + 1;
            UboxAlignment++;
            break;
          }
        }
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, " %x\n", KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit));
      }
    }
  }

  CalculateUboxMmioResources (SocketData, KtiInternalGlobal);

  return KTI_SUCCESS;
}

/**
  This routine assigns Mmioh resources for CPUs/stacks that are populated. It consults the appropriate inputs given to KTIRC and
  applies the following set of rules:
  For hot plug systems resources will be preallocated

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param TmpTotCpu         - NumberofCpus for purpose of resource allocation

  @retval KTI_SUCCESS

**/
KTI_STATUS
CalculateMmiohResources (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      TmpTotCpu
  )
{
  UINT8          Idx1;
  UINT16         TmpMmiohRatio;
  UINT8          Stack;
  UINT8          StackPersonality;
  UINT16         MmiohSize;
  UINT64_STRUCT  MmiohBase, TmpMmiohSize, MaxMmiohSizeCheck;

  //
  // Get MmiohBase from setup
  //
  MmiohBase.lo = 0x0;
  MmiohBase.hi = KTISETUP->mmiohBase;

  //
  // Mmioh granularity size.
  // Interleave size = 2**(size*2) GB.
  //
  MmiohSize = KtiInternalGlobal->MmiohTargetSize;

  //
  // Check for valid address
  //
  if (MmiohBase.hi == 0) {
    KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_MMIOH_BASE_UNSUPPORTED);
    KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_14);
  }

  //
  // Evenly split Mmioh across all enabled system stacks
  //
  KTI_ASSERT (KtiInternalGlobal->TotSadTargets, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_11);
  KTI_ASSERT (KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.TotEnabledStacks <= MAX_LOGIC_IIO_STACK, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_12);
  TmpMmiohRatio = 1<<(MmiohSize*2) ;

  //
  // Assign Mmioh to SBSP
  //
  TmpMmiohSize.lo = 1*1024*1024*1024;
  TmpMmiohSize.hi = 0;
  TmpMmiohSize = MultUINT64x32(TmpMmiohSize, (KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.TotEnabledStacks * TmpMmiohRatio));
  KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.MmiohBase  = MmiohBase;
  KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.MmiohLimit = AddUINT64(MmiohBase, SubUINT64x32(TmpMmiohSize, 1));

  //
  // Assign Mmioh to each stack
  //
  TmpMmiohSize.lo = 1*1024*1024*1024;
  TmpMmiohSize.hi = 0;
  TmpMmiohSize = MultUINT64x32(TmpMmiohSize, TmpMmiohRatio);
  for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
    StackPersonality = KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].Personality;
    if ((StackPersonality == TYPE_UBOX_IIO)
      || (StackPersonality == TYPE_MCP)
      || (StackPersonality == TYPE_FPGA)
      || (StackPersonality == TYPE_HFI)
      || (StackPersonality == TYPE_NAC)
      || (StackPersonality == TYPE_DINO))
    {
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiohBase = MmiohBase;
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiohLimit = AddUINT64(MmiohBase, SubUINT64x32(TmpMmiohSize, 1));
      MmiohBase = AddUINT64x32(KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiohLimit, 1);
    } else {
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiohBase.lo = 0xFFFFFFFF;
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiohBase.hi = 0xFFFFFFFF;
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiohLimit.lo = 0;
      KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].CpuRes.StackRes[Stack].MmiohLimit.hi = 0;
    }
  }

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (Idx1 == KtiInternalGlobal->SbspSoc) {
      continue;
    }
    //
    // if socket exists or Hot Plug enabled (reserved for onlining sockets)
    //
    if ( ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU) ) {
      KTIVAR->CpuInfo[Idx1].CpuRes.MmiohBase  = MmiohBase;
      // Evenly assign mmioh amongst all enabled stacks.
      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        StackPersonality = KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].Personality;
        if ((StackPersonality == TYPE_UBOX_IIO)
          || (StackPersonality == TYPE_MCP)
          || (StackPersonality == TYPE_FPGA)
          || (StackPersonality == TYPE_HFI)
          || (StackPersonality == TYPE_NAC)
          || (StackPersonality == TYPE_DINO))
        {
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohBase = MmiohBase;
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohLimit = AddUINT64(MmiohBase, SubUINT64x32(TmpMmiohSize, 1));
          KTIVAR->CpuInfo[Idx1].CpuRes.MmiohLimit = AddUINT64(MmiohBase, SubUINT64x32(TmpMmiohSize, 1));
          MmiohBase = AddUINT64x32(KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohLimit, 1);
        } else {
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohBase.lo = 0xFFFFFFFF;
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohBase.hi = 0xFFFFFFFF;
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohLimit.lo = 0;
          KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohLimit.hi = 0;
        }
      }
    } else { // Indicate the resources are not valid by setting Base > Limit
      KTIVAR->CpuInfo[Idx1].CpuRes.MmiohBase.lo = 0xFFFFFFFF;
      KTIVAR->CpuInfo[Idx1].CpuRes.MmiohBase.hi = 0xFFFFFFFF;
      KTIVAR->CpuInfo[Idx1].CpuRes.MmiohLimit.lo = 0;
      KTIVAR->CpuInfo[Idx1].CpuRes.MmiohLimit.hi = 0;

      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohBase.lo = 0xFFFFFFFF;
        KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohBase.hi = 0xFFFFFFFF;
        KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohLimit.lo = 0;
        KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohLimit.hi = 0;
      }
    }
  }

  //
  // Check that Mmioh did not exceed upper limit
  //
  MmiohBase = SubUINT64x32(MmiohBase, 1);
  MaxMmiohSizeCheck.hi = 0;
  MaxMmiohSizeCheck.lo = 0;

  MaxMmiohSizeCheck = RShiftUINT64 (MmiohBase, KTIVAR->MaxAddress);
  KTI_ASSERT (MaxMmiohSizeCheck.lo == 0, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_13);

  return KTI_SUCCESS;
}

/**
  This routine determines the granularity of the mmiol resources

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS

**/
KTI_STATUS
CalculateMmiolGranularity (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8          Idx1;
  UINT8          Stack;
  UINT32         AddressMap = 0;
  UINT8          BitPos;


  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    //
    // if socket exists or Hot Plug enabled (reserved for onlining sockets)
    //
    if ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) {
      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        if (KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolBase != 0xFFFFFFFF) {
          AddressMap = AddressMap | KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolBase;
        }
      }
    }
  }

  BitPos = 0;
  while (!(AddressMap & 1)) {
    BitPos = BitPos + 1;
    AddressMap = AddressMap >> 1;
  }

  KtiInternalGlobal->MmiolGranularity = BitPos;

  return KTI_SUCCESS;
}

/**
  This routine determines the granularity of the mmioh resources

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS

**/
KTI_STATUS
CalculateMmiohGranularity (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8          Idx1;
  UINT8          Stack;
  UINT64_STRUCT  AddressMap;
  UINT8          BitPos;

  AddressMap.lo = 0;
  AddressMap.hi = 0;


  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    //
    // if socket exists or Hot Plug enabled (reserved for onlining sockets)
    //
    if ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) {
      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        if (KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohBase.hi != 0xFFFFFFFF) {
          AddressMap.lo = AddressMap.lo | KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohBase.lo;
          AddressMap.hi = AddressMap.hi | KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiohBase.hi;
        }
      }
    }
  }


  if (AddressMap.lo == 0) {
    AddressMap.lo = AddressMap.hi;
    BitPos = 32;
  } else {
    BitPos = 0;
  }

  while (!(AddressMap.lo & 1)) {
    BitPos = BitPos + 1;
    AddressMap.lo = AddressMap.lo >> 1;
  }

  KtiInternalGlobal->MmiohGranularity = BitPos;

  return KTI_SUCCESS;
}

/**
  This routine prints the column headers in the CPU Resource Map.
  To print the details of the Resource Map according to these column headings,
  call PrintCpuResourceMapDetails ()

  @param None

**/
VOID
PrintResourceMapHeader (
  VOID
  )
{
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n-------------------------------------------------------------------------------------------------------------------------------------------------------%a", ""));
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO1, "\n       | Seg |    Bus      |        Io       |          IoApic         |          Mmiol          |                   Mmioh                   | StkBmp |"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n-------------------------------------------------------------------------------------------------------------------------------------------------------%a", ""));
}

/**
  This routine prints the details of a CPU Resource map.
  This should be called immediately after printing the column headings, PrintResourceMapHeader()

  @param Socket       - Socket Id
  @param SocketData   - socket specific data structure
  @param CpuRes       - CPU Resource map data structure

**/
VOID
PrintCpuResourceMapDetails (
  UINT8            Socket,
  KTI_SOCKET_DATA  *SocketData,
  KTI_CPU_RESOURCE *CpuRes
  )
{
  UINT8 Idx1;
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n%a%u   |  %1u  | 0x%02X - 0x%02X | 0x%04X - 0x%04X | 0x%08X - 0x%08X ",
        (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_FPGA)  ? "PGA":"CPU",
        Socket,
        CpuRes->PciSegment,
        CpuRes->BusBase,
        CpuRes->BusLimit,
        CpuRes->IoBase,
        CpuRes->IoLimit,
        CpuRes->IoApicBase,
        CpuRes->IoApicLimit
  ));
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "| 0x%08X - 0x%08X | 0x%08X %08X - 0x%08X %08X |  0x%02X  |",
        CpuRes->MmiolBase,
        CpuRes->MmiolLimit,
        CpuRes->MmiohBase.hi,
        CpuRes->MmiohBase.lo,
        CpuRes->MmiohLimit.hi,
        CpuRes->MmiohLimit.lo,
        CpuRes->stackPresentBitmap
  ));
  for (Idx1=0; Idx1 < MAX_LOGIC_IIO_STACK; Idx1++) {
    if (CpuRes->StackRes[Idx1].Personality == TYPE_UBOX) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Ubox "));
    } else if (CpuRes->StackRes[Idx1].Personality == TYPE_RESERVED) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Rsvd "));
    } else {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Stk%02d", Idx1));
    }
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " |  %1u  | 0x%02X - 0x%02X | 0x%04X - 0x%04X | 0x%08X - 0x%08X ",
        CpuRes->PciSegment,
        CpuRes->StackRes[Idx1].BusBase,
        CpuRes->StackRes[Idx1].BusLimit,
        CpuRes->StackRes[Idx1].IoBase,
        CpuRes->StackRes[Idx1].IoLimit,
        CpuRes->StackRes[Idx1].IoApicBase,
        CpuRes->StackRes[Idx1].IoApicLimit
    ));

    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "| 0x%08X - 0x%08X | 0x%08X %08X - 0x%08X %08X |        |",
        CpuRes->StackRes[Idx1].MmiolBase,
        CpuRes->StackRes[Idx1].MmiolLimit,
        CpuRes->StackRes[Idx1].MmiohBase.hi,
        CpuRes->StackRes[Idx1].MmiohBase.lo,
        CpuRes->StackRes[Idx1].MmiohLimit.hi,
        CpuRes->StackRes[Idx1].MmiohLimit.lo
    ));
  }
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n"));
}

/**
  We can derive some segment data from the CpuResource map.
  Check each segment's bus assignments. Each PCI Segment should have 255 Buses

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuRes            - Array of CPU Resource Maps. Can be initialized in the OEM hook OemGetResourceMapUpdate

  @retval ReturnValue - KTI_FAILURE if Resource Map requirements are not met, else KTI_SUCCESS

**/
KTI_STATUS
CheckOemSegmentResources (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         *CpuRes
)
{
  UINT8  SegmentValidatedBitMap = 0;
  UINT8  CurrentSegment = 0;
  UINT8  ExpectedSegments = 1;
  UINT8  SegmentMax = 0;
  UINT32 BusIterator = 0;
  UINT32 BusMax = 0;
  UINT8  SbspSocketId = KtiInternalGlobal->SbspSoc;
  UINT8  Idx1 = 0;
  UINT8  Idx2 = 0;
  UINT32 MmcfgSizeMB = 0;
  UINT32 MmcfgGranularity = SAD_BUS_GRANTY;

  KTI_STATUS ReturnValue = KTI_SUCCESS;

  //
  //Calculate MMCFG Granularity, MMCFG allocated per socket, Upper limit of MMCFG
  //
  MmcfgSizeMB = (KTIVAR->mmCfgSize / 1024 / 1024);
  if (MmcfgSizeMB >= 256) {
    //Greater than 256 MB MMCFG
    BusMax = 255;
    MmcfgGranularity = (MmcfgSizeMB / SAD_BUS_TARGETS);
    ExpectedSegments = (UINT8) (MmcfgSizeMB / 256);
  } else {
    //Less than 256 MB MMCFG
    BusMax = MmcfgSizeMB - 1;
    MmcfgGranularity = SAD_BUS_GRANTY;
    ExpectedSegments = 1;
  }
  if ((CpuRes[SbspSocketId].SegMmcfgBase.lo + KTIVAR->mmCfgSize - 1) > (MMIOL_LIMIT)) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nMMCFG Limit is too high\n", Idx1));
    ReturnValue = KTI_FAILURE;
  }

  //
  // Validate MMCFG Resources for each segment
  // Note that Idx1 is NOT iterated by this for expression
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET;) {

    //Select the next segment to validate. Always start with SBSP segment
    if (SegmentValidatedBitMap == 0) {
      //Since we haven't validated any segments yet, start with SBSP segment
      Idx1 = SbspSocketId;
    } else {
      //Iterate through the non-SBSP sockets to find a segment not yet validated
      if ((Idx1 == SbspSocketId) ||
          ((SocketData->Cpu[Idx1].Valid == FALSE) && (KtiInternalGlobal->KtiCpuSktHotPlugEn == FALSE)) ||
          (((SegmentValidatedBitMap >> CpuRes[Idx1].PciSegment) & 1) == 1)) {
        Idx1++;
        continue;
      }
    }

    //Check the MMCFG boundaries of this segment
    CurrentSegment = CpuRes[Idx1].PciSegment;
    BusIterator = 0;
    if (CurrentSegment > SegmentMax) {
      SegmentMax = CurrentSegment;
    }
    if (CpuRes[Idx1].SegMmcfgBase.hi != 0) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d MMCFG Base %X%08X is above 4GB\n",
                           Idx1, CpuRes[Idx1].SegMmcfgBase.hi, CpuRes[Idx1].SegMmcfgBase.lo));
      ReturnValue = KTI_FAILURE;
    }
    if (CpuRes[Idx1].SegMmcfgBase.lo % 0x10000000 != 0) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d MMCFG Base %X%08X is not 256MB aligned\n",
                           Idx1, CpuRes[Idx1].SegMmcfgBase.hi, CpuRes[Idx1].SegMmcfgBase.lo));
      ReturnValue = KTI_FAILURE;
    }
    if (CpuRes[Idx1].BusBase != 0) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "Socket %d is 1st in segment %d, but does not start with bus 0", Idx1, CurrentSegment));
      ReturnValue = KTI_FAILURE;
    }

    //Validate MMCFG resources for all sockets that belong to this segment
    for (Idx2 = 0; Idx2 < MAX_SOCKET; Idx2++) {
      if (((SocketData->Cpu[Idx2].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) && (CpuRes[Idx2].PciSegment == CurrentSegment)) {
        //Update the Bus Iterator according to this socket's Bus Limit
        if (BusIterator == CpuRes[Idx2].BusBase) {
          BusIterator = CpuRes[Idx2].BusLimit + 1;
        } else {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d first numbered bus is not the next available in segment %d\n", Idx2, CurrentSegment));
          ReturnValue = KTI_FAILURE;
        }
        // Ensure that at least the minimum number of buses are assigned
        if ((CpuRes[Idx2].BusLimit - CpuRes[Idx2].BusBase) < (UINT8) (MmcfgGranularity - 1)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%d buses must be assigned to Socket %d, got only %d\n",
                               MmcfgGranularity, Idx2, CpuRes[Idx2].BusLimit - CpuRes[Idx2].BusBase));
          ReturnValue = KTI_FAILURE;
        }
        //Ensure that this socket's segment MMCFG base matches all others in this segment
        if (IsXEqualYUINT64(CpuRes[Idx1].SegMmcfgBase, CpuRes[Idx2].SegMmcfgBase) == FALSE) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d MMCFG Base does not match others in this segment\n", Idx1));
          ReturnValue = KTI_FAILURE;
        }
      }
      if (ReturnValue == KTI_FAILURE) {
        break;
      }
    }

    //Ensure correct number of bus resources are assigned for the current segment
    BusIterator--;
    if (BusIterator != BusMax){
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSegment %d has incomplete bus assignments (%d != %d)\n",
                           CurrentSegment, BusIterator, BusMax));
      ReturnValue = KTI_FAILURE;
    }

    //Iterate Idx1. Always start with SBSP, then check sockets in ascending order by socket ID
    if (SegmentValidatedBitMap == 0) {
      SegmentValidatedBitMap = (1 << CpuRes[SbspSocketId].PciSegment);
      Idx1 = 0;
    } else {
      SegmentValidatedBitMap = SegmentValidatedBitMap | (1 << CpuRes[Idx1].PciSegment);
      Idx1++;
    }

    if (ReturnValue == KTI_FAILURE) {
      break;
    }
  }

  //
  // Ensure that the number of segments encountered is acceptable based on the MMCFG Size allocated
  //
  if (ExpectedSegments == 1) {
    if (SegmentMax != 0) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "We expect exactly 1 segment in resource map, based on MMCFG Size"));
      ReturnValue = KTI_FAILURE;
    }
  } else {
    //Don't reject when less than expected number of segments are found - invalid sockets could "own" the wasted buses
    if (SegmentMax > (ExpectedSegments - 1)) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "We expect up to %d segments in resource map, based on MMCFG Size", ExpectedSegments));
      ReturnValue = KTI_FAILURE;
    }
  }

  return ReturnValue;
}

/**
  This routine checks each socket's resource map. Make sure valid and present, and resources fit within expected ranges.

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuRes            - Array of CPU Resource Maps. Can be initialized in the OEM hook OemGetResourceMapUpdate

  @retval ReturnValue - KTI_FAILURE if Resource Map requirements are not met, else KTI_SUCCESS

**/
KTI_STATUS
CheckOemSocketResources (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         *CpuRes
)
{
  UINT8 Idx1;
  KTI_STATUS ReturnValue = KTI_SUCCESS; // bitmask for CpuRes socket index that fails the resource checks (non-zero indicates failure)
  UINT64_STRUCT NegativeOne64Bit;
  UINT64_STRUCT Zero64Bit;
  UINT8 IoStarved;
  UINT8 IoApicStarved;
  UINT8 MmiolStarved;
  UINT8 MmiohStarved;

  NegativeOne64Bit.hi = 0xFFFFFFFF;
  NegativeOne64Bit.lo = 0xFFFFFFFF;
  Zero64Bit.hi = 0;
  Zero64Bit.lo = 0;

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) {
      if (CpuRes[Idx1].stackPresentBitmap != KTIVAR->CpuInfo[Idx1].CpuRes.stackPresentBitmap) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nOEM Socket %d stack present bitmap, 0x%x, does not match known stack present bitmap, 0x%x"));
        ReturnValue = KTI_FAILURE;
      }

      //Determine which resources are starved for socket Idx1
      IoStarved = ((CpuRes[Idx1].IoLimit == 0) && (CpuRes[Idx1].IoBase == 0xFFFF));
      IoApicStarved = ((CpuRes[Idx1].IoApicLimit == 0) && (CpuRes[Idx1].IoApicBase == 0xFFFFFFFF));
      MmiolStarved = ((CpuRes[Idx1].MmiolLimit == 0) && (CpuRes[Idx1].MmiolBase == 0xFFFFFFFF));
      MmiohStarved = (IsXEqualYUINT64(CpuRes[Idx1].MmiohLimit, Zero64Bit) && IsXEqualYUINT64(CpuRes[Idx1].MmiohBase, NegativeOne64Bit));

      /** LEGACY IO **/
      if (IoStarved == FALSE) {
        //Check Io Limit > Io Base
        if ((CpuRes[Idx1].IoLimit < CpuRes[Idx1].IoBase)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d IO Limit is less than IO Base\n", Idx1));
          ReturnValue = KTI_FAILURE;
        }
        //Check Legacy IO is a multiple of 2KB
        if ((CpuRes[Idx1].IoLimit - CpuRes[Idx1].IoBase + 1) % 0x0800 != 0) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d IO range is not a multiple of 2KB\n", Idx1));
          ReturnValue = KTI_FAILURE;
        }
      }

      /** IO APIC **/
      if (IoApicStarved == FALSE) {
        //Ensure Limit > Base
        if (CpuRes[Idx1].IoApicLimit < CpuRes[Idx1].IoApicBase) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d IO APIC limit is less than IO APIC base\n", Idx1));
          ReturnValue = KTI_FAILURE;
        }
      }

      /** MMIOL **/
      if (MmiolStarved == FALSE) {
        //Check Mmiol Limit >= Mmiol Base
        if (CpuRes[Idx1].MmiolLimit < CpuRes[Idx1].MmiolBase) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d MMIOL limit is less than MMIOL base\n", Idx1));
          ReturnValue = KTI_FAILURE;
        }
        //MMIOL Base must be 4MB aligned
        if ((CpuRes[Idx1].MmiolBase % 0x400000 != 0)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d MMIOL is not 4MB aligned\n", Idx1));
          ReturnValue = KTI_FAILURE;
        }
        //Limit must be less than 64MB Below 4GB
        if (CpuRes[Idx1].MmiolLimit > (MMIOL_LIMIT)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d MMIOL limit is too high\n", Idx1));
          ReturnValue = KTI_FAILURE;
        }
      }

      /** MMIOH **/
      if (MmiohStarved == FALSE) {
        //Check Mmioh Base >= 4GB
        if (CpuRes[Idx1].MmiohBase.hi == 0) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d MMIOH base is below 4GB\n", Idx1));
          ReturnValue = KTI_FAILURE;
        }
        //Check Mmioh Limit >= Mmioh Base
        if (IsXGreaterThanYUINT64 (CpuRes[Idx1].MmiohBase, CpuRes[Idx1].MmiohLimit)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d MMIOH Limit is less than MMIOH base\n", Idx1));
          ReturnValue = KTI_FAILURE;
        }
      }
    } else {
      //This socket is not marked valid, should have no resources assigned to it
      if ((CpuRes[Idx1].BusBase != 0xFF) || (CpuRes[Idx1].BusLimit != 0)) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d has %a resources assigned, but is invalid\n", Idx1, "bus"));
        ReturnValue = KTI_FAILURE;
      }
      if ((CpuRes[Idx1].IoBase != 0xFFFF) || (CpuRes[Idx1].IoLimit != 0)) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d has %a resources assigned, but is invalid\n", Idx1, "IO"));
        ReturnValue = KTI_FAILURE;
      }
      if ((CpuRes[Idx1].IoApicBase != 0xFFFFFFFF) || (CpuRes[Idx1].IoApicLimit != 0)) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d has %a resources assigned, but is invalid\n", Idx1, "IoApic"));
        ReturnValue = KTI_FAILURE;
      }
      if ((CpuRes[Idx1].MmiolBase != 0xFFFFFFFF) || (CpuRes[Idx1].MmiolLimit != 0)) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d has %a resources assigned, but is invalid\n", Idx1, "MMIOL"));
        ReturnValue = KTI_FAILURE;
      }
      if ((CpuRes[Idx1].MmiohBase.lo != 0xFFFFFFFF) || (CpuRes[Idx1].MmiohBase.hi != 0xFFFFFFFF) || (CpuRes[Idx1].MmiohLimit.lo != 0) || (CpuRes[Idx1].MmiohLimit.hi != 0)) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d has %a resources assigned, but is invalid\n", Idx1, "MMIOH"));
        ReturnValue = KTI_FAILURE;
      }
    }
    if (ReturnValue != KTI_SUCCESS) {
      break;
    }
  }
  return ReturnValue;
}

/**
  "Walk" each Socket resource, starting from each lowest base to each highest limit.
  There should be no gaps, and no overlapping ranges

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuRes            - Array of CPU Resource Maps. Can be initialized in the OEM hook OemGetResourceMapUpdate

  @retval ReturnValue - KTI_FAILURE if Resource Map requirements are not met, else KTI_SUCCESS

**/
KTI_STATUS
WalkOemSocketResources (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         *CpuRes
)
{
  UINT8         Idx1;
  KTI_STATUS    ReturnValue = KTI_SUCCESS;
  UINT64_STRUCT MmiohTotal;
  UINT64_STRUCT MmiohGranularity;
  UINT8         SocketCoveredBitMap = 0;
  BOOLEAN IoStarved;
  BOOLEAN IoApicStarved;
  BOOLEAN MmiolStarved;
  BOOLEAN MmiohStarved;

  //
  // Check sockets against each other, ensure they don't overlap or have gaps
  //
  UINT32        IoIterator     = CpuRes[KtiInternalGlobal->SbspSoc].IoBase;
  UINT32        IoApicIterator = CpuRes[KtiInternalGlobal->SbspSoc].IoApicBase;
  UINT32        MmiolIterator  = CpuRes[KtiInternalGlobal->SbspSoc].MmiolBase;
  UINT64_STRUCT MmiohIterator  = CpuRes[KtiInternalGlobal->SbspSoc].MmiohBase;

  MmiohGranularity.lo = 1;
  MmiohGranularity.hi = 0;

  //Walk through each resource's range
  //NOTE: Idx1 is NOT iterated by this for() statement!
  for (Idx1 = 0; Idx1 < MAX_SOCKET;) {
    //Always start with SBSP, then ensure all IO ranges are in ascending order by Socket ID
    if (SocketCoveredBitMap == 0) {
      Idx1 = KtiInternalGlobal->SbspSoc;
    } else {
      if (Idx1 == KtiInternalGlobal->SbspSoc) {
        Idx1++;
        continue;
      }
    }

    //If this socket is enabled
    if ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) {

      //Determine which resources are starved for socket Idx1
      IoStarved = ((CpuRes[Idx1].IoLimit == 0) && (CpuRes[Idx1].IoBase == 0xFFFF));
      IoApicStarved = ((CpuRes[Idx1].IoApicLimit == 0) && (CpuRes[Idx1].IoApicBase == 0xFFFFFFFF));
      MmiolStarved = ((CpuRes[Idx1].MmiolLimit == 0) && (CpuRes[Idx1].MmiolBase == 0xFFFFFFFF));
      MmiohStarved = ((CpuRes[Idx1].MmiohLimit.lo == 0) && (CpuRes[Idx1].MmiohLimit.hi == 0) &&
                      (CpuRes[Idx1].MmiohBase.hi == 0xFFFFFFFF) && (CpuRes[Idx1].MmiohBase.lo == 0xFFFFFFFF));

      //Update all iterator values, if they follow the correct socket order
      //IO
      if (IoStarved == FALSE) {
        if (IoIterator == CpuRes[Idx1].IoBase) {
          IoIterator = CpuRes[Idx1].IoLimit + 1;
        } else {
          ReturnValue = KTI_FAILURE;
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nIO resources are assigned to sockets out-of-order!\n"));
        }
      }

      //IO APIC
      if (IoApicStarved == FALSE) {
        if (IoApicIterator == CpuRes[Idx1].IoApicBase) {
          IoApicIterator = CpuRes[Idx1].IoApicLimit + 1;
        } else {
          ReturnValue = KTI_FAILURE;
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nIO APIC resources are assigned to sockets out-of-order!\n"));
        }
      }

      //MMIOL
      if (MmiolStarved == FALSE) {
        if (MmiolIterator == CpuRes[Idx1].MmiolBase) {
          MmiolIterator = CpuRes[Idx1].MmiolLimit + 1;
        } else {
          ReturnValue = KTI_FAILURE;
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nMMIOL Resources are assigned to sockets out-of-order!\n"));
        }
      }

      //MMIOH
      if (MmiohStarved == FALSE) {
        if (IsXEqualYUINT64(MmiohIterator, CpuRes[Idx1].MmiohBase)) {
          MmiohIterator.hi = CpuRes[Idx1].MmiohLimit.hi;
          MmiohIterator.lo = CpuRes[Idx1].MmiohLimit.lo + 1;
          if (MmiohIterator.lo == 0) {
            MmiohIterator.hi++;
          }
        } else {
          ReturnValue = KTI_FAILURE;
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nMMIOH Ranges are assigned to sockets out-of-order!\n"));
        }
      }

      SocketCoveredBitMap |= (1 << Idx1);
    } else {
      //This socket is invalid, move to next socket ID
      Idx1++;
      continue;
    }

    //Break if a socket's resources are assigned out of order, continue of no problems found
    if (ReturnValue == KTI_FAILURE){
      break;
    } else {
      Idx1++;
    }
  }//Socket Loop

  //subtract one from each iterator variable, now indicates upper range for the system
  IoIterator--;
  MmiolIterator--;
  MmiohIterator.lo--;
  if (MmiohIterator.lo == 0xFFFFFFFF) {
    MmiohIterator.hi--;
  }
  MmiohTotal = SubUINT64(MmiohIterator, CpuRes[KtiInternalGlobal->SbspSoc].MmiohBase);
  MmiohGranularity = LShiftUINT64(MmiohGranularity, (2 * KtiInternalGlobal->MmiohTargetSize) + 30);

  if (ReturnValue == KTI_SUCCESS) {
    //last part of cleanup, examine the upper ranges of each resource
    if ((IoIterator != 0xFFFF) && (CpuRes[KtiInternalGlobal->SbspSoc].IoBase != 0xFFFF)) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nIO Range is less than 64KB!\n"));
      ReturnValue = KTI_FAILURE;
    }
    if (RShiftUINT64(MmiohIterator, KTIVAR->MaxAddress).lo != 0) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nMMIOH upper limit is too high! only %d address lines allowed\n", KTIVAR->MaxAddress));
      ReturnValue = KTI_FAILURE;
    }
    if (KtiInternalGlobal->TotCpu == 2) {
      if (KtiDetermineIfRequireNonInterleavedDecoder (SocketData, KtiInternalGlobal, CpuRes) == KTI_FAILURE) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nMMIOH resources are more than can be covered by non-interleaved decoders\n"));
        ReturnValue = KTI_FAILURE;
      }
    } else {
      if (IsSkxFamilyCpu (KtiInternalGlobal->CpuType)) {
        if (IsXGreaterThanYUINT64(MmiohTotal, MultUINT64x32(MmiohGranularity, 32))) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nMMIOH upper limit is too high! only allow up to 32*MmiohGranularity of MMIOH\n"));
          ReturnValue = KTI_FAILURE;
        }
      }
    }

    //MMCFG cannot overlap MMIOL region
    if (CpuRes[KtiInternalGlobal->SbspSoc].MmiolBase > CpuRes[KtiInternalGlobal->SbspSoc].SegMmcfgBase.lo) {
      if (CpuRes[KtiInternalGlobal->SbspSoc].SegMmcfgBase.lo + KTIVAR->mmCfgSize > CpuRes[KtiInternalGlobal->SbspSoc].MmiolBase) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nMMCFG limit extends into MMIOL range\n"));
        ReturnValue = KTI_FAILURE;
      }
    }

    //Ensure that there is no MMIO hole between MMIOL and 4GB-64MB
    if (MmiolIterator != MMIOL_LIMIT) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nMMIO hole exists between MMIOL Limit and 4GB-64MB!\n"));
      ReturnValue = KTI_FAILURE;
    }
  }
  return ReturnValue;
}

/**
  This routine checks each stack resource map in each socket. Make sure stack is valid and present, and resources fit within expected ranges.

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuRes            - Array of CPU Resource Maps. Can be initialized in the OEM hook OemGetResourceMapUpdate

  @retval ReturnValue - KTI_FAILURE if Resource Map requirements are not met, else KTI_SUCCESS

**/
KTI_STATUS
CheckOemStackResources (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         *CpuRes
  )
{
  UINT8 Idx1;
  UINT8 StackIdx1;
  KTI_STATUS ReturnValue = KTI_SUCCESS;
  BOOLEAN IoStarved;
  BOOLEAN IoApicStarved;
  BOOLEAN MmiolStarved;
  BOOLEAN MmiohStarved;
  UINT64_STRUCT MmiohGranularity;
  UINT64_STRUCT NegativeOne64Bit;
  UINT64_STRUCT Zero64Bit;
  NegativeOne64Bit.hi = 0xFFFFFFFF;
  NegativeOne64Bit.lo = 0xFFFFFFFF;
  Zero64Bit.hi = 0;
  Zero64Bit.lo = 0;
  MmiohGranularity.lo = 1;
  MmiohGranularity.hi = 0;

  MmiohGranularity = LShiftUINT64(MmiohGranularity, (2 * KtiInternalGlobal->MmiohTargetSize) + 30);
  MmiohGranularity = SubUINT64x32(MmiohGranularity, 1);
  //Perform checks on each stack, on each socket
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((SocketData->Cpu[Idx1].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) {
      for (StackIdx1 = 0; StackIdx1 < MAX_LOGIC_IIO_STACK; StackIdx1++) {
        IoStarved = ((CpuRes[Idx1].StackRes[StackIdx1].IoLimit == 0) && (CpuRes[Idx1].StackRes[StackIdx1].IoBase == 0xFFFF));
        IoApicStarved = ((CpuRes[Idx1].StackRes[StackIdx1].IoApicLimit == 0) && (CpuRes[Idx1].StackRes[StackIdx1].IoApicBase == 0xFFFFFFFF));
        MmiolStarved = ((CpuRes[Idx1].StackRes[StackIdx1].MmiolLimit == 0) && (CpuRes[Idx1].StackRes[StackIdx1].MmiolBase == 0xFFFFFFFF));
        MmiohStarved = (IsXEqualYUINT64(CpuRes[Idx1].StackRes[StackIdx1].MmiohLimit, Zero64Bit) && IsXEqualYUINT64(CpuRes[Idx1].StackRes[StackIdx1].MmiohBase, NegativeOne64Bit));
        //Stacks 0-3 must have at least one bus each
        if (StackIdx1 <= 3) {
          if (KTIVAR->CpuInfo[Idx1].CpuRes.stackPresentBitmap & (1 << StackIdx1)) {
            if (CpuRes[Idx1].StackRes[StackIdx1].BusLimit < CpuRes[Idx1].StackRes[StackIdx1].BusBase) {
              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Stack %d does not have 1 or more buses assigned\n", Idx1, StackIdx1));
              ReturnValue = KTI_FAILURE;
            }
          }
        }

        //Ensure that each stack's range base >= Socket's range base, for all resources
        if (CpuRes[Idx1].StackRes[StackIdx1].IoBase < CpuRes[Idx1].IoBase) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Stack %d IO Base is below socket's IO Range\n", Idx1, StackIdx1));
          ReturnValue = KTI_FAILURE;
        }
        if (CpuRes[Idx1].StackRes[StackIdx1].IoApicBase < CpuRes[Idx1].IoApicBase) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Stack %d IO APIC Base is below socket's IO APIC Range\n", Idx1, StackIdx1));
          ReturnValue = KTI_FAILURE;
        }
        if (CpuRes[Idx1].StackRes[StackIdx1].MmiolBase < CpuRes[Idx1].MmiolBase) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Stack %d MMIOL Base is below socket's MMIOL Range\n", Idx1, StackIdx1));
          ReturnValue = KTI_FAILURE;
        }
        if ((IsXGreaterThanYUINT64(CpuRes[Idx1].MmiohBase, CpuRes[Idx1].StackRes[StackIdx1].MmiohBase) == TRUE) &&
           (IsXEqualYUINT64(CpuRes[Idx1].MmiohBase, CpuRes[Idx1].StackRes[StackIdx1].MmiohBase) == FALSE)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Stack %d MMIOH Base is below socket's MMIOH Range\n", Idx1, StackIdx1));
          ReturnValue = KTI_FAILURE;
        }


        //Check Stack's Io Limit >= Stack's Io Base
        if (IoStarved == FALSE) {
          if (CpuRes[Idx1].StackRes[StackIdx1].IoLimit < CpuRes[Idx1].StackRes[StackIdx1].IoBase) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Stack %d IO Limit < IO Base\n", Idx1, StackIdx1));
            ReturnValue = KTI_FAILURE;
          }
        }

        //Check Stack's IoApic Limit >= Stack's IoApic Base
        if (IoApicStarved == FALSE) {
          if (CpuRes[Idx1].StackRes[StackIdx1].IoApicLimit < CpuRes[Idx1].StackRes[StackIdx1].IoApicBase) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Stack %d IO APIC Limit < IO APIC Base\n", Idx1, StackIdx1));
            ReturnValue = KTI_FAILURE;
          }
        }

        //Check Stack's Mmiol Limit >= Stack's Mmiol Base, and stack's MMIOL alignment
        if (MmiolStarved == FALSE) {
          if (CpuRes[Idx1].StackRes[StackIdx1].MmiolLimit < CpuRes[Idx1].StackRes[StackIdx1].MmiolBase) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Stack %d MMIOL Limit < MMIOL Base\n", Idx1, StackIdx1));
            ReturnValue = KTI_FAILURE;
          }
          //Stack Mmiol must be 4MB aligned
          if ((CpuRes[Idx1].StackRes[StackIdx1].MmiolBase % 0x400000 != 0) && !(CpuRes[Idx1].StackRes[StackIdx1].MmiolBase == 0xFFFFFFFF)) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Stack %d MMIOL is not 4MB aligned\n", Idx1, StackIdx1));
            ReturnValue = KTI_FAILURE;
          }
          //UBOX Stack Mmiol must be 8MB aligned
          if (CpuRes[Idx1].StackRes[StackIdx1].Personality == TYPE_UBOX) {
            if ((CpuRes[Idx1].StackRes[StackIdx1].MmiolBase & 0x7FFFFF) != 0) {
              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d UBOX Stack %d MMIOL is not 8MB aligned\n", Idx1, StackIdx1));
              ReturnValue = KTI_FAILURE;
            }
          }
        }

        //Ensure Stack's MMIOH Limit >= Stack's MMIOH Base
        if (MmiohStarved == FALSE) {
          if (IsXGreaterThanYUINT64(CpuRes[Idx1].StackRes[StackIdx1].MmiohBase, CpuRes[Idx1].StackRes[StackIdx1].MmiohLimit) &&
             (IsXEqualYUINT64(CpuRes[Idx1].StackRes[StackIdx1].MmiohBase, CpuRes[Idx1].StackRes[StackIdx1].MmiohLimit) == FALSE)) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Stack %d MMIOH Limit < MMIOH Base\n", Idx1, StackIdx1));
            ReturnValue = KTI_FAILURE;
          }
          if (IsXGreaterThanYUINT64(MmiohGranularity, SubUINT64(CpuRes[Idx1].StackRes[StackIdx1].MmiohLimit, CpuRes[Idx1].StackRes[StackIdx1].MmiohBase)) &&
             (IsXEqualYUINT64(MmiohGranularity, SubUINT64(CpuRes[Idx1].StackRes[StackIdx1].MmiohLimit, CpuRes[Idx1].StackRes[StackIdx1].MmiohBase)) == FALSE)) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Stack %d MMIOH resources are less than the granularity\n", Idx1, StackIdx1));
            ReturnValue = KTI_FAILURE;
          }
        }

        //Exit the Stack loop if any problems are found with the resource map
        if (ReturnValue == KTI_FAILURE) {
          break;
        }
      } //Stack Loop
    } //if socket valid

    //Exit the Socket loop if any problems are found with the resource map
    if (ReturnValue == KTI_FAILURE) {
      break;
    }
  }//Socket loop

  return ReturnValue;
}

/**
  "Walk" each Stack resource, starting from each lowest base to each highest limit.
  There should be no gaps, and no overlapping ranges

  Operating with assumption that each stack resource range is above the previous stack.
  (unless a stack has no resources of that type allocated)

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuRes            - Array of CPU Resource Maps. Can be initialized in the OEM hook OemGetResourceMapUpdate

  @retval ReturnValue - KTI_FAILURE if Resource Map requirements are not met, else KTI_SUCCESS

**/
KTI_STATUS
WalkOemStackResources (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         *CpuRes
  )
{
  UINT8 Idx1;
  UINT8 StackIdx1;
  KTI_STATUS ReturnValue = KTI_SUCCESS;

  UINT8         BusIterator;
  UINT16        IoIterator;
  UINT32        IoApicIterator;
  UINT32        MmiolIterator;
  UINT64_STRUCT MmiohIterator;
  UINT64_STRUCT NegativeOne64Bit;
  UINT64_STRUCT Zero64Bit;
  BOOLEAN       IoStarved;
  BOOLEAN       IoApicStarved;
  BOOLEAN       MmiolStarved;
  BOOLEAN       MmiohStarved;

  NegativeOne64Bit.hi = 0xFFFFFFFF;
  NegativeOne64Bit.lo = 0xFFFFFFFF;
  Zero64Bit.hi = 0;
  Zero64Bit.lo = 0;

  //Check each stack against other stacks in the socket
  /**Walk map to make sure that each range is complete, with no gaps**/
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((SocketData->Cpu[Idx1].Valid == TRUE ) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) {
      if (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU) {
        BusIterator = CpuRes[Idx1].BusBase;
        IoIterator = CpuRes[Idx1].IoBase;
        IoApicIterator = CpuRes[Idx1].IoApicBase;
        MmiolIterator = CpuRes[Idx1].MmiolBase;
        MmiohIterator = CpuRes[Idx1].MmiohBase;

        //Stacks must be in ascending order for all resources, if resources are assigned
        for (StackIdx1 = 0; StackIdx1 < MAX_LOGIC_IIO_STACK; StackIdx1++) {

          if (((KTIVAR->CpuInfo[Idx1].CpuRes.stackPresentBitmap >> StackIdx1) & 1) || (CpuRes[Idx1].StackRes[StackIdx1].Personality == TYPE_RESERVED) || (CpuRes[Idx1].StackRes[StackIdx1].Personality == TYPE_UBOX)) {
            IoStarved = ((CpuRes[Idx1].StackRes[StackIdx1].IoLimit == 0) && (CpuRes[Idx1].StackRes[StackIdx1].IoBase == 0xFFFF));
            IoApicStarved = ((CpuRes[Idx1].StackRes[StackIdx1].IoApicLimit == 0) && (CpuRes[Idx1].StackRes[StackIdx1].IoApicBase == 0xFFFFFFFF));
            MmiolStarved = ((CpuRes[Idx1].StackRes[StackIdx1].MmiolLimit == 0) && (CpuRes[Idx1].StackRes[StackIdx1].MmiolBase == 0xFFFFFFFF));
            MmiohStarved = (IsXEqualYUINT64(CpuRes[Idx1].StackRes[StackIdx1].MmiohLimit, Zero64Bit) && IsXEqualYUINT64(CpuRes[Idx1].StackRes[StackIdx1].MmiohBase, NegativeOne64Bit));

            if ((BusIterator == CpuRes[Idx1].StackRes[StackIdx1].BusBase) &&
                !((CpuRes[Idx1].StackRes[StackIdx1].BusBase == 0xFF) && (CpuRes[Idx1].StackRes[StackIdx1].BusLimit == 0))) {
              BusIterator = CpuRes[Idx1].StackRes[StackIdx1].BusLimit + 1;
            } else {
              if ((CpuRes[Idx1].StackRes[StackIdx1].BusBase != 0xFF) && (CpuRes[Idx1].StackRes[StackIdx1].BusLimit != 0x00)) {
                KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d stack %d %a assignments are incomplete or overlapping\n", Idx1, StackIdx1, "Bus"));
                ReturnValue = KTI_FAILURE;
              }
            }
            if (IoIterator == CpuRes[Idx1].StackRes[StackIdx1].IoBase) {
              IoIterator = CpuRes[Idx1].StackRes[StackIdx1].IoLimit + 1;
            } else {
              if (IoStarved == FALSE) {
                KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d stack %d %a assignments are incomplete or overlapping\n", Idx1, StackIdx1, "IO"));
                ReturnValue = KTI_FAILURE;
              }
            }
            if (IoApicIterator == CpuRes[Idx1].StackRes[StackIdx1].IoApicBase) {
              IoApicIterator = CpuRes[Idx1].StackRes[StackIdx1].IoApicLimit + 1;
            } else {
              if (IoApicStarved == FALSE) {
                KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d stack %d %a assignments are incomplete or overlapping\n", Idx1, StackIdx1, "IO APIC"));
                ReturnValue = KTI_FAILURE;
              }
            }
            if (MmiolIterator == CpuRes[Idx1].StackRes[StackIdx1].MmiolBase) {
              MmiolIterator = CpuRes[Idx1].StackRes[StackIdx1].MmiolLimit + 1;
            } else {
              if (MmiolStarved == FALSE) {
                KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d stack %d %a assignments are incomplete or overlapping\n", Idx1, StackIdx1, "MMIOL"));
                ReturnValue = KTI_FAILURE;
              }
            }
            if ((MmiohIterator.hi == CpuRes[Idx1].StackRes[StackIdx1].MmiohBase.hi) && (MmiohIterator.lo == CpuRes[Idx1].StackRes[StackIdx1].MmiohBase.lo)){
              MmiohIterator.lo = CpuRes[Idx1].StackRes[StackIdx1].MmiohLimit.lo + 1;
              MmiohIterator.hi = CpuRes[Idx1].StackRes[StackIdx1].MmiohLimit.hi;
              if (!MmiohIterator.lo) {
                MmiohIterator.hi++;
              }
            } else {
              if (MmiohStarved == FALSE) {
                KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d stack %d %a assignments are incomplete or overlapping\n", Idx1, StackIdx1, "MMIOH"));
                ReturnValue = KTI_FAILURE;
              }
            }
          }
        }
        if (ReturnValue == KTI_FAILURE) {
          return KTI_FAILURE;
        }

        BusIterator--;
        IoIterator--;
        IoApicIterator--;
        MmiolIterator--;
        MmiohIterator.lo--;
        if (MmiohIterator.lo == 0xFFFFFFFF) {
          MmiohIterator.hi--;
        }

        if (BusIterator != CpuRes[Idx1].BusLimit) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d stack %A assignments are incomplete or overlapping\n", Idx1, "Bus"));
          ReturnValue = KTI_FAILURE;
        }
        if ((IoIterator != CpuRes[Idx1].IoLimit) && (CpuRes[Idx1].IoBase != 0xFFFF)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d stack %a assignments are incomplete or overlapping\n", Idx1, "IO"));
          ReturnValue = KTI_FAILURE;
        }
        if ((IoApicIterator != CpuRes[Idx1].IoApicLimit) && (CpuRes[Idx1].IoApicBase != 0xFFFFFFFF)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d stack %a assignments are incomplete or overlapping\n", Idx1, "IO APIC"));
          ReturnValue = KTI_FAILURE;
        }
        if ((MmiolIterator != CpuRes[Idx1].MmiolLimit) && (CpuRes[Idx1].MmiolBase != 0xFFFFFFFF)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d stack %a assignments are incomplete or overlapping\n", Idx1, "MMIOL"));
          ReturnValue = KTI_FAILURE;
        }
        if (((MmiohIterator.hi != CpuRes[Idx1].MmiohLimit.hi) || (MmiohIterator.lo != CpuRes[Idx1].MmiohLimit.lo)) &&
           (((CpuRes[Idx1].MmiohBase.hi == 0xFFFFFFFF) && (CpuRes[Idx1].MmiohBase.lo == 0xFFFFFFFF)) == FALSE)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d stack %a assignments are incomplete or overlapping\n", Idx1, "MMIOH"));
          ReturnValue = KTI_FAILURE;
        }

      } else if (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_FPGA) {
        for (StackIdx1 = 0; StackIdx1 < MAX_IIO_STACK; StackIdx1++) {
          if ((KTIVAR->CpuInfo[Idx1].CpuRes.stackPresentBitmap >> StackIdx1) & 1) {
            if (!((CpuRes[Idx1].StackRes[StackIdx1].IoLimit == 0) && (CpuRes[Idx1].StackRes[StackIdx1].IoBase == 0xFFFF))) {
              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nFPGA %d %a resource is illegal!!!\n", Idx1, "IO"));
              ReturnValue = KTI_FAILURE;
            }
            if (!((CpuRes[Idx1].StackRes[StackIdx1].IoApicLimit == 0) && (CpuRes[Idx1].StackRes[StackIdx1].IoApicBase == 0xFFFFFFFF))) {
              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nFPGA %d %a resource is illegal!!!\n", Idx1, "IoApic"));
              ReturnValue = KTI_FAILURE;
            }
            if (!((CpuRes[Idx1].StackRes[StackIdx1].MmiolLimit == 0) && (CpuRes[Idx1].StackRes[StackIdx1].MmiolBase == 0xFFFFFFFF))) {
              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nFPGA %d %a resource is illegal!!!\n", Idx1, "mmiol"));
              ReturnValue = KTI_FAILURE;
            }
            if (!(IsXEqualYUINT64(CpuRes[Idx1].StackRes[StackIdx1].MmiohLimit, Zero64Bit) && IsXEqualYUINT64(CpuRes[Idx1].StackRes[StackIdx1].MmiohBase, NegativeOne64Bit))) {
              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nFPGA %d %a resource is illegal!!!\n", Idx1, "mmioh"));
              ReturnValue = KTI_FAILURE;
            }
          }
        }
      }
    }
    if (ReturnValue != KTI_SUCCESS){
      break;
    }
  }

  return ReturnValue;
}

/**
  This routine calls out for OEM modifications to the resource map

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS

**/
KTI_STATUS
CheckForOemResourceUpdate (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  KTI_CPU_RESOURCE CpuRes[MAX_SOCKET];
  KTI_OEMCALLOUT   SystemParams;
  INTN             ResourceMapChange;
  UINT64_STRUCT    MmiohGranularitySize;
  UINT8            Socket;
  UINT32           OemMmiolBase;
  UINT32           OemMmcfgBase;
  UINT8            OemTolm;
  KTI_STATUS       OemResourceMapStatus;
  OEM_GET_RESOURCE_MAP_UPDATE  OemGetResourceMapUpdatePtr;

  OemResourceMapStatus = KTI_SUCCESS;
  Socket = 0;
  OemMmcfgBase = KTIVAR->mmCfgBase;
  OemMmiolBase = KTIVAR->mmiolBase;
  OemTolm = FOUR_GB_MEM - (OemMmcfgBase >> 26);

  SystemParams.SizeOfCpuRes= sizeof (CpuRes);
  SystemParams.CpuList = KtiInternalGlobal->CpuList;
  SystemParams.Sbsp = KtiInternalGlobal->SbspSoc;
  SystemParams.mmcfgSize = KTIVAR->mmCfgSize;
  MmiohGranularitySize.lo = 1;
  MmiohGranularitySize.hi = 0;
  SystemParams.mmiohGranularity = LShiftUINT64(MmiohGranularitySize, (2 * KtiInternalGlobal->MmiohTargetSize) + 30);
  for (Socket=0; Socket < MAX_SOCKET; Socket++) {
    CopyMem ((UINT8 *)&CpuRes[Socket], (UINT8 *)&KTIVAR->CpuInfo[Socket].CpuRes, sizeof (KTI_CPU_RESOURCE));
  }

  //
  // Call to OEM hook
  //
  OemGetResourceMapUpdatePtr = (OEM_GET_RESOURCE_MAP_UPDATE) (UINTN)(KTISETUP->OemGetResourceMapUpdate);
  if (OemGetResourceMapUpdatePtr != NULL) {

    OemGetResourceMapUpdatePtr (CpuRes, SystemParams);

  } else {
    // OEM hook is Null, no OEM change, exit before we do anything else
    return SUCCESS;
  }

  //IF OEM did not change resource map, do nothing and return success.
  ResourceMapChange = 0;
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    ResourceMapChange = CompareMem ((UINT8 *)&CpuRes[Socket], (UINT8 *)&KTIVAR->CpuInfo[Socket].CpuRes, sizeof (KTI_CPU_RESOURCE));
    if (ResourceMapChange){
      break; //Something changed, need to validate entire resource map.
    }
  }
  if (!ResourceMapChange) {
    //No OEM change, exit before we do anything else
    return SUCCESS;
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  OEM Overriding Resource Allocation\n"));

  // Cannot support MMCFG above 4GB, and SBSP must be in first segment/lowest bus
  if (KTIVAR->mmCfgBase < KTIVAR->mmiolBase) {
    if (KTIVAR->mmiolBase != CpuRes[KtiInternalGlobal->SbspSoc].MmiolBase) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nOEM Resource map MMIOL Base does not match setup variable\n"));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "This usually indicates MMCFG base or MMCFG size has changed\n"));
      OemResourceMapStatus = KTI_FAILURE;
    }
  }

  //Validate CpuResourceMap from OEM hook.
  //1. Check Segment Resources
  if (OemResourceMapStatus == KTI_SUCCESS) {
    OemResourceMapStatus = CheckOemSegmentResources (SocketData, KtiInternalGlobal, CpuRes);
  }

  //2. Check intra-Socket Resources
  if (OemResourceMapStatus == KTI_SUCCESS) {
    OemResourceMapStatus = CheckOemSocketResources (SocketData, KtiInternalGlobal, CpuRes);
  }

  //3. Check Sockets against each other, walking from SBSP resource bases to the highest limits on each socket
  if (OemResourceMapStatus == KTI_SUCCESS) {
    OemResourceMapStatus = WalkOemSocketResources (SocketData, KtiInternalGlobal, CpuRes);
  }

  //4. Check that each stack resource makes sense in the context of its parent socket
  if (OemResourceMapStatus == KTI_SUCCESS) {
    OemResourceMapStatus = CheckOemStackResources (SocketData, KtiInternalGlobal, CpuRes);
  }

  //5. Starting from each resource base on each socket, iterate through stacks to reach the socket's resource limit
  //   ensure that stacks consume entire resources of socket
  if (OemResourceMapStatus == KTI_SUCCESS) {
    OemResourceMapStatus = WalkOemStackResources (SocketData, KtiInternalGlobal, CpuRes);
  }

  if (OemResourceMapStatus == KTI_SUCCESS) {
    // Find top of "low" memory, and assign it to Host's lowGap member variable
    // Find MmcfgBase, assign back to KtiInternalGlobal's MmcfgBase  member variable
    if (OemMmcfgBase != CpuRes[KtiInternalGlobal->SbspSoc].SegMmcfgBase.lo) {
      OemMmcfgBase = CpuRes[KtiInternalGlobal->SbspSoc].SegMmcfgBase.lo;
    }

    //
    // Check for new mmiolbase
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++){
      //
      // if socket exists or Hot Plug enabled (reserved for onlining sockets)
      //
      if ((SocketData->Cpu[Socket].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)){
        if (OemMmiolBase > CpuRes[Socket].MmiolBase) {
          OemMmiolBase = CpuRes[Socket].MmiolBase;
        }
      }
    }

    if (OemMmiolBase < OemMmcfgBase) {
      KtiInternalGlobal->IsMmcfgAboveMmiol = TRUE;
      OemTolm = FOUR_GB_MEM - (OemMmiolBase >> 26);
    } else {
      KtiInternalGlobal->IsMmcfgAboveMmiol = FALSE;
      OemTolm = FOUR_GB_MEM - (OemMmcfgBase >> 26);
    }

    //
    // KTI_HOST_IN_PTR->lowgap should be moved to var.common.lowgap to for consistent usage, but will require MRC change
    //
    KTIVAR->lowGap = OemTolm;
    KtiInternalGlobal->MmcfgBase = OemMmcfgBase;
    //
    // need to update this var based on MmiohBase but MRC requires update first to consume
    //
    //KTISETUP->highGap = HIGH_GAP;            // High MMIO gap = 256 MB in 256MB chunks
    //

    //
    // Check for special case in 2S where need to make use of non-interleaved decoder
    //

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      CopyMem ((UINT8 *)&KTIVAR->CpuInfo[Socket].CpuRes, (UINT8 *)&CpuRes[Socket], sizeof (KTI_CPU_RESOURCE));
    }
  } else {
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n OEM Resource Map is invalid. Using default resource map"));
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nThe following OEM Defined CPU Resource map will be discarded: "));
    PrintResourceMapHeader ();
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if ((SocketData->Cpu[Socket].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)){
        PrintCpuResourceMapDetails (Socket, SocketData, &(CpuRes[Socket]));
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  This routine calls out for OEM modifications to the resource map

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuRes            - Custom CpuRes definitions.

  @retval KTI_SUCCESS

**/
KTI_STATUS
KtiDetermineIfRequireNonInterleavedDecoder (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         *CpuRes
  )
{
  UINT8          Idx1, Idx2;
  UINT64_STRUCT  MmiohGranularitySize, MaxMmiohInterleavedSize, TotalMmiohSize;
  UINT64_STRUCT  SocketMmiohRequestedSize[2] = {{0, 0}, {0, 0}};

  if (!IsSkxFamilyCpu (KtiInternalGlobal->CpuType)) {
    //
    // SKX/CLX has limitation by the 32 way interleave decoder MAX_SAD_TARGETS.
    // 10nm has removed this limitation and OEM resource map can allocate MMIOH up to MAX_ADDR_LINES
    //
    return KTI_SUCCESS;
  }
  MmiohGranularitySize.lo = 1;
  MmiohGranularitySize.hi = 0;
  MmiohGranularitySize = LShiftUINT64(MmiohGranularitySize, (2 * KtiInternalGlobal->MmiohTargetSize) + 30);
  MaxMmiohInterleavedSize = MultUINT64x32(MmiohGranularitySize, MAX_SAD_TARGETS);

  if (KtiInternalGlobal->TotCpu == 2) {
    Idx2 = 0;
    for (Idx1=0; Idx1 < KtiInternalGlobal->TotCpu; Idx1++) {
      if (SocketData->Cpu[Idx1].Valid && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {
        SocketMmiohRequestedSize[Idx2] = AddUINT64x32(SubUINT64(CpuRes[Idx1].MmiohLimit, CpuRes[Idx1].MmiohBase), (UINT32)1);
        Idx2++;
      }
    }

    TotalMmiohSize = AddUINT64(SocketMmiohRequestedSize[0], SocketMmiohRequestedSize[1]);

    if (IsXGreaterThanYUINT64(TotalMmiohSize, MaxMmiohInterleavedSize)) {
      //
      // request is for more than interleaved decoder can cover
      //
      if ((IsXGreaterThanYUINT64(MaxMmiohInterleavedSize, SocketMmiohRequestedSize[0]) || IsXEqualYUINT64(MaxMmiohInterleavedSize, SocketMmiohRequestedSize[0])) &&
          (IsXGreaterThanYUINT64(MaxMmiohInterleavedSize, SocketMmiohRequestedSize[1]) || IsXEqualYUINT64(MaxMmiohInterleavedSize, SocketMmiohRequestedSize[1]))) {
        //
        // But each individual socket can fit within the limit, so we will use interleaved to map local mmioh and non-interleaved to map remote mmioh
        //
        KtiInternalGlobal->UseNonInterleavedMmioh = TRUE;
      } else {
        return KTI_FAILURE;
      }
    }
  }
  return KTI_SUCCESS;
}

/**
  Determines if X > Y

  @param X
  @param Y

  @retval TRUE/FALSE

**/
BOOLEAN
IsXGreaterThanYUINT64 (
  UINT64_STRUCT X,
  UINT64_STRUCT Y
  )
{
  BOOLEAN Rc;
  if (X.hi > Y.hi) {
   Rc = TRUE;
  } else if (X.hi < Y.hi) {
    Rc = FALSE;
  } else if (X.lo >= Y.lo) {
    Rc = TRUE;
  } else {
    Rc = FALSE;
  }
  return Rc;
}

/**
  Determines if X == Y

  @param X
  @param Y

  @retval TRUE/FALSE

**/
BOOLEAN
IsXEqualYUINT64 (
  UINT64_STRUCT X,
  UINT64_STRUCT Y
  )
{
  BOOLEAN Rc;
  if ((X.hi == Y.hi) && (X.lo == Y.lo)) {
    Rc = TRUE;
  } else {
    Rc = FALSE;
  }
  return Rc;
}

/**
  Routine to display Error/Status code; it also updates a scratch register with additional
  info such as socket, link related to the Error/Status.

  @param SocId     - Bit mask, where bit x corresponds to CPU (CPU:0...3). Set to 0xFF if the Error/Status is System Wide.
  @param SocType   - 0: CPU socket. Set to 0xFF if the Error/Status is System Wide.
  @param KtiLink   - Bit mask, where bit x corresponds to CPU KTI port 0/1. Set to 0xFF if the Error/Status is system wide, 0 if Link Not Applicable.
  @param MajorCode - Major Status or Error code
  @param MinorCode - Minor Status or Error code

  @retval None.

**/
VOID
KtiCheckPoint (
  UINT8                      SocId,
  UINT8                      SocType,
  UINT8                      KtiLink,
  UINT8                      MajorCode,
  UINT8                      MinorCode
  )
{
  UINT16  data;
  // APTIOV_SERVER_OVERRIDE_RC_START : Added OEM hook support for KtiCheckPoint
  AmiKtiCheckPointWrapper(SocId, SocType, KtiLink, MajorCode, MinorCode);
  // APTIOV_SERVER_OVERRIDE_RC_END : Added OEM hook support for KtiCheckPoint

  //
  // Update the scratch register.
  //
  // Register Usage:
  // [31:24] - Error/Status Major Code
  // [23:16] - Error/Status Minor Code
  // [15:13] - Reserved for future use
  // [12:5] - Bitmask of CPU Sockets affected, 0xF - System Wide Error/Status
  // [4:3] - Socket Type. 0: CPU Socket, 0x3 System Wide Error/Status
  // [2:0] - Bit mask of KTI Links affected, 0 - Link Not Applicable, 0x3 - System Wide Error/Status

  data = ((SocId & 0xFF) << 5) | ((SocType & 0x3) << 3) | ((KtiLink & 0x7) << 0);
  OutputCheckpointSocket (0, MajorCode, MinorCode, data);

  return ;
}

/**
  Routine to call platformfatal and assert


  @param MajorCode - Major Status or Error code
  @param MinorCode - Minor Status or Error code

  @retval Does not return

**/
VOID
KtiAssert (
  UINT8                      majorCode,
  UINT8                      minorCode
  )
{
  EFI_STATUS Status;
  UINT8           haltOnError;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformFatalError (majorCode, minorCode, &haltOnError);
  }

  //
  //haltOnError is used by OEM to indicate if wants to halt or not.  This is useful in MRC, but not Kti.  Kti will ignore and always halt
  //
  haltOnError = 1;
  while (*(volatile UINT8 *) &haltOnError);

  return;
}

/**
  Routine to Log Warning in KTIRC output structure.

  These warnings will only be added to the Enhanced Warning Log, if it is defined at build time

  @param SocId    - Bit mask, where bit x corresponds to CPU (CPU:0...3). Set to 0xFF if the Warning is system wide.
  @param SocType  - 0: CPU socket. Set to 0xFF if the Warning is system wide.
  @param KtiLink  - Bit mask, where bit x corresponds to CPU KTI port 0/1. Set to 0xFF if the Warning is system wide, 0 if Link Not Applicable.
  @param WarnCode - Warning Code

  @retval None.

**/
VOID
KtiLogWarning (
  UINT8                      SocId,
  UINT8                      SocType,
  UINT8                      KtiLink,
  UINT8                      WarnCode
  )
{
  EWL_ENTRY_TYPE6 type6;

  type6.Header.Size     = sizeof (EWL_ENTRY_TYPE6);
  type6.Header.Type     = EwlType6;
  type6.Header.Severity = EwlSeverityWarning;

  type6.Context.MajorCheckpoint  = ((GetCheckPoint ()) >> 24) & 0xFF;
  type6.Context.MinorCheckpoint  = ((GetCheckPoint ()) >> 16) & 0xFF;
  type6.Context.MajorWarningCode = WarnCode;
  type6.Context.MinorWarningCode = 0xFF;//No minor code passed to KtiLogWarning

  type6.SocketMask = SocId;
  type6.SocketType = SocType;
  type6.Port       = KtiLink;

  //
  // Output warning details to the Serial Log
  //
  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n\n**** KTI Warning ****"));
  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\nWarning code:         0x%02x", WarnCode));
  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\nMajor Checkpoint:     0x%02x", type6.Context.MajorCheckpoint));
  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\nMinor Checkpoint:     0x%02x", type6.Context.MinorCheckpoint));
  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\nAffected Socket mask: 0x%02x", SocId));
  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\nSocketType:           0x%02x", SocType));
  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\nAffected port mask:   0x%02x\n\n", KtiLink));

  //
  //log the warning in the Enhanced Warning Log structure
  //

  EwlLogEntry (&type6.Header);

  // APTIOV_SERVER_OVERRIDE_RC_START : To provide OEM hook for logging KTI warnings.
  AmiKtiLogWarningWrapper(SocId,SocType,KtiLink,WarnCode);
  // APTIOV_SERVER_OVERRIDE_RC_END : To provide OEM hook for logging KTI warnings.
  return;
}

/**
  KTI specific read PCI Configuration

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param RegOffset - Register offset; values come from the auto generated header file

  @retval register value read

**/
UINT32
KtiReadPciCfg (
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT32                RegOffset
)
{
#ifndef RAS_SUPPORT
  return UsraCsrRead (SocId, BoxInst, RegOffset);
#else
  return ReadCpuPciCfgRas (SocId, BoxInst, RegOffset);
#endif // RAS_SUPPORT
}

/**
  KTI specific write PCI Configuration

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param RegOffset - Register offset; values come from the auto generated header file
  @param data      - Data to write

  @retval register value read

**/
VOID
KtiWritePciCfg (
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT32                RegOffset,
  UINT32                data
  )
{
#ifndef RAS_SUPPORT
  UsraCsrWrite (SocId, BoxInst, RegOffset, data);
#else
  WriteCpuPciCfgRas (SocId, BoxInst, RegOffset, data);
#endif // RAS_SUPPORT

}

/**
  Delay routine

  @param MicroSecs     - Number of micro seconds to delay

**/
VOID
KtiFixedDelay (
  IN UINT32          MicroSecs
  )
{

  FixedDelayMicroSecond (MicroSecs);

  return;
}

/**
  Send BIOS to pCode mailbox command

  @param Cpu     - Socket ID on which command is to be sent
  @param Command - Encoded command
  @param Data    - Data attached to command

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
EFIAPI
SendMailBoxCmdToPcode (
  UINT8               Cpu,
  UINT32              Command,
  UINT32              Data
  )
{
  ReadModifyWritePcuMailbox (
    Cpu,
    MAILBOX_BIOS_CMD_ZERO,
    Command,
    &Data,
    0,
    0
    );
  return KTI_SUCCESS;
}

/**
  Reads 32 bit IO port with serial debug output

  @param ioAddress - IO Address to use

  @retval Value read from ioAddress

**/
UINT32
KtiInPort32 (
  UINT16   ioAddress
  )
{
  if (UbiosGenerationEnabled ()) {
    RcDebugPrint (SDBG_DEFAULT, "\n  mov dx,  0%04xh\n", ioAddress);
    RcDebugPrint (SDBG_DEFAULT, "  in  eax, dx\n");
  }
  return InPort32 (ioAddress);
}

/**
  Writes 32 bit IO port

  @param ioAddress - IO Address to use
  @param data      - Data to write to IO Address

  @retval VOID

**/
VOID
KtiOutPort32 (
  UINT16   ioAddress,
  UINT32   data
  )
{
  OutPort32 (ioAddress, data);

  if (UbiosGenerationEnabled ()) {
    RcDebugPrint (SDBG_DEFAULT, "\n  mov dx,  0%04xh\n", ioAddress);
    RcDebugPrint (SDBG_DEFAULT, "  mov eax, 0%08xh\n", data);
    RcDebugPrint (SDBG_DEFAULT, "  out dx,  eax\n");
  }
}

/**
  Dumps the resource allocation info to log

  @param SocketData        - socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval Nothing

**/
VOID
DataDumpKtiResourceAllocation (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{

  UINT8     Idx1;
  UINT8     TmpTotCpu;

  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE) {
    if (KTISETUP->KtiCpuSktHotPlugTopology == 0) {
      TmpTotCpu =  4;         //online/offline based on 4S topology
    } else {
      TmpTotCpu =  8;         //online/offline based on 8S topology
    }
  } else {
    TmpTotCpu =  MAX_SOCKET;
  }

  //
  // Check we don't overrun structures
  //
  KTI_ASSERT ((TmpTotCpu <= MAX_SOCKET), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_45);

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n\nCPU Resource Allocation"));
  PrintResourceMapHeader ();

  for (Idx1 = 0; Idx1 < TmpTotCpu; Idx1++) {
    if (SocketData->Cpu[Idx1].Valid == TRUE || KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE ){
      // Make sure the Base & Limits are naturally aligned
      KTI_ASSERT (((KTIVAR->CpuInfo[Idx1].CpuRes.BusBase % SAD_BUS_GRANTY) == 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_46);
      KTI_ASSERT ((((KTIVAR->CpuInfo[Idx1].CpuRes.BusLimit + 1) % SAD_BUS_GRANTY) == 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_47);
      if (KTIVAR->CpuInfo[Idx1].CpuRes.IoBase < KTIVAR->CpuInfo[Idx1].CpuRes.IoLimit) {
        KTI_ASSERT (((KTIVAR->CpuInfo[Idx1].CpuRes.IoBase % SAD_LEG_IO_GRANTY) == 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_48);
        KTI_ASSERT ((((KTIVAR->CpuInfo[Idx1].CpuRes.IoLimit + 1) % SAD_LEG_IO_GRANTY) == 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_49);
      }
      if (KTIVAR->CpuInfo[Idx1].CpuRes.IoApicBase < KTIVAR->CpuInfo[Idx1].CpuRes.IoApicLimit) {
        KTI_ASSERT (((KTIVAR->CpuInfo[Idx1].CpuRes.IoApicBase % SAD_IOAPIC_GRANTY) == 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_4A);
        KTI_ASSERT ((((KTIVAR->CpuInfo[Idx1].CpuRes.IoApicLimit + 1) % SAD_IOAPIC_GRANTY) == 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_4B);
      }
      if (KTIVAR->CpuInfo[Idx1].CpuRes.MmiolBase < KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit) {
        KTI_ASSERT (((KTIVAR->CpuInfo[Idx1].CpuRes.MmiolBase % SAD_MMIOL_GRANTY) == 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_4C);
        KTI_ASSERT ((((KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit + 1) % SAD_MMIOL_GRANTY) == 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_4D);
      }
      if (KTIVAR->CpuInfo[Idx1].CpuRes.MmiohBase.hi < KTIVAR->CpuInfo[Idx1].CpuRes.MmiohLimit.hi) {
        KTI_ASSERT (((KTIVAR->CpuInfo[Idx1].CpuRes.MmiohBase.lo % 0x40000000) == 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_4E); // 1GB
        KTI_ASSERT ((((KTIVAR->CpuInfo[Idx1].CpuRes.MmiohLimit.lo + 1) % 0x40000000) == 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_4F);
      }

      PrintCpuResourceMapDetails (Idx1, SocketData, &(KTIVAR->CpuInfo[Idx1].CpuRes));
    }
  }
}

/**
  Returns the total Iios enabled for a given CPU socket.

  @param SocId             - Soc ID of the CPU who's enabled IIOs info is requested, 0...3
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
GetIioAndM2PcieCount (
  UINT8                      SocId,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8   SbspSocketId;

  SbspSocketId = GetSysSbspSocketIdNv ();

  //
  // There is no need to change and restore CPUBUSNO_UBOX and MMCFG_LOCAL_RULE on NBSP
  // since a CAPID register is used to get the IIO count.
  //

  GetIioCount (KtiInternalGlobal, SocId, &KtiInternalGlobal->TotIio[SocId]);
  // Get the number of M2Pcie Link present
  GetM2PcieCount (SocId, KtiInternalGlobal);

  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "Socket %d IioCount: %02d.\n", SocId, KtiInternalGlobal->TotIio[SocId]));

  return KTI_SUCCESS;
}

/**
  Verify the CPU support 46-bit or 52-bit physical address

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

**/
VOID
DetectMaxAddress (
  IN     KTI_SOCKET_DATA            *SocketData,
  IN OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8    CpuType;
  UINT32   RegEax;

  RegEax      = 0;
  CpuType     = KtiInternalGlobal->CpuType;

  //
  // TO-DO: Detect the CSR register from all sockets.
  //
  if (CpuType == CPU_ICXSP || IsSprFamilyCpu(CpuType) || IsGnrSrfFamilyCpu (CpuType)) {
    // ICX-S & SPR support 52b or 46b PA mode, get the PA supported by CPUID
    AsmCpuid (0x80000000, &RegEax, NULL, NULL, NULL);
    if (RegEax >= 0x80000008) {
      AsmCpuid (0x80000008, &RegEax, NULL, NULL, NULL);
      KTIVAR->MaxAddress = (UINT8) RegEax;
    } else {
      KTIVAR->MaxAddress = 36;
    }
    ASSERT (KTIVAR->MaxAddress <= 52);
  } else if (CpuType == CPU_ICXD) {
    // ICX-D use 52b
    KTIVAR->MaxAddress = MAX_ADDR_LINE_ICX;
  } else if (CpuType == CPU_SNR || CpuType == CPU_TNR) {
    // SNR use 42b
    KTIVAR->MaxAddress = MAX_ADDR_LINE_SNR;
  } else {
    // SKX and KNH use 46b addressing
    KTIVAR->MaxAddress = MAX_ADDR_LINE;
  }
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "MaxAddress=%d\n", KTIVAR->MaxAddress));

}

/**
  Get CpuType then save them in KTIRC Internal Global data.

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval None

**/
VOID
KtiGetCpuTypeInfo (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8 CpuType;

  //
  // Initialize CpuType/CpuStepping in KtiInternalGlobal
  //
  if (IsCpuAndRevision (CPU_SKX, REV_ALL)) {
    CpuType = CPU_SKX;
  } else if (IsCpuAndRevision (CPU_CLX, REV_ALL)) {
    CpuType = CPU_CLX;
  } else if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    CpuType = CPU_CPX;
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    CpuType = CPU_ICXSP;
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    CpuType = CPU_SPRSP;
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    CpuType = CPU_ICXD;
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    CpuType = CPU_SNR;
  } else if (IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    CpuType = CPU_TNR;
  } else if (IsCpuAndRevision (CPU_GNRSP, REV_ALL)) {
    CpuType = CPU_GNRSP;
  } else if (IsCpuAndRevision (CPU_SRFSP, REV_ALL)) {
    CpuType = CPU_SRFSP;
  } else {
    KtiCheckPoint (0xFF, 0xFF, 0xFF, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Unknown Cpu Type"));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
    return;
  }

  KtiInternalGlobal->CpuType     = CpuType;
  KtiInternalGlobal->ChopType[KtiInternalGlobal->SbspSoc] = PcuGetPhysicalChop (KtiInternalGlobal->SbspSoc);
}

/**
  This routine determines the latest consumed physical address by system

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS

**/
KTI_STATUS
CalculateLatestConsumedPA (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8          Socket;
  UINT64_STRUCT  AddressMap;
  UINT8          BitPos;

  AddressMap.lo = 0;
  AddressMap.hi = 0;


  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // if socket exists or Hot Plug enabled (reserved for onlining sockets)
    //
    if ((SocketData->Cpu[Socket].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) {
      if (KTIVAR->CpuInfo[Socket].CpuRes.MmiohLimit.hi != 0) {
        AddressMap.hi = AddressMap.hi | KTIVAR->CpuInfo[Socket].CpuRes.MmiohLimit.hi;
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  S%x - AddressMap.hi=%x\n", Socket, AddressMap.hi));
      }
    }
  }


  BitPos = 64;

  while (!(AddressMap.hi & 0x80000000)) {
    BitPos = BitPos - 1;
    AddressMap.hi = AddressMap.hi << 1;
  }

  KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  BitPos=%x\n", BitPos));

  KTIVAR->LatestConsumedPA = BitPos;

  return KTI_SUCCESS;
}

/**

  Or current resetRequired with the new required type of reset
  @param[in] resetRequired     - type of reset required

**/
VOID
EFIAPI
SetNewResetRequired (
  UINT8 NewResetRequired
  )
{
  if (NewResetRequired == POST_RESET_NO_RESET) {
    SetSysResetRequired (POST_RESET_NO_RESET);
  } else {
    SetSysResetRequired (GetSysResetRequired() | NewResetRequired);
  }
}

/**
  Returns the total Chas and List of Chas (& LLCs) enabled for a given CPU socket.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocId             - Soc ID of the CPU who's enabled LLC slice info is requested, 0...3

  @retval KTI_SUCCESS

**/
KTI_STATUS
GetChaCountAndList (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    SocId
  )
{
  UINT8   MaxCha, MaxChaMap;
  UINT8   Count = 0, MapIdx, BitIdx;
  UINT8   CpuType;

  //
  // Make sure we are not accessing more than what is supported for the given CPU
  //
  CpuType = KtiInternalGlobal->CpuType;
  switch (CpuType) {
    case CPU_SKX:
    case CPU_CPX:
      MaxCha = MAX_SKX_CHA;
      break;
    case CPU_SPRSP:
      MaxCha = MAX_SPR_CHA;
      break;
    case CPU_GNRSP:
    case CPU_SRFSP:
      MaxCha = MAX_GNR_CHA;
      break;
    case CPU_SNR:
    case CPU_TNR:
      MaxCha = MAX_SNR_CHA;
      break;
    case CPU_ICXD:
      MaxCha = MAX_ICXD_CHA;
      break;
    default:
      MaxCha = MAX_ICX_CHA;
      break;
  }

  PcuGetLlcSlice (SocId, KtiInternalGlobal->ChaList[SocId]);

  MaxChaMap = MaxCha / 32;
  if ((MaxCha % 32) != 0) {
     MaxChaMap++;
  }

  for (MapIdx = 0; MapIdx < MaxChaMap; MapIdx++) {
    for (BitIdx = 0; BitIdx < 32; BitIdx++) {
      if ((KtiInternalGlobal->ChaList[SocId][MapIdx] & (1 << BitIdx)) != 0) {
        Count++;
      }
    }
  }

  KtiInternalGlobal->TotCha[SocId] = Count;
  return KTI_SUCCESS;
}
