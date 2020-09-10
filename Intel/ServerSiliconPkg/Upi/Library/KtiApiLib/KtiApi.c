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

#include <Base.h>
#include <Uefi.h>
#include <Library/KtiApi.h>
#include <Library/DebugLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcuMailBoxLib.h>
#include <KtiSetupDefinitions.h>
#include <Library/CpuAndRevisionLib.h>

/**

  Check if feature specified by TypeFeature is enabled
  @param[in]    TypeFeature    - Type of Feature to be inspected

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
IfFeatureEnabled (
  IN UINT8    TypeFeature
  )
{
  KTI_HOST_OUT *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  switch (TypeFeature) {
  case TypeSnc:
    return (KtiHostOutPtr->OutSncEn != 0) ? TRUE : FALSE;
  case TypeHitMe:
    return (KtiHostOutPtr->OutHitMeEn == 1) ? TRUE : FALSE;
  case TypeKtiPrefetch:
    return (KtiHostOutPtr->OutKtiPrefetch == KTI_ENABLE) ? TRUE : FALSE;
  case TypeDirectoryMode:
    return (KtiHostOutPtr->OutSysDirectoryModeEn == 1) ? TRUE : FALSE;
  case TypeXptPrefetch:
    return (KtiHostOutPtr->OutXptPrefetch == KTI_ENABLE) ? TRUE : FALSE;
  case TypeIsoc:
    return (KtiHostOutPtr->OutIsocEn == 1) ? TRUE : FALSE;
  default:
    ASSERT (FALSE && "Invalid TYPE_FEATURE_ENUM value passed, or code needs update to handle new value");
    break;
  }

  return FALSE;
}

/**

  Override CxlVid to 0x1e98 or not
  @param[in] SocId    - Socket Index

  @retval    Set CxlVid to 0x1e98 or not

**/
BOOLEAN
EFIAPI
OverrideCxlVid (
  VOID
  )
{
  return (KTI_HOST_OUT_PTR->OutDfxCxlVid == 1) ? TRUE : FALSE;
}

/**

  Get Total Chas/LLCs. All CPUs must have same number of Cbo/LLCs
  @param[in] SocId    - Socket Index

  @retval    Tot Cha Count in the socket

**/
UINT8
EFIAPI
GetTotChaCount (
  IN UINT8    SocId
  )
{
  return KTI_HOST_OUT_PTR->CpuInfo[SocId].TotCha;
}

/**

  Get Num Of Cluster Per System
  @param[in] none

  @retval    Num Of Cluster Per System

**/
UINT8
EFIAPI
GetNumOfClusterPerSystem (
  VOID
  )
{

  return  KTI_HOST_OUT_PTR->OutSncEn;
}

/**

  Get UMA Clustering setting determined by UPI RC
  @param[in] none

  @retval    UMA Clustering mode

**/
UINT8
EFIAPI
GetKtiOutputUmaClusterSetting (
  VOID
  )
{
  return  KTI_HOST_OUT_PTR->OutUmaClustering;
}

/**

  Check if GB Alignment required for SNC
  @param   none

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
SncGbAlignRequired (
  VOID
  )
{
  return (KTI_HOST_OUT_PTR->OutSncGbAlignRequired == 1) ? TRUE : FALSE;
}


/**

  Check system topology
  @param[in] SysTopology - Topology Type to be checked

  @retval    TRUE        Topology Type is same as input
  @retval    FALSE       Topology Type is not same as input

**/
BOOLEAN
EFIAPI
CheckSysConfig (
  IN UINT8    SysTopology
  )
{
  return (KTI_HOST_OUT_PTR->SysConfig == SysTopology) ? TRUE : FALSE;
}

/**

  Return if fpga is enabled in this socket or not
  @param[in] SocId    - Socket Index

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
IfFpgaActive (
  IN UINT8     SocId
  )
{
  return (KTI_HOST_OUT_PTR->OutKtiFpgaEnable[SocId] == 1) ? TRUE : FALSE;
}

/**

  Get IRQ Threshold
  @param   none

  @retval    IRQ Threshold

**/
UINT8
EFIAPI
GetIRQThreshold (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->OutIRQThreshold;
}

/**

  Get Request Threshold
  @param   none

  @retval  RequestThreshold

**/
UINT8
EFIAPI
GetRequestThreshold (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->OutRrqThreshold;
}

