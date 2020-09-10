/** @file
  Interface source file for the HBM library class.

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

#include "HbmCore.h"
#include <Library/DebugLib.h>
#include <Library/SysHostPointerLib.h>

/**

  Gets architectural variables for HBM

  @param[out]               HbmMaxIoInst        Pointer to specify the max number of instances
  @param[out]               HbmMaxChannel       Pointer to specify the max number of channels
  @param[out]               HbmNumChPerIo       Pointer to specify the max number of channels per IO

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
HbmPopulateLegacyVariables (
  OUT   UINT8           *HbmMaxIoInst,
  OUT   UINT8           *HbmMaxChannel,
  OUT   UINT8           *HbmNumChPerIo
  )
{
  if (HbmMaxIoInst != NULL) {
    *HbmMaxIoInst   = MAX_HBM_IO;
  }

  if (HbmMaxChannel != NULL) {
    *HbmMaxChannel  = MAX_HBM_CH;
  }

  if (HbmNumChPerIo != NULL) {
    *HbmNumChPerIo  = MAX_HBM_IO_CH;
  }

  return EFI_SUCCESS;
}

/**

  Populate HBM global variables based on the MSM table

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmPopulateArchVariables (
  IN        UINT8           Socket
  )
{
  return EFI_SUCCESS;
}

/**

  Verify HBM arch variables

  @param[in]                MsmListPtr          Pointer to MSM List

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
VerifyHbmMsmList (
  IN      MRC_MSM           *MsmListPtr
  )
{
  ASSERT (MsmListPtr->NumDataCh <= MAX_HBM_IO_CH);
  ASSERT (MsmListPtr->NumDataIoFubsPerCh <= MAX_HBM_STROBE);
  ASSERT (MsmListPtr->NumDataIoFubsPerSubCh <= (MAX_HBM_IO_CH / MAX_HBM_STROBE));
  ASSERT (MsmListPtr->NumDqLanesPerCh <= MAX_HBM_BITS_PER_CH);

  return EFI_SUCCESS;
}
/**

  Get max strobes based on current memory subsystem

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSsIndex          Current Memory Sub System

  @return                   Max strobes

**/
UINT8
EFIAPI
HbmGetMaxStrobesMemSs (
  IN        UINT8           Socket,
  IN        UINT8           MemSsIndex
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();
  return (UINT8) HbmHost->Var.Socket[Socket].HbmArchInfo.MaxStrobes[MemSsIndex];
}

/**

  Get max strobes based on channel

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @return                   Max strobes

**/
UINT8
EFIAPI
HbmGetMaxStrobesChannel (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  )
{
  UINT32      MemSsIndex;
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  MemSsIndex = ConvertChannelToMemSsIndex (Socket, Channel);

  if (Channel >= HbmHost->Var.Socket[Socket].HbmArchInfo.MaxChannelAll) {
    HbmDebugPrint (HBM_SDBG_MAX, "Invalid Channel: %d >= %d\n", Channel, HbmHost->Var.Socket[Socket].HbmArchInfo.MaxChannelAll);
    ASSERT (FALSE);
  }

  return (UINT8)HbmHost->Var.Socket[Socket].HbmArchInfo.MaxStrobes[MemSsIndex];
}

/**

  Get max bits based on current memory subsystem

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSsIndex          Current Memory Sub System

  @return                   Max Bits

**/
UINT8
EFIAPI
HbmGetMaxBitsMemSs (
  IN        UINT8           Socket,
  IN      UINT8             MemSsIndex
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  return (UINT8)HbmHost->Var.Socket[Socket].HbmArchInfo.MaxBits[MemSsIndex];
}

