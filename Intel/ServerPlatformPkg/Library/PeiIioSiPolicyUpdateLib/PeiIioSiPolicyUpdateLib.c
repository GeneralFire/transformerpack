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

//
// EDK and EDKII have different GUID formats
//
#include <PiPei.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <IioPlatformData.h>
#include <IioRegs.h>
#include <IioSetupDefinitions.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/KtiApi.h>
#include <Library/IioSysInfoLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Ppi/IioSiPolicyPpi.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/X2ApicIdDetectLib.h>
#include <Guid/PlatformInfo.h>
#include <Library/SystemInfoLib.h>
#include <Library/DebugLib.h>
#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#include <Guid/SocketVariable.h>
#include <Library/SetupLib.h>
#include <Library/UbaIioPortBifurcationInitLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/PeiIioSiPolicyLib.h>

UINT8
GetPlatformInfo (
  VOID
  )
{
  EFI_PLATFORM_INFO             *PlatformInfo;
  EFI_HOB_GUID_TYPE             *GuidHob;

  GuidHob    = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT(GuidHob != NULL);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR,"GetPlatformInfo failed to get gEfiPlatformInfoGuid Hob! \n"));
    return 0;
  }

  PlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  return PlatformInfo->BoardId;
}

/**

    This function update SRIS configuration settings
    based on the IOUx bifurcation.

    @retval None

**/
VOID
UpdateSrisConfigSkx(
  IN IIO_GLOBALS   *IioGlobalData,
  UINT8            Iou,
  UINT8            IioIndex
  )
{
  UINT8 MaxPortNumberPerSocket;
  UINT8 PXPOffset;
  UINT8 CurrentIOUConfigValue = 0;

  MaxPortNumberPerSocket = GetMaxPortNumPerSocket ();

  PXPOffset = (IioIndex * MaxPortNumberPerSocket);
  switch (Iou) {
    case IioIou0:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][0];
      PXPOffset+= PORT_1A_INDEX;
      break;
    case IioIou1:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][1];
      PXPOffset+= PORT_2A_INDEX;
      break;
    case IioIou2:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][2];
      PXPOffset+= PORT_3A_INDEX;
      break;
    case IioMcp0:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][3];
      PXPOffset+= PORT_4A_INDEX;
      break;
    case IioMcp1:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][4];
      PXPOffset += PORT_5A_INDEX;
      break;
    default:
      DEBUG ((DEBUG_WARN, "default case.\n"));  //Auto added. Please review.
      break;
  }// switch Iou

  DEBUG ((DEBUG_INFO, "SRIS: Iou = %x, CurrentIOUConfigValue = %x \n", Iou, CurrentIOUConfigValue));

  switch(CurrentIOUConfigValue){
    case IIO_BIFURCATE_x4x4xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE){
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8x4x4:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE){
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE){
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE){
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxxxxx16:
      if( IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE){
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    default:
      // Do nothing we will conserve user configuration
      break;
  }// CurrentIOUConfigValue
}

VOID
UpdateSrisConfig10nm (
  IN IIO_GLOBALS   *IioGlobalData,
  UINT8            Iou,
  UINT8            IioIndex
  )
{
  UINT8 MaxPortNumberPerSocket;
  UINT8 PXPOffset;
  UINT8 CurrentIOUConfigValue = 0;

  MaxPortNumberPerSocket = GetMaxPortNumPerSocket ();

  PXPOffset = IioIndex * MaxPortNumberPerSocket;
  switch (Iou) {
  case IioIou010nm:
    CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][0];
    PXPOffset += PORT_1A_INDEX_1;
    break;
  case IioIou110nm:
    CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][1];
    PXPOffset += PORT_2A_INDEX_2;
    break;
  case IioIou210nm:
    CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][2];
    PXPOffset += PORT_3A_INDEX_3;
    break;
  case IioIou310nm:
    CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][3];
    PXPOffset += PORT_4A_INDEX_4;
    break;
  case IioIou410nm:
    CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][4];
    PXPOffset += PORT_5A_INDEX_5;
    break;
  default:
    DEBUG ((EFI_D_WARN, "default case.\n"));  //Auto added. Please review.
    break;
  }// switch Iou

  DEBUG ((DEBUG_INFO, "SRIS: Iou = %x, CurrentIOUConfigValue = %x\n", Iou, CurrentIOUConfigValue));

  switch (CurrentIOUConfigValue) {
    case IIO_BIFURCATE_x4x4xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8x4x4:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxxxxx16:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    default:
      // Do nothing we will conserve user configuration
      break;
  }// CurrentIOUConfigValue
}

VOID
UpdateSrisConfig10nmD (
  IN IIO_GLOBALS   *IioGlobalData,
  UINT8            Iou,
  UINT8            IioIndex
  )
{
  UINT8 MaxPortNumberPerSocket;
  UINT8 PXPOffset;
  UINT8 CurrentIOUConfigValue = 0;

  MaxPortNumberPerSocket = GetMaxPortNumPerSocket ();

  PXPOffset = IioIndex * MaxPortNumberPerSocket;
  switch (Iou) {
    case IioIou010nm:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][0];
      PXPOffset+= PORT_1A_INDEX_S1;
      break;
    case IioIou110nm:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][1];
      PXPOffset+= PORT_2A_INDEX_S1;
      break;
    default:
      break;
  }// switch Iou

  DEBUG ((DEBUG_INFO, "SRIS: Iou = %x, CurrentIOUConfigValue = %x\n", Iou, CurrentIOUConfigValue));

  switch (CurrentIOUConfigValue) {
    case IIO_BIFURCATE_x4x4xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8x4x4:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxxxxx16:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    default:
      // Do nothing we will conserve user configuration
      break;
  }// CurrentIOUConfigValue
}

/**

  Initialize the IIO_GLOBALS with the IOUx Bifurcation
  based on Board layout or user selection

  @param IioGlobalData - Pointer to IIO_GLOBALS

  @retval VOID  All other error conditions encountered result in an ASSERT

**/
VOID
GetIioPortBifurcationInfo (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{
  UINT8             Iou;
  UINT8             IioIndex;

  DEBUG ((DEBUG_INFO,"Calling GetIioPortBifurcationInfo Start\n"));

  //
  // Initialize all Bifurcation settings once
  //
  IioPortBifurcationInit (IioGlobalData); // from UBA code

  DEBUG ((DEBUG_INFO,"Calling GetIioPortBifurcationInfo End\n"));

  DEBUG ((DEBUG_INFO, "UpdateSrisConfig Start"));
  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++){
    if (!SocketPresent (IioIndex)) {
      continue;
    }

    if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      for(Iou = IioIou0; Iou < IioIouMax; Iou++) {
        UpdateSrisConfigSkx (IioGlobalData, Iou, IioIndex);
      }
    } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
      UpdateSrisConfig10nm (IioGlobalData, IioIou010nm, IioIndex);
      UpdateSrisConfig10nm (IioGlobalData, IioIou110nm, IioIndex);
      UpdateSrisConfig10nm (IioGlobalData, IioIou210nm, IioIndex);
      UpdateSrisConfig10nm (IioGlobalData, IioIou310nm, IioIndex);
      UpdateSrisConfig10nm (IioGlobalData, IioIou410nm, IioIndex);
    } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
      UpdateSrisConfig10nmD (IioGlobalData, IioIou010nm, IioIndex);
    } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
      UpdateSrisConfig10nmD (IioGlobalData, IioIou010nm, IioIndex);
      UpdateSrisConfig10nmD (IioGlobalData, IioIou110nm, IioIndex);
    }
  }// IioIndex
}

VOID
UpdateSkuPersonality (
  IIO_GLOBALS  *IioGlobalData
  )
{
  UINT8  MaxStackSocket;
  UINT8  StackIndex;
  UINT8  IioIndex;

  for (IioIndex = 0; IioIndex < MAX_SOCKET; IioIndex++) {

    MaxStackSocket = GetMaxStackNumPerSocket ();
    for(StackIndex = 0; StackIndex < MaxStackSocket; StackIndex++) {

      if (CheckStackPersonality (IioIndex, StackIndex, TYPE_MCP)) {
        IioGlobalData->IioVar.IioVData.SkuPersonality[IioIndex] = TYPE_MCP;
      }

      if (CheckStackPersonality (IioIndex, StackIndex, TYPE_FPGA)) {
        if (IfFpgaActive (IioIndex)) {
          IioGlobalData->IioVar.IioVData.SkuPersonality[IioIndex] = TYPE_FPGA;
        }
      }
    }
  }
}

/**
  Initialize ConfigIOU settings for specified socket and IOU

  @param IioGlobalData   Pointer to IIO_GLOBALS
  @param Socket    the socket for which we are initializing settings
  @param Iou       the Iou for which we are initializing settings
  @param Settings  the settings to apply
*/
STATIC VOID
InitConfigIou (
  IN IIO_GLOBALS   *IioGlobalData,
  UINT8 Socket,
  UINT8 Iou,
  UINT8 Settings
  )
{
  ASSERT (Socket < ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU));
  ASSERT (Iou < ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0]));

  if (Socket >= ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU)) {
    return;
  }
  if (Iou >= ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0])) {
    return;
  }

  IioGlobalData->SetupData.ConfigIOU[Socket][Iou] = Settings;
}


/**
  Change any bifurcations settings that are AUTO into something
  specific.  Current implementation makes an arbitrary choice to
  bifurcate to a x16.

  @param IioGlobalData Pointer to IIO_GLOBALS
*/
STATIC VOID
UpdateAutoBifurcation (
  IN OUT IIO_GLOBALS *IioGlobalData
  )
{
  UINT8 Socket;
  UINT8 Port;
  IIO_CONFIG *SetupData;

  ASSERT (IioGlobalData != NULL);
  if (IioGlobalData == NULL) {
    return;
  }

  SetupData = &IioGlobalData->SetupData;

  for (Socket = 0; Socket < ARRAY_SIZE (SetupData->ConfigIOU); Socket++) {
    for (Port = 0; Port < ARRAY_SIZE (SetupData->ConfigIOU[0]); Port++) {
      if (SetupData->ConfigIOU[Socket][Port] == CONFIG_IOU_AUTO) {
        SetupData->ConfigIOU[Socket][Port] = IIO_BIFURCATE_xxxxxx16;
        DEBUG ((DEBUG_INFO, "updated socket %d port %d from AUTO to x16 bifurcation\n", Socket, Port));
      }
    }
  }
}