/**

  Check if KTI RAS is in progress
  @param   none

  @retval    TRUE       KTIRC is called to handle a CPU/IOH O*L event
  @retval    FALSE

**/
BOOLEAN
EFIAPI
RasInProgress (
  VOID
  )
{
  return (KTI_HOST_OUT_PTR->RasInProgress == 1) ? TRUE : FALSE;
}

/**
   Determine the RAS (Reliability Availability Servicabliity)
   type configured for the system.

   @return System RAS type (see RasTypes.h for values)
**/
UINT8
GetSystemRasType (
  VOID
  )
{
  UINT8 Answer;

  Answer = KTI_HOST_OUT_PTR->OutSystemRasType;

  return Answer;
}


/**

  Get RAS event socket ID
  @param   none

  @retval    RAS event socket ID

**/
UINT8
EFIAPI
GetRasSocId (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->RasSocId;
}

/**

  Check stack's personality
  @param[in] SocId    - Socket Index
  @param[in] StackId  - Stack Index
  @param[in] Personality - Personality Type to be checked

  @retval    TRUE       Personality Type is same as input
  @retval    FALSE      Personality Type is not same as input

**/
BOOLEAN
EFIAPI
CheckStackPersonality (
  IN UINT8     SocId,
  IN UINT8     StackId,
  IN UINT8     Personality
  )
{
  return (KTI_HOST_OUT_PTR->CpuInfo[SocId].CpuRes.StackRes[StackId].Personality == Personality) ? TRUE : FALSE;
}

/**

  Return if the stack indexed is present or not
  @param[in] SocId    - Socket Index
  @param[in] StackId    - Stack Index

  @retval    TRUE       Stack present
  @retval    FALSE      Stack not present

**/
BOOLEAN
EFIAPI
IfStackPresent (
  IN UINT8     SocId,
  IN UINT8     StackId
  )
{
  return ((KTI_HOST_OUT_PTR->CpuInfo[SocId].CpuRes.stackPresentBitmap & (BIT0 << StackId)) != 0) ? TRUE : FALSE;
}

/**

  Get Socket Resource
  @param[in] SocId    - Socket Index
  @param[out] SocketResouce    - pointer to SocketResouce

  @retval    KTI_CPU_RESOURCE

**/
VOID
EFIAPI
GetSocketResouce (
  IN UINT8     SocId,
  OUT KTI_CPU_RESOURCE *SocketResouce
  )
{
  UINT8        Stack;
  KTI_HOST_OUT *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  SocketResouce->PciSegment = KtiHostOutPtr->CpuInfo[SocId].CpuRes.PciSegment;
  SocketResouce->TotEnabledStacks = KtiHostOutPtr->CpuInfo[SocId].CpuRes.TotEnabledStacks;
  SocketResouce->BusBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.BusBase;
  SocketResouce->BusLimit = KtiHostOutPtr->CpuInfo[SocId].CpuRes.BusLimit;
  SocketResouce->IoBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.IoBase;
  SocketResouce->IoLimit = KtiHostOutPtr->CpuInfo[SocId].CpuRes.IoLimit;
  SocketResouce->IoApicBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.IoApicBase;
  SocketResouce->IoApicLimit = KtiHostOutPtr->CpuInfo[SocId].CpuRes.IoApicLimit;
  SocketResouce->MmiolBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.MmiolBase;
  SocketResouce->MmiolLimit = KtiHostOutPtr->CpuInfo[SocId].CpuRes.MmiolLimit;
  SocketResouce->UboxMmioBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.UboxMmioBase; //For 10nm server, ubox mmio base
  SocketResouce->MmiohBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.MmiohBase;
  SocketResouce->MmiohLimit = KtiHostOutPtr->CpuInfo[SocId].CpuRes.MmiohLimit;
  SocketResouce->SegMmcfgBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.SegMmcfgBase;
  SocketResouce->stackPresentBitmap = KtiHostOutPtr->CpuInfo[SocId].CpuRes.stackPresentBitmap;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++){
    GetStackResouce (SocId, Stack, &(SocketResouce->StackRes[Stack]));
  }
}