/**

  Get max bits based on channel

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @return                   Max bits

**/
UINT8
EFIAPI
HbmGetMaxBitsChannel (
  IN        UINT8           Socket,
  IN      UINT8             Channel
  )
{
  UINT32  MemSsIndex;
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  MemSsIndex = ConvertChannelToMemSsIndex (Socket, Channel);

  if (Channel >= HbmHost->Var.Socket[Socket].HbmArchInfo.MaxChannelAll) {
    HbmDebugPrint (HBM_SDBG_MAX, "Invalid Channel: %d >= %d\n", Channel, HbmHost->Var.Socket[Socket].HbmArchInfo.MaxChannelAll);
    ASSERT (FALSE);
  }

  return (UINT8)HbmHost->Var.Socket[Socket].HbmArchInfo.MaxBits[MemSsIndex];
}

/**

  Get max channels per MemSs based on current MemSs (0-Based)

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSsIndex          Current Memory Sub System

  @return                   Max channels for current HBM-MemSs

**/
UINT8
EFIAPI
HbmGetMaxChannelMemSs (
  IN      UINT8             Socket,
  IN      UINT8             MemSsIndex
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  return (UINT8)HbmHost->Var.Socket[Socket].HbmArchInfo.MaxChannelPerMemSs[MemSsIndex];
}

/**

  Get max HBM Memory subsystems

  @param[in]                Socket              Current Socket (0-Based)

  @return                   Max HBM-MemSs

**/
UINT8
EFIAPI
HbmGetMaxMemSs (
  IN        UINT8           Socket
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  return (UINT8)HbmHost->Var.Socket[Socket].HbmArchInfo.MaxMemSs;
}

/**
  Converts the input channel to proper MemSs Index

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @return                   Hbm Memory Sub system Index based on input Channel

**/
UINT8
EFIAPI
ConvertChannelToMemSsIndex (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  )
{
  UINT8     MemSsIndexLoop;
  UINT8     HbmMemSsIndex;
  UINT32    Accumlative;

  Accumlative       = 0;
  HbmMemSsIndex     = 0xFF;

  //
  // Determine the HBM MemSs index by counting the Hbm2MemSs types from MemSsList
  //
  for (MemSsIndexLoop = 0; MemSsIndexLoop < GetNumMemSs (Socket); MemSsIndexLoop++) {
    if (GetMemSsTypeFromSsList (Socket, MemSsIndexLoop) == Hbm2MemSs) {
      Accumlative += GetNumDataChFromSsList (Socket, MemSsIndexLoop);
      HbmMemSsIndex++;

      if (Channel < Accumlative) {
        break;
      }
    }
  }

  //
  // Not found any Hbm2MemSs types in the MemSsList
  //
  if (HbmMemSsIndex == 0xff) {
    HbmDebugPrint (HBM_SDBG_MAX, "HbmMemSsIndex = %d, MemSsIndexLoop = %d\n", HbmMemSsIndex, MemSsIndexLoop);
    return 0;
  }

  if (HbmMemSsIndex >= MAX_HBM_IO) {
    HbmDebugPrint (HBM_SDBG_MAX, "HbmMemSsIndex = %d\n", HbmMemSsIndex);
    return 0;
  }

  return HbmMemSsIndex;

}

/**

  Determine if an specific step needs to be executed

  @param[in]                FlowBit             An step represented by one or more bits

  @retval                   TRUE                All bits are set
  @retval                   FALSE               At least 1 bit is not set for current flow

**/
BOOLEAN
EFIAPI
IsHbmFlowEnabled (
  IN        UINT32          FlowBit
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  if ((HbmHost->HbmFlows & FlowBit) == FlowBit) {
    return TRUE;
  }

  return FALSE;
}

/**

  Set the flow bits

  @param[in]                Flows               Value that represents all steps for HBM

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetHbmFlows (
  IN        UINT32          Flows
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  HbmHost->HbmFlows = Flows;

  return EFI_SUCCESS;
}

/**

  Returns the flow bits

  @return                   Flow bits

**/
UINT32
EFIAPI
GetHbmFlows (
  VOID
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  return HbmHost->HbmFlows;
}

/**
  This function enables all HBM flows.

  @param VOID

  @retval EFI_SUCCESS   All HBM flows are enabled successfully.
**/
EFI_STATUS
EFIAPI
EnableAllHbmFlows (
  VOID
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();
  HbmHost->HbmFlows = ~(HbmHost->HbmFlows & 0);

  return EFI_SUCCESS;
}