/*++

Routine Description:

  This function performs Iio Silicon init policy initialzation

Returns:

--*/
VOID
UpdateIioSiPolicy (
  VOID
  )
{
  EFI_STATUS                  Status;
  // APTIOV_SERVER_OVERRIDE_RC_START : To solve structure redefinition error in AMI.   
  INTEL_SETUP_DATA                  SetupData;
  // APTIOV_SERVER_OVERRIDE_RC_END : To solve structure redefinition error in AMI.
  UINT16                      Index;
  UINT8                       Socket; // Used to copy setup option from S0-S3 into S4-S
  UINT8                       Stack;
  UINT8                       Func;
  UINT8                       MaxPortNumberPerSocket;
  UINT8                       MaxNtbPortPerSocket;
  UINT32                      MaxTotalPorts;
  UINT8                       MaxPortsPerStack;
  UINT8                       StackIndex;
  UINT8                       StackInSocket;
  UINT8                       TotalStackIndex;
  UINT8                       PortInStack;
  UINT16                      PortIndex = 0;
  UINT16                      PortIndexInSetup = 0;
  UINT8                       IioIndex;
  UINT8                       NtbPortNumPerSkt;
  UINT8                       PortNtbIndex;
  EFI_HOB_GUID_TYPE           *GuidHob;
  IIO_SI_POLICY_PPI           *IioSiPolicyPpi;
  IIO_GLOBALS                 *IioGlobalData;
  EFI_PEI_PPI_DESCRIPTOR      *InstallIioSiPolicyPpiDesc;
  UINT8                       RootPortIndex;
  UINT8                       MaxPortPerSocket;
  UINT8                       MaxPortPerIou;
  UINT8                       PortIdx;
  UINT8                       IouIndex;
  BOOLEAN                     Is14nmCpu;

  InstallIioSiPolicyPpiDesc   = NULL;
  IioSiPolicyPpi              = NULL;
  IioGlobalData               = NULL;
  // APTIOV_SERVER_OVERRIDE_RC_START : To solve structure redefinition error in AMI.   
  ZeroMem (&SetupData, sizeof( INTEL_SETUP_DATA));
  // APTIOV_SERVER_OVERRIDE_RC_END : To solve structure redefinition error in AMI.  
  Status = GetEntireConfig (&SetupData);
  ASSERT_EFI_ERROR (Status);

  GuidHob = GetFirstGuidHob (&gIioSiPolicyHobGuid);
  ASSERT(GuidHob != NULL);
  IioGlobalData = (IIO_GLOBALS *) GET_GUID_HOB_DATA (GuidHob);

  Is14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);

  DEBUG ((DEBUG_INFO, "IioUpdateSiPolicy: LocatePpi gIioSiPolicyPpiGuid = %r\n", Status));
  if (!EFI_ERROR (Status)) {

    IioGlobalData->SetupData.VTdSupport                   = SetupData.SocketConfig.IioConfig.VTdSupport;

    //
    // For InterruptRemap Auto option
    //
    if (SetupData.SocketConfig.IioConfig.InterruptRemap == IIO_OPTION_AUTO) {
      if (SetupData.SocketConfig.IioConfig.VTdSupport) {
        IioGlobalData->SetupData.InterruptRemap = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.InterruptRemap = IIO_OPTION_DISABLE;
      }
    } else {
      if (SetupData.SocketConfig.IioConfig.VTdSupport) {
        IioGlobalData->SetupData.InterruptRemap = SetupData.SocketConfig.IioConfig.InterruptRemap;
      } else {
        IioGlobalData->SetupData.InterruptRemap = IIO_OPTION_DISABLE;
      }
    }
    IioGlobalData->SetupData.CoherencySupport             = SetupData.SocketConfig.IioConfig.CoherencySupport;
    IioGlobalData->SetupData.ATS                          = SetupData.SocketConfig.IioConfig.ATS;

    IioGlobalData->SetupData.PostedInterrupt              = SetupData.SocketConfig.IioConfig.PostedInterrupt;
    IioGlobalData->SetupData.VtdAcsWa                     = SetupData.SocketConfig.IioConfig.VtdAcsWa;
    IioGlobalData->SetupData.PerformanceTuningMode        = SetupData.SocketConfig.IioConfig.PerformanceTuningMode;

    IioGlobalData->SetupData.CompletionTimeoutGlobal      = SetupData.SocketConfig.IioConfig.CompletionTimeoutGlobal;
    IioGlobalData->SetupData.CompletionTimeoutGlobalValue = SetupData.SocketConfig.IioConfig.CompletionTimeoutGlobalValue;
    IioGlobalData->SetupData.CoherentReadPart             = SetupData.SocketConfig.IioConfig.CoherentReadPart;
    IioGlobalData->SetupData.CoherentReadFull             = SetupData.SocketConfig.IioConfig.CoherentReadFull;
    IioGlobalData->SetupData.PcieGlobalAspm               = SetupData.SocketConfig.IioConfig.PcieGlobalAspm;
    IioGlobalData->SetupData.StopAndScream                = SetupData.SocketConfig.IioConfig.StopAndScream;
    IioGlobalData->SetupData.SnoopResponseHoldOff         = SetupData.SocketConfig.IioConfig.SnoopResponseHoldOff;
    IioGlobalData->SetupData.IioPcieConfig.PCIe_LTR       = SetupData.SocketConfig.IioConfig.PCIe_LTR;
    IioGlobalData->SetupData.PcieExtendedTagField         = SetupData.SocketConfig.IioConfig.PcieExtendedTagField;
    IioGlobalData->SetupData.Pcie10bitTag                 = SetupData.SocketConfig.IioConfig.Pcie10bitTag;
    IioGlobalData->SetupData.PCIe_AtomicOpReq             = SetupData.SocketConfig.IioConfig.PCIe_AtomicOpReq;
    IioGlobalData->SetupData.PcieMaxReadRequestSize       = SetupData.SocketConfig.IioConfig.PcieMaxReadRequestSize;
    IioGlobalData->SetupData.IioPcieConfig.PciePtm        = SetupData.SocketConfig.IioConfig.PciePtm;

    IioGlobalData->SetupData.DisableTPH                   = SetupData.SocketConfig.IioConfig.DisableTPH;
    IioGlobalData->SetupData.PrioritizeTPH                = SetupData.SocketConfig.IioConfig.PrioritizeTPH;
    IioGlobalData->SetupData.CbRelaxedOrdering            = SetupData.SocketConfig.IioConfig.CbRelaxedOrdering;
    IioGlobalData->SetupData.PCUF6Hide                    = SetupData.SocketConfig.IioConfig.PCUF6Hide;
    IioGlobalData->SetupData.EN1K                         = SetupData.SocketConfig.IioConfig.EN1K;
    IioGlobalData->SetupData.DualCvIoFlow                 = SetupData.SocketConfig.IioConfig.DualCvIoFlow;
    IioGlobalData->SetupData.PcieBiosTrainEnable          = SetupData.SocketConfig.IioConfig.PcieBiosTrainEnable;
    IioGlobalData->SetupData.MultiCastEnable              = SetupData.SocketConfig.IioConfig.MultiCastEnable;

    if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
      IioGlobalData->SetupData.DfxLtssmLogger               = SetupData.SocketConfig.IioConfig.DfxLtssmLogger;
      IioGlobalData->SetupData.DfxLtssmLoggerStop           = SetupData.SocketConfig.IioConfig.DfxLtssmLoggerStop;
      IioGlobalData->SetupData.DfxLtssmLoggerSpeed          = SetupData.SocketConfig.IioConfig.DfxLtssmLoggerSpeed;
      IioGlobalData->SetupData.DfxLtssmLoggerMask           = SetupData.SocketConfig.IioConfig.DfxLtssmLoggerMask;
      IioGlobalData->SetupData.DfxJitterLogger              = SetupData.SocketConfig.IioConfig.DfxJitterLogger;

      IioGlobalData->SetupData.DfxIioRcFlow                 = SetupData.SocketConfig.IioConfig.DfxIioRcFlow;
      IioGlobalData->SetupData.DfxIioLinkTraining           = SetupData.SocketConfig.IioConfig.DfxIioLinkTraining;
      if (IsSiliconWorkaroundEnabled ("S1408825355")) {
        IioGlobalData->SetupData.DfxOtcPipeHazardThresh     = SetupData.SocketConfig.IioConfig.DfxOtcPipeHazardThresh;
      } else {
        //
        // This forces the suppressed setup option to a disabled value
        //
        IioGlobalData->SetupData.DfxOtcPipeHazardThresh     = 0;
      }
      IioGlobalData->SetupData.DfxBadTransactionTypeWa      = SetupData.SocketConfig.IioConfig.DfxBadTransactionTypeWa;
      IioGlobalData->SetupData.DfxNpkAllocatingFlow         = SetupData.SocketConfig.IioConfig.DfxNpkAllocatingFlow;
      IioGlobalData->SetupData.SkipPortPersonalityLock = SetupData.SocketConfig.IioConfig.SkipPortPersonalityLock != IIO_OPTION_DISABLE;
      IioGlobalData->SetupData.CxlHeaderBypass = SetupData.SocketConfig.IioConfig.CxlHeaderBypass != IIO_OPTION_DISABLE;
      IioGlobalData->SetupData.DFXEnable                      = SetupData.SocketConfig.IioConfig.DFXEnable;
      IioGlobalData->SetupData.DfxDisableBiosDone             = SetupData.SocketConfig.IioConfig.DfxDisableBiosDone;

      IioGlobalData->IioVar.IioVData.EVMode = 0;
      if (IioGlobalData->SetupData.DFXEnable) {
        IioGlobalData->IioVar.IioVData.EVMode = 1;
      }
    }

    IioGlobalData->SetupData.CbDmaMultiCastEnable         = SetupData.SocketConfig.IioConfig.CbDmaMultiCastEnable;
    IioGlobalData->SetupData.McastBaseAddrRegion          = SetupData.SocketConfig.IioConfig.McastBaseAddrRegion;
    IioGlobalData->SetupData.McastIndexPosition           = SetupData.SocketConfig.IioConfig.McastIndexPosition;
    IioGlobalData->SetupData.McastNumGroup                = SetupData.SocketConfig.IioConfig.McastNumGroup;
    //
    // PCIE Global Option
    //
    IioGlobalData->SetupData.NoSnoopRdCfg                 = SetupData.SocketConfig.IioConfig.NoSnoopRdCfg;
    IioGlobalData->SetupData.NoSnoopWrCfg                 = SetupData.SocketConfig.IioConfig.NoSnoopWrCfg;
    IioGlobalData->SetupData.MaxReadCompCombSize          = SetupData.SocketConfig.IioConfig.MaxReadCompCombSize;
    IioGlobalData->SetupData.ProblematicPort              = SetupData.SocketConfig.IioConfig.ProblematicPort;
    IioGlobalData->SetupData.DmiAllocatingFlow            = SetupData.SocketConfig.IioConfig.DmiAllocatingFlow;
    IioGlobalData->SetupData.PcieAllocatingFlow           = SetupData.SocketConfig.IioConfig.PcieAllocatingFlow;
    IioGlobalData->SetupData.IioPcieConfig.PcieHotPlugEnable = SetupData.SocketConfig.IioConfig.PcieHotPlugEnable;
    IioGlobalData->SetupData.PcieAcpiHotPlugEnable        = SetupData.SocketConfig.IioConfig.PcieAcpiHotPlugEnable;
    IioGlobalData->SetupData.HaltOnDmiDegraded            = SetupData.SocketConfig.IioConfig.HaltOnDmiDegraded;
    IioGlobalData->SetupData.RxClockWA                    = SetupData.SocketConfig.IioConfig.RxClockWA;
    IioGlobalData->SetupData.GlobalPme2AckTOCtrl          = SetupData.SocketConfig.IioConfig.GlobalPme2AckTOCtrl;
    IioGlobalData->SetupData.DelayBeforePCIeLinkTraining  = SetupData.SocketConfig.IioConfig.DelayBeforePCIeLinkTraining;

    IioGlobalData->SetupData.PcieSlotOprom1                 = SetupData.SocketConfig.IioConfig.PcieSlotOprom1;
    IioGlobalData->SetupData.PcieSlotOprom2                 = SetupData.SocketConfig.IioConfig.PcieSlotOprom2;
    IioGlobalData->SetupData.PcieSlotOprom3                 = SetupData.SocketConfig.IioConfig.PcieSlotOprom3;
    IioGlobalData->SetupData.PcieSlotOprom4                 = SetupData.SocketConfig.IioConfig.PcieSlotOprom4;
    IioGlobalData->SetupData.PcieSlotOprom5                 = SetupData.SocketConfig.IioConfig.PcieSlotOprom5;
    IioGlobalData->SetupData.PcieSlotOprom6                 = SetupData.SocketConfig.IioConfig.PcieSlotOprom6;
    IioGlobalData->SetupData.PcieSlotOprom7                 = SetupData.SocketConfig.IioConfig.PcieSlotOprom7;
    IioGlobalData->SetupData.PcieSlotOprom8                 = SetupData.SocketConfig.IioConfig.PcieSlotOprom8;
    IioGlobalData->SetupData.PcieSlotItemCtrl               = SetupData.SocketConfig.IioConfig.PcieSlotItemCtrl;
    IioGlobalData->SetupData.PcieRelaxedOrdering            = SetupData.SocketConfig.IioConfig.PcieRelaxedOrdering;
    IioGlobalData->SetupData.PciePhyTestMode                = SetupData.SocketConfig.IioConfig.PciePhyTestMode;
    IioGlobalData->SetupData.PcieEnqCmdSupport              = SetupData.SocketConfig.IioConfig.PcieEnqCmdSupport;
    IioGlobalData->SetupData.Xppdef                         = SetupData.SocketConfig.IioConfig.Xppdef;
    IioGlobalData->SetupData.Pci64BitResourceAllocation     = SetupData.SocketConfig.IioConfig.Pci64BitResourceAllocation;

  IioGlobalData->SetupData.ProcessorMsrLockControl = 1;
    IioGlobalData->SetupData.Serr                          = SetupData.SystemConfig.PropagateSerr;
    IioGlobalData->SetupData.Perr                          = SetupData.SystemConfig.PropagatePerr;

    IioGlobalData->SetupData.LockChipset                   = SetupData.SocketConfig.SocketProcessorCoreConfiguration.LockChipset;

    IioGlobalData->SetupData.PeciInTrustControlBit         = SetupData.SocketConfig.SocketProcessorCoreConfiguration.PeciInTrustControlBit;
    IioGlobalData->SetupData.LegacyVgaSoc                  = SetupData.SocketConfig.UpiConfig.LegacyVgaSoc;
    IioGlobalData->SetupData.LegacyVgaStack                = SetupData.SocketConfig.UpiConfig.LegacyVgaStack;

    //
    // PCH
    //
    IioGlobalData->SetupData.PciePllSsc                   = SetupData.PchSetup.PciePllSsc;

    UpdateSkuPersonality (IioGlobalData);

    // Get IOAPIC setup option for S4-S7 in the same way as S0-S3
    for (Stack = 0; Stack < TOTAL_IIO_STACKS; Stack ++) {
      IioGlobalData->SetupData.DevPresIoApicIio[Stack] = SetupData.SocketConfig.IioConfig.DevPresIoApicIio[(Stack < 24) ? Stack : Stack - 24];
    }

    // Disable the IOAPIC in the stack where FPGA connected.
    if (Is14nmCpu) {
      for (Socket=0; Socket < MaxIIO; Socket++) {
        if (IioGlobalData->IioVar.IioVData.SkuPersonality[Socket] == TYPE_FPGA) {
          IioGlobalData->SetupData.DevPresIoApicIio[Socket * MAX_IIO_STACK + IIO_PSTACK3] = 0;
        }
      }
    }

    // PCIE RAS (Errors)
    if (IsSiliconWorkaroundEnabled ("S2208074954") && SetupData.SystemConfig.IioErrorEn && SetupData.SocketConfig.IioConfig.VTdSupport) {

      IioGlobalData->SetupData.IioErrorEn = FALSE;
      DEBUG ((DEBUG_WARN, "[IIO] WARNING: ICX R0 VT-d workaround enforces IIO errors disable\n"));

    } else {

      IioGlobalData->SetupData.IioErrorEn = SetupData.SystemConfig.IioErrorEn;
    }
    IioGlobalData->SetupData.LerEn = SetupData.SystemConfig.LerEn;
    IioGlobalData->SetupData.WheaPcieErrInjEn = SetupData.SystemConfig.WheaPcieErrInjEn;
    IioGlobalData->SetupData.ProcessorX2apic = SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorX2apic;
    //
    // Setup Option per Socket
    //
    for (Socket = 0; Socket < MaxIIO; Socket++) {
      if (!SocketPresent (Socket)) {
        continue;
      }
      // Copy bifurcation setup option from S0-S3 into S4-S7
      Index = (Socket < 4) ? Socket : Socket - 4;

      if (ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0]) > 0) {
        InitConfigIou (IioGlobalData, Socket, 0, SetupData.SocketConfig.IioConfig.ConfigIOU0[Index]);
      }
      if (ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0]) > 1) {
        InitConfigIou (IioGlobalData, Socket, 1, SetupData.SocketConfig.IioConfig.ConfigIOU1[Index]);
      }
      if (ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0]) > 2) {
        InitConfigIou (IioGlobalData, Socket, 2, SetupData.SocketConfig.IioConfig.ConfigIOU2[Index]);
      }
      if (ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0]) > 3) {
        InitConfigIou (IioGlobalData, Socket, 3, SetupData.SocketConfig.IioConfig.ConfigIOU3[Index]);
      }

      if (ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0]) > 4) {
        InitConfigIou (IioGlobalData, Socket, 4, SetupData.SocketConfig.IioConfig.ConfigIOU4[Index]);
      }
      if (ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0]) > 5) {
        InitConfigIou (IioGlobalData, Socket, 5, SetupData.SocketConfig.IioConfig.ConfigIOU5[Index]);
      }
      if (ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0]) > 6) {
        InitConfigIou (IioGlobalData, Socket, 6, SetupData.SocketConfig.IioConfig.ConfigIOU6[Index]);
      }
      if (ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0]) > 7) {
        InitConfigIou (IioGlobalData, Socket, 7, SetupData.SocketConfig.IioConfig.ConfigIOU7[Index]);
      }

      IioGlobalData->SetupData.CompletionTimeout[Socket]               = SetupData.SocketConfig.IioConfig.CompletionTimeout[Index];
      IioGlobalData->SetupData.CompletionTimeoutValue[Socket]          = SetupData.SocketConfig.IioConfig.CompletionTimeoutValue[Index];
      IioGlobalData->SetupData.IioPcieConfig.RpCorrectableErrorEsc[Socket]           = SetupData.SocketConfig.IioConfig.RpCorrectableErrorEsc[Index];
      IioGlobalData->SetupData.IioPcieConfig.RpUncorrectableNonFatalErrorEsc[Socket] = SetupData.SocketConfig.IioConfig.RpUncorrectableNonFatalErrorEsc[Index];
      IioGlobalData->SetupData.IioPcieConfig.RpUncorrectableFatalErrorEsc[Socket]    = SetupData.SocketConfig.IioConfig.RpUncorrectableFatalErrorEsc[Index];

      for (Index = 0; Index < MAX_IOU_PER_SOCKET; Index++) {
        IioGlobalData->SetupData.PcieSubSystemMode[Socket][Index] = SetupData.SocketConfig.IioConfig.PcieSubSystemMode[(Socket * MAX_IOU_PER_SOCKET) + Index];
      } // for MaxIouNumPerSocket

      //
      // Skip Stack 0 - PCH ports config is stored in VMDPchPortEnable
      // Skip PortIndex0 - no VMD support on DMI
      //
      PortIndex = 1;
      MaxPortsPerStack = IsCpuAndRevision (CPU_SPRSP, REV_ALL) ? NUMBER_PORTS_PER_STACK_SPR : NUMBER_PORTS_PER_NON_STACK0_10NM;
      for (StackInSocket = 1; StackInSocket < MAX_VMD_STACKS_PER_SOCKET; StackInSocket++) {
        for (PortInStack = 0; PortInStack < MaxPortsPerStack; PortInStack ++) {

          StackIndex = (Socket * MAX_VMD_STACKS_PER_SOCKET) + StackInSocket;
          IioGlobalData->SetupData.VMDPortEnable[Socket][PortIndex] =
              SetupData.SocketConfig.IioConfig.VMDPortEnable[(StackIndex * MaxPortsPerStack) + PortInStack];
          PortIndex++;
        }
      }

      //
      // Setup Options per NUMBER_PORTS_PER_SOCKET
      //
      for (Index = 0; Index < NUMBER_PORTS_PER_SOCKET; Index++) {
        PortIndex = (Socket * NUMBER_PORTS_PER_SOCKET) + Index;
        IioGlobalData->SetupData.PcieAICPortEnable[PortIndex] = SetupData.SocketConfig.IioConfig.PcieAICPortEnable[PortIndex];
        IioGlobalData->SetupData.IioPcieConfig.PciePortClkGateEnable[PortIndex] = SetupData.SocketConfig.IioConfig.PciePortClkGateEnable[PortIndex];
        IioGlobalData->SetupData.PcieDataLinkFeatureExchangeEnable[PortIndex] = SetupData.SocketConfig.IioConfig.PcieDataLinkFeatureExchangeEnable[PortIndex];
      }

      for (Stack = 0; Stack < MAX_STACKS_PER_SOCKET; Stack++) {
        StackIndex = (Socket * MAX_STACKS_PER_SOCKET) + Stack;

        IioGlobalData->SetupData.PcieAICEnabled[StackIndex]       = SetupData.SocketConfig.IioConfig.PcieAICEnabled[StackIndex];
        IioGlobalData->SetupData.PcieAICHotPlugEnable[StackIndex] = SetupData.SocketConfig.IioConfig.PcieAICHotPlugEnable[StackIndex];
      }

      for (StackIndex = 0; StackIndex < MAX_VMD_STACKS_PER_SOCKET; StackIndex++) {
        TotalStackIndex = (Socket * MAX_VMD_STACKS_PER_SOCKET) + StackIndex;

        IioGlobalData->SetupData.VMDEnabled[Socket][StackIndex]       = SetupData.SocketConfig.IioConfig.VMDEnabled[TotalStackIndex];
        IioGlobalData->SetupData.VMDHotPlugEnable[Socket][StackIndex] = SetupData.SocketConfig.IioConfig.VMDHotPlugEnable[TotalStackIndex];
        IioGlobalData->SetupData.VMDCfgBarSz[Socket][StackIndex]      = SetupData.SocketConfig.IioConfig.VMDCfgBarSz[TotalStackIndex];
        IioGlobalData->SetupData.VMDCfgBarAttr[Socket][StackIndex]    = SetupData.SocketConfig.IioConfig.VMDCfgBarAttr[TotalStackIndex];
        IioGlobalData->SetupData.VMDMemBarSz1[Socket][StackIndex]     = SetupData.SocketConfig.IioConfig.VMDMemBarSz1[TotalStackIndex];
        IioGlobalData->SetupData.VMDMemBar1Attr[Socket][StackIndex]   = SetupData.SocketConfig.IioConfig.VMDMemBar1Attr[TotalStackIndex];
        IioGlobalData->SetupData.VMDMemBarSz2[Socket][StackIndex]     = SetupData.SocketConfig.IioConfig.VMDMemBarSz2[TotalStackIndex];
        IioGlobalData->SetupData.VMDMemBar2Attr[Socket][StackIndex]   = SetupData.SocketConfig.IioConfig.VMDMemBar2Attr[TotalStackIndex];
        IioGlobalData->SetupData.VMDDirectAssign[Socket][StackIndex]  = SetupData.SocketConfig.IioConfig.VMDDirectAssign[TotalStackIndex];
      }

      for (RootPortIndex = 0; RootPortIndex < MAX_VMD_ROOTPORTS_PER_PCH; RootPortIndex++) {
        IioGlobalData->SetupData.VMDPchPortEnable[Socket][RootPortIndex] =
            SetupData.SocketConfig.IioConfig.VMDPchPortEnable[(Socket*MAX_VMD_ROOTPORTS_PER_PCH) + RootPortIndex];
      }

      for (StackIndex = 0; StackIndex < MAX_STACKS_PER_SOCKET; StackIndex++) {
        IioGlobalData->SetupData.PoisonMmioReadEn[Socket][StackIndex] =
          (SetupData.SocketConfig.IioConfig.PoisonMmioReadEn[Socket * MAX_STACKS_PER_SOCKET + StackIndex] == 1);
      }

      //
      // Setup Options for MAX_TOTAL_PORTS
      //
      MaxTotalPorts = MAX_TOTAL_PORTS;
      MaxPortNumberPerSocket = GetMaxPortNumPerSocket ();

      for (Index=0; Index < MaxPortNumberPerSocket; Index++) {
        PortIndex = (Socket * MaxPortNumberPerSocket) + Index;
        if (Socket > 3 && FixedPcdGetBool (PcdSocketCopy)) {
          PortIndexInSetup = (3 * MaxPortNumberPerSocket) + Index;
        } else {
          PortIndexInSetup = (Socket * MaxPortNumberPerSocket) + Index;
        }
        if (PortIndex > MaxTotalPorts) {
          continue;
        }

        IioGlobalData->SetupData.PcieHotPlugOnPort[Socket][Index]         = SetupData.SocketConfig.IioConfig.PcieHotPlugOnPort[PortIndexInSetup];
        IioGlobalData->SetupData.SLOTHPSUP[PortIndex]                     = SetupData.SocketConfig.IioConfig.SLOTHPSUP[PortIndexInSetup];
        IioGlobalData->SetupData.PcieLinkDis[PortIndex]                   = SetupData.SocketConfig.IioConfig.PcieLinkDis[PortIndexInSetup];
        IioGlobalData->SetupData.IioPcieConfig.PcieAspm[Socket][Index]    = SetupData.SocketConfig.IioConfig.PcieAspm[PortIndexInSetup];
        IioGlobalData->SetupData.PcieCommonClock[PortIndex]               = SetupData.SocketConfig.IioConfig.PcieCommonClock[PortIndexInSetup];
        IioGlobalData->SetupData.IioPcieConfig.PcieMaxPayload[PortIndex]  = SetupData.SocketConfig.IioConfig.PcieMaxPayload[PortIndexInSetup];
        IioGlobalData->SetupData.PcieDState[PortIndex]                    = SetupData.SocketConfig.IioConfig.PcieDState[PortIndexInSetup];
        IioGlobalData->SetupData.PcieL1Latency[PortIndex]                 = SetupData.SocketConfig.IioConfig.PcieL1Latency[PortIndexInSetup];
        IioGlobalData->SetupData.MsiEn[PortIndex]                         = SetupData.SocketConfig.IioConfig.MsiEn[PortIndexInSetup];
        IioGlobalData->SetupData.IioPcieConfig.ExtendedSync[PortIndex]    = SetupData.SocketConfig.IioConfig.ExtendedSync[PortIndexInSetup];
        IioGlobalData->SetupData.IioPcieConfig.PciePortEnable[PortIndex] = SetupData.SocketConfig.IioConfig.PciePortEnable[PortIndexInSetup];
        IioGlobalData->SetupData.IODC[PortIndex]                          = SetupData.SocketConfig.IioConfig.IODC[PortIndexInSetup];
        IioGlobalData->SetupData.MctpEn[PortIndex]                        = SetupData.SocketConfig.IioConfig.MctpEn[PortIndexInSetup];
        //
        // PCIE setup options for Link Control2
        //
        IioGlobalData->SetupData.IioPcieConfig.PciePortLinkSpeed[PortIndex] = SetupData.SocketConfig.IioConfig.PciePortLinkSpeed[PortIndexInSetup];
        IioGlobalData->SetupData.IioPcieConfig.ComplianceMode[PortIndex]    = SetupData.SocketConfig.IioConfig.ComplianceMode[PortIndexInSetup];
        IioGlobalData->SetupData.PciePortLinkMaxWidth[PortIndex]            = SetupData.SocketConfig.IioConfig.PciePortLinkMaxWidth[PortIndexInSetup];
        IioGlobalData->SetupData.DeEmphasis[PortIndex]                      = SetupData.SocketConfig.IioConfig.DeEmphasis[PortIndexInSetup];

        //
        // PCIE setup options for MISCCTRLSTS
        //
        IioGlobalData->SetupData.EOI[PortIndex]         = SetupData.SocketConfig.IioConfig.EOI[PortIndexInSetup];
        IioGlobalData->SetupData.MSIFATEN[PortIndex]    = SetupData.SocketConfig.IioConfig.MSIFATEN[PortIndexInSetup];
        IioGlobalData->SetupData.MSINFATEN[PortIndex]   = SetupData.SocketConfig.IioConfig.MSINFATEN[PortIndexInSetup];
        IioGlobalData->SetupData.MSICOREN[PortIndex]    = SetupData.SocketConfig.IioConfig.MSICOREN[PortIndexInSetup];
        IioGlobalData->SetupData.ACPIPMEn[PortIndex]    = SetupData.SocketConfig.IioConfig.ACPIPMEn[PortIndexInSetup];
        IioGlobalData->SetupData.P2PRdDis[PortIndex]    = SetupData.SocketConfig.IioConfig.P2PRdDis[PortIndexInSetup];
        IioGlobalData->SetupData.DisPMETOAck[PortIndex] = SetupData.SocketConfig.IioConfig.DisPMETOAck[PortIndexInSetup];
        IioGlobalData->SetupData.ACPIHP[PortIndex]      = SetupData.SocketConfig.IioConfig.ACPIHP[PortIndexInSetup];
        IioGlobalData->SetupData.ACPIPM[PortIndex]      = SetupData.SocketConfig.IioConfig.ACPIPM[PortIndexInSetup];
        IioGlobalData->SetupData.SRIS[PortIndex]        = SetupData.SocketConfig.IioConfig.SRIS[PortIndexInSetup];
        IioGlobalData->SetupData.TXEQ[PortIndex]        = SetupData.SocketConfig.IioConfig.TXEQ[PortIndexInSetup];
        IioGlobalData->SetupData.SERRE[PortIndex]       = SetupData.SocketConfig.IioConfig.SERRE[PortIndexInSetup];

        IioGlobalData->SetupData.IioPcieConfig.PcieUnsupportedRequests[PortIndex] = SetupData.SocketConfig.IioConfig.PcieUnsupportedRequests[PortIndexInSetup];

        IioGlobalData->SetupData.PEXPHIDE[PortIndex]      = SetupData.SocketConfig.IioConfig.PEXPHIDE[PortIndexInSetup];
        IioGlobalData->SetupData.HidePEXPMenu[PortIndex]  = SetupData.SocketConfig.IioConfig.HidePEXPMenu[PortIndexInSetup];

        if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
          //
          // DFX Gen3
          //
          IioGlobalData->SetupData.IioPcieConfig.DfxGen3OverrideMode[PortIndex]         = SetupData.SocketConfig.IioConfig.DfxGen3OverrideMode[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen3TestCard[PortIndex]             = SetupData.SocketConfig.IioConfig.DfxGen3TestCard[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Precursor[PortIndex]  = SetupData.SocketConfig.IioConfig.DfxGen3ManualPh2_Precursor[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Postcursor[PortIndex] = SetupData.SocketConfig.IioConfig.DfxGen3ManualPh2_Postcursor[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Cursor[PortIndex]     = SetupData.SocketConfig.IioConfig.DfxGen3ManualPh2_Cursor[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Precursor[PortIndex]  = SetupData.SocketConfig.IioConfig.DfxGen3ManualPh3_Precursor[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Cursor[PortIndex]     = SetupData.SocketConfig.IioConfig.DfxGen3ManualPh3_Cursor[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Postcursor[PortIndex] = SetupData.SocketConfig.IioConfig.DfxGen3ManualPh3_Postcursor[PortIndexInSetup];

          //
          // DFX Gen4
          //
          IioGlobalData->SetupData.IioPcieConfig.DfxGen4OverrideMode[PortIndex]         = SetupData.SocketConfig.IioConfig.DfxGen4OverrideMode[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen4TestCard[PortIndex]             = SetupData.SocketConfig.IioConfig.DfxGen4TestCard[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Precursor[PortIndex]  = SetupData.SocketConfig.IioConfig.DfxGen4ManualPh2_Precursor[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Postcursor[PortIndex] = SetupData.SocketConfig.IioConfig.DfxGen4ManualPh2_Postcursor[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Cursor[PortIndex]     = SetupData.SocketConfig.IioConfig.DfxGen4ManualPh2_Cursor[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Precursor[PortIndex]  = SetupData.SocketConfig.IioConfig.DfxGen4ManualPh3_Precursor[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Cursor[PortIndex]     = SetupData.SocketConfig.IioConfig.DfxGen4ManualPh3_Cursor[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Postcursor[PortIndex] = SetupData.SocketConfig.IioConfig.DfxGen4ManualPh3_Postcursor[PortIndexInSetup];

          //
          // DFX Common
          //
          IioGlobalData->SetupData.IioPcieConfig.DfxDnTxPreset[PortIndex]               = SetupData.SocketConfig.IioConfig.DfxDnTxPreset[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxRxPreset[PortIndex]                 = SetupData.SocketConfig.IioConfig.DfxRxPreset[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxUpTxPreset[PortIndex]               = SetupData.SocketConfig.IioConfig.DfxUpTxPreset[PortIndexInSetup];

          IioGlobalData->SetupData.IioPcieConfig.DfxDnTxPresetGen4[PortIndex]               = SetupData.SocketConfig.IioConfig.DfxDnTxPresetGen4[PortIndexInSetup];
          IioGlobalData->SetupData.IioPcieConfig.DfxUpTxPresetGen4[PortIndex]               = SetupData.SocketConfig.IioConfig.DfxUpTxPresetGen4[PortIndexInSetup];

          IioGlobalData->SetupData.DfxLinkRetrainEn[PortIndex]     = SetupData.SocketConfig.IioConfig.DfxLinkRetrainEn[PortIndexInSetup];
          IioGlobalData->SetupData.InboundConfiguration[PortIndex] = SetupData.SocketConfig.IioConfig.InboundConfiguration[PortIndexInSetup];
        }
      }   // for (Index ...

      //
      // Setup Options per MAX_NTB_PORTS
      // Need to enable these setup options for S4-S7
      //
      IioGlobalData->SetupData.NtbLinkBiosTrainEn                = SetupData.SocketConfig.IioConfig.NtbLinkBiosTrainEn;
      if (IioGlobalData->SetupData.NtbLinkBiosTrainEn == IIO_OPTION_AUTO) {
        if (IsSiliconWorkaroundEnabled ("S14010021199")) {
          IioGlobalData->SetupData.NtbLinkBiosTrainEn = IIO_OPTION_ENABLE;
        } else {
          IioGlobalData->SetupData.NtbLinkBiosTrainEn = IIO_OPTION_DISABLE;
        }
      }
      MaxNtbPortPerSocket = GetMaxNtbNumPerSocket ();
      for (Index=0; Index < MaxNtbPortPerSocket; Index++){
        PortIndex = (Socket * MaxNtbPortPerSocket) + Index;

        IioGlobalData->SetupData.NtbPpd[PortIndex]               = SetupData.SocketConfig.IioConfig.NtbPpd[PortIndex];
        IioGlobalData->SetupData.NtbBarSizeOverride[PortIndex]   = SetupData.SocketConfig.IioConfig.NtbBarSizeOverride[PortIndex];
        IioGlobalData->SetupData.NtbSplitBar[PortIndex]          = SetupData.SocketConfig.IioConfig.NtbSplitBar[PortIndex];
        IioGlobalData->SetupData.NtbBarSizeImBar1[PortIndex]     = SetupData.SocketConfig.IioConfig.NtbBarSizeImBar1[PortIndex];
        IioGlobalData->SetupData.NtbBarSizeImBar2[PortIndex]     = SetupData.SocketConfig.IioConfig.NtbBarSizeImBar2[PortIndex];
        IioGlobalData->SetupData.NtbBarSizeImBar2_0[PortIndex]   = SetupData.SocketConfig.IioConfig.NtbBarSizeImBar2_0[PortIndex];
        IioGlobalData->SetupData.NtbBarSizeImBar2_1[PortIndex]   = SetupData.SocketConfig.IioConfig.NtbBarSizeImBar2_1[PortIndex];
        IioGlobalData->SetupData.NtbBarSizeEmBarSZ1[PortIndex]   = SetupData.SocketConfig.IioConfig.NtbBarSizeEmBarSZ1[PortIndex];
        IioGlobalData->SetupData.NtbBarSizeEmBarSZ2[PortIndex]   = SetupData.SocketConfig.IioConfig.NtbBarSizeEmBarSZ2[PortIndex];
        IioGlobalData->SetupData.NtbBarSizeEmBarSZ2_0[PortIndex] = SetupData.SocketConfig.IioConfig.NtbBarSizeEmBarSZ2_0[PortIndex];
        IioGlobalData->SetupData.NtbBarSizeEmBarSZ2_1[PortIndex] = SetupData.SocketConfig.IioConfig.NtbBarSizeEmBarSZ2_1[PortIndex];
        IioGlobalData->SetupData.NtbXlinkCtlOverride[PortIndex]  = SetupData.SocketConfig.IioConfig.NtbXlinkCtlOverride[PortIndex];
      } // for (Index ...

      if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
        //
        // first copy settings for IIO stacks
        // Note: NUM_DEVHIDE_STACKS = NUM_DEVHIDE_IIO_STACKS + NUM_DEVHIDE_UNCORE_STACKS
        //
        for (Stack = 0; Stack < NUM_DEVHIDE_IIO_STACKS; Stack++) {
          for (Func = 0; Func < NUM_DEVHIDE_REGS_PER_STACK; Func++) {
            if (Is14nmCpu) {
              // let's assume S4-S7 has the same setup as S0-S3
              Index = (Socket % 4) * MAX_DEVHIDE_REGS_PER_SOCKET;   // find right socket
            } else {
              Index = (Socket) * MAX_DEVHIDE_REGS_PER_SOCKET;       // find right socket
            }
            Index += (Stack * NUM_DEVHIDE_REGS_PER_STACK) + Func;   // find right stack & func

            IioGlobalData->SetupData.DfxDevHideOnIioStacks[Socket][Stack][Func] =
                SetupData.SocketConfig.IioConfig.DfxSocketDevFuncHide[Index];
          } // for each function
        } // for each IIO stack

        if (!Is14nmCpu) {
          // then copy settings for uncore stacks - skip this for 14nm - there are no uncore stacks
          for (Stack = 0; Stack < NUM_DEVHIDE_UNCORE_STACKS; Stack++) {
            for (Func = 0; Func < NUM_DEVHIDE_REGS_PER_STACK; Func++) {
              Index =  Socket * MAX_DEVHIDE_REGS_PER_SOCKET;                          // find right socket
              //
              // uncore stacks are after IIO stacks of each socket
              //
              Index += (Stack + NUM_DEVHIDE_IIO_STACKS) * NUM_DEVHIDE_REGS_PER_STACK; // find right stack
              Index += Func;                                                          // find right function

              IioGlobalData->SetupData.DfxDevHideOnUncoreStacks[Socket][Stack][Func] =
                  SetupData.SocketConfig.IioConfig.DfxSocketDevFuncHide[Index];
            } // for each function
          } //for each uncore stack
        }
      }

      MaxPortPerSocket = GetMaxPortNumPerSocket();
      MaxPortPerIou = GetMaxPortNumPerIou();
      IioGlobalData->IioVar.IioVData.StackPerPort[Socket][0] = 0; //DMI
      for (PortIdx = 1; PortIdx < MaxPortPerSocket; PortIdx ++){
        IouIndex = (PortIdx - 1) / MaxPortPerIou;
        StackIndex = GetStackIndexByIouIndex (IouIndex);
        IioGlobalData->IioVar.IioVData.StackPerPort[Socket][PortIdx] = StackIndex;
      }

      for (Index = 0; Index < NUMBER_TRACE_HUB_PER_SOCKET; Index++) {
        IioGlobalData->SetupData.CpuTraceHubConfig[Socket][Index].EnableMode =
          SetupData.SocketConfig.IioConfig.NorthTraceHubMode[Socket * NUMBER_TRACE_HUB_PER_SOCKET + Index];
        IioGlobalData->SetupData.CpuTraceHubConfig[Socket][Index].MemReg0Size =
          SetupData.SocketConfig.IioConfig.NorthTraceHubMemReg0Size[Socket * NUMBER_TRACE_HUB_PER_SOCKET + Index];
        IioGlobalData->SetupData.CpuTraceHubConfig[Socket][Index].MemReg1Size =
          SetupData.SocketConfig.IioConfig.NorthTraceHubMemReg1Size[Socket * NUMBER_TRACE_HUB_PER_SOCKET + Index];
      }
    }

    //
    // Setup option for SierraPeak (SPK)
    //
    CopyMem (
        IioGlobalData->SetupData.SierraPeakMemBufferSize,
        SetupData.SocketConfig.IioConfig.SierraPeakMemBufferSize,
        MAX_SOCKET * sizeof(UINT8));

    //
    // Setup Options per DSA/IAX Devices
    //
    CopyMem (IioGlobalData->SetupData.DsaEn, SetupData.SocketConfig.IioConfig.DsaEn, sizeof(IioGlobalData->SetupData.DsaEn));
    CopyMem (IioGlobalData->SetupData.IaxEn, SetupData.SocketConfig.IioConfig.IaxEn, sizeof(IioGlobalData->SetupData.IaxEn));
    CopyMem (IioGlobalData->SetupData.CpmEn, SetupData.SocketConfig.IioConfig.CpmEn, sizeof(IioGlobalData->SetupData.CpmEn));
    CopyMem (IioGlobalData->SetupData.HqmEn, SetupData.SocketConfig.IioConfig.HqmEn, sizeof(IioGlobalData->SetupData.HqmEn));
    //
    // Setup Options per CB3 Devices
    //
    CopyMem (IioGlobalData->SetupData.Cb3DmaEn,SetupData.SocketConfig.IioConfig.Cb3DmaEn, sizeof(IioGlobalData->SetupData.Cb3DmaEn));
    CopyMem (IioGlobalData->SetupData.Cb3NoSnoopEn,SetupData.SocketConfig.IioConfig.Cb3NoSnoopEn, sizeof(IioGlobalData->SetupData.Cb3NoSnoopEn));

    //
    // Override ProcessorX2apic and VtdSupport Item per CPU threads number,
    // if threads > 256, the X2apic and Vtd support should be enabled.
    //
    if (X2ApicIdDetect (NULL) ) {
      IioGlobalData->SetupData.VTdSupport = TRUE;
      IioGlobalData->SetupData.ProcessorX2apic = TRUE;
    }

    IioGlobalData->IioVar.IioVData.Pci64BitResourceAllocation = IioGlobalData->SetupData.Pci64BitResourceAllocation;

    IioGlobalData->IioVar.IioVData.PlatformType = GetPlatformInfo ();

    //
    // Update Other variables required for IIO Init
    //
    IioGlobalData->IioVar.IioVData.IsocEnable = IfFeatureEnabled (TypeIsoc);
    //todo: IIO RC Separation common definition need to figure out how to do next step in RC separation design group meeting.
    IioGlobalData->IioVar.IioVData.meRequestedSize = GetMeRequestedSize ();
    IioGlobalData->IioVar.IioVData.ieRequestedSize = GetIeRequestedSize ();
    IioGlobalData->IioVar.IioVData.SystemRasType = GetSystemRasType ();

    IioGlobalData->IioVar.IioVData.IOxAPICCallbackBootEvent = FALSE;

    IioGlobalData->IioVar.IioVData.Emulation = GetEmulation ();
    IioGlobalData->IioVar.IioVData.IODC = GetIoDcMode ();
    IioGlobalData->IioVar.IioOutData.MemResetRequired = GetSysResetRequired ();

    for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
      DEBUG ((DEBUG_INFO, "Socket[%x] is socketValid = %x\n", IioIndex, SocketPresent (IioIndex)));
      IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex] = SocketPresent (IioIndex);
      DEBUG ((DEBUG_INFO, "[%x]SocketPresent %x\n", IioIndex, IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]));
        IioGlobalData->IioVar.IioVData.FpgaActive[IioIndex]  = IfFpgaActive (IioIndex);

      if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
        continue;
      }

      IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex] = GetMaxPortNumPerSocket ();

    }

    NtbPortNumPerSkt = GetMaxNtbNumPerSocket ();

    for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {

      if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
        continue;
      }

      for (PortIndex = 0; PortIndex < NUMBER_PORTS_PER_SOCKET; PortIndex++) {

        Status = GetNtbPortPerPortIndex ((UINT8)PortIndex, &PortNtbIndex);
        if (Status != EFI_SUCCESS) {
          continue;
        }

        if (IioGlobalData->SetupData.NtbPpd[(IioIndex * NtbPortNumPerSkt) + PortNtbIndex] != NTB_PORT_DEF_TRANSPARENT) {
          IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex] = IioGlobalData->SetupData.NtbPpd[(IioIndex * NtbPortNumPerSkt) + PortNtbIndex];
        }
      }
    }

    GetIioPortBifurcationInfo (IioGlobalData);
    UpdateAutoBifurcation (IioGlobalData);

    IioSiPolicyPpi = (IIO_SI_POLICY_PPI*)AllocateZeroPool (sizeof (IIO_SI_POLICY_PPI));
    ASSERT (IioSiPolicyPpi != NULL);
    IioSiPolicyPpi->IioGlobalData = IioGlobalData;

    InstallIioSiPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
    ASSERT (InstallIioSiPolicyPpiDesc != NULL);
    if (InstallIioSiPolicyPpiDesc != NULL) {
      InstallIioSiPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
      InstallIioSiPolicyPpiDesc->Guid  = &gIioSiPolicyPpiGuid;
      IioSiPolicyPpi->Revision         = IIO_SI_POLICY_PPI_REVISION;
      InstallIioSiPolicyPpiDesc->Ppi   = IioSiPolicyPpi;
      DEBUG ((DEBUG_INFO, "Installing IioSiPolicy \n"));
      Status = PeiServicesInstallPpi (InstallIioSiPolicyPpiDesc);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[IIO] ERROR: Installing IioSiPolicy failed (%r)\n", Status));
        ASSERT_EFI_ERROR (Status);
      }
    }
  }
}


/**
  This function prints the Iio Pei policy.
**/
VOID
PrintIioSiPolicy (
  VOID
  )
{
  IIO_GLOBALS         *IioGlobalData;
  IIO_SI_POLICY_PPI   *IioSiPolicyPpi;
  EFI_STATUS          Status;
  UINT16              Index;
  UINT8               Socket; // Used to copy setup option from S0-S3 into S4-S
  UINT8               Stack;
  UINT8               Func;
  UINT8               MaxPortNumberPerSocket;
  UINT8               MaxNtbPortPerSocket;
  UINT32              MaxTotalPorts;
  UINT8               MaxStackSocket;
  UINT8               MaxStackPort;
  UINT8               StackIndex;
  UINT16              PortIndex = 0;
  UINT8               IioIndex;
  UINT8               PortNtbIndex;
  UINT8               Iou;

  IioGlobalData = NULL;
  IioSiPolicyPpi = NULL;

  Status = PeiServicesLocatePpi (&gIioSiPolicyPpiGuid, 0, NULL, (VOID **)&IioSiPolicyPpi);
 if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[IIO] ERROR: Locate gIioSiPolicyPpiGuid failed (%r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return;
  } else {
    IioGlobalData = IioSiPolicyPpi->IioGlobalData;
  }
  DEBUG ((DEBUG_INFO, "[IIO] IioGlobalData->SetupData:\n"));
  DEBUG ((DEBUG_INFO, "VTdSupport = 0x%x\n", IioGlobalData->SetupData.VTdSupport                  ));
  DEBUG ((DEBUG_INFO, "InterruptRemap = 0x%x\n", IioGlobalData->SetupData.InterruptRemap              ));
  DEBUG ((DEBUG_INFO, "CoherencySupport = 0x%x\n", IioGlobalData->SetupData.CoherencySupport            ));
  DEBUG ((DEBUG_INFO, "ATS = 0x%x\n", IioGlobalData->SetupData.ATS                         ));
  DEBUG ((DEBUG_INFO, "PostedInterrupt = 0x%x\n", IioGlobalData->SetupData.PostedInterrupt             ));
  DEBUG ((DEBUG_INFO, "VtdAcsWa = 0x%x\n", IioGlobalData->SetupData.VtdAcsWa                    ));
  DEBUG ((DEBUG_INFO, "PerformanceTuningMode = 0x%x\n", IioGlobalData->SetupData.PerformanceTuningMode));
  DEBUG ((DEBUG_INFO, "CompletionTimeoutGlobal = 0x%x\n", IioGlobalData->SetupData.CompletionTimeoutGlobal     ));
  DEBUG ((DEBUG_INFO, "CompletionTimeoutGlobalVal = 0x%x\n",IioGlobalData->SetupData.CompletionTimeoutGlobalValue));
  DEBUG ((DEBUG_INFO, "CoherentReadPart = 0x%x\n", IioGlobalData->SetupData.CoherentReadPart            ));
  DEBUG ((DEBUG_INFO, "CoherentReadFull = 0x%x\n", IioGlobalData->SetupData.CoherentReadFull            ));
  DEBUG ((DEBUG_INFO, "PcieGlobalAspm = 0x%x\n", IioGlobalData->SetupData.PcieGlobalAspm              ));
  DEBUG ((DEBUG_INFO, "StopAndScream = 0x%x\n", IioGlobalData->SetupData.StopAndScream               ));
  DEBUG ((DEBUG_INFO, "SnoopResponseHoldOff = 0x%x\n", IioGlobalData->SetupData.SnoopResponseHoldOff        ));
  DEBUG ((DEBUG_INFO, "PCIe_LTR = 0x%x\n", IioGlobalData->SetupData.IioPcieConfig.PCIe_LTR                    ));
  DEBUG ((DEBUG_INFO, "PcieExtendedTagField = 0x%x\n", IioGlobalData->SetupData.PcieExtendedTagField        ));
  DEBUG ((DEBUG_INFO, "Pcie10bitTag = 0x%x\n", IioGlobalData->SetupData.Pcie10bitTag      ));
  DEBUG ((DEBUG_INFO, "PCIe_AtomicOpReq = 0x%x\n", IioGlobalData->SetupData.PCIe_AtomicOpReq            ));
  DEBUG ((DEBUG_INFO, "PcieMaxReadRequestSize = 0x%x\n", IioGlobalData->SetupData.PcieMaxReadRequestSize      ));
  DEBUG ((DEBUG_INFO, "PciePtm = 0x%x\n", IioGlobalData->SetupData.IioPcieConfig.PciePtm      ));
  DEBUG ((DEBUG_INFO, "DfxLtssmLogger = 0x%x\n", IioGlobalData->SetupData.DfxLtssmLogger              ));
  DEBUG ((DEBUG_INFO, "DfxLtssmLoggerStop = 0x%x\n", IioGlobalData->SetupData.DfxLtssmLoggerStop          ));
  DEBUG ((DEBUG_INFO, "DfxLtssmLoggerSpeed = 0x%x\n", IioGlobalData->SetupData.DfxLtssmLoggerSpeed         ));
  DEBUG ((DEBUG_INFO, "DfxLtssmLoggerMask = 0x%x\n", IioGlobalData->SetupData.DfxLtssmLoggerMask          ));
  DEBUG ((DEBUG_INFO, "DfxJitterLogger = 0x%x\n", IioGlobalData->SetupData.DfxJitterLogger             ));
  DEBUG ((DEBUG_INFO, "DisableTPH = 0x%x\n", IioGlobalData->SetupData.DisableTPH                  ));
  DEBUG ((DEBUG_INFO, "PrioritizeTPH = 0x%x\n", IioGlobalData->SetupData.PrioritizeTPH               ));
  DEBUG ((DEBUG_INFO, "CbRelaxedOrdering = 0x%x\n", IioGlobalData->SetupData.CbRelaxedOrdering           ));
  DEBUG ((DEBUG_INFO, "PCUF6Hide = 0x%x\n", IioGlobalData->SetupData.PCUF6Hide                   ));
  DEBUG ((DEBUG_INFO, "EN1K = 0x%x\n", IioGlobalData->SetupData.EN1K                        ));
  DEBUG ((DEBUG_INFO, "DualCvIoFlow = 0x%x\n", IioGlobalData->SetupData.DualCvIoFlow                ));
  DEBUG ((DEBUG_INFO, "PcieBiosTrainEnable = 0x%x\n", IioGlobalData->SetupData.PcieBiosTrainEnable         ));
  DEBUG ((DEBUG_INFO, "DelayBeforePCIeLinkTraining = 0x%x\n", IioGlobalData->SetupData.DelayBeforePCIeLinkTraining));
  DEBUG ((DEBUG_INFO, "MultiCastEnable = 0x%x\n", IioGlobalData->SetupData.MultiCastEnable             ));
  DEBUG ((DEBUG_INFO, "DfxIioRcFlow = 0x%x\n", IioGlobalData->SetupData.DfxIioRcFlow                ));
  DEBUG ((DEBUG_INFO, "DfxIioLinkTraining = 0x%x\n", IioGlobalData->SetupData.DfxIioLinkTraining          ));
  DEBUG ((DEBUG_INFO, "DfxOtcPipeHazardThresh = 0x%x\n", IioGlobalData->SetupData.DfxOtcPipeHazardThresh));
  DEBUG ((DEBUG_INFO, "DfxBadTransactionTypeWa = 0x%x\n", IioGlobalData->SetupData.DfxBadTransactionTypeWa));
  DEBUG ((DEBUG_INFO, "DfxNpkAllocatingFlow = 0x%x\n", IioGlobalData->SetupData.DfxNpkAllocatingFlow));
  DEBUG ((DEBUG_INFO, "CbDmaMultiCastEnable = 0x%x\n", IioGlobalData->SetupData.CbDmaMultiCastEnable        ));
  DEBUG ((DEBUG_INFO, "McastBaseAddrRegion = 0x%x\n", IioGlobalData->SetupData.McastBaseAddrRegion         ));
  DEBUG ((DEBUG_INFO, "McastIndexPosition = 0x%x\n", IioGlobalData->SetupData.McastIndexPosition          ));
  DEBUG ((DEBUG_INFO, "McastNumGroup = 0x%x\n", IioGlobalData->SetupData.McastNumGroup               ));
  //
  // PCIE Global Option
  //
  DEBUG ((DEBUG_INFO, "NoSnoopRdCfg = 0x%x\n", IioGlobalData->SetupData.NoSnoopRdCfg              ));
  DEBUG ((DEBUG_INFO, "NoSnoopWrCfg  = 0x%x\n", IioGlobalData->SetupData.NoSnoopWrCfg              ));
  DEBUG ((DEBUG_INFO, "MaxReadCompCombSize = 0x%x\n", IioGlobalData->SetupData.MaxReadCompCombSize       ));
  DEBUG ((DEBUG_INFO, "ProblematicPort = 0x%x\n", IioGlobalData->SetupData.ProblematicPort           ));
  DEBUG ((DEBUG_INFO, "DmiAllocatingFlow = 0x%x\n", IioGlobalData->SetupData.DmiAllocatingFlow         ));
  DEBUG ((DEBUG_INFO, "PcieAllocatingFlow = 0x%x\n", IioGlobalData->SetupData.PcieAllocatingFlow        ));
  DEBUG ((DEBUG_INFO, "PcieHotPlugEnable = 0x%x\n", IioGlobalData->SetupData.IioPcieConfig.PcieHotPlugEnable ));
  DEBUG ((DEBUG_INFO, "PcieAcpiHotPlugEnable = 0x%x\n", IioGlobalData->SetupData.PcieAcpiHotPlugEnable     ));
  DEBUG ((DEBUG_INFO, "HaltOnDmiDegraded = 0x%x\n", IioGlobalData->SetupData.HaltOnDmiDegraded         ));
  DEBUG ((DEBUG_INFO, "RxClockWA = 0x%x\n", IioGlobalData->SetupData.RxClockWA                 ));
  DEBUG ((DEBUG_INFO, "GlobalPme2AckTOCtrl = 0x%x\n", IioGlobalData->SetupData.GlobalPme2AckTOCtrl       ));
  DEBUG ((DEBUG_INFO, "PcieSlotOprom1 = 0x%x\n", IioGlobalData->SetupData.PcieSlotOprom1            ));
  DEBUG ((DEBUG_INFO, "PcieSlotOprom2 = 0x%x\n", IioGlobalData->SetupData.PcieSlotOprom2            ));
  DEBUG ((DEBUG_INFO, "PcieSlotOprom3 = 0x%x\n", IioGlobalData->SetupData.PcieSlotOprom3            ));
  DEBUG ((DEBUG_INFO, "PcieSlotOprom4 = 0x%x\n", IioGlobalData->SetupData.PcieSlotOprom4            ));
  DEBUG ((DEBUG_INFO, "PcieSlotOprom5 = 0x%x\n", IioGlobalData->SetupData.PcieSlotOprom5            ));
  DEBUG ((DEBUG_INFO, "PcieSlotOprom6 = 0x%x\n", IioGlobalData->SetupData.PcieSlotOprom6            ));
  DEBUG ((DEBUG_INFO, "PcieSlotOprom7 = 0x%x\n", IioGlobalData->SetupData.PcieSlotOprom7            ));
  DEBUG ((DEBUG_INFO, "PcieSlotOprom8 = 0x%x\n", IioGlobalData->SetupData.PcieSlotOprom8            ));
  DEBUG ((DEBUG_INFO, "PcieSlotItemCtrl = 0x%x\n", IioGlobalData->SetupData.PcieSlotItemCtrl          ));
  DEBUG ((DEBUG_INFO, "PcieRelaxedOrdering = 0x%x\n", IioGlobalData->SetupData.PcieRelaxedOrdering       ));
  DEBUG ((DEBUG_INFO, "PciePhyTestMode = 0x%x\n", IioGlobalData->SetupData.PciePhyTestMode           ));
  DEBUG ((DEBUG_INFO, "PcieEnqCmdSupport = 0x%x\n", IioGlobalData->SetupData.PcieEnqCmdSupport           ));
  DEBUG ((DEBUG_INFO, "DFXEnable = 0x%x\n", IioGlobalData->SetupData.DFXEnable                 ));
  DEBUG ((DEBUG_INFO, "DfxDisableBiosDone = 0x%x\n", IioGlobalData->SetupData.DfxDisableBiosDone));
  DEBUG ((DEBUG_INFO, "Xppdef = 0x%x\n", IioGlobalData->SetupData.Xppdef));
  DEBUG ((DEBUG_INFO, "Pci64BitResourceAllocation = 0x%x\n", IioGlobalData->SetupData.Pci64BitResourceAllocation));
  DEBUG ((DEBUG_INFO, "ProcessorMsrLockControl = 0x%x\n", IioGlobalData->SetupData.ProcessorMsrLockControl   ));
  DEBUG ((DEBUG_INFO, "Serr = 0x%x\n", IioGlobalData->SetupData.Serr                      ));
  DEBUG ((DEBUG_INFO, "Perr = 0x%x\n", IioGlobalData->SetupData.Perr                      ));
  DEBUG ((DEBUG_INFO, "LockChipset = 0x%x\n", IioGlobalData->SetupData.LockChipset               ));
  DEBUG ((DEBUG_INFO, "PeciInTrustControlBit = 0x%x\n", IioGlobalData->SetupData.PeciInTrustControlBit     ));
  DEBUG ((DEBUG_INFO, "LegacyVgaSoc = 0x%x\n", IioGlobalData->SetupData.LegacyVgaSoc              ));
  DEBUG ((DEBUG_INFO, "LegacyVgaStack = 0x%x\n", IioGlobalData->SetupData.LegacyVgaStack            ));
  DEBUG ((DEBUG_INFO, "PciePllSsc = 0x%x\n", IioGlobalData->SetupData.PciePllSsc                ));

  for (Stack = 0; Stack < TOTAL_IIO_STACKS; Stack ++) {
    DEBUG ((DEBUG_INFO, "DevPresIoApicIio[%d] = 0x%x\n", Stack, IioGlobalData->SetupData.DevPresIoApicIio[Stack]));
  }
  // PCIE RAS (Errors)
  DEBUG ((DEBUG_INFO, "IioErrorEn = 0x%x\n", IioGlobalData->SetupData.IioErrorEn ));
  DEBUG ((DEBUG_INFO, "LerEn = 0x%x\n", IioGlobalData->SetupData.LerEn ));
  DEBUG ((DEBUG_INFO, "ProcessorX2apic = 0x%x\n", IioGlobalData->SetupData.ProcessorX2apic ));
  DEBUG ((DEBUG_INFO, "ProcessorMsrLockControl = 0x%x\n", IioGlobalData->SetupData.ProcessorMsrLockControl ));

  //
  // Setup Option per Socket
  //
  for (Socket = 0; Socket < MaxIIO; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }
    // Copy BIFUR setup option from S0-S3 into S4-S7
    Index = (Socket < 4) ? Socket : Socket - 4;

    for (Iou = 0; Iou < ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0]); Iou++) {
      DEBUG ((DEBUG_INFO, "ConfigIOU[%d][%d] = 0x%X\n", Socket, Iou, IioGlobalData->SetupData.ConfigIOU[Socket][Iou]));
    }

    for (Iou = 0; Iou < ARRAY_SIZE (IioGlobalData->SetupData.PcieSubSystemMode[Socket]); Iou++) {
      DEBUG ((DEBUG_INFO, "PcieSubSystemMode[%d][%d] = 0x%X\n", Socket, Iou, IioGlobalData->SetupData.PcieSubSystemMode[Socket][Iou]));
    }

    DEBUG ((DEBUG_INFO, "CompletionTimeout[%d] = 0x%x\n", Socket, IioGlobalData->SetupData.CompletionTimeout[Socket]));
    DEBUG ((DEBUG_INFO, "CompletionTimeoutValue[%d] = 0x%x\n", Socket, IioGlobalData->SetupData.CompletionTimeoutValue[Socket]));
    DEBUG ((DEBUG_INFO, "RpUncorrectableNonFatalErrorEsc[%d] = 0x%x\n", Socket, IioGlobalData->SetupData.IioPcieConfig.RpUncorrectableNonFatalErrorEsc[Socket]));
    DEBUG ((DEBUG_INFO, "RpUncorrectableFatalErrorEsc[%d] = 0x%x\n", Socket, IioGlobalData->SetupData.IioPcieConfig.RpUncorrectableFatalErrorEsc[Socket]));
    //
    // Setup Options per NUMBER_PORTS_PER_SOCKET
    //
    for (Index = 0; Index < NUMBER_PORTS_PER_SOCKET; Index++) {
      PortIndex = (Socket * NUMBER_PORTS_PER_SOCKET) + Index;
      DEBUG ((DEBUG_INFO, "VMDPortEnable[%d][%d] = 0x%x\n", Socket, Index, IioGlobalData->SetupData.VMDPortEnable[Socket][Index]));
      DEBUG ((DEBUG_INFO, "PcieAICPortEnable[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.PcieAICPortEnable[PortIndex]));
      DEBUG ((DEBUG_INFO, "PcieDataLinkFeatureExchangeEnable[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.PcieDataLinkFeatureExchangeEnable[PortIndex]));
    }
    for (Stack = 0; Stack < MAX_STACKS_PER_SOCKET; Stack++) {
      StackIndex = (Socket * MAX_STACKS_PER_SOCKET) + Stack;
      DEBUG ((DEBUG_INFO, "PcieAICEnabled[%d] = 0x%x\n", StackIndex, IioGlobalData->SetupData.PcieAICEnabled[StackIndex]));
      DEBUG ((DEBUG_INFO, "PcieAICHotPlugEnable[%d] = 0x%x\n", StackIndex, IioGlobalData->SetupData.PcieAICHotPlugEnable[StackIndex]));
    }
    for (StackIndex = 0; StackIndex < MAX_VMD_STACKS_PER_SOCKET; StackIndex++) {
      DEBUG ((DEBUG_INFO, "VMDEnabled[%d][%d] = 0x%x\n", Socket, StackIndex, IioGlobalData->SetupData.VMDEnabled[Socket][StackIndex]));
      DEBUG ((DEBUG_INFO, "VMDHotPlugEnable[%d][%d] = 0x%x\n", Socket, StackIndex, IioGlobalData->SetupData.VMDHotPlugEnable[Socket][StackIndex]));
      DEBUG ((DEBUG_INFO, "VMDCfgBarSz[%d][%d] = 0x%x\n", Socket, StackIndex, IioGlobalData->SetupData.VMDCfgBarSz[Socket][StackIndex]));
      DEBUG ((DEBUG_INFO, "VMDCfgBarAttr[%d][%d] = 0x%x\n", Socket, StackIndex, IioGlobalData->SetupData.VMDCfgBarAttr[Socket][StackIndex]));
      DEBUG ((DEBUG_INFO, "VMDMemBarSz1[%d][%d] = 0x%x\n", Socket, StackIndex, IioGlobalData->SetupData.VMDMemBarSz1[Socket][StackIndex]));
      DEBUG ((DEBUG_INFO, "VMDMemBar1Attr[%d][%d] = 0x%x\n", Socket, StackIndex, IioGlobalData->SetupData.VMDMemBar1Attr[Socket][StackIndex]));
      DEBUG ((DEBUG_INFO, "VMDMemBarSz2[%d][%d] = 0x%x\n", Socket, StackIndex, IioGlobalData->SetupData.VMDMemBarSz2[Socket][StackIndex]));
      DEBUG ((DEBUG_INFO, "VMDMemBar2Attr[%d][%d] = 0x%x\n", Socket, StackIndex, IioGlobalData->SetupData.VMDMemBar2Attr[Socket][StackIndex]));
      DEBUG ((DEBUG_INFO, "VMDDirectAssign[%d][%d] = 0x%x\n", Socket, StackIndex, IioGlobalData->SetupData.VMDDirectAssign[Socket][StackIndex]));
    }

    //
    // Setup Options for MAX_TOTAL_PORTS
    //
    MaxTotalPorts = MAX_TOTAL_PORTS;

     MaxPortNumberPerSocket = GetMaxPortNumPerSocket ();
      for (Index=0; Index < MaxPortNumberPerSocket; Index++) {
        PortIndex = (Socket * MaxPortNumberPerSocket) + Index;
        if (PortIndex > MaxTotalPorts) {
          continue;
        }

        DEBUG ((DEBUG_INFO, "PcieHotPlugOnPort[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.PcieHotPlugOnPort[Socket][Index]));
        DEBUG ((DEBUG_INFO, "SLOTHPSUP[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.SLOTHPSUP[PortIndex]            ));
        DEBUG ((DEBUG_INFO, "PcieLinkDis[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.PcieLinkDis[PortIndex]          ));
        DEBUG ((DEBUG_INFO, "PcieAspm[%d][%d] = 0x%x\n", Socket, Index, IioGlobalData->SetupData.IioPcieConfig.PcieAspm[Socket][Index]));
        DEBUG ((DEBUG_INFO, "PcieCommonClock[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.PcieCommonClock[PortIndex]      ));
        DEBUG ((DEBUG_INFO, "PcieMaxPayload[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.IioPcieConfig.PcieMaxPayload[PortIndex]       ));
        DEBUG ((DEBUG_INFO, "PcieDState[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.PcieDState[PortIndex]           ));
        DEBUG ((DEBUG_INFO, "PcieL1Latency[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.PcieL1Latency[PortIndex]        ));
        DEBUG ((DEBUG_INFO, "MsiEn[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.MsiEn[PortIndex]                ));
        DEBUG ((DEBUG_INFO, "ExtendedSync[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.IioPcieConfig.ExtendedSync[PortIndex]         ));
        DEBUG ((DEBUG_INFO, "PciePortEnable[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.IioPcieConfig.PciePortEnable[PortIndex]      ));
        DEBUG ((DEBUG_INFO, "IODC[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.IODC[PortIndex]                 ));
        DEBUG ((DEBUG_INFO, "MctpEn[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.MctpEn[PortIndex]));
        //
        // PCIE setup options for Link Control2
        //
        DEBUG ((DEBUG_INFO, "PciePortLinkSpeed[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.PciePortLinkSpeed[PortIndex]     ));
        DEBUG ((DEBUG_INFO, "ComplianceMode[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.ComplianceMode[PortIndex]        ));
        DEBUG ((DEBUG_INFO, "PciePortLinkMaxWidth[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.PciePortLinkMaxWidth[PortIndex]  ));
        DEBUG ((DEBUG_INFO, "DeEmphasis[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.DeEmphasis[PortIndex]            ));

        //
        // PCIE setup options for MISCCTRLSTS
        //
        DEBUG ((DEBUG_INFO, "EOI[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.EOI[PortIndex]         ));
        DEBUG ((DEBUG_INFO, "MSIFATEN[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.MSIFATEN[PortIndex]    ));
        DEBUG ((DEBUG_INFO, "MSINFATEN[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.MSINFATEN[PortIndex]   ));
        DEBUG ((DEBUG_INFO, "MSICOREN[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.MSICOREN[PortIndex]    ));
        DEBUG ((DEBUG_INFO, "ACPIPMEn[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.ACPIPMEn[PortIndex]    ));
        DEBUG ((DEBUG_INFO, "P2PRdDis[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.P2PRdDis[PortIndex]    ));
        DEBUG ((DEBUG_INFO, "DisPMETOAck[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.DisPMETOAck[PortIndex] ));
        DEBUG ((DEBUG_INFO, "ACPIHP[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.ACPIHP[PortIndex]      ));
        DEBUG ((DEBUG_INFO, "ACPIPM[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.ACPIPM[PortIndex]      ));
        DEBUG ((DEBUG_INFO, "SRIS[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.SRIS[PortIndex]        ));
        DEBUG ((DEBUG_INFO, "TXEQ[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.TXEQ[PortIndex]        ));

        DEBUG ((DEBUG_INFO, "PcieUnsupportedRequests[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.IioPcieConfig.PcieUnsupportedRequests[PortIndex]));

        DEBUG ((DEBUG_INFO, "PEXPHIDE[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.PEXPHIDE[PortIndex]      ));
        DEBUG ((DEBUG_INFO, "HidePEXPMenu[%d] = 0x%x\n", PortIndex,IioGlobalData->SetupData.HidePEXPMenu[PortIndex]  ));

        //
        // DFX Gen3
        //
        DEBUG ((DEBUG_INFO, "DfxGen3OverrideMode[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen3OverrideMode[PortIndex]         ));
        DEBUG ((DEBUG_INFO, "DfxGen3TestCard[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen3TestCard[PortIndex]             ));
        DEBUG ((DEBUG_INFO, "DfxGen3ManualPh2_Precursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Precursor[PortIndex]  ));
        DEBUG ((DEBUG_INFO, "DfxGen3ManualPh2_Cursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Cursor[PortIndex]     ));
        DEBUG ((DEBUG_INFO, "DfxGen3ManualPh2_Postcursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Postcursor[PortIndex]     ));
        DEBUG ((DEBUG_INFO, "DfxGen3ManualPh3_Precursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Precursor[PortIndex]  ));
        DEBUG ((DEBUG_INFO, "DfxGen3ManualPh3_Cursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Cursor[PortIndex]     ));
        DEBUG ((DEBUG_INFO, "DfxGen3ManualPh3_Postcursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Postcursor[PortIndex] ));

        //
        // DFX Gen4
        //
        DEBUG ((DEBUG_INFO, "DfxGen4OverrideMode[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen4OverrideMode[PortIndex]         ));
        DEBUG ((DEBUG_INFO, "DfxGen4TestCard[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen4TestCard[PortIndex]             ));
        DEBUG ((DEBUG_INFO, "DfxGen4ManualPh2_Precursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Precursor[PortIndex]  ));
        DEBUG ((DEBUG_INFO, "DfxGen4ManualPh2_Cursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Cursor[PortIndex]     ));
        DEBUG ((DEBUG_INFO, "DfxGen4ManualPh2_PostCursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Postcursor[PortIndex]     ));
        DEBUG ((DEBUG_INFO, "DfxGen4ManualPh3_Precursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Precursor[PortIndex]  ));
        DEBUG ((DEBUG_INFO, "DfxGen4ManualPh3_Cursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Cursor[PortIndex]     ));
        DEBUG ((DEBUG_INFO, "DfxGen4ManualPh3_Postcursor[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Postcursor[PortIndex] ));

        //
        // DFX Common
        //
        DEBUG ((DEBUG_INFO, "DfxDnTxPreset[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxDnTxPreset[PortIndex]               ));
        DEBUG ((DEBUG_INFO, "DfxRxPreset[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxRxPreset[PortIndex]                 ));
        DEBUG ((DEBUG_INFO, "DfxUpTxPreset[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxUpTxPreset[PortIndex]               ));
        DEBUG ((DEBUG_INFO, "DfxDnTxPresetGen4[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxDnTxPresetGen4[PortIndex]               ));
        DEBUG ((DEBUG_INFO, "DfxUpTxPresetGen4[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.IioPcieConfig.DfxUpTxPresetGen4[PortIndex]               ));
        DEBUG ((DEBUG_INFO, "DfxLinkRetrainEn[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.DfxLinkRetrainEn[PortIndex]));
        DEBUG ((DEBUG_INFO, "InboundConfiguration[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.InboundConfiguration[PortIndex]));
    } // for (Index ...

    DEBUG ((DEBUG_INFO, "SkipPortPersonalityLock = %x\n", IioGlobalData->SetupData.SkipPortPersonalityLock));
    DEBUG ((DEBUG_INFO, "CxlHeaderBypass = %x\n", IioGlobalData->SetupData.CxlHeaderBypass));

    //
    // Setup Options per MAX_NTB_PORTS
    // Need to enable these setup options for S4-S7
    //

    MaxNtbPortPerSocket = GetMaxNtbNumPerSocket ();

    for (Index=0; Index < MaxNtbPortPerSocket; Index++){
      PortIndex = (Socket * MaxNtbPortPerSocket) + Index;

      DEBUG ((DEBUG_INFO, "NtbPpd[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbPpd[PortIndex]               ));
      DEBUG ((DEBUG_INFO, "NtbBarSizeOverride[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbBarSizeOverride[PortIndex]   ));
      DEBUG ((DEBUG_INFO, "NtbSplitBar[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbSplitBar[PortIndex]          ));
      DEBUG ((DEBUG_INFO, "NtbBarSizeImBar1[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbBarSizeImBar1[PortIndex]     ));
      DEBUG ((DEBUG_INFO, "NtbBarSizeImBar2[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbBarSizeImBar2[PortIndex]     ));
      DEBUG ((DEBUG_INFO, "NtbBarSizeImBar2_0[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbBarSizeImBar2_0[PortIndex]   ));
      DEBUG ((DEBUG_INFO, "NtbBarSizeImBar2_1[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbBarSizeImBar2_1[PortIndex]   ));
      DEBUG ((DEBUG_INFO, "NtbBarSizeEmBarSZ1[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbBarSizeEmBarSZ1[PortIndex]   ));
      DEBUG ((DEBUG_INFO, "NtbBarSizeEmBarSZ2[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbBarSizeEmBarSZ2[PortIndex]   ));
      DEBUG ((DEBUG_INFO, "NtbBarSizeEmBarSZ2_0[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbBarSizeEmBarSZ2_0[PortIndex] ));
      DEBUG ((DEBUG_INFO, "NtbBarSizeEmBarSZ2_1[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbBarSizeEmBarSZ2_1[PortIndex] ));
      DEBUG ((DEBUG_INFO, "NtbXlinkCtlOverride[%d] = 0x%x\n", PortIndex, IioGlobalData->SetupData.NtbXlinkCtlOverride[PortIndex]  ));
    } // for (Index ...

    for (Stack = 0; Stack < NUM_DEVHIDE_STACKS; Stack++) {
      for (Func = 0; Func < NUM_DEVHIDE_REGS_PER_STACK; Func++) {
        if (Stack < NUM_DEVHIDE_IIO_STACKS) {
          //
          // This is IIO stack
          //
          DEBUG ((DEBUG_INFO,"DfxDevHideOnIioStacks[%d.%d:f%d] = 0x%x\n",
              Socket, Stack, Func, IioGlobalData->SetupData.DfxDevHideOnIioStacks[Socket][Stack][Func]));
        } else {
          if (!IsCpuAndRevision (CPU_SKX, REV_ALL) && !IsCpuAndRevision (CPU_CLX, REV_ALL) && !IsCpuAndRevision (CPU_CPX, REV_ALL)) {
            //
            // This is uncore stack - skipped for 14nm
            //
            DEBUG ((DEBUG_INFO,"DfxDevHideOnUncoreStacks[%d.%d:f%d] = 0x%x\n",
                Socket, Stack-NUM_DEVHIDE_IIO_STACKS, Func, IioGlobalData->SetupData.DfxDevHideOnUncoreStacks[Socket][Stack][Func]));
          }
        }
      }
    }

    MaxStackSocket = GetMaxStackNumPerSocket ();
    for (StackIndex = 0; StackIndex < MaxStackSocket; StackIndex++) {
      if (IfStackPresent (Socket, StackIndex)) {
        MaxStackPort = GetMaxPortNumPerStack (StackIndex);
        for (PortIndex = 0; PortIndex < MaxStackPort; PortIndex++) {
          DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioVData.StackPerPort[%d][%d] = 0x%x\n", Socket, PortIndex, IioGlobalData->IioVar.IioVData.StackPerPort[Socket][PortIndex]));
        }
      }
    }
  }
  //
  // Setup Options per CB3 Devices
  //
  for (Index=0; Index < TOTAL_CB3_DEVICES; Index++) {
    DEBUG ((DEBUG_INFO,"Cb3DmaEn[%d] = 0x%x\n", Index, IioGlobalData->SetupData.Cb3DmaEn[Index]));
    DEBUG ((DEBUG_INFO,"Cb3NoSnoopEn[%d] = 0x%x\n", Index, IioGlobalData->SetupData.Cb3NoSnoopEn[Index]));
  }

  DEBUG ((DEBUG_INFO,"VTdSupport = 0x%x\n", IioGlobalData->SetupData.VTdSupport)) ;
  DEBUG ((DEBUG_INFO,"ProcessorX2apic = 0x%x\n", IioGlobalData->SetupData.ProcessorX2apic)) ;

  DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioVData.EVMode = 0x%x\n", IioGlobalData->IioVar.IioVData.EVMode));

  DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioVData.Pci64BitResourceAllocation = 0x%x\n", IioGlobalData->IioVar.IioVData.Pci64BitResourceAllocation));
  DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioVData.PlatformType = 0x%x\n", IioGlobalData->IioVar.IioVData.PlatformType));

  //
  // Update Other variables required for IIO Init
  //
  DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioVData.IsocEnable = 0x%x\n", IioGlobalData->IioVar.IioVData.IsocEnable));
  DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioVData.meRequestedSize = 0x%x\n", IioGlobalData->IioVar.IioVData.meRequestedSize));
  DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioVData.ieRequestedSize = 0x%x\n", IioGlobalData->IioVar.IioVData.ieRequestedSize));
  DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioVData.IOxAPICCallbackBootEvent = 0x%x\n", IioGlobalData->IioVar.IioVData.IOxAPICCallbackBootEvent));
  DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioVData.Emulation = 0x%x\n", IioGlobalData->IioVar.IioVData.Emulation));
  DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioVData.IODC = 0x%x\n", IioGlobalData->IioVar.IioVData.IODC));
  DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioOutData.MemResetRequired = 0x%x\n", IioGlobalData->IioVar.IioOutData.MemResetRequired));

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    DEBUG ((DEBUG_INFO, "IioGlobalData->IioVar.IioVData.SocketPresent[%d] = 0x%x\n", IioIndex, IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]));
    DEBUG ((DEBUG_INFO, "IioGlobalData->IioVar.IioVData.FpgaActive[%d] = 0x%x\n", IioIndex, IioGlobalData->IioVar.IioVData.FpgaActive[IioIndex]));

    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }

    DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[%d] = 0x%x\n", IioIndex, IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex]));

    for (PortIndex = 0; PortIndex < NUMBER_PORTS_PER_SOCKET; PortIndex++) {
      Status = GetNtbPortPerPortIndex ((UINT8)PortIndex, &PortNtbIndex);
      if (Status != EFI_SUCCESS) {
        continue;
      }
      DEBUG ((DEBUG_INFO,"IioGlobalData->IioVar.IioOutData.PciePortOwnership[(%d * %d) + %d] = 0x%x\n", IioIndex, NUMBER_PORTS_PER_SOCKET,
        PortIndex, IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex]));
    }
  }
}


/**
  Updates IIO config block policies from setup data.

  @param[in,out] IioPolicyPpi         The pointer to get IIO Policy PPI instance

  @retval EFI_SUCCESS          The function completed successfully
  @retval EFI_ERROR            Failed to update policies
**/
EFI_STATUS
EFIAPI
IioUpdateConfigBlockPolicy (
  IN OUT  IIO_POLICY_STRUCT *IioPolicyPpi
)
{
  //
  // TO_DO: Update IIO config block policies from setup data
  //
  return EFI_SUCCESS;
}