/**

  Get Stack Resource
  @param[in] SocId    - Socket Index
  @param[in] Stack    - Stack Index
  @param[out] StackResouce    - pointer to StackResouce

  @retval    KTI_STACK_RESOURCE

**/
VOID
EFIAPI
GetStackResouce (
  IN UINT8     SocId,
  IN UINT8     Stack,
  OUT KTI_STACK_RESOURCE *StackResouce
  )
{
  KTI_HOST_OUT *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  StackResouce->Personality = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].Personality;
  StackResouce->BusBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].BusBase;
  StackResouce->BusLimit = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].BusLimit;
  StackResouce->IoBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].IoBase;
  StackResouce->IoLimit = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].IoLimit;
  StackResouce->IoApicBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].IoApicBase;
  StackResouce->IoApicLimit = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].IoApicLimit;
  StackResouce->MmiolBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].MmiolBase;
  StackResouce->MmiolLimit = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].MmiolLimit;
  StackResouce->MmiohBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].MmiohBase;
  StackResouce->MmiohLimit = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].MmiohLimit;
  StackResouce->RcrbBase = KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[Stack].RcrbBase;
}

/**

  Get IODC Mode: 0 - Disable IODC,  1 - AUTO (default), 2 - IODC_EN_REM_INVITOM_PUSH, 3 - IODC_EN_REM_INVITOM_ALLOCFLOW
  @param[in] none

  @retval    IODC Mode

**/
UINT8
EFIAPI
GetIoDcMode (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->OutIoDcMode;
}

/**

  Get Segment Num
  @param[in] SocId

  @retval    SegmentNum

**/
UINT8
EFIAPI
GetSegmentNum (
  IN UINT8     SocId
  )
{
  return KTI_HOST_OUT_PTR->CpuInfo[SocId].CpuRes.PciSegment;
}

/**

  Get whether Board VS CPU conflicts.

  @param   none

  @retval  Board VS CPU conflicts

**/
UINT8
EFIAPI
GetBoardVsCpuConflicts (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->OutBoardVsCpuConflict;
}

/**

  Get KTI DebugPrintLevel.

  @param   none

  @retval  KTI DebugPrintLevel

**/
UINT8
EFIAPI
GetKtiDebugPrintLevel (
  VOID
  )
{
  return KTI_HOST_IN_PTR->DebugPrintLevel;
}

/**

  Set KTI DebugPrintLevel.

  @param   [in] DbgPrintLevel

  @retval  KTI DebugPrintLevel

**/
VOID
EFIAPI
SetKtiDebugPrintLevel (
  IN UINT8   DbgPrintLevel
  )
{
  KTI_HOST_IN_PTR->DebugPrintLevel = DbgPrintLevel;
}

/**

  Check if KTI Cpu Socket HotPlug Enabled
  @param   none

  @retval    TRUE       KTI Cpu Socket HotPlug Enabled
  @retval    FALSE

**/
BOOLEAN
EFIAPI
KtiCpuSktHotPlugEnabled (
  VOID
  )
{
  return (KTI_HOST_OUT_PTR->OutKtiCpuSktHotPlugEn == 1) ? TRUE : FALSE;
}

/**

  Get KTI LEP info
  @param[in]  SocId      - Socket Index
  @param[in]  PortId     - Port Index
  @param[out] KtiLepInfo - pointer to KtiLepInfo

  @retval    KTI_LINK_DATA

**/
VOID
EFIAPI
GetKtiLepInfo (
  IN UINT8     SocId,
  IN UINT8     PortId,
  OUT KTI_LINK_DATA *KtiLepInfo
  )
{
  KTI_HOST_OUT *KtiHostOutPtr;

  ASSERT (KtiLepInfo != NULL);
  if (KtiLepInfo == NULL) {
    return;
  }

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  KtiLepInfo->Valid       = KtiHostOutPtr->CpuInfo[SocId].LepInfo[PortId].Valid;
  KtiLepInfo->PeerSocId   = KtiHostOutPtr->CpuInfo[SocId].LepInfo[PortId].PeerSocId;
  KtiLepInfo->PeerSocType = KtiHostOutPtr->CpuInfo[SocId].LepInfo[PortId].PeerSocType;
  KtiLepInfo->PeerPort    = KtiHostOutPtr->CpuInfo[SocId].LepInfo[PortId].PeerPort;
  KtiLepInfo->DualLink    = KtiHostOutPtr->CpuInfo[SocId].LepInfo[PortId].DualLink;
  KtiLepInfo->TwoSkt3Link = KtiHostOutPtr->CpuInfo[SocId].LepInfo[PortId].TwoSkt3Link;
}