/**

  Allocates the required resources for HBM Host

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors
  @retval                   EFI_OUT_OF_RESOURCES  Pei services not able to allocate resources for HBM Host

**/
EFI_STATUS
EFIAPI
HbmHostAllocation (
  VOID
  )
{
  VOID *HbmHostV;

  HbmHostV = (VOID *) (UINTN) AllocateZeroPool (sizeof (HBM_HOST));

  if (HbmHostV == NULL) {
    ASSERT (HbmHostV != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  SetHbmHostPointer (HbmHostV);

  return EFI_SUCCESS;
}

/**
  Gets the value for the setup option specified

  @param[in]                HBM_POLICY_OPTIONS  Specific HBM Setup Option

  @return                   Value for specified setup option

**/
UINT32
EFIAPI
GetHbmSetupOption (
  IN        HBM_POLICY_OPTIONS  HbmSetupOption
  )
{
  HBM_HOST    *HbmHost;
  UINT32      ConfigValue;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  switch (HbmSetupOption) {
  case HbmOptionCacheEnable:
    ConfigValue = HbmHost->Policy.HbmCacheEnable;
    break;

  case HbmOptionCacheRatio:
    ConfigValue = HbmHost->Policy.HbmCacheRatio;
    break;

  case HbmOptionCacheFmType:
    ConfigValue = HbmHost->Policy.HbmCacheFmType;
    break;

  case HbmOptionCacheDdrtGran:
    ConfigValue = HbmHost->Policy.HbmCacheDdrtGran;
    break;

  case HbmOptionSrefProgramming:
    ConfigValue = HbmHost->Policy.HbmSrefProgramming;
    break;

  case HbmOptionSrefIdleTimer:
    ConfigValue = HbmHost->Policy.HbmSrefIdleTimer;
    break;

  case HbmOptionOppSrefEn:
    ConfigValue = HbmHost->Policy.HbmOppSrefEn;
    break;

  case HbmOptionPkgcSrefEn:
    ConfigValue = HbmHost->Policy.HbmPkgcSrefEn;
    break;

  case HbmOptionMiscOptions:
    ConfigValue = HbmHost->Policy.HbmMiscOptions;
    break;

  default:
    ConfigValue = 0;
  }

  return ConfigValue;
}

/**
  Sets the value for the Policy option specified

  @param[in]                HBM_POLICY_OPTIONS  Specific HBM Setup Option
  @param[in]                ConfigValue          Value

**/
VOID
EFIAPI
HbmSetPolicyOption (
  IN        HBM_POLICY_OPTIONS  HbmSetupOption,
  IN        UINT32              ConfigValue
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  switch (HbmSetupOption) {
  case HbmOptionCacheEnable:
    HbmHost->Policy.HbmCacheEnable     = (UINT8) ConfigValue;
    break;

  case HbmOptionCacheRatio:
    HbmHost->Policy.HbmCacheRatio      = (UINT8) ConfigValue;
    break;

  case HbmOptionCacheFmType:
    HbmHost->Policy.HbmCacheFmType     = (UINT8) ConfigValue;
    break;

  case HbmOptionCacheDdrtGran:
    HbmHost->Policy.HbmCacheDdrtGran   = (UINT8) ConfigValue;
    break;

  case HbmOptionSrefProgramming:
    HbmHost->Policy.HbmSrefProgramming = (UINT8) ConfigValue;
    break;

  case HbmOptionSrefIdleTimer:
    HbmHost->Policy.HbmSrefIdleTimer   = (UINT8) ConfigValue;
    break;

  case HbmOptionOppSrefEn:
    HbmHost->Policy.HbmOppSrefEn       = (UINT8) ConfigValue;
    break;

  case HbmOptionPkgcSrefEn:
    HbmHost->Policy.HbmPkgcSrefEn      = (UINT8) ConfigValue;
    break;

  case HbmOptionMiscOptions:
    HbmHost->Policy.HbmMiscOptions     = ConfigValue;
    break;
  }
}

/**

  Determines if channels is enabled or not

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @retval                   TRUE                Channel is enabled
  @retval                   FALSE               Channels is not enabled

**/
BOOLEAN
EFIAPI
IsHbmChannelEnabled (
  IN        UINT8               Socket,
  IN        UINT8               Channel
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  if (HbmHost->NvRam.Socket[Socket].HbmChData[Channel].ChEnable == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  Determines if HBM memory subsystem is enabled or disabled.

  @param[in] Socket         Current Socket (0-Based)
  @param[in] MemSs          Current HBM memory subsystem

  @retval TRUE              HBM memory subsystem is enabled
  @retval FALSE             HBM memory subsystem is disabled

**/
BOOLEAN
EFIAPI
IsHbmMemSsEnabled (
  IN UINT8    Socket,
  IN UINT8    MemSs
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  if (HbmHost->Var.Socket[Socket].HbmIoEn[MemSs] == 1) {
    return TRUE;
  }

  return FALSE;
}

/**

  Sets if MemSs is enabled or not

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[in]                Enable              Value to set

**/
VOID
EFIAPI
SetHbmMemSsEnable (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
  IN        UINT8               Enable
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  HbmHost->Var.Socket[Socket].HbmIoEn[MemSs] = Enable;
}

/**
  This function gets the HBM frequency from HBM Host.

  @param[in] SocketId         Socket index.

  @retval  The value of HBM frequency.
**/
HBM_FREQUENCY
EFIAPI
HbmGetFrequency (
  IN UINT8  SocketId
  )
{
  HBM_HOST  *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();
  return HbmHost->Var.Socket[SocketId].HbmFrequency;
}

/**
  This function gets the HBM frequency in MHz from HBM Host for the specified socket.

  @param[in] SocketId         Socket index.

  @retval    The value of HBM frequency in Mhz.
**/
UINT16
EFIAPI
HbmGetFrequencyInMhz (
  IN UINT8  SocketId
  )
{
  HBM_HOST  *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();
  switch (HbmHost->Var.Socket[SocketId].HbmFrequency) {

  case HBM2_1p6:
    return 1600;

  case HBM2_2p0:
    return 2000;

  case HBM2_2p4:
    return 2400;

  case HBM2E_2p8:
    return 2800;

  case HBM2E_3p2:
    return 3200;

  default:
    HbmDebugPrintLine ("Unsupported HBM frequency, force to 0!\n");
    return 0;
  }
}

/**
  This function sets the HBM frequency into HBM Host.

  @param[in] SocketId         Socket index.
  @param[in] HbmFrequency     HBM frequency to set.

  @retval N/A.
**/
VOID
EFIAPI
HbmSetFrequency (
  IN UINT8          SocketId,
  IN HBM_FREQUENCY  HbmFrequency
  )
{
  HBM_HOST  *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();
  HbmHost->Var.Socket[SocketId].HbmFrequency = HbmFrequency;
}

/**

  Sets (Enables/Disables) an HBM channel

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Enable              Value to set

**/
VOID
EFIAPI
SetHbmChEnable (
  IN        UINT8               Socket,
  IN        UINT8               Channel,
  IN        UINT8               Enable
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  if (Channel < MAX_HBM_CH) {
    HbmHost->NvRam.Socket[Socket].HbmChData[Channel].ChEnable = Enable;
  }
}

/**

  Gets the start/end channel of a particular MemSs

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[out]               StartCh             Start channel of current MemSs (Pointer)
  @param[out]               EndCh               End channel of current MemSs (Pointer)

**/
VOID
EFIAPI
GetHbmStartEndCh (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
  OUT       UINT8               *StartCh,
  OUT       UINT8               *EndCh
  )
{
  UINT8       MaxChannelPerMemSs;

  *StartCh  = 0;
  *EndCh    = 0;

  MaxChannelPerMemSs  = HbmGetMaxChannelMemSs (Socket, MemSs);
  *StartCh            = MemSs * MaxChannelPerMemSs;
  *EndCh              = *StartCh  + MaxChannelPerMemSs;
}

/**

  Gets the cached value for the specified MRS

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[in]                HbmCacheMr          HBM MRS

  @return                   Gets the cached value for the specified MRS

**/
UINT8
EFIAPI
GetHbmCacheMr (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
  IN        HBM_CACHE_MR        HbmCacheMr
  )
{
  UINT8       CacheValue;
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  switch (HbmCacheMr) {
  case HbmCacheMr00:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr00.Data;
    break;

  case HbmCacheMr01:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr01.Data;
    break;

  case HbmCacheMr02:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr02.Data;
    break;

  case HbmCacheMr03:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr03.Data;
    break;

  case HbmCacheMr04:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr04.Data;
    break;

  case HbmCacheMr05:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr05.Data;
    break;

  case HbmCacheMr06:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr06.Data;
    break;

  case HbmCacheMr07:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr07.Data;
    break;

  case HbmCacheMr08:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr08.Data;
    break;

  case HbmCacheMr09:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr09.Data;
    break;

  case HbmCacheMr10:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr10.Data;
    break;

  case HbmCacheMr11:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr11.Data;
    break;

  case HbmCacheMr12:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr12.Data;
    break;

  case HbmCacheMr13:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr13.Data;
    break;

  case HbmCacheMr14:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr14.Data;
    break;

  case HbmCacheMr15:
    CacheValue = HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr15.Data;
    break;

  default:
    CacheValue = 0;
  }

  return CacheValue;
}

/**

  Sets the cached value for the specified MRS

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[in]                HbmCacheEye         HBM MRS
  @param[in]                CacheValue          Value to set

**/
VOID
EFIAPI
SetHbmCacheMr (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
  IN        HBM_CACHE_MR        HbmCacheMr,
  IN        UINT8               CacheValue
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  switch (HbmCacheMr) {
  case HbmCacheMr00:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr00.Data = CacheValue;
    break;

  case HbmCacheMr01:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr01.Data = CacheValue;
    break;

  case HbmCacheMr02:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr02.Data = CacheValue;
    break;

  case HbmCacheMr03:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr03.Data = CacheValue;
    break;

  case HbmCacheMr04:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr04.Data = CacheValue;
    break;

  case HbmCacheMr05:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr05.Data = CacheValue;
    break;

  case HbmCacheMr06:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr06.Data = CacheValue;
    break;

  case HbmCacheMr07:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr07.Data = CacheValue;
    break;

  case HbmCacheMr08:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr08.Data = CacheValue;
    break;

  case HbmCacheMr09:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr09.Data = CacheValue;
    break;

  case HbmCacheMr10:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr10.Data = CacheValue;
    break;

  case HbmCacheMr11:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr11.Data = CacheValue;
    break;

  case HbmCacheMr12:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr12.Data = CacheValue;
    break;

  case HbmCacheMr13:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr13.Data = CacheValue;
    break;

  case HbmCacheMr14:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr14.Data = CacheValue;
    break;

  case HbmCacheMr15:
    HbmHost->Var.Socket[Socket].HbmCh[MemSs].HbmMr15.Data = CacheValue;
    break;

  default:
    CacheValue = 0;
  }

}

/**
  Gets the cache value for Mem Size

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System

  @return                   Cache value for Mem Size

**/
UINT16
EFIAPI
GetHbmCacheMemSize (
  IN        UINT8               Socket,
  IN        UINT8               MemSs
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  return HbmHost->Var.Socket[Socket].HbmCh[MemSs].MemSize;
}

/**
  Sets the cache value for Mem Size

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[in]                Value               Value to set

**/
VOID
EFIAPI
SetHbmCacheMemSize (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
  IN        UINT16              Value
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  HbmHost->Var.Socket[Socket].HbmCh[MemSs].MemSize = Value;
}

/**
  Get the HBM manufacture ID

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            The HBMIO instance index (0~ max HBMIO in socket -1)

**/
UINT32
EFIAPI
GetHbmManufacturerId (
  IN UINT8     Socket,
  IN UINT8     HbmIoIndex
  )
{
  HBM_HOST  *HbmHost;
  HbmHost = (HBM_HOST *) GetHbmHostPointer ();
  return HbmHost->Var.Socket[Socket].HbmCh[HbmIoIndex].DevId.Bits.manufacturer_id;
}

/**
  Get the pointer to HBM device ID data.

  @param[in] Socket              Current Socket (0-Based)
  @param[in] HbmIoIndex          The HBMIO instance index (0~ max HBMIO in socket -1)

  @retval  The pointer to HBM device ID data.
**/
DEVICE_ID_WDR *
EFIAPI
GetHbmDeviceId (
  IN UINT8     Socket,
  IN UINT8     HbmIoIndex
  )
{
  HBM_HOST     *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();
  return & (HbmHost->Var.Socket[Socket].HbmCh[HbmIoIndex].DevId);
}

/**
  Get gen2_test from DEVICE_ID_WDR

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                HbmIoIndex          The HBMIO instance index (0~ max HBMIO in socket -1)

**/
BOOLEAN
EFIAPI
GetHbmGen2Test (
  IN UINT8     Socket,
  IN UINT8     HbmIoIndex
  )
{
  HBM_HOST  *HbmHost;
  HbmHost = (HBM_HOST *) GetHbmHostPointer ();
  return (BOOLEAN)HbmHost->Var.Socket[Socket].HbmCh[HbmIoIndex].DevId.Bits.gen2_test;
}

/**
  Get the HBM Pannic Handle Info

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            The HBMIO instance index (0~ max HBMIO in socket -1)

**/
UINT32
EFIAPI
GetHbmPanHandleInfo (
  IN UINT8     Socket,
  IN UINT8     HbmIoIndex
  )
{
  HBM_HOST  *HbmHost;
  HbmHost = (HBM_HOST *) GetHbmHostPointer ();
  return HbmHost->Var.Socket[Socket].HbmCh[HbmIoIndex].PanHandleInfo;
}

/**

  Programs input parameters for the HBM MRC

**/
UINT32
EFIAPI
HbmGetPolicyOption (
  IN        HBM_POLICY_OPTIONS  HbmPolicyOption
  )
{
  HBM_HOST    *HbmHost;
  UINT32      SetupValue;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  switch (HbmPolicyOption) {
  case HbmOptionCacheEnable:
    SetupValue = HbmHost->Policy.HbmCacheEnable;
    break;

  case HbmOptionCacheRatio:
    SetupValue = HbmHost->Policy.HbmCacheRatio;
    break;

  case HbmOptionCacheFmType:
    SetupValue = HbmHost->Policy.HbmCacheFmType;
    break;

  case HbmOptionCacheDdrtGran:
    SetupValue = HbmHost->Policy.HbmCacheDdrtGran;
    break;

  case HbmOptionSrefProgramming:
    SetupValue = HbmHost->Policy.HbmSrefProgramming;
    break;

  case HbmOptionSrefIdleTimer:
    SetupValue = HbmHost->Policy.HbmSrefIdleTimer;
    break;

  case HbmOptionOppSrefEn:
    SetupValue = HbmHost->Policy.HbmOppSrefEn;
    break;

  case HbmOptionPkgcSrefEn:
    SetupValue = HbmHost->Policy.HbmPkgcSrefEn;
    break;

  case HbmOptionMiscOptions:
    SetupValue = HbmHost->Policy.HbmMiscOptions;
    break;

  default:
    SetupValue = 0;
  }

  return SetupValue;
}

/**
  Sets the number of Max Sockets in the HBM Host structure

  @param[in]                MaxSockets          Specifies the number for Max Sockets

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetHbmMaxSockets (
  IN        UINT8           MaxSockets
  )
{
  HBM_HOST    *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  HbmHost->HbmData.NumSockets = MaxSockets;

  return EFI_SUCCESS;
}