/**

  Get KTI Cpu Setting
  @param[in]  SocId         - Socket Index
  @param[out] KtiCpuSetting - KTI_CPU_SETTING structure

  @retval    TRUE  - Get KtiCpuSetting success
  @retval    FALSE - Get KtiCpuSetting Fail

**/
BOOLEAN
EFIAPI
GetPhyLinkPerPortSetting (
  IN  UINT8           SocId,
  OUT KTI_CPU_SETTING *KtiCpuSetting
  )
{
  KTI_HOST_IN  *KtiHostInPtr;

  KtiHostInPtr  = KTI_HOST_IN_PTR;

  if ((KtiHostInPtr == NULL) || (KtiCpuSetting == NULL)) {
    return FALSE;
  }

  if (SocId >= MAX_SOCKET) {
    return FALSE;
  }
  CopyMem ((CHAR8 *)KtiCpuSetting, (CHAR8 *)(&KtiHostInPtr->PhyLinkPerPortSetting[SocId]), sizeof (KTI_CPU_SETTING));

  return TRUE;
}


/**

  Get mmCfgSize
  @retval    mmCfgSize

**/
UINT32
EFIAPI
GetmmCfgSize (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->mmCfgSize;
}

/**

  Return if the socket indexed is present or not
  @param[in] SocId    - Socket Index

  @retval    TRUE       Socket present
  @retval    FALSE      Socket not present

**/
BOOLEAN
EFIAPI
SocketPresent (
  IN UINT32     SocId
  )
{
  return ((KTI_HOST_OUT_PTR->SocketPresentBitMap & (BIT0 << SocId)) != 0) ? TRUE : FALSE;
}

/**

  Get current socketPresentBitMap
  @retval    current socketPresentBitMap

**/
UINT32
EFIAPI
GetSocketPresentBitMap (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->SocketPresentBitMap;
}

/**

  Set socketPresentBitMap
  @param[in] SocketPresentBitMap     - SocketPresentBitMap to be set

**/
VOID
EFIAPI
SetSocketPresentBitMap (
  UINT32 SocketPresentBitMap
  )
{
  KTI_HOST_OUT_PTR->SocketPresentBitMap = SocketPresentBitMap;
}

/**

  Get mmCfgBase
  @retval    mmCfgBase

**/
UINT32
EFIAPI
GetmmCfgBase (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->mmCfgBase;
}

/**

  Get LowGap
  @retval    LowGap

**/
UINT8
EFIAPI
GetLowGap (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->lowGap;
}

/**

  Get mmCfgBaseH in requested Socket
  @param[in] SocId    - Socket Index

  @retval             - mmCfgBaseH
**/
UINT32
EFIAPI
GetmmCfgBaseH (
  IN UINT32    SocId
  )
{
  return KTI_HOST_OUT_PTR->mmCfgBaseH[SocId];
}

/**

  Get mmCfgBaseL in requested Socket
  @param[in] SocId    - Socket Index

  @retval             - mmCfgBaseL
**/
UINT32
EFIAPI
GetmmCfgBaseL (
  IN UINT32    SocId
  )
{
  return KTI_HOST_OUT_PTR->mmCfgBaseL[SocId];
}

/*++

  Checks current type of reset

  @param CurrentReset   - Type of reset to be check

@retval Returns TRUE or FALSE

--*/
BOOLEAN
EFIAPI
CheckCurrentReset (
  UINT8     CurrentReset
  )
{
  if (KTI_HOST_OUT_PTR->CurrentReset == CurrentReset) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/*++

  Checks top of low memory limit

  @param N/A

@retval Returns UINT16 value for TolLimit

--*/
UINT16
EFIAPI
GetKtiTolmLimit (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->tolmLimit;
}

/**

  Get the maximum used physical address bits (42, 46, 52, etc.)
  @param[in]          - none

  @retval             - the maximum used physical address bits

**/
UINT8
EFIAPI
GetMaxPhysicalAddrBits (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->MaxAddress;
}

/**

  This routine calculates and returns the 256MB boundary aligned Top Of Lower Memory (TOLM) address in units of 64MB

  @retval TOLM in units of 64MB

**/

UINT16
EFIAPI
GetTolmAddress (
  VOID
  )
{
  UINT16 Tolm;
  // TOLM in 64MB units
  Tolm = (UINT16)(FOUR_GB_MEM - KTI_HOST_OUT_PTR->lowGap);

  // Has to be on a 256MB boundary
  Tolm = Tolm & ~0x3;

  return Tolm;
}

/**

  Get the maximum consume physical address bits (42, 46, 52, etc.)
  @param[in]          - none

  @retval             - the maximum used physical address bits

**/
UINT8
EFIAPI
GetLatestConsumePA (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->LatestConsumedPA;
}

/**

Get Max Kti Agent
@param[in] SocId

@retval    MaxKtiAgnet

**/
UINT8
EFIAPI
GetMaxKtiAgent (
  IN UINT8    SocId
  )
{
  return KTI_HOST_OUT_PTR->MaxKtiAgent[SocId];
}

/**

  Get DDRT Qos Mode

  @retval    DDRTQos Mode

**/
UINT8
EFIAPI
GetDdrtQosMode (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->DdrtQosMode;
}

/**

  Get Socket's ChopType
  @param[in] SocId

  @retval    Socket's Chop Type --- 11:XCC; 10:HCC; 01:MCC; 00:LCC;

**/
TYPE_CHOPTYPE_ENUM
EFIAPI
GetChopType (
  UINT8 SocId
  )
{
  if (KTI_HOST_OUT_PTR->SocketPresentBitMap & (1 << SocId)) {
    //
    // Now the ChopType is 1:1 mapped to the TYPE_CHOPTYPE_ENUM
    //
    switch (KTI_HOST_OUT_PTR->CpuInfo[SocId].ChopType) {
      case TypeLcc:
        return TypeLcc;
      case TypeMcc:
        return TypeMcc;
      case TypeHcc:
        return TypeHcc;
      case TypeXcc:
        return TypeXcc;
      default:
        return TypeChopTypeMax;
    }
  } else {
    return TypeChopTypeMax;
  }
}

/**

  Returns if SBSP Chop data is valid in KtiApi

  @retval    BOOLEAN - TRUE  - SBSP chop is ready
                       FALSE - Not ready

**/
BOOLEAN
EFIAPI
IsChopTypeValid (
  VOID
)
{
  return KTI_HOST_OUT_PTR->CpuInfo->ChopTypeValid;
}

/**

  Get Socket's Die Count
  @param[in] SocId

  @retval    The number of Die in the Socket

**/
UINT8
EFIAPI
GetDieCount (
  UINT8 SocId
  )
{
  if (KTI_HOST_OUT_PTR->SocketPresentBitMap & (1 << SocId)) {
    return KTI_HOST_OUT_PTR->CpuInfo[SocId].DieCount;
  } else {
    return INVALID_DIE_COUNT;
  }
}

/**
  Get the CXL status of the IIO stack indexed by SocId/StackId.

  @param[in]    SocId             - Socket number
  @param[in]    StackId           - Stack number

  @retval       0/1/2/3           - Please refer to the encoding in enum IIO_LINK_STATUS
**/
UINT8
EFIAPI
GetCxlStatus (
  IN  UINT8     SocId,
  IN  UINT8     StackId
  )
{
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    if (StackId < MAX_CXL_PER_SOCKET) {
      return (UINT8) KTI_HOST_OUT_PTR->CxlInfo[SocId][StackId].CxlStatus;
    } else {
      return NotSupportCxlMode;
    }
  } else if (IsCpuAndRevision (CPU_GNRSP, REV_ALL) || IsCpuAndRevision (CPU_SRFSP, REV_ALL)) {
    //
    // GNR/SRF just copy the logic of SPR, but may need to update in the future.
    //
    if (StackId < MAX_CXL_PER_SOCKET) {
      return (UINT8) KTI_HOST_OUT_PTR->CxlInfo[SocId][StackId].CxlStatus;
    } else {
      return NotSupportCxlMode;
    }
  } else {
    return NotSupportCxlMode;
  }
}

/**

  Set SlaveDieBitMap. Bit is 1 indicates the socket is Slave Die, 0 indicates Master Die.
  This SlaveDieBitMap is a subset of SocketPresentBitMap.
  The bit in SlaveDieBitMap is only valid if the same bit is set to 1 in SocketPresentBitMap.

  @param[in] SlaveDieBitMap     - SlaveDieBitMap to be set.

**/
VOID
EFIAPI
SetSlaveDieBitMap (
  UINT32 SlaveDieBitMap
  )
{
  KTI_HOST_OUT_PTR->SlaveDieBitMap = SlaveDieBitMap;
}

/**

  Get SlaveDieBitMap. Bit is 1 indicates the socket is Slave Die, 0 indicates Master Die.
  This SlaveDieBitMap is a subset of SocketPresentBitMap.
  The bit in SlaveDieBitMap is only valid if the same bit is set to 1 in SocketPresentBitMap.

**/
UINT32
EFIAPI
GetSlaveDieBitMap (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->SlaveDieBitMap;
}

/**
  Set the CXL status for the IIO stack indexed by SocId/StackId.

  @param[in]    SocId             - Socket number
  @param[in]    StackId           - Stack number
  @param[in]    Value             - Please refer to the encoding in enum IIO_LINK_STATUS

  @retval       VOID
**/
VOID
EFIAPI
SetCxlStatus (
  IN  UINT8     SocId,
  IN  UINT8     StackId,
  IN  UINT8     Value
  )
{
  //
  // If statement to resolve KW issue
  //
  if (SocId >= MAX_SOCKET) {
    ASSERT (FALSE);
    return;
  }

  if (StackId < MAX_CXL_PER_SOCKET) {
    KTI_HOST_OUT_PTR->CxlInfo[SocId][StackId].CxlStatus = (Value & IIO_LINK_STATUS_MASK);
  } else {
    KTI_HOST_OUT_PTR->CxlInfo[SocId][StackId].CxlStatus = NotSupportCxlMode;
  }
}

/**
  Set the CXL status for the IIO stack indexed by SocId/StackId.

  @param[in]    SocId             - Socket number
  @param[in]    StackId           - Stack number
  @param[in]    Value             - Please refer to the encoding in enum IIO_LINK_STATUS

  @retval       VOID
**/
VOID
EFIAPI
SetCxlBitMap (
  IN  UINT8     SocketId,
  IN  UINT8     StackId
)
{
  KTI_HOST_OUT_PTR->CxlPresentBitmap[SocketId] |= BIT0 << StackId;
}

/**
  Save CXL Uncorrectable Error Status.

  @param[in] SocId       - Socket number
  @param[in] StackId     - Box Instane, 0 based
  @param[in] ErrorStatus - The error status to be saved

  @retval    VOID
**/
VOID
EFIAPI
SaveCxlUncorrectableErrorStatus (
  IN UINT8   SocId,
  IN UINT8   StackId,
  IN UINT32  ErrorStatus
  )
{
  if ((SocId < MAX_SOCKET) && (StackId < MAX_CXL_PER_SOCKET)) {
    KTI_HOST_OUT_PTR->CxlInfo[SocId][StackId].UncorrectableErrorStatus = ErrorStatus;
  }
}

/**
  Get CXL Uncorrectable Error Status.

  @param[in] SocId       - Socket number
  @param[in] StackId     - Box Instane, 0 based

  @retval    The saved Uncorrectable Error Status or 0 when input invalid parameter.
**/
UINT32
EFIAPI
GetCxlUncorrectableErrorStatus (
  IN UINT8 SocId,
  IN UINT8 StackId
  )
{
  if ((SocId < MAX_SOCKET) && (StackId < MAX_CXL_PER_SOCKET)) {
    return KTI_HOST_OUT_PTR->CxlInfo[SocId][StackId].UncorrectableErrorStatus;
  } else {
    return 0;
  }
}

/**
  Save CXL Correctable Error Status.

  @param[in] SocId       - Socket number
  @param[in] StackId     - Box Instane, 0 based
  @param[in] ErrorStatus - The error status to be saved

  @retval    VOID
**/
VOID
EFIAPI
SaveCxlCorrectableErrorStatus (
  IN UINT8   SocId,
  IN UINT8   StackId,
  IN UINT32  ErrorStatus
  )
{
  if ((SocId < MAX_SOCKET) && (StackId < MAX_CXL_PER_SOCKET)) {
    KTI_HOST_OUT_PTR->CxlInfo[SocId][StackId].CorrectableErrorStatus = ErrorStatus;
  }
}

/**
  Get CXL Correctable Error Status.

  @param[in] SocId       - Socket number
  @param[in] StackId     - Box Instane, 0 based

  @retval    The saved Correctable Error Status or 0 when input invalid parameter.
**/
UINT32
EFIAPI
GetCxlCorrectableErrorStatus (
  IN UINT8 SocId,
  IN UINT8 StackId
  )
{
  if ((SocId < MAX_SOCKET) && (StackId < MAX_CXL_PER_SOCKET)) {
    return KTI_HOST_OUT_PTR->CxlInfo[SocId][StackId].CorrectableErrorStatus;
  } else {
    return 0;
  }
}

/**

  Return if UPI Affinity feature enabled or not
  @param[in] VOID

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
IsUpiAffinityEnabled (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->UpiAffinityEn;
}

/**

  Return if UPI Twisted links exist
  @param[in] VOID

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
IsUpiTwistedLinks (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->TwistedKtiLinks;
}

/**
  This API is invoked to get the current PCIE Gen4 Speed Limit setting of
  all stacks of the specified socket.

  @param[in] Socket      - Socket ID

  @retval    The current PCIE Gen4 Speed Limit setting of the specified socket.
             in which each bit map to a stack.
             1'b1: Limit to Gen4 Speed.
             1'b0: No limit, PCIE can go up to Gen5.
**/
UINT8
EFIAPI
GetPcieGen4SpeedLimit (
  IN  UINT8    Socket
  )
{
  if (Socket < MAX_SOCKET) {
    return KTI_HOST_OUT_PTR->PcieGen4SpeedLimit[Socket];
  }

  return 0;
}

/**
  This API is invoked to update the PCIE Gen4 Speed Limit settings for each stack.
  Note: Stack[0] of Socket-L is for DMI, in this case the input LimitNeeded should be FALSE.

  @param[in] Socket      - Socket ID
  @param[in] Stack       - Stack ID
  @param[in] LimitNeeded - TRUE: Limit to Gen4 Speed; FALSE: No limit, PCIE can go up to Gen5.

  @retval    VOID
**/
VOID
EFIAPI
SetPcieGen4SpeedLimit (
  IN  UINT8    Socket,
  IN  UINT8    Stack,
  IN  BOOLEAN  LimitNeeded
  )
{
  if ((Socket < MAX_SOCKET) && (Stack < MAX_PI5_SS_PER_SOCKET)) {
    if (LimitNeeded) {
      KTI_HOST_OUT_PTR->PcieGen4SpeedLimit[Socket] |= (1 << Stack);
    } else {
      KTI_HOST_OUT_PTR->PcieGen4SpeedLimit[Socket] &= (UINT8) (~(1 << Stack));
    }
  }
}

/**
  Return if it's HBM Sku
  @param[in] VOID

  @retval    TRUE
  @retval    FALSE

**/
BOOLEAN
EFIAPI
IsHbmSku (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->HbmSku;
}

/**
  Get PCH present bit map. Bit seeting indicates the corresponding socket has legay PCH or PCH.IO expander present.
  For instance, 0x03 indicates socket 0 and socket 1 have Pches.

  @param[in] VOID

  @retval    UINT8       - PCH present bit map
**/
UINT8
EFIAPI
GetPchPresentBitMap (
  VOID
  )
{
  //
  // Currently, magic number used here to indicate only socket 0 has PCH. In the future, this function shall be enchanced
  // to convey which sockets have PCHes when the design is settle down.
  //
  return KTI_HOST_OUT_PTR->PchPresentBitMap = 1;
}

/**
  Get the size of reserved MMIOL resouce for DMI mem bar.

  @param[in] UINT8  Socket  - Socket Index

  @retval    UINT32         - return the size of reserved MMIOL resouce for DMI mem bar.
**/
UINT32
EFIAPI
GetDmiMemBarSize (
  IN  UINT8   Socket
  )
{
  return KTI_HOST_OUT_PTR->DmiMemBarSize[Socket];
}

/**
  This routine is used by IIO to pass the PI5 supported protocol information to KTI RC.

  @param[in] Socket       - Socket ID
  @param[in] Stack        - Stack ID
  @param[in] Protocol     - Current PI5 protocol supported, which is encoded as following:
                            0x00: PI5_IP_DISABLE_CONFIG_DMI_PCIEGEN4_ONLY_VALUE
                            0x01: PI5_IP_DISABLE_CONFIG_SINGLE_PROTOCOL_VALUE
                            0x02: PI5_IP_DISABLE_CONFIG_MUTI_PROTOCOL_VALUE
                            0xFF: PI5_IP_DISABLE_CONFIG_INVALID_VALUE

  @retval    VOID
**/
VOID
EFIAPI
SetPi5ProtocolStatus (
  IN  UINT8    Socket,
  IN  UINT8    Stack,
  IN  UINT8    Protocol
  )
{
  if ((Socket < MAX_SOCKET) && (Stack < MAX_CXL_PER_SOCKET)) {
    KTI_HOST_OUT_PTR->Pi5Protocol[Socket][Stack] = Protocol;
  }
}

/**
  This routine is to get the PI5 supported protocol information of a given stack.

  @param[in] Socket       - Socket ID
  @param[in] Stack        - Stack ID

  @retval    Current supported PI5 protocol of the given stack, which is encoded as following:
                            0x00: PI5_IP_DISABLE_CONFIG_DMI_PCIEGEN4_ONLY_VALUE
                            0x01: PI5_IP_DISABLE_CONFIG_SINGLE_PROTOCOL_VALUE
                            0x02: PI5_IP_DISABLE_CONFIG_MUTI_PROTOCOL_VALUE
                            0xFF: PI5_IP_DISABLE_CONFIG_INVALID_VALUE
**/
UINT8
EFIAPI
GetPi5ProtocolStatus (
  IN  UINT8    Socket,
  IN  UINT8    Stack
  )
{
  if ((Socket < MAX_SOCKET) && (Stack < MAX_CXL_PER_SOCKET)) {
    return KTI_HOST_OUT_PTR->Pi5Protocol[Socket][Stack];
  }

  return PI5_IP_DISABLE_CONFIG_INVALID_VALUE;
}

/**

  Get the UPI txeq adaptation table Pointer

  @param[out] TablePointer    - pointer to UPI txeq adaptation table

  @retval
          - EFI_SUCCESS           Command succeeded.
          - EFI_INVALID_PARAMETER Invalid read data detected from pcode.
          - EFI_NOT_FOUND         No valid txeq adaptation table.

**/
EFI_STATUS
EFIAPI
GetUpiTxeqAdaptationTable (
  OUT KTI_ADAPTATION_TABLE *TablePointer
  )
{
  KTI_HOST_OUT *KtiHostOutPtr;
  UINT8        AdaptedSpeed;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  if (TablePointer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  for (AdaptedSpeed = 0; AdaptedSpeed < KTI_SPEEDS_SUPPORTED; AdaptedSpeed++) {
    if (KtiHostOutPtr->KtiAdaptationTable[AdaptedSpeed].TableValid) {
      CopyMem ((CHAR8 *)TablePointer, (CHAR8 *)(&KtiHostOutPtr->KtiAdaptationTable[0]), sizeof (KtiHostOutPtr->KtiAdaptationTable));
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/**
  This routine can be used to get the MicroCode Revision loaded on Sbsp.

  @param[in] VOID

  @retval    MicroCode Revision
**/
UINT32
EFIAPI
GetSbspMicrocodeRev (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->MicroCodeRevision;
}

/**
  Get CXL port number.

  @param          - VOID

  @retval         The number of CXL Ports in this system.
**/
UINT8
EFIAPI
GetCxlPortNumber (
  VOID
  )
{
  UINT8                       Socket;
  UINT8                       Stack;
  UINT8                       CxlPortCount;

  CxlPortCount = 0;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {  //Socket is not present
      continue;
    }

    for (Stack = 0; Stack < MAX_CXL_PER_SOCKET; Stack++) {
      if(!IfStackPresent (Socket, Stack)) {  //Stack is not present
        continue;
      }

      if (GetCxlStatus (Socket, Stack) == AlreadyInCxlMode) {  // This stack works as CXL
        CxlPortCount++;
      }
    }  //for (Stack ...)
  }  //for (Socket ...)

  return CxlPortCount;
}

/**
  Get the MMIO32 resource size used by CXL DP/UP Port.

  @param[in]        Socket     -  Socket number
  @param[in]        Stack      -  Stack number

  @retval           MMIO32 resource size (Unit: in Byte)
**/
UINT32
EFIAPI
GetCxlDpUpReservedMmio32Size (
  IN  UINT8       Socket,
  IN  UINT8       Stack
  )
{
  //
  // Reserve MMIOL resource(size: 128KB x 3) for CXL Downstream and Upstream port, details as below
  //  CXL DP: 4KB(RCRB)
  //  CXL UP: 4KB(RCRB)
  //          120KB(Reserved to meet the MEMBAR0 alignment)
  //  CXL DP: 128KB(MEMBAR0)
  //  CXL UP: 128KB(MEMBAR0)
  //

  if ((Socket < MAX_SOCKET) && (Stack < MAX_CXL_PER_SOCKET)) {
    if (GetCxlStatus (Socket, Stack) == AlreadyInCxlMode) {
      return (CXL_MEMBAR0_SIZE_PER_PORT * 3);
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

/**

  Returns if the UPI port is the UPIMDF subsystem

  @param[in] Socket  - Socket Index
  @param[in] UpiPort - UPI Port number

  @retval    BOOLEAN - TRUE  - The port is a UPIMDF subsystem
                       FALSE - The port is normal UPI port

**/
BOOLEAN
EFIAPI
IsUpiPortUpimdf (
  IN UINT8      Socket,
  IN UINT8      UpiPort
  )
{
  if ((KTI_HOST_OUT_PTR->UpimdfPortBitmap[Socket] & (1 << UpiPort)) != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}
